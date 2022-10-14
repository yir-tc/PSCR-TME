#include "Banque.h"

#include <iostream>

using namespace std;

namespace pr {

void Banque::transfert(size_t deb, size_t cred, unsigned int val) {
	unique_lock<mutex> l(m);

	Compte & debiteur = comptes[deb];
	Compte & crediteur = comptes[cred];

	unique_lock<recursive_mutex> lb(debiteur.getMutex(), defer_lock);
	unique_lock<recursive_mutex> lc(crediteur.getMutex(), defer_lock);

	if (deb < cred) {
		lb.lock();
		lc.lock();
	} else {
		//std::cout << "locking " << cred << std::endl;
		lc.lock();
		//std::cout << "locking " << deb << std::endl;
		lb.lock();
	}

	// On va lock encore dans debiter/crediter donc il faut un lock recursif
	if (debiteur.debiter(val)) {
		crediteur.crediter(val);
	}

	//std::cout << "unlocking " << deb << std::endl;
	//std::cout << "unlocking " << deb << std::endl;
}

size_t Banque::size() const {
	return comptes.size();
}
bool Banque::comptabiliser (int attendu) {
	int bilan = 0;
	int id = 0;
	for (auto & compte : comptes) {
		//std::cout << "locking " << id << std::endl;
		compte.getMutex().lock();
		if (compte.getSolde() < 0) {
			cout << "Compte " << id << " en négatif : " << compte.getSolde() << endl;
		}
		bilan += compte.getSolde();
		id++;
	}
	// for (auto & compte : comptes) {
	for (size_t i = 0; i < size(); i++) {
		//std::cout << "unlocking " << i << std::endl;
		auto& compte = comptes[i];
		compte.getMutex().unlock();
	}
	if (bilan != attendu) {
		cout << "Bilan comptable faux : attendu " << attendu << " obtenu : " << bilan << endl;
	}
	return bilan == attendu;
}
}
