#include "worker.h"

Worker::Worker(QObject *parent)
    : QObject(parent)
    , readyToStart(false)
    , stopped(false)
    , available(true)
    , lastResult(false)
{
    pthread_mutex_init(&mutex, NULL);
}

Worker::~Worker()
{
    pthread_mutex_destroy(&mutex);
}

bool Worker::isAvailable() const
{
    return available;
}

void Worker::perform()
{
    pthread_mutex_lock(&mutex);
    readyToStart = false;
    available = false;

//    qDebug() << "Worker::perform" << fileName;

    lame_global_flags *gfp;
    gfp = lame_init();

    /* version  0=MPEG-2  1=MPEG-1  (2=MPEG-2.5)     */
//    qDebug() << "Lame v." << lame_get_version(gfp);

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "File not open";
        finishedFunc(false);
        return;
    }

    WavHeader hdr;
    file.read((char*)&hdr, sizeof(hdr));

    QByteArray riff(hdr.RIFF, 4);
    QByteArray wave(hdr.format, 4);

    if ((riff != QByteArray("RIFF")) || (wave != QByteArray("WAVE")))
    {
        finishedFunc(false);
        return;
    }

    qDebug() << "Wav header:"
             << riff
             << wave
             << hdr.numChannels
             << hdr.sampleRate
             << hdr.byteRate
             << hdr.blockAlign
             << hdr.bitsPerSample;

    lame_set_in_samplerate(gfp, hdr.sampleRate);
    lame_set_num_channels(gfp, hdr.numChannels);
    lame_set_quality(gfp, 5);
    lame_set_mode(gfp, (hdr.numChannels == 2)? STEREO : MONO);
    lame_set_brate(gfp, 320);

    if ((lame_init_params(gfp)) < 0)
    {
        qDebug() << "Unable to initialize MP3 parameters";
        finishedFunc(false);
        return;
    }

    const int BUFFER_SIZE = 8192*hdr.numChannels;
    const int sampleSize = hdr.blockAlign;

    switch (sampleSize)
    {
    case 1://?!
    {
        lame_set_scale(gfp, 2);
    }
    case 2:
    {
        short inBuf[BUFFER_SIZE];
        unsigned char outBuf[BUFFER_SIZE*sampleSize];

        memset(inBuf, 0, sampleSize*BUFFER_SIZE);
        memset(outBuf, 0, sampleSize*BUFFER_SIZE);

        QFile outFile(fileName+"_encoded.mp3");
        outFile.open(QIODevice::WriteOnly);

        //encoding
        int readCount = 0;
        do
        {
            int encodedCount = 0;
            readCount = file.read((char*)inBuf, BUFFER_SIZE*sampleSize);
            if (readCount > 0)
            {
                if (hdr.numChannels == 2)
                    encodedCount = lame_encode_buffer_interleaved(gfp, inBuf, (readCount/sampleSize)/2, outBuf, BUFFER_SIZE*sampleSize);
                else
                    encodedCount = lame_encode_buffer(gfp, inBuf, nullptr, readCount/sampleSize, outBuf, BUFFER_SIZE*sampleSize);
            }
            else
                encodedCount = lame_encode_flush(gfp, outBuf, BUFFER_SIZE);

            outFile.write((const char*)outBuf, encodedCount);
            memset(inBuf, 0, sampleSize*BUFFER_SIZE);
            memset(outBuf, 0, sampleSize*BUFFER_SIZE);
        }
        while(readCount > 0);

        outFile.close();
    }
        break;
    case 4:
    {
        int inBuf[BUFFER_SIZE];
        unsigned char outBuf[BUFFER_SIZE*sampleSize];

        QFile outFile(fileName+"_encoded.mp3");
        outFile.open(QIODevice::WriteOnly);

        //encoding
        int readCount = 0;
        do
        {
            int encodedCount = 0;
            readCount = file.read((char*)inBuf, BUFFER_SIZE*sampleSize);
            if (readCount > 0)
            {
                if (hdr.numChannels == 2)
                    encodedCount = lame_encode_buffer_interleaved_int(gfp, inBuf, (readCount/sampleSize)/2, outBuf, BUFFER_SIZE*sampleSize);
                else
                    encodedCount = lame_encode_buffer_int(gfp, inBuf, nullptr, readCount/sampleSize, outBuf, BUFFER_SIZE*sampleSize);
            }
            else
                encodedCount = lame_encode_flush(gfp, outBuf, BUFFER_SIZE);

            outFile.write((const char*)outBuf, encodedCount);
            memset(inBuf, 0, sampleSize*BUFFER_SIZE);
            memset(outBuf, 0, sampleSize*BUFFER_SIZE);
        }
        while(readCount > 0);

        outFile.close();
    }
        break;
    }

    pthread_mutex_unlock(&mutex);

    finishedFunc(true);

    return;
}

bool Worker::isReady() const
{
    return readyToStart;
}

bool Worker::isStopped() const
{
    if (isAvailable())
        return stopped;
    return false;
}


void Worker::start(const QString& _fileName)
{

    pthread_mutex_lock(&mutex);
    fileName = _fileName;
    readyToStart = true;
    pthread_mutex_unlock(&mutex);
}

void Worker::stop()
{
    pthread_mutex_lock(&mutex);
    stopped = true;
    pthread_mutex_unlock(&mutex);
}

bool Worker::getLastResult() const
{
    return lastResult;
}

void Worker::finishedFunc(bool res)
{
    lastResult = res;
    available = true;
}
