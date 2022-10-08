#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>

#include <iterator>
#include <algorithm>
#include <utility>

#include "HashMap.h"
#include "Vector.h"
#include "List.h"

template<class iterator_type>
size_t count(iterator_type begin, iterator_type end) {
	size_t n = 0;
	while (begin != end) {
		n++;
		++begin;
	}
	return n;
}

template<class iterator_type, typename T>
size_t count_if_equal(iterator_type begin, iterator_type end, const T& val) {
	size_t n = 0;
	while (begin != end) {
		if (*begin == val)
			n++;
		++begin;
	}
	return n;
}

int main () {/*
	pr::Vector<int> v;
	v.push_back(101);
	v.push_back(90);

	for (auto it : v) {
		std::cout << it << std::endl;
	}

	pr::List<int> l;
	l.push_front(10);
	l.push_front(444);
	l.push_front(1337);
	
	for (pr::List<int>::iterator it = l.begin(); it != l.end(); it++) {
		std::cout << *it << std::endl;
	}

	pr::HashMap<int, std::string> h;
	for (size_t i = 0; i < 1000; i++) {
		h.put(i, "test");
	}//h.put("test2", -2);

	for (auto it : h) {
		std::cout << it.key_ << std::endl;
	}
	std::cout << count(h.begin(), h.end()) << std::endl;
*/

	using namespace std;
	using namespace std::chrono;
	ifstream input = ifstream("./WarAndPeace.txt");

	auto start = steady_clock::now();
	cout << "Parsing War and Peace" << endl;

	pr::HashMap<std::string, size_t> hmap;

	size_t nombre_lu = 0;
	// prochain mot lu
	string word;
	// une regex qui reconnait les caractères anormaux (négation des lettres)
	regex re( R"([^a-zA-Z])");
	while (input >> word) {
		// élimine la ponctuation et les caractères spéciaux
		word = regex_replace ( word, re, "");
		// passe en lowercase
		transform(word.begin(),word.end(),word.begin(),::tolower);

		size_t *old = hmap.get(word);
		if (old == nullptr) 
			hmap.put(word, 1);
		else 
			hmap.put(word, *old + 1);

		// word est maintenant "tout propre"
		if (nombre_lu % 10000 == 0)
			// on affiche un mot "propre" sur 100
			cout << nombre_lu << ": "<< word << endl;
		nombre_lu++;
	}
	input.close();

	cout << "Finished Parsing War and Peace" << endl;

	auto end = steady_clock::now();
    cout << "Parsing took "
              << duration_cast<milliseconds>(end - start).count()
              << "ms.\n";

    cout << "Found a total of " << count(hmap.begin(), hmap.end()) << " words." << endl;

	// On devrait vérifier ici aussi qu'on a pas reçu un nullptr en valeur de retour
	cout << "Count of \"war\": " << *hmap.get("war") << endl;
	cout << "Count of \"peace\": " << *hmap.get("peace") << endl;
	
	cout << "Count of \"toto\": ";
	size_t *toto_count = hmap.get("toto");
	if (toto_count == nullptr)
		cout << 0;
	else
		cout << *toto_count;
	cout << endl;

	vector<pr::HashMap<string, size_t>::Entry> ranks(hmap.begin(), hmap.end());
	std::sort(ranks.begin(), ranks.end(), 
		[] (const auto p, const auto q) {
			return p.value_ > q.value_;
	});

	for (size_t i = 0; i < 10; i++) {
		cout << "Rank " << i + 1 << ": \"" << ranks[i].key_;
		cout << "\" with " << ranks[i].value_ << " occurrences";
		cout << endl;
	}

    return 0;
}


