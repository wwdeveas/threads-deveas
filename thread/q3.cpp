//q3.cpp
//walter deveas
//04/29/24
#include <iostream>
#include <thread>
#include <semaphore.h>
#include <queue>
#include <string>
#include <chrono>
#include <ctime>
using namespace std;

struct reqstruct {
    int requestID;
    string ip;
    string pageReq;
};
const int BUFFER_SIZE = 5;
queue<reqstruct> msg_queue;
string pages[10] = {"myspace.com", "github.com", "hulu.com", "twitter.com", "facebook.com", "reddit.com", "amazon.com", "instagram.com", "google.com", "netflix.com"};
sem_t empty;
sem_t full;
sem_t lock;
int requestID = 0;
void listen() {
    while (true) {
        this_thread::sleep_for(chrono::seconds(1));
        reqstruct new_request;
        new_request.requestID = requestID;
        new_request.ip = "";
        new_request.pageReq = pages[rand() % 10];
        sem_wait(&empty);
        sem_wait(&lock);
        msg_queue.push(new_request);
        cout << "reqID: " << requestID << " appended" << endl;
        requestID++;
        sem_post(&lock);
        sem_post(&full);
    }
}
void do_request(int threadID) {
    while (true) {
        sem_wait(&full);
        sem_wait(&lock);
        if (msg_queue.empty()) {
            sem_post(&lock);
            continue;
        }
        reqstruct req = msg_queue.front();
        msg_queue.pop();
        cout << "threadID: " << threadID << "\n req: " << req.requestID << "\n page " << req.pageReq << endl;
        sem_post(&lock);
        sem_post (&empty);

    }
}

int main() {
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    sem_init(&lock, 0, 1);
    thread listen_thread(listen);
    int numThread = 4;
    thread currentThread[numThread];
    for (int i = 0; i < numThread; i++) {
        currentThread[i] = thread(do_request, i);
    }
    listen_thread.join();
    for (int i = 0; i < numThread; i++) {
        currentThread[i].join();
    }
    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&lock);
    return 0;
}