#pragma once

#include "Queue.h"
#include "Job.h"
#include <vector>
#include <thread>
#include <iostream>

namespace pr {

void poolWorker(int id, Queue<Job>& queue) {
    while (true) {
        Job *j = queue.pop();
        if (j == nullptr) {
            // La queue n'est plus bloquante 
            // (sinon le pop n'aurait pas retourné)
            break;
        }
        j->run();
        delete j;
    }
}

class Pool {
	Queue<Job> queue;
	std::vector<std::thread> threads;
public:
	Pool(int qsize) : queue(qsize) {}
	void start (int nbthread) {
		for (int i = 0; i < nbthread; i++) {
			threads.emplace_back(poolWorker, i, std::ref(queue));
		}
	}
	void submit (Job * job) {
		queue.push(job);
	}
	void stop() {
		queue.setBlocking(false);
	}
	~Pool() {
		for (size_t i = 0; i < threads.size(); i++) {
			threads[i].join();
		}
	}
};

}
