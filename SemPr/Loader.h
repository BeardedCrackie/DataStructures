#pragma once

#include <libds/heap_monitor.h>
#include <libds/amt/implicit_sequence.h>
#include <libds/amt/explicit_hierarchy.h>
#include <libds/adt/table.h>
#include "NetworkRoute.h"
#include <iostream>
#include <fstream>
#include <string>
#include "simpleLogger.h"

using namespace ds::amt;
using namespace ds::adt;


class Loader
{
public:
	void load(std::string filePath, ImplicitSequence<NetworkBlock>& routeSequence);
	void loadNetworkHierarchy(ImplicitSequence<NetworkBlock>& routeSequence, MultiWayExplicitHierarchy<NetworkHierarchyBlock>& networkHierarchy);
	void loadNetworkTable(ImplicitSequence<NetworkBlock>& routeSequence, SortedSequenceTable<std::string, ImplicitSequence<NetworkBlock>*>& networkTable);
	//void loadNetworkTable(ImplicitSequence<NetworkBlock>& routeSequence, Table<std::string, NetworkBlock>& networkTable);
};



void Loader::load(std::string filePath, ImplicitSequence<NetworkBlock>& routeSequence) {
	SimpleLogger::log(LOG_DEBUG, "Start sequence loading");
	std::fstream inputFile(filePath);
	if (!inputFile.is_open()) {
		SimpleLogger::log(LOG_ERROR,"File is not present!");
		return;
	}
	
	std::string value;
	getline(inputFile, value); //first getLine is source, not important so skip

	do	{
		getline(inputFile, value, ';'); 
        NetworkRoute* newRoute = new NetworkRoute();
		getline(inputFile, value, '/'); //address
		newRoute->setNetworkAddress(value);
		getline(inputFile, value, ';'); //prefix
		newRoute->setNetworkPrefix(stoi(value));
		getline(inputFile, value, ';'); //metrics, not important so skip
		getline(inputFile, value, ';'); //next-hop
		newRoute->setNextHop(value.substr(3));
		getline(inputFile, value, ';'); //ttl
		newRoute->setTtl(value);
		routeSequence.insertLast().data_.route = newRoute;
	} while (getline(inputFile, value) && value != "");

	inputFile.close();
	inputFile.clear();

	SimpleLogger::log(LOG_INFO, "Sequence loaded with size: " + std::to_string(routeSequence.size()));
}


void Loader::loadNetworkHierarchy(ImplicitSequence<NetworkBlock>& routeSequence, MultiWayExplicitHierarchy<NetworkHierarchyBlock>& networkHierarchy)
{
	SimpleLogger::log(LOG_DEBUG, "Start hierarchy loading");

	/*
	for (auto current = routeSequence.begin(); current != routeSequence.end(); ++current) {
		NetworkBlock rtBlock = *current;
		auto currNode = networkHierarchy.accessRoot();
		size_t octetValue = 0;

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
			}

		}
		currNode->data_.route = rtBlock.route;
	}
	*/

	MultiWayExplicitHierarchyBlock<NetworkHierarchyBlock>* currOctetNode[] = { nullptr, nullptr, nullptr, nullptr };	//tmp octet node init

	auto rootNode = networkHierarchy.accessRoot();	//root node cache
	size_t octetValue = 0;	//tmp value for octet caching

	for (auto current = routeSequence.begin(); current != routeSequence.end(); ++current) {
		NetworkBlock rtBlock = *current;

		auto currNode = rootNode; //first curr is always root

		for (size_t i = 0; i < 3; i++)	//4th node is always unique, only 3 loops are needed
		{
			octetValue = rtBlock.route->getOctet(i);
			if (currOctetNode[i] == nullptr || currOctetNode[i]->data_.octetValue != octetValue) {	//if octet does not exists
				currNode = &networkHierarchy.emplaceSon(*currNode, networkHierarchy.degree(*currNode));	//create octet node as child of current and assign is as current
				currNode->data_.octetValue = octetValue;	//assign octet value for current
				currOctetNode[i] = currNode;	//assign current to octet node pointer (cache)
				currOctetNode[i + 1] = nullptr;	//set cache of next octet node as null because new line has been created
			}
			else {
				currNode = currOctetNode[i];	//if value exists, use it and save it as current
			}			
		}

		currNode = &networkHierarchy.emplaceSon(*currNode, networkHierarchy.degree(*currNode));	//4th node is always unique, so only create without checking
		currNode->data_.octetValue = rtBlock.route->getOctet(3);	//set octet value for 4th octet node
		currNode->data_.route = rtBlock.route;	//set route for 4th octet node

	}

	SimpleLogger::log(LOG_INFO, "Hierarchy loaded with size: " + std::to_string(networkHierarchy.size()));
}


void Loader::loadNetworkTable(ImplicitSequence<NetworkBlock>& routeSequence, SortedSequenceTable<std::string, ImplicitSequence<NetworkBlock>*>& networkTable)
{

	NetworkRoute* route = nullptr;
	std::string nextHop = "";
	ImplicitSequence<NetworkBlock>** seqBlock = nullptr;
	for (auto current = routeSequence.begin(); current != routeSequence.end(); ++current) {
		NetworkBlock route = *current;
		nextHop = NetworkRoute::bitsetToIp(route.route->getNextHop());
		if (!networkTable.tryFind(nextHop, seqBlock)) {
			networkTable.insert(nextHop, new ImplicitSequence<NetworkBlock>());
		}
		networkTable.find(nextHop)->insertLast().data_.route = route.route;
	}
}
