#pragma once

#include "CliMenu.h"
#include <libds/heap_monitor.h>
#include "NetworkRoute.h"
#include "Algorithm.h"
#include "Loader.h"
#include "simpleLogger.h"

#include <libds/amt/explicit_hierarchy.h>
#include <libds/amt/hierarchy.h>
#include <libds/adt/table.h>
#include <libds/amt/abstract_memory_type.h>
using namespace ds::amt;
using namespace ds::adt;

typedef Hierarchy<MultiWayExplicitHierarchyBlock<NetworkHierarchyBlock>>::PreOrderHierarchyIterator NetworkHierarchyIterator;


class ConsoleApp {
private:
	CliMenu main_menu;
	ImplicitSequence<NetworkBlock>* networkRoutes;
	MultiWayExplicitHierarchy<NetworkHierarchyBlock>* networkHierarchy;
	MultiWayExplicitHierarchyBlock<NetworkHierarchyBlock>* currentNode;
	SortedSequenceTable<std::string, ImplicitSequence<NetworkBlock>*>* networkTable;
	//SortedSequenceTable<std::string, NetworkBlock>* networkTable;
	AlgorithmSorter<NetworkBlock> algSorter;

	template<typename Iterator>
	void matchWithAddress(Iterator start, Iterator end);

	template<typename Iterator>
	void matchLifetime(Iterator start, Iterator end);

	template<typename Iterator>
	void printRoutes(Iterator start, Iterator end);

	AlgorithmProcessor<NetworkBlock> algp;

public:
	ConsoleApp();
	~ConsoleApp();
	void Start();
	void hierarchyIteratorDown();
	void hierarchyIteratorUp();
};

ConsoleApp::ConsoleApp() : main_menu("Main menu") {
	AlgorithmProcessor<NetworkRoute*> algp;
	networkRoutes = new ImplicitSequence<NetworkBlock>();
	networkHierarchy = new MultiWayExplicitHierarchy<NetworkHierarchyBlock>();
	networkHierarchy->emplaceRoot();
	currentNode = networkHierarchy->accessRoot();
	networkTable = new SortedSequenceTable<std::string, ImplicitSequence<NetworkBlock>*>();
	//networkTable = new SortedSequenceTable<std::string, NetworkBlock>();

	AlgorithmSorter<NetworkBlock> algSorter;
	
	
}


ConsoleApp::~ConsoleApp() {

	delete networkHierarchy;
	
	for (size_t i = 0; i < networkRoutes->size(); ++i) {
		delete networkRoutes->access(i)->data_.route;
	}
	delete networkRoutes;

	for (auto seq : *networkTable) {
		delete seq.data_;
	}
	delete networkTable;

}

