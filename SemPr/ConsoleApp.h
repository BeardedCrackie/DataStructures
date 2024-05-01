#pragma once

#include "CliMenu.h"
#include <libds/heap_monitor.h>
#include "NetworkRoute.h"
#include "Algorithm.h"
#include "Loader.h"
#include "simpleLogger.h"

#include <libds/amt/explicit_hierarchy.h>
#include <libds/amt/hierarchy.h>

class ConsoleApp {
private:
	CliMenu main_menu;
	ImplicitSequence<NetworkRoute*>* networkRoutes;
	MultiWayExplicitHierarchy<NetworkHierarchyBlock>* networkHierarchy;
	MultiWayExplicitHierarchyBlock<NetworkHierarchyBlock>* currentNode;
public:
	ConsoleApp();
	~ConsoleApp();
	void Start();
};

ConsoleApp::ConsoleApp() : main_menu("Main menu") {
	networkRoutes = new ImplicitSequence<NetworkRoute*>();
	networkHierarchy = new MultiWayExplicitHierarchy<NetworkHierarchyBlock>();
	networkHierarchy->emplaceRoot();
	//currentNode = networkHierarchy->begin();
	currentNode = networkHierarchy->accessRoot();
}

ConsoleApp::~ConsoleApp() {
	networkHierarchy->clear();
	delete networkHierarchy;
	for (size_t i = 0; i < networkRoutes->size(); ++i) {
		delete networkRoutes->access(i)->data_;
	}
	//networkRoutes->clear();
	delete networkRoutes;
	networkRoutes = nullptr;
}

