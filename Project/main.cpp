

#include "stdafx.h"
#include <iostream>    // Provides cout
#include <cstdlib>     // Provides EXIT_SUCCESS
#include <string>
#include <map>
#include <limits>
#include <vector>
#include <sstream>
#include <iostream>
#include <assert.h>
using namespace std;   // Allows all standard library items to be used

// Constants:
const float INFTY = numeric_limits<float>::infinity();

// Prototypes:
void get_names(vector<string>&, size_t&);
bool is_float(const string &input);
void init_distances(vector<map<size_t, float>>&, const size_t );
void get_distances(vector<map<size_t, float>>&, vector<map<size_t, float>>&, const size_t, vector <string>&);
size_t get_min_unused_index(size_t, vector<bool>, vector<map<size_t, float>>&);
void optimize(vector<map<size_t, float>>&, vector<map<size_t, float>>&, const size_t, vector <string>&);
void Dijkstra(vector<map<size_t, float>>& distance, vector<map<size_t, float>>& optimized_distance, const size_t count, vector <string>& node_names);

int main()
{

	vector <string> node_names;
	size_t count = 0;
	// If distance from node A to C is 3, then distance[0] will contain a map with the points (2,3) because node C is represented by 2, the distance is 3, and the start is A which is representd by 0.

	get_names(node_names, count);

	cout << "Total number of nodes: " << count << endl;

	vector<map<size_t, float> > distance(count);			//user-inputted direct distances between nodes
	vector<map<size_t, float> > optimized_distance(count);

	get_distances(distance, optimized_distance, count, node_names);

	Dijkstra(distance, optimized_distance, count, node_names);

	// wait for user to press ENTER before exiting
	cin.get();
	cin.get();
	return EXIT_SUCCESS;
}

void get_names(vector<string>& node_names, size_t& count) {
	string name;
	cout << "Enter \"done\" to finish entering node names.\n";
	do {
		cout << "Node " << count + 1 << ": ";
		getline(cin, name);
		if (name != "done") {
			node_names.push_back(name);
			++count;
		}
	} while (name != "done");

	cout << "Node Names: \n";

	for (size_t i = 0; i < count; ++i) {
		cout << ' ' << node_names[i] << endl;
	}
}

void init_distances(vector<map<size_t, float>>& distance, const size_t count) {
	// essentially creating a square matrix (like multiplication table)
	for (size_t i = 0; i < count; ++i) {
		for (size_t j = 0; j < count; ++j) {
			if (i == j) {
				distance[i].insert(pair<size_t, float>(j, 0));	// init distance from node to itself to 0
			}
			else {
				distance[i].insert(pair<size_t, float>(j,INFTY));	// init all distance combinations to infty, except when the node matches with itself (i.e. distance from node to itself = 0)
			}
		}
	}
}

//initialize distance vector maps
void get_distances(vector<map<size_t, float>>& distance, vector<map<size_t, float>>& optimized_distance, const size_t count, vector <string>& node_names) {
	
	string current_distance;
	float current_distance_float;
	bool valid = true;

	init_distances(distance, count);
	//init_distances(optimized_distance, count);


	for (size_t i = 0; i < count; ++i) {
		for (size_t j = 0; j < count; ++j) {
			if (i != j) {	//distance between a node and itself is already initialized to 0
				do {
					cout << "Enter the direct distance between node \"" << node_names[i] << "\" and node \"" << node_names[j] << "\": ";
					cin >> current_distance; //enty "inf" or "infinity" for no connection
					//check for validity
					if (is_float(current_distance)) {
						current_distance_float = stof(current_distance);
						distance[i].insert(pair<size_t, float>(j, current_distance_float));
						valid = true;
					}
					else if (current_distance != "infinity" && current_distance != "inf") {
						valid = false;
						cout << "Not a valid value. Enter a numerical value or \'inf\' or \'infinity\' for no connection\n";
					}
					//else distance = infinity which is the already-initialized value, so do nothing for that
				} while (!valid);
			}
			
		}
	}
}

void optimize(vector<map<size_t, float>>& distance, vector<map<size_t, float>>& optimized_distance, const size_t count, vector <string>& node_names) {
	map<size_t, float>::iterator itr;
	optimized_distance = distance;
	vector <bool> used(count,false);
	size_t num_unused, key;
	num_unused = count;

	for (size_t i = 0; i < count; ++i) {
		used[i] = true;
		--num_unused;
		while (num_unused > 0) {
			key = get_min_unused_index(count, used, optimized_distance);
			used[key] = true;
			for (size_t j = 0; j < count; ++j) {
				float sum = optimized_distance[i].find(key)->second + optimized_distance[key].find(j)->second;
				if (!used[j] && (sum < optimized_distance[i].find(j)->second)) {
					optimized_distance[i].insert(pair <size_t, float>(j, sum));
				}
			}
			--num_unused;
		}
		used = { false };	//reset, get ready to do again for next starting node
	}
}

size_t get_min_unused_index(size_t count, vector<bool> used, vector<map<size_t, float>>& optimized_distance) {
	float min[2] = { -1,INFTY };	//min[0] = key of min, min[1] = value of min
	for (size_t i = 0; i < count; ++i) {
		if (!used[i] && optimized_distance[i].find(i)->second < min[1]) {
			min[1] = optimized_distance[i].find(i)->second;
			min[0] = i;
		}	//if there are multiple instances of the min distance, it doesn't matter which is used next
	}
	assert(min[0] >= 0);	//assert that this function has actually done something
	return min[0];
}


bool is_float( const string &input) {
// This function was taken from: https://www.quora.com/How-can-I-check-if-a-std-string-is-a-floating-point-number-in-C++
// Written by Richard Liu

	stringstream sstr(input);
	float new_float;
	return !((sstr >> noskipws >> new_float).rdstate() ^ ios_base::eofbit);
} 

void Dijkstra(vector<map<size_t, float>>& distance, vector<map<size_t, float>>& optimized_distance, const size_t count, vector <string>& node_names) {
	
	optimize(distance, optimized_distance, count, node_names);
	cout << "Final results: \n";
	for (size_t i = 0; i < count; ++i) {
		for (size_t j = 0; j < count; ++j) {
			cout << " From \"" << node_names[i] << "\' to \"" << node_names[j] << "'\": " << optimized_distance[i].find(j)->second << '\n';
		}
	}
}