//q2_part1 
//walter deveas
//04/29/24
#include <iostream>
#include <thread>
#include <semaphore.h>
#include <mutex>
#include <chrono>

using namespace std;

mutex mtx;
sem_t sem;

void first()
{
    cout << "run first" << endl;
}

void second()
{
    sem_wait(&sem);
    cout << "run second" << endl;
}

void signal()
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    sem_post(&sem);
}

int main()
{
    const int num = 2;

    sem_init(&sem, 0, 0);
    thread t[num];

    t[0] = thread(first);
    t[1] = thread(second);
    thread signalThread(signal);

    for (int i = 0; i < num; ++i)
    {
        t[i].join();
    }
    signalThread.join();

    sem_destroy(&sem);
    return 0;
}