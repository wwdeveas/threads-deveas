//q3.cpp
//walter deveas
//04/29/24
#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <random>

using namespace std;

struct RequestStructure {
    int request_id;
    string ip_address;
    string page_requested;
};

queue<RequestStructure> msg_queue;
mutex mtx;
condition_variable cv;
int request_counter = 0;
string webPages[10] = {"google.com", "yahoo.com", "stackoverflow.com", "github.com", "amazon.com", "ebay.com", "wikipedia.org", "reddit.com", "linkedin.com", "twitter.com"};


void listen() {
    while (true) {
        this_thread::sleep_for(chrono::seconds(rand() % 3 + 1));
        RequestStructure req;
        req.request_id = ++request_counter;
        req.ip_address = "";
        req.page_requested = webPages[rand() % 10];
        unique_lock<mutex> lock(mtx);
        msg_queue.push(req);
        lock.unlock();
        cv.notify_one();
    }
}

void do_request(int thread_id) {
    while (true) {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [] { return !msg_queue.empty(); });

        RequestStructure req = msg_queue.front();
        msg_queue.pop();

        lock.unlock();

        cout << "Thread " << thread_id << " completed request " << req.request_id << " requesting webpage " << req.page_requested << endl;
    }
}

int main() {
    const int num_listener_threads = 3; 
    thread listener_threads[num_listener_threads];
    for (int i = 0; i < num_listener_threads; ++i) {
        listener_threads[i] = thread(listen);
    }
    const int num_request_threads = 5; 
    thread request_threads[num_request_threads];
    cout<<num_request_threads;
    for (int i = 0; i < num_request_threads; ++i) {
        request_threads[i] = thread(do_request, i + 1);}
    for (int i = 0; i < num_listener_threads; ++i) {
        listener_threads[i].join();}
    for (int i = 0; i < num_request_threads; ++i) {
        request_threads[i].join();}
    return 0;
}