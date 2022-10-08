#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>

#include <iterator>
#include <algorithm>
#include <utility>
#include <unordered_map>

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

int main () {
	using namespace std;
	using namespace std::chrono;
	ifstream input = ifstream("./WarAndPeace.txt");

	auto start = steady_clock::now();
	cout << "Parsing War and Peace" << endl;

	unordered_map<string, size_t> hmap;

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

		auto p = hmap.insert({word, 1});
		if (!p.second) 
			(*p.first).second++;

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
	cout << endl;

	// On devrait vérifier ici aussi qu'on a pas reçu un nullptr en valeur de retour
	cout << "Count of \"war\": " << (*hmap.find("war")).second << endl;
	cout << "Count of \"peace\": " << (*hmap.find("peace")).second << endl;
	
	cout << "Count of \"toto\": ";
	auto it = hmap.find("toto");
	if (it == hmap.end())
		cout << 0;
	else
		cout << (*it).second;
	cout << endl << endl;

	vector<std::pair<string, size_t>> ranks(hmap.begin(), hmap.end());
	std::sort(ranks.begin(), ranks.end(), 
		[] (const auto p, const auto q) {
			return p.second > q.second;
	});

	for (size_t i = 0; i < 10; i++) {
		cout << "Rank " << i + 1 << ": \"" << ranks[i].first;
		cout << "\" with " << ranks[i].second << " occurrences";
		cout << endl;
	}

	cout << endl;

	unordered_map<int, forward_list<string>> rev;
	for (auto it : hmap) {
		auto it_rev = rev.find(it.second);
		if (it_rev == rev.end()) {
			forward_list<string> fl;
			fl.push_front(it.first);
			rev.insert({it.second, fl});
		} else {
			(*it_rev).second.push_front(it.first);
		}
	}

	size_t freqs_N[] = {1, 23, 100, 1337};
	for (size_t i = 0; i < 4; i++) {
		cout << "Words with " << freqs_N[i] << " occurrences : " << endl;
		auto it_rev = rev.find(freqs_N[i]);
		if (it_rev != rev.end()) {
			for (auto it_fl : (*it_rev).second) {
				cout << " - " << it_fl << endl;
			}
		}
		cout << endl;
	}

    return 0;
}


