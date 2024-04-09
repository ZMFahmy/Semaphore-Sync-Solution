#include <iostream>
#include <vector>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>
#include <signal.h>

#define MCOUNTERSIZE 10
#define BUFFER_SIZE 5

sem_t mutex_1, mutex_2, full, emptyy;
int buffer[BUFFER_SIZE];
int counter = 0;
int producer_next = 0;
int consumer_next = 0;

using namespace std;

struct thread_args
{
    int id;
};

void *mCounter_func(void *arg)
{
    thread_args *args = (thread_args *)arg;

    while(1)
    {
        int sleep_time = rand() % 10;
        sleep(sleep_time);
        cout << "Counter thread " << args->id << ": received a message" << endl << endl;

        cout << "Counter thread " << args->id << ": waiting to write" << endl << endl;
        sem_wait(&mutex_1);


        counter ++;

        cout << "Counter thread " << args->id << ": now adding to counter, counter value = " << counter << endl << endl;

        sem_post(&mutex_1);
    }

    return NULL;
}

void *mMonitor_func(void *arg)
{
    int val = 0;
    int full_val;
    while(1)
    {
        int sleep_time = rand() % 10;
        sleep(sleep_time);

        ////////////// getting counter value //////////////

        cout << "Monitor thread: waiting to read counter" << endl << endl;
        sem_wait(&mutex_1);

        cout << "Monitor thread: reading a count value of " << counter << endl << endl;
        val = counter;
        counter = 0;

        sem_post(&mutex_1);

        ////////////// writing value to buffer //////////////

        sem_getvalue(&full,&full_val);
        if(full_val == BUFFER_SIZE)
            cout << "Monitor thread: Buffer full!!" << endl << endl;

        sem_wait(&emptyy);
        sem_wait(&mutex_2);

        cout << "Monitor thread: writing to buffer at position " << producer_next << endl << endl;

        buffer[producer_next] = val;
        producer_next = (producer_next + 1) % BUFFER_SIZE;

        sem_post(&mutex_2);
        sem_post(&full);
    }
}

void *mCollector_func(void *arg)
{
    int val = 0;
    int empty_val;
    while(1)
    {
        int sleep_time = rand() % 10;
        sleep(sleep_time);
        ////////////// reading value from buffer //////////////

        sem_getvalue(&emptyy,&empty_val);
        if(empty_val == BUFFER_SIZE)
            cout << "Collector thread: nothing is in the buffer!" << endl << endl;

        sem_wait(&full);
        sem_wait(&mutex_2);

        cout << "Collector thread: reading from the buffer at position " << consumer_next << endl << endl;

        val = buffer[consumer_next];
        consumer_next = (consumer_next + 1) % BUFFER_SIZE;

        sem_post(&mutex_2);
        sem_post(&emptyy);
    }
}

int main()
{
    vector<pthread_t> mCounter;
    pthread_t mMonitor;
    pthread_t mCollector;

    srand(time(NULL));

    sem_init(&mutex_1, 0, 1);
    sem_init(&mutex_2, 0, 1);
	sem_init(&full, 0, 0);
	sem_init(&emptyy, 0, BUFFER_SIZE);

    for(int i=0; i<MCOUNTERSIZE; i++)
    {
        pthread_t t;
        mCounter.push_back(t);
    }

    for(int i=0; i<MCOUNTERSIZE; i++)
    {
        thread_args *t = new thread_args();
        t->id = i + 1;

        int pid = pthread_create(&mCounter[i], NULL, mCounter_func, t);
        if (pid !=0)
            cout << "Error creating mCounter thread no: " << mCounter[i] << ". Return code:" << pid <<  endl;
    }

    int mMonitor_pid = pthread_create(&mMonitor, NULL, mMonitor_func, NULL);
    if (mMonitor_pid !=0)
        cout << "Error creating mMonitor thread. Return code:" << mMonitor_pid <<  endl;

    int mCollector_pid = pthread_create(&mCollector, NULL, mCollector_func, NULL);
    if (mCollector_pid !=0)
        cout << "Error creating mCollector thread. Return code:" << mCollector_pid <<  endl;

    pthread_join(mCollector, NULL);
    pthread_join(mMonitor, NULL);

    for(int i=0; i<MCOUNTERSIZE; i++)
        pthread_join(mCounter[i], NULL);

    sem_destroy(&mutex_1);
    sem_destroy(&mutex_2);
	sem_destroy(&full);
	sem_destroy(&emptyy);

    return 0;
}
