/*
	Dijkstra's Algorithm
	Implemented by David Sheppard
	Created: 2/5/2019
	Last Updated: 2/12/2019 (improved UI)
	Purpose: Finds the shortest distance between to nodes on a graph
		and prints out the optimized distances and paths
*/


#include <iostream>		// Provides cout
#include <cstdlib>		// Provides EXIT_SUCCESS
#include <string>		// provides strings
#include <limits>		// provides infinity value
#include <vector>		// provides vectors
#include <sstream>		// used for testing in is_float()
#include <iostream>		// in/out streams
#include <assert.h>		// provides assertions
using namespace std;	// Allows all standard library items to be used

// Constants:
const float INFTY = numeric_limits<float>::infinity();		//system's value of infinity

// Node struct - represents a node on a graph
struct node
{
	vector <float> direct_distance;		// direct_distance[i] is direct distance from current node to node i
	vector <float> dijkstra_distance;	// dijkstra_distance[i] is shortest distance from current node to node i
	vector <string> dijkstra_path;		// dijkstra_path[i] is the shortest path from current node to node i (as a string)
	string name;						// name of the node
};

// Prototypes:
size_t get_names(vector<string>&);											// user inputs names of each node and returns number of nodes entered
bool is_float(const string &input);											// determines if a number can be converted to a float (credit: Richard Liu) 
void get_distances(vector<node>&, const size_t);							// lets user choose if graph is one or two way and calls appropriate distance function
void get_directed_distances(vector<node>&, const size_t);					// lets user input one-way distance between each node (both directions)
void get_undirected_distances(vector<node>&, const size_t);					// lets user input two-way distance between each node
size_t get_min_unused_index(size_t, vector<bool>, vector<node>&, size_t);	// determines which node to focus on for next iteration of Dijkstra's algorithm
void set_names(vector<node>&, vector<string>&, size_t);						// assigns the user-inputted name to each node
void optimize(vector<node>&, const size_t);									// performs Dijstra's algorithm
void print_dijkstra(vector<node>&, const size_t);							// calls optimize(...) and prints results of optimization
void init_paths(vector<node>&, size_t);										// initializes the starting values of each node's paths
void finish_paths(vector<node>&, size_t);									// puts ending node name in each node's path
bool restart_question();													// asks user if they want to run the program again or quit


int main()
{
	do {											// rum program until user says to stop
		vector <string> node_names;					// create vector of node names

		size_t count = get_names(node_names);		// get node names and return number of nodes

		vector <node> graph(count);					// create graph (a vector of nodes)
			// example of syntax: node_name[2].direct_distance[3] is the distance from the 3rd node to the 4th node

		set_names(graph, node_names, count);		// put user-inputted names into the nodes

		get_distances(graph, count);				// get distances between each node from user

		print_dijkstra(graph, count);				// print optimized results

	} while (restart_question());					// ask user if they want to start over

	return EXIT_SUCCESS;							// finish
}

size_t get_names(vector<string>& node_names) {
	// purpose: get names of each vector from user
	// utilizes: vector of string to put names into, called by reference

	size_t count = 0;							//number of nodes that are created
	string name;								// name of current node
	bool valid = true;							// used to ensure that user enters something other than endline

	// print directions:
	cout << "Enter the names of the nodes.\nType \"done\" to finish entering node names.\n";
	
	// Get names
	do {
		if (valid) {	//don't print if user only entered '\n' so far
			cout << "Node " << count + 1 << ": ";	// print number of node we are asking for
		}
		getline(cin, name);							// get name from user
		if (name != "done" && name != "") {			// if name is given and user isn't done entering names
			node_names.push_back(name);				// add name to vector of names
			++count;								// increment number of nodes as long as valid name is entered
			valid = true;							// valid is true
		}
		else if (name == "") {						// if user just hit ENTER, do nothing
			valid = false;							// invalid = true, so keep waiting for same node to be entered
		}
	} while (name != "done");						// keep getting names until user enters "done"

	// print entered names:
	cout << "Node Names: \n";
	for (size_t i = 0; i < count; ++i) {
		cout << ' ' << node_names[i];
		if (i != count - 1) {						// if not printing last node name, put comma
			cout << ", ";
		}
		else {										// else if printing last node, put endl
			cout << endl;
		}
	}
	
	// print number of nodes entered:
	cout << "Total number of nodes: " << count << endl << endl;

	return count;									// return number of nodes
}

