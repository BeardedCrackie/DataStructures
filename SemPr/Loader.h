#pragma once

#include <libds/heap_monitor.h>
#include <libds/amt/implicit_sequence.h>
#include <libds/amt/explicit_hierarchy.h>
//include <libds/adt/table.h>
#include "NetworkRoute.h"
#include <iostream>
#include <fstream>
#include <string>
#include "simpleLogger.h"

using namespace ds::amt;
//ing namespace ds::adt;


class Loader
{
public:
	void load(std::string filePath, ImplicitSequence<NetworkBlock>& routeSequence);
	void loadNetworkHierarchy(ImplicitSequence<NetworkBlock>& routeSequence, MultiWayExplicitHierarchy<NetworkHierarchyBlock>& networkHierarchy);
	//void loadNetworkTable(ImplicitSequence<NetworkBlock>& routeSequence, Table<std::string, ImplicitSequence<NetworkBlock>>& networkTable);
};


void Loader::load(std::string filePath, ImplicitSequence<NetworkBlock>& routeSequence) {
	SimpleLogger::log(LOG_DEBUG, "Start sequence loading");
	std::fstream inputFile(filePath);
	if (!inputFile.is_open()) {
		SimpleLogger::log(LOG_ERROR,"File is not present!");
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
		routeSequence.insertLast().data_.route = newRoute;
	}
	inputFile.close();
	inputFile.clear();
	SimpleLogger::log(LOG_INFO, "Sequence loaded with size: " + std::to_string(routeSequence.size()));
}


void Loader::loadNetworkHierarchy(ImplicitSequence<NetworkBlock>& routeSequence, MultiWayExplicitHierarchy<NetworkHierarchyBlock>& networkHierarchy)
{
	SimpleLogger::log(LOG_DEBUG, "Start hierarchy loading");

	/*
	for (auto current = routeSequence.begin(); current != routeSequence.end(); ++current) {
		NetworkRoute* route = *current;
		auto currNode = networkHierarchy.accessRoot();
		int octetValue = 0;
		//networkHierarchy.emplaceSon(*currNode, 4); //todo tu to pada pri 4 synovi

		
		//std::cout << "hierarchy: ";
		for (size_t octet = 0; octet < 4; ++octet) {
			octetValue = route->getOctet(octet);
			//std::cout << octetValue << ".";
			if (!networkHierarchy.hasNthSon(*currNode, octetValue)) {
				networkHierarchy.emplaceSon(*currNode, octetValue); //todo tu to pada pri 4 synovi
				//networkHierarchy.changeSon(*currNode, octetValue, nullptr); //todo tu to pada pri 4 synovi
			}
			currNode = networkHierarchy.
				accessSon(*currNode, octetValue);
		}
		//std::cout << std::endl;
		//route->printRoute();
		currNode->data_ = route;
		*/

	for (auto current = routeSequence.begin(); current != routeSequence.end(); ++current) {
		NetworkBlock rtBlock = *current;
		auto currNode = networkHierarchy.accessRoot();
		size_t octetValue = 0;
		//networkHierarchy.emplaceSon(*currNode, 4); //todo tu to pada pri 4 synovi


		//std::cout << "hierarchy: ";
		for (size_t octet = 0; octet < 4; ++octet) {
			octetValue = rtBlock.route->getOctet(octet);

			bool found = false;
			for (auto son : *currNode->sons_) {
				if (son->data_.octetValue == octetValue) {
					found = true;
					currNode = son;
					break;
				}
			}

			if (!found) {
				currNode = &networkHierarchy.emplaceSon(*currNode, networkHierarchy.degree(*currNode));
				currNode->data_.octetValue = octetValue;
				//std::cout << "creating node: " << octetValue << " in " << octet << ".th octet" << std::endl;
			}

		}
		currNode->data_.route = rtBlock.route;
		//std::cout << "adding route: ";
		//route->printRoute();
	}
	SimpleLogger::log(LOG_INFO, "Hierarchy loaded with size: " + std::to_string(networkHierarchy.size()));
}

/*
void Loader::loadNetworkTable(ImplicitSequence<NetworkBlock>& routeSequence, Table<std::string, ImplicitSequence<NetworkBlock>>& networkTable)
{
	NetworkRoute* route = nullptr;
	std::string nextHop = "";
	for (auto current = routeSequence.begin(); current != routeSequence.end(); ++current) {
		NetworkBlock route = *current;
		nextHop = route.route->getNextHop().to_string();
		//ImplicitSequence<NetworkRoute*>* sequence = &networkTable.find(nextHop);
		if (!networkTable.contains(nextHop)) {
			networkTable.insert(nextHop, ImplicitSequence<NetworkBlock>());
		}
		networkTable.find(nextHop).insertLast().data_.route = route.route;
	}
}
*/

