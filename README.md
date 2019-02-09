# Dijkstras_Algorithm
## Basic C++ implementation of Dijkstra's Algorithm
### Purpose
The algorithm is designed for finding the shortest distance between 2 nodes in a graph. It is an implementation of Dijkstra's Algorithm.
### Use
On startup, the program prompts for names of each node. After entering each node name, type "done" to finish entering the names. Then, the program will prompt the user for the distance between each node. Distances are one-way only. To represent no connection between nodes, type "i" or "infinity" instead of a number. Once this is finished, the program outputs the shortest distance between each node for every possible combination of nodes. additionally, the path that must be followed to achieve the shortest distance for each case is printed. At the end, the user can choose to start over with a different graph (set of nodes) or exit the program.
### Implementation:
* Nodes are defined as a struct with 4 elements:
'struct node
{
	vector <float> direct_distance;	 	 // direct_distance[i] is direct distance from current node to node i
	vector <float> dijkstra_distance;	 // dijkstra_distance[i] is shortest distance from current node to node i
	vector <string> dijkstra_path;		 // dijkstra_path[i] is the shortest path from current node to node i (as a string)
	string name;						           // name of the node
};'
* The overall implementation uses a vector of nodes.
  * Example of syntax: node[2].direct_distance[3] is the distance from the 3rd node to the 4th node
### Last Update:
9 Feb 2019 - finished successful implementation and comments