void set_names(vector<node>& input_node, vector<string>& node_names, size_t number) {
	// purpose: takes user-inputted names from vector of strings and puts them into correct nodes
	// utilizes: vector of nodes as destinations, vector of strings as source, size_t as number of names (and thus # of nodes)

	for (size_t i = 0; i < number; ++i) {			// iterate thru each node
		input_node[i].name = node_names[i];			// put user-entered names into appropriate nodes
	}
}

void get_distances(vector<node>& input_node, const size_t count) {
	// purpose: asks user if they want a digraph or undirected graph and calls appropriate function
	// utilizes: vector of nodes as input graph and size_t value as graph size (number of nodes)
	char option;											// answer entered by user
	bool valid = true;										// true iif user enteres valid option: U, u, D, d
	do {													// ask while user has not entered a valid option
		// give directions:
		if (valid) {										// only give regular directions on first time
			cout << "Do you want to create a digraph (one-way) or an undirected graph (two-way)?";
			cout << "\nEnter \'D\' for Digraph or \'U\' for Undirected graph: ";
		}
		cin >> option;										// get user's input
		if (option == 'D' || option == 'd') {				// check if digraph
			get_directed_distances(input_node, count);		// if digraph, need directed distances
			valid = true;									// valid answer
		}
		else if (option == 'U' || option == 'u') {			// check if undirected graph
			get_undirected_distances(input_node, count);	// if undirected, only need one set of directions per combination
			valid = true;									// valid answer
		}
		else {												// if invalid answer, say so
			cout << "Invalid option. Enter either \'U\' or \'D\': ";	// print error with directions
			valid = false;									// need to repeat question
		}
	} while (!valid);										// run until valid answer is given
}

void get_directed_distances(vector<node>& input_node, const size_t count) {
	// purpose: gest direct (one-way) distances between nodes from user for digraph
	// utilizes: vector of nodes as destination for distances and source of names, size_t as number of nodes

	string current_distance;						// distance between 2 nodes as a string
	float current_distance_float;					// distance between 2 nodes as a float
	bool valid = true;								// true when users enters valid distance

	// print directions
	cout << "\nEnter the direct distances between the nodes\n(type \"i\" or \"infinity\" for no connection)\n\n";

	// get distances
	for (size_t i = 0; i < count; ++i) {			// iterate for each node as starting node	
		for (size_t j = 0; j < count; ++j) {		// iterate for each node as ending node
			if (i != j) {							//distance between a node and itself is 0
				do {
					cout << " \"" << input_node[i].name << "\" to \"" << input_node[j].name << "\": ";	// print start and end node that we are asking distance for
					cin >> current_distance;													// get distance from user (enter "i" or "infinity" for no connection)
					if (is_float(current_distance)) {											// check for validity: if it can be converted to float, it is valid
						current_distance_float = stof(current_distance);						// do actual conversion of string to float
						if (current_distance_float >= 0) {										// ensure distance is positive
							input_node[i].direct_distance.push_back(current_distance_float);	// put validated distance into node
							input_node[i].dijkstra_distance.push_back(current_distance_float);	// also put validated distance into optimized distance vector for now
							valid = true;														// input was valid
						}
						else {																	// if input was negative
							valid = false;														// invalid if negative
							cout << "ERROR - Value cannot be negative. Try again.\n";			// print erro message
						}
					}
					else if (current_distance != "infinity" && current_distance != "i") {		// if non-float distance is not "i" or "infinity" it is bad
						valid = false;															// invalid
						// print error:
						cout << "ERROR - Not a valid value. Enter a numerical value or \'i\' or \'infinity\' for no connection. Try again.\n";
					}
					else {																		// else, entered distance is infinity
						input_node[i].direct_distance.push_back(INFTY);							// set validated distance equal to system's representation of infinity
						input_node[i].dijkstra_distance.push_back(INFTY);						// also put validated distance equal to system's representation of infinity in optimized list
						valid = true;															// value was valid
					}
				} while (!valid);	//keep asking for distance until valid one is entered

			}			//end if i != j
			else {		// if i == j
				input_node[i].direct_distance.push_back(0);		//distance between a node and itself is 0
				input_node[i].dijkstra_distance.push_back(0);	//distance between a node and itself is 0
			}
		}		// end lloping thru destination node
	}		// end looping thru source node
}

