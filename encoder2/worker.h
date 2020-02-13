#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QDebug>
#include <QFile>
#include <include/lame.h>
#include <pthread.h>

//#define BUFFER_SIZE 8192

struct WavHeader
{
    char RIFF[4];
    unsigned long chunkSize;
    char format[4];
    char subchunk1ID[4];
    unsigned long subchunk1Size;
    unsigned short audioFormat;
    unsigned short numChannels;
    unsigned long sampleRate;
    unsigned long byteRate;
    unsigned short blockAlign;
    unsigned short bitsPerSample;
    unsigned long subchunk2Id;
    int data;
};

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = nullptr);
    ~Worker();

    void perform();

    bool isAvailable() const;
    bool isStopped() const;
    bool isReady() const;

    bool getLastResult() const;

    void start(const QString &_fileName);
    void stop();

private:
    pthread_mutex_t  mutex;
    QString fileName;
    bool readyToStart;
    bool stopped;
    bool available;
    bool lastResult;
    void finishedFunc(bool res);
};

#endif // WORKER_H
