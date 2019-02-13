# Dijkstras_Algorithm
## Basic C++ implementation of Dijkstra's Algorithm
### Purpose
The algorithm is designed for finding the shortest distance between two nodes in a graph. It is an implementation of Dijkstra's Algorithm.
### Files
* Visual Studio Implementation: Project/main.cpp
* Linux (Ubuntu) Implementation: Linux folder
* Windows Executable: Debug/Program.exe
### Running the program
* Windows: A Windows installer is provided in the "Windows Installer" folder. Download this folder and run "setup" to install the program on your PC.
* Linux (Ubuntu): An executable file called "Program.out" can be found in the "Linux" folder. This file can be downloaded and run by typing ./Program.out in the terminal (once the terminal is open in the directory of the .out file).
### Use
On startup, the program prompts for names of each node. After entering each node name, type "done" to finish entering the names. Then, the program will ask the user if they want a digraph (where each distance is one-way) or an undirected graph (where each distance is two-way). After this, the program will prompt the user for the distance between each node. Distances are one-way only. To represent no connection between nodes, type "i" or "infinity" instead of a number. Once this is finished, the program outputs the shortestdistance between each node for every possible combination of nodes. additionally, the path that must be followed to achieve the shortest distance for each case is printed. At the end, the user can choose to start over with a different graph (set of nodes) or exit the program.
### Implementation Details:
* Nodes are defined as a struct with 4 elements:<br />
  * vector <float> direct_distance;	 	 // direct_distance[i] is direct distance from current node to node i
  * vector <float> dijkstra_distance;	 // dijkstra_distance[i] is shortest distance from current node to node i
  * vector <string> dijkstra_path;		 // dijkstra_path[i] is the shortest path from current node to node i (as a string)
  * string name;						           // name of the node
* The overall implementation uses a vector of nodes.
  * Example of syntax: node_name[2].direct_distance[3] is the distance from the 3rd node to the 4th node
### Notes
* This project can be found on Github at: https://github.com/sheppardd1/Dijkstras_Algorithm
## Dependencies
* This project requires a minimum of C++ 11 when compiling (with g++ use command -std=c++11) for the stof() function.
### Last Update:
10 Feb 2019 - added support for undirected graphs