void get_undirected_distances(vector<node>& input_node, const size_t count) {
	// purpose: gest direct two-way distances between nodes from user for undirected graph
	// utilizes: vector of nodes as destination for distances and source of names, size_t as number of nodes

	string current_distance;						// distance between 2 nodes as a string
	float current_distance_float;					// distance between 2 nodes as a float
	bool valid = true;								// true when users enters valid distance

													// print directions
	cout << "\nEnter the direct distances between the nodes\n(type \"i\" or \"infinity\" for no connection)\n\n";

	// get distances
	for (size_t i = 0; i < count; ++i) {			// iterate for each node as starting node	
		for (size_t j = 0; j < count; ++j) {		// iterate for each node as ending node
			if (i < j) {							//distance between a node and itself is 0
				do {
					cout << " \"" << input_node[i].name << "\" and \"" << input_node[j].name << "\": ";	// print start and end node that we are asking distance for
					cin >> current_distance;													// get distance from user (enter "i" or "infinity" for no connection)
					if (is_float(current_distance)) {											// check for validity: if it can be converted to float, it is valid
						current_distance_float = stof(current_distance);						// do actual conversion of string to float
						if (current_distance_float >= 0) {										// ensure distance is positive
							input_node[i].direct_distance.push_back(current_distance_float);	// put validated distance into node
							input_node[i].dijkstra_distance.push_back(current_distance_float);	// also put validated distance into optimized distance vector for now
							valid = true;														// input was valid
						}
						else {																	// if input was negative
							valid = false;														// invalid if negative
							cout << "ERROR - Value cannot be negative. Try again.\n";			// print erro message
						}
					}
					else if (current_distance != "infinity" && current_distance != "i") {		// if non-float distance is not "i" or "infinity" it is bad
						valid = false;															// invalid
																								// print error:
						cout << "ERROR - Not a valid value. Enter a numerical value or \'i\' or \'infinity\' for no connection. Try again.\n";
					}
					else {																		// else, entered distance is infinity
						input_node[i].direct_distance.push_back(INFTY);							// set validated distance equal to system's representation of infinity
						input_node[i].dijkstra_distance.push_back(INFTY);						// also put validated distance equal to system's representation of infinity in optimized list
						valid = true;															// value was valid
					}
				} while (!valid);	//keep asking for distance until valid one is entered

			}			//end if i != j
			else if (i > j) {	//distance from i to j can be set to already-defined distance from j to i
				input_node[i].direct_distance.push_back(input_node[j].direct_distance[i]);		// put validated distance into node
				input_node[i].dijkstra_distance.push_back(input_node[j].dijkstra_distance[i]);	// also put validated distance into optimized distance vector for now
			}
			else {		// if i == j
				input_node[i].direct_distance.push_back(0);		//distance between a node and itself is 0
				input_node[i].dijkstra_distance.push_back(0);	//distance between a node and itself is 0
			}
		}		// end lloping thru destination node
	}		// end looping thru source node
}

void print_dijkstra(vector<node>& input_node, const size_t count) {
	// purpose: calls optimizing function and then prints result of optimized distances and paths
	// utilizes: vector of nodes as source of names and input to optimization function, size_t for number of nodes
	
	optimize(input_node, count);								// run thru dijkstra's alogrithm
	// print header for results
	cout << "\n\n----------------------------\nShortest Possible Distances:\n----------------------------\n";
	for (size_t i = 0; i < count; ++i) {						// loop through each source
		for (size_t j = 0; j < count; ++j) {					// loop thru each destination
			if (input_node[i].dijkstra_distance[j] < INFTY) {	// print non-infinite distance
				// print value of shortest distance value:
				cout << " From \"" << input_node[i].name << "\" to \"" << input_node[j].name << "\": " << input_node[i].dijkstra_distance[j] << '\n';
				// print path corresponsing to shortest distance:
				cout << "     Path: " << input_node[i].dijkstra_path[j] << endl;
			}
			else {
				// print that distance is infinite:
				cout << " From \"" << input_node[i].name << "\" to \"" << input_node[j].name << "\": Infinity\n";
				// print that path is Not Applicable sice distance is infinite:
				cout << "     Path: NA\n";
			}
		}
		cout << endl;
	}
}

