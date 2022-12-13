#pragma once

#include <cstring> // size_t,memset
#include <semaphore.h>
#include <mutex>
#include <iostream>

namespace pr {

#define STACKSIZE 100

template<typename T>
class Stack {
	T tab [STACKSIZE];
	size_t sz;
	sem_t empty_sm;
	sem_t full_sm;
	mutable std::mutex m;

public :
	Stack () : sz(0) { 
		sem_init(&empty_sm, 1, 0);
		sem_init(&full_sm, 1, STACKSIZE);
		memset(tab,0,sizeof tab);
	}

	T pop () {
		// bloquer si vide
		sem_wait(&empty_sm);
		m.lock();
		T toret = tab[--sz];
		std::cout << sz << std::endl;
		m.unlock();
		sem_post(&full_sm);
		return toret;
	}

	void push(T elt) {
		//bloquer si plein
		sem_wait(&full_sm);
		m.lock();
		tab[sz++] = elt;
		std::cout << sz << std::endl;
		m.unlock();
		sem_post(&empty_sm);
	}
};

}
