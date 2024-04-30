//q2_part2 
//walter deveas
//04/29/24
#include <iostream>
#include <thread>
#include <semaphore.h>
#include <mutex>

using namespace std;

mutex mtx;
sem_t sem1, sem2;

void first()
{
    cout << "run first" << endl;
    sem_post(&sem1);
}

void second()
{
    sem_wait(&sem1);
    cout << "run second" << endl;
    sem_post(&sem2);
}

void third()
{
    sem_wait(&sem2);
    cout << "run third" << endl;
}

int main()
{
    const int num = 3;
    sem_init(&sem1, 0, 0);
    sem_init(&sem2, 0, 0);
    thread t[num];

    t[0] = thread(first);
    t[1] = thread(second);
    t[2] = thread(third);

    for (int i = 0; i < num; ++i)
    {
        t[i].join();
    }

    sem_destroy(&sem1); // destroy the semaphores
    sem_destroy(&sem2);
    return 0;
}