void optimize(vector<node>& input_node, const size_t count) {
	// purpose: performs dijkstra's algorithm to find shortest distance and path from each source to each destination
	// utilizes: vector of nodes as source of graph that needs to be optimized, size_t for number of nodes in graph

	vector <bool> used(count, false);									// keeps track of which nodes have been optimized so far as destinations
	size_t num_unused, index;											// number of nodes that have not been used as source yet, index of smallest distance value that hasn't beeen used yet
	float sum;															// distance from source node to node[index] + distance from node[index] to destination node

	init_paths(input_node, count);										// put each node as its own starting node it its own path string

	for (size_t i = 0; i < count; ++i) {								// i is the starting node
		used[i] = true;													// node[i] is now used
		num_unused = count - 1;											// decrement number of nodes that are used so far
		while (num_unused > 0) {										// while there are still unused nodes left
			index = get_min_unused_index(count, used, input_node, i);	// find out smallest distance that isn't used yet
			used[index] = true;											// this node is now used
			for (size_t j = 0; j < count; ++j) {						// j is the destination node
				// sum = distance from source node to index node + distance from index node to destination node
				sum = input_node[i].dijkstra_distance[index] + input_node[index].dijkstra_distance[j];
				if (!used[j] && (sum < input_node[i].dijkstra_distance[j])) {		// is this new path shorter?
					input_node[i].dijkstra_distance[j] = sum;						// if new path is shorter, set new shorter distance
					// new path string = path from source node to index node + path from index node to destination node:
					input_node[i].dijkstra_path[j] = input_node[i].dijkstra_path[index] + " - " + input_node[index].dijkstra_path[j];
				}
			}					//finished looping thru each node as destination
			--num_unused;		// one more node has now been analyzed as the source
		}
		fill(used.begin(), used.end(), false);		// reset used array to all false, get ready to do run for next starting node
	}
	finish_paths(input_node, count);				// make each node's path end with the appropriate destination node
}

size_t get_min_unused_index(size_t count, vector<bool> used, vector<node>& input_node, size_t starting_node) {
	// purpose: determines which node has the smallest distance from source. this node is then locked
	// utilizes: size_t as number of nodes, vector of bools to see which ones can still be used, 
	//		vector of nodes as source of graph, size_t to represent which is the starting node that we are looking at right now

	float min[2] = { -1,INFTY };					// min[0] = index of min, min[1] = value of min; initialize index to negative number and distance to inifnity
	for (size_t i = 0; i < count; ++i) {			// loop thru each node to see which has smallest optimized distance
		if (!used[i] && input_node[starting_node].dijkstra_distance[i] <= min[1]) {	// if this is the smallest one so far, store it in min[]
			min[1] = input_node[starting_node].dijkstra_distance[i];				// storing its value in min[1]
			min[0] = i;																// storing its index in min[0]
		}	//if there are multiple instances of the min distance, it doesn't matter which is used next
	}
	assert(min[0] >= 0);	//assert that this function has actually done something. if it has, index will not be the original negative number
	return min[0];			// return the index of the smallest optimized distance
}

void init_paths(vector<node>& input_node, size_t count) {
	// purpose: initializes the starting values of each node's paths
	// utilizes: vector of nodes and source of graph, size_t as number of nodes

	string start;											// name of node
	for (size_t i = 0; i < count; ++i) {					// iterate for each possible starting node
		start = input_node[i].name;							// set start of be name of the node
		for (size_t j = 0; j < count; ++j) {				// iterate thru each possible destination node (gven the start node i)
			input_node[i].dijkstra_path.push_back(start);	// set start node for each possible destination node
		}
	}
}

void finish_paths(vector<node>& input_node, size_t count) {
	// purpose: puts ending node name in each node's path
	// utilizes: vector of nodes and source of graph, size_t as number of nodes

	string end;												// name of ending/destination node
	for (size_t i = 0; i < count; ++i) {					// iterate for each possible source node
		for (size_t j = 0; j < count; ++j) {				// iterate for each possible ending node
			input_node[i].dijkstra_path[j].append(" - " + input_node[j].name);	// put string end on the ending of each path
		}
	}
}

bool is_float(const string &input) {
	// purpse: determines if a string can be converted to a float
	// utilizes: a string passed in to the function
	// returns: true if string can be converted to float, else false
	// Source: This function was taken from: 
	// https://www.quora.com/How-can-I-check-if-a-std-string-is-a-floating-point-number-in-C++
	// Written by Richard Liu

	stringstream sstr(input);		// input string
	float new_float;				// declare new float
	return !((sstr >> noskipws >> new_float).rdstate() ^ ios_base::eofbit);		// try to make string into a float
}

bool restart_question() {
	// purpose: asks user if they want to restart the program
	// returns: true if program should restart, else false

	char answer;																// answer given by user
	do {
		cout << "\nDo you want to run again?\nY or N: ";						// ask question
		cin >> answer;															// get answer from user
	} while (answer != 'Y' && answer != 'y' && answer != 'N' && answer != 'n');	// keep asking until they enter valid response
	cout << endl;																//new line
	if (answer == 'Y' || answer == 'y')											// if they want to continue running program, return true
		return true;
	else
		return false;															// otherwise, they want to stop, return false
}
