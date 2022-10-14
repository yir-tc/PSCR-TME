#include "Banque.h"

#include <iostream>

#define K 10
#define SOLDEINITIAL 1000

using namespace std;
const int NB_THREAD = 10;

void work(pr::Banque& b, int n) {
	for (size_t _ = 0; _ < 1000; _++) {
		int i = ::rand() % K;
		int j = ::rand() % K;
		int m = 1 + (::rand() % 100);
		b.transfert(i, j, m);

		int s = ::rand() % 21;
		std::this_thread::sleep_for(std::chrono::milliseconds(s));
	}
	std::cout << "thread " << n << " died" << std::endl;
}

void watch(pr::Banque& b) {
	while (1) {
		b.comptabiliser(NB_THREAD * SOLDEINITIAL);
	}
}

int main () {
	::srand(::time(nullptr));

	pr::Banque b(K, SOLDEINITIAL);
	vector<thread> threads;

	// TODO : creer des threads qui font ce qui est demandé
	for (size_t i = 0; i < NB_THREAD; i++) {
		threads.emplace_back(work, std::ref(b), i);
	}
	//threads.emplace_back(watch, std::ref(b));
	thread watcher(watch, std::ref(b));

	for (auto & t : threads) {
		t.join();
	}
	
	watcher.detach();
	
	// TODO : tester solde = NB_THREAD * JP
	b.comptabiliser(NB_THREAD * SOLDEINITIAL);
	return 0;
}
