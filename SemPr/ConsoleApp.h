#pragma once

#include "CliMenu.h"
#include <libds/heap_monitor.h>
#include "NetworkRoute.h"
#include "Algorithm.h"
#include "Loader.h"
#include "simpleLogger.h"

#include <libds/amt/explicit_hierarchy.h>
#include <libds/amt/hierarchy.h>
//#include <libds/adt/table.h>

class ConsoleApp {
private:
	CliMenu main_menu;
	ImplicitSequence<NetworkBlock>* networkRoutes;
	MultiWayExplicitHierarchy<NetworkHierarchyBlock>* networkHierarchy;
	MultiWayExplicitHierarchyBlock<NetworkHierarchyBlock>* currentNode;
	//Table<std::string, ImplicitSequence<NetworkRoute*>>* networkTable;

public:
	ConsoleApp();
	~ConsoleApp();
	void Start();
};

ConsoleApp::ConsoleApp() : main_menu("Main menu") {
	networkRoutes = new ImplicitSequence<NetworkBlock>();
	networkHierarchy = new MultiWayExplicitHierarchy<NetworkHierarchyBlock>();
	networkHierarchy->emplaceRoot();
	currentNode = networkHierarchy->accessRoot();
	//networkTable = new SortedSequenceTable<std::string, ImplicitSequence<NetworkRoute*>>();
}

ConsoleApp::~ConsoleApp() {
	networkHierarchy->clear();
	delete networkHierarchy;
	for (size_t i = 0; i < networkRoutes->size(); ++i) {
		delete networkRoutes->access(i)->data_.route;
	}
	//networkRoutes->clear();
	delete networkRoutes;
	networkRoutes = nullptr;
}

