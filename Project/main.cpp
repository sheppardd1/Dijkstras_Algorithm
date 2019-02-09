#include "stdafx.h"		// for Visual Studio
#include <iostream>		// Provides cout
#include <cstdlib>		// Provides EXIT_SUCCESS
#include <string>		// provides strings
#include <map>			// provides maps
#include <limits>		// provides infinity value
#include <vector>		// provides vectors
#include <sstream>		// used for testing in is_float()
#include <iostream>		// in/out streams
#include <assert.h>		// provides assertions
using namespace std;   // Allows all standard library items to be used

// Constants:
const float INFTY = numeric_limits<float>::infinity();		//system's value of infinity

// Prototypes:
void get_names(vector<string>&, size_t&);
bool is_float(const string &input);
void get_distances(vector<map<size_t, float>>&, vector<map<size_t, float>>&, const size_t, vector <string>&);
size_t get_min_unused_index(size_t, vector<bool>, vector<map<size_t, float>>&, size_t);
void optimize(vector<map<size_t, float>>&, vector<map<size_t, float>>&, const size_t, vector <string>&);
void Dijkstra(vector<map<size_t, float>>&, vector<map<size_t, float>>&, const size_t, vector <string>&);

int main()
{
	vector <string> node_names;
	size_t count = 0;
	// If distance from node A to C is 3, then distance[0] will contain a map with the points (2,3) because node C is represented by 2, the distance is 3, and the start is A which is representd by 0.

	get_names(node_names, count);

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
	bool valid = true;
	cout << "Enter the names of the nodes.\nType \"done\" to finish entering node names.\n";
	do {
		if (valid) {	//don't print if user only entered '\n' so far
			cout << "Node " << count + 1 << ": ";
		}
		getline(cin, name);
		if (name != "done" && name != "") {
			node_names.push_back(name);
			++count;
			valid = true;
		}
		else if (name == "") {
			valid = false;
		}
	} while (name != "done");

	cout << "Node Names: \n";

	for (size_t i = 0; i < count; ++i) {
		cout << ' ' << node_names[i] << endl;
	}

	cout << "Total number of nodes: " << count << endl;
}


//initialize distance vector maps
void get_distances(vector<map<size_t, float>>& distance, vector<map<size_t, float>>& optimized_distance, const size_t count, vector <string>& node_names) {
	
	string current_distance;
	float current_distance_float;
	bool valid = true;
	map<size_t, float> tempMap;

	cout << "\nEnter the direct distances between the nodes\n(type \"i\" or \"infinity\" for no connection)\n\n";
	for (size_t i = 0; i < count; ++i) {
		for (size_t j = 0; j < count; ++j) {
			if (i != j) {	//distance between a node and itself is 0
				do {
					cout << " \"" << node_names[i] << "\" and \"" << node_names[j] << "\": ";
					cin >> current_distance; //enty "i" or "infinity" for no connection
					//check for validity
					if (is_float(current_distance)) {
						current_distance_float = stof(current_distance);
						if (current_distance_float >= 0) {
							tempMap.insert(pair<size_t, float>(j, current_distance_float));
							valid = true;
						}
						else {
							valid = false;
							cout << "ERROR - Value cannot be negative. Try again.\n";
						}
					}
					else if (current_distance != "infinity" && current_distance != "i" && current_distance != "undo") {
						valid = false;
						cout << "ERROR - Not a valid value. Enter a numerical value or \'i\' or \'infinity\' for no connection. Try again.\n";
					}
					else if(current_distance != "undo") {
						tempMap.insert(pair<size_t, float>(j, INFTY));
						valid = true;
					}
					//else distance = infinity which is the already-initialized value, so do nothing for that
				} while (!valid);

				/*if (current_distance == "undo" && j != 0) {
					--j;
				}
				else if (current_distance == "undo")
				{
					j = count-1;
					--i;
				}*/
			}//end if i != j
			else {
				tempMap.insert(pair<size_t, float>(j, 0));
			}
		}
		distance[i] = tempMap;
		tempMap.clear();
	}
}

void optimize(vector<map<size_t, float>>& distance, vector<map<size_t, float>>& optimized_distance, const size_t count, vector <string>& node_names) {
	optimized_distance = distance;
	vector <bool> used(count,false);
	size_t num_unused, key;

	for (size_t i = 0; i < count; ++i) {			//loop for node variable as starting node(?)
		used[i] = true;
		num_unused = count-1;
		while (num_unused > 0) {
			key = get_min_unused_index(count, used, optimized_distance, i);
			used[key] = true;
			for (size_t j = 0; j < count; ++j) {
				float sum = optimized_distance[i].find(key)->second + optimized_distance[key].find(j)->second;
				if (!used[j] && (sum < optimized_distance[i].find(j)->second)) {
					optimized_distance[i].erase(j);
					optimized_distance[i].insert(pair <size_t, float>(j, sum));
				}
			}
			--num_unused;
		}
		fill(used.begin(), used.end(), false);		// reset used array to all false, get ready to do run for next starting node
	}
}

size_t get_min_unused_index(size_t count, vector<bool> used, vector<map<size_t, float>>& optimized_distance, size_t starting_node) {

	map<size_t, float> tempMap;

	float min[2] = { -1,INFTY };	//min[0] = key of min, min[1] = value of min
	for (size_t i = 0; i < count; ++i) {
		tempMap = optimized_distance[starting_node];
		//cout << endl << tempMap.find(i)->second << endl << min[1] << endl << endl;
		if (!used[i] && tempMap.find(i)->second <= min[1]) { //THIS KEEPS THROWING AN EXCEPTION!!!!!!!!!!!!!!
			min[1] = tempMap.find(i)->second;
			min[0] = i;
		}	//if there are multiple instances of the min distance, it doesn't matter which is used next
	}
	assert(min[0] >= 0);	//assert that this function has actually done something
	return min[0];
}


bool is_float(const string &input) {
// This function was taken from: 
// https://www.quora.com/How-can-I-check-if-a-std-string-is-a-floating-point-number-in-C++
// Written by Richard Liu

	stringstream sstr(input);
	float new_float;
	return !((sstr >> noskipws >> new_float).rdstate() ^ ios_base::eofbit);
} 

void Dijkstra(vector<map<size_t, float>>& distance, vector<map<size_t, float>>& optimized_distance, const size_t count, vector <string>& node_names) {
	
	optimize(distance, optimized_distance, count, node_names);
	cout << "\n\n----------------------------\nShortest Possible Distances:\n----------------------------\n";
	for (size_t i = 0; i < count; ++i) {
		for (size_t j = 0; j < count; ++j) {
			if (optimized_distance[i].find(j)->second < INFTY) {
				cout << " From \"" << node_names[i] << "\" to \"" << node_names[j] << "\": " << optimized_distance[i].find(j)->second << '\n';
			}
			else {
				cout << " From \"" << node_names[i] << "\" to \"" << node_names[j] << "\": Infinity\n";
			}
		}
	}
}