void ConsoleApp::Start() {
	
	// ========== initialization ==========
	SimpleLogger::log(LOG_INFO, "Console App init");
	AlgorithmProcessor<NetworkRoute*> algp;
	Loader().load("C:\\Users\\potoc\\source\\repos\\BeardedCrackie\\DataStructures\\SemPr\\RT.csv", *networkRoutes);
	Loader().loadNetworkHierarchy(*networkRoutes, *networkHierarchy);

	// ========== main menu ==========
	main_menu.AddItem(new MenuActionItem("Print loaded networks", [&]()
		{
			algp.process(networkRoutes->begin(), networkRoutes->end(), [&](NetworkRoute* rt) {
				return Predicate::print(rt);
				});
			//algp.printRoutes(); //netreba, zbytocne ide 2x
		}));


	// ========== level 1 ==========
	CliMenu* level1 = new CliMenu("1 level");
	main_menu.AddItem(level1);
	
	level1->AddItem(new MenuActionItem("matchWithAddress", [&]()
		{
			AlgorithmProcessor<NetworkRoute*> algp;
			std::cout << "type ip address in format X.X.X.X" << std::endl;
			std::string ipAddr;
			std::cin >> ipAddr;
			std::bitset<32> compareRt = NetworkRoute::ipToBitset(ipAddr);

			algp.process(networkRoutes->begin(), networkRoutes->end(), [&](NetworkRoute* rt) {
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

			AlgorithmProcessor<NetworkRoute*> algp;
			algp.process(networkRoutes->begin(), networkRoutes->end(), [&](NetworkRoute* rt) {
				return Predicate::matchLifetime(lowerBorder, higherBorder, rt, true);
				});
			//algp.printRoutes();
		}));

	level1->AddItem(new MenuActionItem("print routes", [&]() {
		AlgorithmProcessor<NetworkRoute*> algp;
		algp.process(networkRoutes->begin(), networkRoutes->end(), [&](NetworkRoute* rt) {
			return Predicate::print(rt);
			});
		}));

	level1->AddItem(new MenuActionItem("flush algorithm", [&]()
		{
			algp.flush();
		}));

	// ========== level 2 ==========
	CliMenu* level2 = new CliMenu("2 level");
	main_menu.AddItem(level2);

	level2->AddItem(new MenuActionItem("print hierarchy", [&]() {
		//auto iterator = Hierarchy<MultiWayExplicitHierarchyBlock<NetworkRoute*>>::PreOrderHierarchyIterator(networkHierarchy, &currentNode);
		//static_cast<MultiWayExplicitHierarchyBlock<UzemnaJednotka*>*>

		//static_cast<MultiWayExplicitHierarchyBlock<NetworkHierarchyBlock>*>(&currentNode);
		//auto node = ds::amt::Hierarchy<MultiWayExplicitHierarchyBlock<NetworkHierarchyBlock>>::PreOrderHierarchyIterator::PreOrderHierarchyIterator(networkHierarchy, currentNode);
			//MultiWayExplicitHierarchy<NetworkHierarchyBlock>::PreOrderHierarchyIterator(networkHierarchy, currentNode);

		//networkHierarchy->beginPre
		//auto node = static_cast<Iterator>(*currentNode);

		//networkHierarchy = new MultiWayExplicitHierarchy<NetworkHierarchyBlock>();

		AlgorithmProcessor algpHierarchy = AlgorithmProcessor<MultiWayExplicitHierarchy<NetworkHierarchyBlock>>();
		auto start = Hierarchy<MultiWayExplicitHierarchyBlock<NetworkHierarchyBlock>>::PreOrderHierarchyIterator(networkHierarchy, currentNode);
		auto end = Hierarchy<MultiWayExplicitHierarchyBlock<NetworkHierarchyBlock>>::PreOrderHierarchyIterator(networkHierarchy, nullptr);

		algpHierarchy.process(start, end, [&](NetworkRoute* rt) {
			return Predicate::print(rt);
			});
		//algp.printRoutes();

	}));

	level2->AddItem(new MenuActionItem("iterator up", [&]() {
		//todo current is not root if (currentNode != networkHierarchy->accessRoot())
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

		//currentNode = Hierarchy<KWayExplicitHierarchyBlock<NetworkRoute*, 256>>::PreOrderHierarchyIterator(networkHierarchy, networkHierarchy->begin());
		}));
	

	level2->AddItem(new MenuActionItem("matchWithAddress", [&]()
		{
			std::cout << "\ntype ip address in format X.X.X.X" << std::endl;
			std::string ipAddr;
			std::cin >> ipAddr;
			std::cout << std::endl;

			std::bitset<32> compareRt = NetworkRoute::ipToBitset(ipAddr);
			
			auto node = MultiWayExplicitHierarchy<NetworkHierarchyBlock>::PreOrderHierarchyIterator(networkHierarchy, currentNode);

			for (auto networkBlock = node; networkBlock != networkHierarchy->end(); ++networkBlock) {
				NetworkHierarchyBlock network = static_cast<NetworkHierarchyBlock>(*networkBlock);
				if (network.route != nullptr) {
					std::bitset<32> parent = network.route->getNetworkAddress();
					bool found = true;
					for (size_t i = 0; i < network.route->getPrefix(); i++)
					{
						if (parent[31 - i] != compareRt[31 - i]) {
							found = false;
						}
					}
					if (found) {
						network.route->printRoute();
					}
				}
			};

			/*
			auto node = MultiWayExplicitHierarchy<NetworkHierarchyBlock>::PreOrderHierarchyIterator(networkHierarchy, currentNode);

			AlgorithmProcessor algpHierrchy = AlgorithmProcessor<NetworkRoute*>();
			algpHierrchy.processHierarchy(node, networkHierarchy->end(), [&](NetworkHierarchyBlock netBlock) {
				if (node != nullptr && netBlock.route == nullptr) {
					return false;
				}
				std::bitset<32> parent = netBlock.route->getNetworkAddress();
				for (size_t i = 0; i < netBlock.route->getPrefix(); i++)
				{
					if (parent[31 - i] != compareRt[31 - i]) {
						return false;
					}
				}
				return true;
				});
			algpHierrchy.printRoutes();
			*/

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

			auto node = MultiWayExplicitHierarchy<NetworkHierarchyBlock>::PreOrderHierarchyIterator(networkHierarchy, currentNode);

			for (auto networkBlock = node; networkBlock != networkHierarchy->end(); ++networkBlock) {
				NetworkHierarchyBlock network = static_cast<NetworkHierarchyBlock>(*networkBlock);
				if (network.route != nullptr && network.route->getTtl() >= lowerBorder && network.route->getTtl() <= higherBorder) {
					network.route->printRoute();
				}
			}
		}));

	// ======== flush network ========
	this->main_menu.AddItem(new MenuActionItem("flush", [&]()
		{
			delete networkRoutes;
			networkRoutes = nullptr;
		}));

	// ======== start console app ========
	this->main_menu.apply();
}

