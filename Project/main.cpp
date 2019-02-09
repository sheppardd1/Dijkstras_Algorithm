#include "stdafx.h"		// for Visual Studio
#include <iostream>		// Provides cout
#include <cstdlib>		// Provides EXIT_SUCCESS
#include <string>		// provides strings
#include <limits>		// provides infinity value
#include <vector>		// provides vectors
#include <sstream>		// used for testing in is_float()
#include <iostream>		// in/out streams
#include <assert.h>		// provides assertions
using namespace std;   // Allows all standard library items to be used

// Constants:
const float INFTY = numeric_limits<float>::infinity();		//system's value of infinity

// Node struct
struct node
{
	vector <float> direct_distance;
	vector <float> dijkstra_distance;
	vector <string> dijkstra_path;
	string name;
};

// Prototypes:
size_t get_names(vector<string>&);
bool is_float(const string &input);
void get_distances(vector<node>&, const size_t, vector <string>&);
size_t get_min_unused_index(size_t, vector<bool>, vector<node>&, size_t);
void set_names(vector<node>&, vector<string>&, size_t);
void optimize(vector<node>&, const size_t);
void Dijkstra(vector<node>&, const size_t);



int main()
{
	vector <string> node_names;

	size_t count = get_names(node_names);

	vector <node> graph(count);

	set_names(graph, node_names, count);

	get_distances(graph, count, node_names);

	Dijkstra(graph, count);


	// wait for user to press ENTER before exiting
	cin.get();
	cin.get();
	return EXIT_SUCCESS;
}

size_t get_names(vector<string>& node_names) {
	size_t count = 0;
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

	return count;
}

void set_names(vector<node>& input_node, vector<string>& node_names, size_t number) {
	for (size_t i = 0; i < number; ++i) {
		input_node[i].name = node_names[i];
	}
}

void get_distances(vector<node>& input_node, const size_t count, vector <string>& node_names) {
	
	string current_distance;
	float current_distance_float;
	bool valid = true;

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
							input_node[i].direct_distance.push_back(current_distance_float);
							input_node[i].dijkstra_distance.push_back(current_distance_float);
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
						input_node[i].direct_distance.push_back(INFTY);
						input_node[i].dijkstra_distance.push_back(INFTY);
						valid = true;
					}
				} while (!valid);

			}//end if i != j
			else {
				input_node[i].direct_distance.push_back(0);
				input_node[i].dijkstra_distance.push_back(0);
			}
		}

	}
}

void optimize(vector<node>& input_node, const size_t count) {
	
	vector <bool> used(count,false);
	size_t num_unused, key;
	float sum;

	for (size_t i = 0; i < count; ++i) {			//loop for node variable as starting node(?)
		used[i] = true;
		num_unused = count-1;
		while (num_unused > 0) {
			key = get_min_unused_index(count, used, input_node, i);
			used[key] = true;
			for (size_t j = 0; j < count; ++j) {
				sum = input_node[i].dijkstra_distance[key] + input_node[key].dijkstra_distance[j];
				if (!used[j] && (sum < input_node[i].dijkstra_distance[j])){
					input_node[i].dijkstra_distance[j] = sum;
				}
			}
			--num_unused;
		}
		fill(used.begin(), used.end(), false);		// reset used array to all false, get ready to do run for next starting node
	}
}

size_t get_min_unused_index(size_t count, vector<bool> used, vector<node>& input_node, size_t starting_node) {

	float min[2] = { -1,INFTY };	//min[0] = index of min, min[1] = value of min
	for (size_t i = 0; i < count; ++i) {
		if (!used[i] && input_node[starting_node].dijkstra_distance[i] <= min[1]) {
			min[1] = input_node[starting_node].dijkstra_distance[i];
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

void Dijkstra(vector<node>& input_node, const size_t count) {
	
	optimize(input_node, count);
	cout << "\n\n----------------------------\nShortest Possible Distances:\n----------------------------\n";
	for (size_t i = 0; i < count; ++i) {
		for (size_t j = 0; j < count; ++j) {
			if (input_node[i].dijkstra_distance[j] < INFTY) {
				cout << " From \"" << input_node[i].name << "\" to \"" << input_node[j].name << "\": " << input_node[i].dijkstra_distance[j] << '\n';
			}
			else {
				cout << " From \"" << input_node[i].name << "\" to \"" << input_node[j].name << "\": Infinity\n";
			}
		}
	}
}