void ConsoleApp::Start() {
	
	// ========== initialization ==========
	SimpleLogger::log(LOG_INFO, "Console App init");
	Loader().load("C:\\Users\\potoc\\source\\repos\\BeardedCrackie\\DataStructures\\SemPr\\RT.csv", *networkRoutes);
	algp.process(networkRoutes->begin(), networkRoutes->end(), [](NetworkBlock* nb) {
		return true;	//init load of sequence for level 4
		});

	// ========== main menu ==========
	CliMenu* level1 = new CliMenu("1 level - sequence");
	main_menu.AddItem(level1);
	
	CliMenu* level2 = new CliMenu("2 level - hierarchy");
	main_menu.AddItem(level2);

	CliMenu* level3 = new CliMenu("3 level - tables");
	main_menu.AddItem(level3);

	CliMenu* level4 = new CliMenu("4 level - sort");
	main_menu.AddItem(level4);

	main_menu.AddItem(new MenuActionItem("Print loaded networks", [&]() {
		printRoutes(networkRoutes->begin(), networkRoutes->end());
		}));

	// ========== level 1 ==========
	level1->AddItem(new MenuActionItem("print routes", [&]() {
		printRoutes(networkRoutes->begin(), networkRoutes->end());
		}));

	level1->AddItem(new MenuActionItem("matchWithAddress", [&]()
		{
			matchWithAddress(networkRoutes->begin(), networkRoutes->end());
		}));
	
	level1->AddItem(new MenuActionItem("matchLifetime", [&]()
		{
			matchLifetime(networkRoutes->begin(), networkRoutes->end());
		}));

	// ========== level 2 ==========
	Loader().loadNetworkHierarchy(*networkRoutes, *networkHierarchy);
	level2->AddItem(new MenuActionItem("print hierarchy", [&]() {
		printRoutes(
			NetworkHierarchyIterator(networkHierarchy, currentNode),
			NetworkHierarchyIterator(networkHierarchy, nullptr)
		);
	}));

	level2->AddItem(new MenuActionItem("iterator up", [&]() {
		hierarchyIteratorUp();
		}));

	level2->AddItem(new MenuActionItem("iterator down", [&]() {
		hierarchyIteratorDown();
		}));
		
	level2->AddItem(new MenuActionItem("reset iterator", [&]() {
		currentNode = networkHierarchy->accessRoot();
		}));
	
	level2->AddItem(new MenuActionItem("matchWithAddress", [&]()
		{
			matchWithAddress(
				NetworkHierarchyIterator(networkHierarchy, currentNode),
				NetworkHierarchyIterator(networkHierarchy, nullptr)
			);
		}));

	level2->AddItem(new MenuActionItem("matchLifetime", [&]()
		{
			matchLifetime(
				NetworkHierarchyIterator(networkHierarchy, currentNode),
				NetworkHierarchyIterator(networkHierarchy, nullptr)
			);
		}));

	
	// ========== level 3 ==========
	Loader().loadNetworkTable(*networkRoutes, *networkTable);

	level3->AddItem(new MenuActionItem("print all nextHop", [&]() {
		for (auto current = networkTable->begin(); current != networkTable->end(); ++current) {
			TableItem<std::string, ImplicitSequence<NetworkBlock>*> item = *current;
			std::cout << NetworkRoute::bitsetToIp(item.data_->accessFirst()->data_.route->getNextHop()) << std::endl;
		}
	}));

	level3->AddItem(new MenuActionItem("find by nextHop", [&]() {
		std::cout << "\nnext hop address" << std::endl;
		std::string nextHop;
		std::cin >> nextHop;

		ImplicitSequence<NetworkBlock>** seqBlock = nullptr;
		if (networkTable->tryFind(nextHop, seqBlock)) {
			for (NetworkBlock nb : **seqBlock) {
				nb.route->printRoute();
			}
		}
	}));


	// ========== level 4 ==========
	level4->AddItem(new MenuActionItem("sort by prefix", [&]() {
		algSorter.sort(*algp.getSequence(), [&](NetworkBlock rtA, NetworkBlock rtB) {

			size_t prefixA = rtA.route->getPrefix();
			std::bitset<32> ipA= rtA.route->getNetworkAddress();
			size_t prefixB = rtB.route->getPrefix();
			std::bitset<32> ipB = rtB.route->getNetworkAddress();

			//for (size_t bit = 31; bit > prefixA && bit > prefixB; --bit) {
			for (int bit = 31; bit >= 0; --bit) {
				if (ipA[bit] < ipB[bit]) {
					return true;
				}
				else if (ipA[bit] > ipB[bit]) {
					return false;
				}
			}
			return rtA.route->getPrefix() < rtB.route->getPrefix();
			});
		}));

	level4->AddItem(new MenuActionItem("sort by time", [&]() {
		algSorter.sort(*algp.getSequence(), [&](NetworkBlock firstRt, NetworkBlock secondRt) {
			return firstRt.route->getTtl() < secondRt.route->getTtl();
			});
		}));

	level4->AddItem(new MenuActionItem("print routes", [&]() {
		AlgorithmProcessor<NetworkBlock>().process(
			algp.getSequence()->begin(),
			algp.getSequence()->end(),
			[&](NetworkBlock* rt) {
				return Predicate::print(rt->route);
			});
		}));

	// ======== start console app ========
	this->main_menu.apply();
}

void ConsoleApp::hierarchyIteratorDown()
{
	if (currentNode->sons_->size() <= 0) {
		std::cout << "iterator is leaf" << std::endl;
	}
	else {

		std::cout << "\navailable octet values: ";
		for (auto son : *currentNode->sons_) {
			std::cout << son->data_.octetValue << " ";
		}
		int octetValue = 0;
		std::cout << "\n\nchoose octet value:" << std::endl;
		std::cin >> octetValue;
		for (auto son : *currentNode->sons_) {
			if (son->data_.octetValue == octetValue) {
				currentNode = son;
				std::cout << "\niterator moved" << std::endl;
				return;
			}
		}
		std::cout << "\noctet not found: " << std::endl;
	}
}

void ConsoleApp::hierarchyIteratorUp()
{
	if (networkHierarchy->accessParent(*currentNode) != nullptr) {
		currentNode = networkHierarchy->accessParent(*currentNode);
	}
	else {
		std::cout << "iterator is in root" << " ";
	}
}

template<typename Iterator>
void ConsoleApp::matchWithAddress(Iterator start, Iterator end) {
	std::cout << "type ip address in format X.X.X.X" << std::endl;
	std::string ipAddr;
	std::cin >> ipAddr;
	std::bitset<32> compareRt = NetworkRoute::ipToBitset(ipAddr);
	
	algp.process(start, end, [&](NetworkBlock* rt) {
		return Predicate::matchWithAddress(compareRt, rt->route, true);
		});
};

template<typename Iterator>
void ConsoleApp::matchLifetime(Iterator start, Iterator end) {
	std::cout << "type lower ttl boundary" << std::endl;
	std::string lower;
	std::cin >> lower;
	std::cout << "type upper ttl boundary" << std::endl;
	std::string upper;
	std::cin >> upper;
	int lowerBorder = stoi(lower);
	int higherBorder = stoi(upper);

	algp.process(start, end, [&](NetworkBlock* rt) {
		return Predicate::matchLifetime(lowerBorder, higherBorder, rt->route, true);
	});
};

template<typename Iterator>
void ConsoleApp::printRoutes(Iterator start, Iterator end) {
	algp.process(start, end, [&](NetworkBlock* rt) {
		return Predicate::print(rt->route);
		});
};