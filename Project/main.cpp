

#include "stdafx.h"
#include <iostream>    // Provides cout
#include <cstdlib>     // Provides EXIT_SUCCESS
#include <string>
#include <vector>
using namespace std;   // Allows all standard library items to be used


int main()
{

	size_t count = 0;
	vector <string> node_names;
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
	
	for (size_t i = 0; i < count; ++i) {
		cout << node_names[i] << endl;
	}


	cin.get();
	cin.get();
	return EXIT_SUCCESS;
}
