#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>

#include <algorithm>
#include <utility>

int main () {
	using namespace std;
	using namespace std::chrono;
	ifstream input = ifstream("./WarAndPeace.txt");

	auto start = steady_clock::now();
	cout << "Parsing War and Peace" << endl;

	vector<std::pair<std::string, int>> occurrences;

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

		auto it = find_if(
			occurrences.begin(), occurrences.end(), 
			[word](auto e) { 
				return e.first == word; 
		});
		if (it == occurrences.end()) {
			occurrences.push_back(make_pair(word, 1));
		} else {
			(*it).second++;
		}

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

    cout << "Found a total of " << nombre_lu << " words." << endl;

	cout << "Count of \"war\": " << (*find_if(occurrences.begin(), occurrences.end(), 
		[](auto e) { return e.first == "war"; })).second << endl;
	cout << "Count of \"peace\": " << (*find_if(occurrences.begin(), occurrences.end(), 
		[](auto e) { return e.first == "peace"; })).second << endl;
	cout << "Count of \"toto\": " << (*find_if(occurrences.begin(), occurrences.end(), 
		[](auto e) { return e.first == "toto"; })).second << endl;
	
    return 0;
}


