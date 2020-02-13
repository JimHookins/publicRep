#include "myencoder.h"

void *launchWorker(void * param)
{
    qDebug() << "luanchWorker";
    Worker* worker = static_cast<Worker*>(param);

    forever
    {
     if (worker->isStopped())
         break;

     if (worker->isReady())
         worker->perform();
    }

    pthread_exit(0);
}

pthread_t createPThread(Worker * worker)
{
    pthread_t tid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&tid, &attr, launchWorker, worker);
    return tid;
}

MyEncoder::MyEncoder(QString _path, QObject *parent)
    : QObject(parent)
    , path(_path)
{

}

MyEncoder::~MyEncoder()
{
    qDeleteAll(workers);
}

bool MyEncoder::start()
{
    QDir dir(path);

    if (!dir.exists())
        return false;

    dir.setFilter(QDir::Files);
    QStringList extList;
    extList << "*.wav";
    dir.setNameFilters(extList);

    QFileInfoList files = dir.entryInfoList();

    int cores = QThread::idealThreadCount();
    qDebug() << "Cores:" << cores;

    QVector<pthread_t> pthreads;
    pthreads.resize(cores);
    for(quint8 i = 0; i < cores; i++)
    {
        Worker *worker = new Worker();
        pthreads[i] = createPThread(worker);
        workers.append(worker);
    }

    for (QFileInfo temp : files)
    {
        qDebug() << temp;
        findAvailableWorker()->start(temp.absoluteFilePath());
    }

    releaseWorkers();

    for (pthread_t tid: pthreads)
        pthread_join(tid, nullptr);

    return true;
}

Worker* MyEncoder::findAvailableWorker()
{
    QListIterator<Worker*> it(workers);
    forever
    {
        if (it.hasNext())
        {
            if (it.peekNext()->isAvailable())
                return it.next();
        }
        else
            it.toFront();
    }
}

void MyEncoder::releaseWorkers()
{
    for(Worker * worker: workers)
        worker->stop();
}
