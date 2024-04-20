#pragma once

#include "CliMenu.h"
#include "NetworkRoute.h"
#include "Algorithm.h"
#include "Loader.h"


class ConsoleApp {
private:
	CliMenu main_menu;
	ImplicitSequence<NetworkRoute*>* networkRoutes;
public:
	ConsoleApp();
	~ConsoleApp();
	void Start();
	void Stop();
};

ConsoleApp::ConsoleApp() : main_menu("Main menu") {
	networkRoutes = new ImplicitSequence<NetworkRoute*>();
}


ConsoleApp::~ConsoleApp() {
	for (size_t i = 0; i < networkRoutes->size(); ++i) {
		delete networkRoutes->access(i)->data_;
	}
	//networkRoutes->clear();
	delete networkRoutes;
	networkRoutes = nullptr;
}


void ConsoleApp::Start() {

	// ========== initialization ==========
	AlgorithmProcessor algp = AlgorithmProcessor<NetworkRoute*>();
	Loader().load("C:\\Users\\potoc\\source\\repos\\BeardedCrackie\\DataStructures\\SemPr\\RT.csv", *networkRoutes);


	// ========== main menu ==========
	main_menu.AddItem(new MenuActionItem("Print loaded networks", [&]()
		{
			algp.processRouteTable(networkRoutes->begin(), networkRoutes->end(), [&](NetworkRoute* rt) {
				rt->printRoute();
				return true;
				});
		}));


	// ========== level 1 ==========
	CliMenu* level1 = new CliMenu("1 level");
	main_menu.AddItem(level1);
	
	level1->AddItem(new MenuActionItem("matchWithAddress", [&]()
		{
			std::cout << "type ip address in format X.X.X.X" << endl;
			string ipAddr;
			std::cin >> ipAddr;
			std::bitset<32> compareRt = NetworkRoute::ipToBitset(ipAddr);

			algp.processRouteTable(networkRoutes->begin(), networkRoutes->end(), [&](NetworkRoute* rt) {
				std::bitset<32> parent = rt->getNetworkAddress();
				for (int i = 0; i < rt->getPrefix(); i++)
				{
					if (parent[31 - i] != compareRt[31 - i]) {
						return false;
					}
				}
				return true;
				});
		}));
	
	level1->AddItem(new MenuActionItem("matchLifetime", [&]()
		{
			std::cout << "type lower ttl boundary" << endl;
			string lower;
			std::cin >> lower;
			std::cout << "type upper ttl boundary" << endl;
			string upper;
			std::cin >> upper;
			int lowerBorder = stoi(lower);
			int higherBorder = stoi(upper);

			algp.processRouteTable(networkRoutes->begin(), networkRoutes->end(), [&](NetworkRoute* rt) {
				if (rt->getTtl() >= lowerBorder && rt->getTtl() <= higherBorder) {
					return true;
				}
				return false;
				});
		}));
	level1->AddItem(new MenuActionItem("print routes", [&]()
		{
			algp.printRoutes();
		}));
	level1->AddItem(new MenuActionItem("flush algorithm", [&]()
		{
			algp.flush();
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


inline void ConsoleApp::Stop()
{
	for (size_t i = 0; i < networkRoutes->size(); ++i) {
		delete networkRoutes->access(i)->data_;
	}
	networkRoutes->clear();
}
