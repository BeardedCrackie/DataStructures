#pragma once

#include <libds/amt/implicit_sequence.h>
#include "NetworkRoute.h"
#include <iostream>
#include <fstream>
#include <string>

//using namespace std;
using namespace ds::amt;

class Loader
{
public:
	void load(std::string filePath, ImplicitSequence<NetworkRoute*>& routeSequence);
};


void Loader::load(std::string filePath, ImplicitSequence<NetworkRoute*>& routeSequence) {
	std::fstream inputFile(filePath);
	if (!inputFile.is_open()) {
		std::cout << "File is not present!" << std::endl;
		return;
	}
	std::string line;
	getline(inputFile, line); //first line is header

	std::string value;

	//getline(inputFile, line) && line != "")
	while (getline(inputFile, line) && line != "")
	{
		std::stringstream loadedStream(line);
		if (line == "") {
			break;
		}

        NetworkRoute* newRoute = new NetworkRoute();
        getline(loadedStream, value, ';'); //source, not important so skip
		getline(loadedStream, value, '/'); //address
		newRoute->setNetworkAddress(value);
		getline(loadedStream, value, ';'); //prefix
		newRoute->setNetworkPrefix(stoi(value));
		getline(loadedStream, value, ';'); //metrics, not important so skip
		getline(loadedStream, value, ';'); //next-hop
		newRoute->setNextHop(value.substr(3));
		getline(loadedStream, value, ';'); //time in string
		newRoute->setTtl(value);
        routeSequence.insertLast().data_ = newRoute;
	}
}

