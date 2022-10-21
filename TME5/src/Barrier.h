#pragma once

#include <mutex>
#include <condition_variable>
#include <iostream>

namespace pr {

class Barrier {
    std::mutex m;
    int compteur;
    int N;
    std::condition_variable cv;

public:
    Barrier(int N) : compteur(0), N(N) {}
    
    void waitFor() {
        std::unique_lock<std::mutex> l(m);
        while (compteur < N)
            cv.wait(l);
    }

    void done() {
        std::unique_lock<std::mutex> l(m);
        compteur++;
        //std::cout << compteur << "/" << N << std::endl;
        if (compteur == N) 
            cv.notify_all();
    }
};

}