void ConsoleApp::Start() {
	
	// ========== initialization ==========
	SimpleLogger::log(LOG_INFO, "Console App init");
	AlgorithmProcessor<NetworkBlock> algp;
	Loader().load("C:\\Users\\potoc\\source\\repos\\BeardedCrackie\\DataStructures\\SemPr\\RT.csv", *networkRoutes);

	// ========== main menu ==========
	main_menu.AddItem(new MenuActionItem("Print loaded networks", [&]()
		{
			AlgorithmProcessor<NetworkBlock>().process(networkRoutes->begin(), networkRoutes->end(), [&](NetworkRoute* rt) {
				return Predicate::print(rt);
				});
			//algp.printRoutes(); //netreba, zbytocne ide 2x
		}));


	// ========== level 1 ==========
	CliMenu* level1 = new CliMenu("1 level - sequence");
	main_menu.AddItem(level1);
	
	level1->AddItem(new MenuActionItem("matchWithAddress", [&]()
		{
			std::cout << "type ip address in format X.X.X.X" << std::endl;
			std::string ipAddr;
			std::cin >> ipAddr;
			std::bitset<32> compareRt = NetworkRoute::ipToBitset(ipAddr);

			AlgorithmProcessor<NetworkBlock>().process(networkRoutes->begin(), networkRoutes->end(), [&](NetworkRoute* rt) {
				return Predicate::matchWithAddress(compareRt, rt, true);
				});
			//algp.printRoutes();
		}));
	
	level1->AddItem(new MenuActionItem("matchLifetime", [&]()
		{
			std::cout << "type lower ttl boundary" << std::endl;
			std::string lower;
			std::cin >> lower;
			std::cout << "type upper ttl boundary" << std::endl;
			std::string upper;
			std::cin >> upper;
			int lowerBorder = stoi(lower);
			int higherBorder = stoi(upper);

			AlgorithmProcessor<NetworkBlock>().process(networkRoutes->begin(), networkRoutes->end(), [&](NetworkRoute* rt) {
				return Predicate::matchLifetime(lowerBorder, higherBorder, rt, true);
				});
			//algp.printRoutes();
		}));

	level1->AddItem(new MenuActionItem("print routes", [&]() {
		AlgorithmProcessor<NetworkBlock>().process(networkRoutes->begin(), networkRoutes->end(), [&](NetworkRoute* rt) {
			return Predicate::print(rt);
			});
		}));

	level1->AddItem(new MenuActionItem("flush algorithm", [&]()
		{
			algp.flush();
		}));

	// ========== level 2 ==========
	Loader().loadNetworkHierarchy(*networkRoutes, *networkHierarchy);
	CliMenu* level2 = new CliMenu("2 level - hierarchy");
	main_menu.AddItem(level2);

	level2->AddItem(new MenuActionItem("print hierarchy", [&]() {

		AlgorithmProcessor algpHierarchy = AlgorithmProcessor<NetworkHierarchyBlock>();
		auto start = Hierarchy<MultiWayExplicitHierarchyBlock<NetworkHierarchyBlock>>::PreOrderHierarchyIterator(networkHierarchy, currentNode);
		auto end = Hierarchy<MultiWayExplicitHierarchyBlock<NetworkHierarchyBlock>>::PreOrderHierarchyIterator(networkHierarchy, nullptr);

		algpHierarchy.process(start, end, [&](NetworkRoute* rt) {
			return Predicate::print(rt);
			});

	}));

	level2->AddItem(new MenuActionItem("iterator up", [&]() {

		if (networkHierarchy->accessParent(*currentNode) != nullptr) {
			currentNode = networkHierarchy->accessParent(*currentNode);
		}
		else {
			std::cout << "iterator is in root" << " ";
		}
		}));

	level2->AddItem(new MenuActionItem("iterator down", [&]() {
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
		}));
		

	level2->AddItem(new MenuActionItem("reset iterator", [&]() {
		currentNode = networkHierarchy->accessRoot();
		}));
	

	level2->AddItem(new MenuActionItem("matchWithAddress", [&]()
		{
			std::cout << "\ntype ip address in format X.X.X.X" << std::endl;
			std::string ipAddr;
			std::cin >> ipAddr;
			std::cout << std::endl;
			std::bitset<32> compareRt = NetworkRoute::ipToBitset(ipAddr);

			AlgorithmProcessor algpHierarchy = AlgorithmProcessor<NetworkHierarchyBlock>();
			auto start = Hierarchy<MultiWayExplicitHierarchyBlock<NetworkHierarchyBlock>>::PreOrderHierarchyIterator(networkHierarchy, currentNode);
			auto end = Hierarchy<MultiWayExplicitHierarchyBlock<NetworkHierarchyBlock>>::PreOrderHierarchyIterator(networkHierarchy, nullptr);

			algpHierarchy.process(start, end, [&](NetworkRoute* rt) {
				return Predicate::matchWithAddress(compareRt, rt, true);
				});
		}));

	level2->AddItem(new MenuActionItem("matchLifetime", [&]()
		{
			std::cout << "\ntype lower ttl boundary" << std::endl;
			std::string lower;
			std::cin >> lower;
			std::cout << "\ntype upper ttl boundary" << std::endl;
			std::string upper;
			std::cin >> upper;
			std::cout << std::endl;
			int lowerBorder = stoi(lower);
			int higherBorder = stoi(upper);

			AlgorithmProcessor algpHierarchy = AlgorithmProcessor<NetworkHierarchyBlock>();
			auto start = Hierarchy<MultiWayExplicitHierarchyBlock<NetworkHierarchyBlock>>::PreOrderHierarchyIterator(networkHierarchy, currentNode);
			auto end = Hierarchy<MultiWayExplicitHierarchyBlock<NetworkHierarchyBlock>>::PreOrderHierarchyIterator(networkHierarchy, nullptr);

			algpHierarchy.process(start, end, [&](NetworkRoute* rt) {
				return Predicate::matchLifetime(lowerBorder, higherBorder, rt, true);
				});
		}));

	/*
	// ========== level 3 ==========
	Loader().loadNetworkTable(*networkRoutes, *networkTable);

	CliMenu* level3 = new CliMenu("3 level - tables");
	main_menu.AddItem(level3);

	level3->AddItem(new MenuActionItem("find nextHop", [&]() {
		std::cout << "\nnext hop address" << std::endl;
		std::string nextHop;
		std::cin >> nextHop;

		ImplicitSequence<NetworkRoute*>* rtSeq = nullptr;

		if (networkTable->tryFind(nextHop, rtSeq)) {
			for (NetworkRoute* rt : *rtSeq) {
				rt->printRoute();
			}
		}
	}));
	*/

	// ======== flush network ========
	this->main_menu.AddItem(new MenuActionItem("flush", [&]()
		{
			delete networkRoutes;
			networkRoutes = nullptr;
		}));

	// ======== start console app ========
	this->main_menu.apply();
}

