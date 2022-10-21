#ifndef SRC_QUEUE_H_
#define SRC_QUEUE_H_

#include <cstring>
#include <cstdlib>
#include <mutex>
#include <condition_variable>
#include <iostream>

namespace pr {

// MT safe version of the Queue, non blocking.
template <typename T>
class Queue {
	T ** tab;
	const size_t allocsize;
	size_t begin;
	size_t sz;
	mutable std::mutex m;
	std::condition_variable cv;
	bool block;

	// fonctions private, sans protection mutex
	bool empty() const {
		return sz == 0;
	}
	bool full() const {
		return sz == allocsize;
	}
public:
	Queue(size_t size) :allocsize(size), begin(0), sz(0), block(true) {
		tab = new T*[size];
		memset(tab, 0, size * sizeof(T*));
	}
	size_t size() const {
		std::unique_lock<std::mutex> lg(m);
		return sz;
	}
	T* pop() {
		std::unique_lock<std::mutex> lg(m);
		while (empty() && block) {
			cv.wait(lg);
		}
		if (empty() && !block)
			return nullptr;
		if (full()) {
			cv.notify_all();
		}
		auto ret = tab[begin];
		tab[begin] = nullptr;
		sz--;
		begin = (begin + 1) % allocsize;
		return ret;
	}
	bool push(T* elt) {
		std::unique_lock<std::mutex> lg(m);
		while (full() && block) {
			cv.wait(lg);
		}
		if (empty()) {
			// On voit que buffer est vide, et donc
			// que potentielle des threads sont en train de wait
			// en attendant qu'elle se remplisse
			// On les notifies maintenant, sachant que l'on conserve
			// le lock jusqu'à la fin de la fonction ils ne pourront
			// pas reprendre leurs locks avant
			// ATTENTION: en utilisant un notify_all dans un if
			// 		on va réveiller tout le monde et la condition
			// 		ne sera pas reverifiée, on va donc potentiellement
			//		push dans une pile pleine (cf. Spurious wakeup)
			// ATTENTION 2: notify_one ne va pas fonctionner non plus
			//		si on en réveille un, le pop va réussir pour le premier
			//		mais le deuxième va stall 
			cv.notify_all();
		}
		tab[(begin + sz) % allocsize] = elt;
		sz++;
		return true;
	}
	void setBlocking(bool isBlocking) {
		std::unique_lock<std::mutex> lg(m);
		block = isBlocking;
		cv.notify_all();
	}
	~Queue() {
		// ?? lock a priori inutile, ne pas detruire si on travaille encore avec
		for (size_t i = 0; i < sz; i++) {
			auto ind = (begin + i) % allocsize;
			delete tab[ind];
		}
		delete[] tab;
	}
};

}

#endif /* SRC_QUEUE_H_ */
