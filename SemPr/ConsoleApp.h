#pragma once
#include "CliMenu.h"
#include "NetworkRoute.h"
#include "Algorithm.h"
#include "Loader.h"


class ConsoleApp {
private:
	CliMenu main_menu;

public:
	ConsoleApp() : main_menu("Main menu") {};
	void Start();
};

void ConsoleApp::Start() {

	// ========== support items ==========
	string dataPath = "C:\\Users\\potoc\\source\\repos\\BeardedCrackie\\DataStructures\\SemPr\\RT.csv";
	ImplicitSequence<NetworkRoute*>* networkRoutes = new ImplicitSequence<NetworkRoute*>();

	MenuActionItem clearSequence = MenuActionItem("Clear Sequence", [&]()
		{
			for (size_t i = 0; i < networkRoutes->size(); i++)
			{
				delete networkRoutes->access(i)->data_;
			}
			networkRoutes->clear();
			delete networkRoutes;
			networkRoutes = new ImplicitSequence<NetworkRoute*>();
		});

	// ========== main menu ==========

	AlgorithmProcessor algp = AlgorithmProcessor();

	MenuActionItem itemPrintSequence = MenuActionItem("Print Sequence", [&]()
		{
			ImplicitSequence<NetworkRoute*>* tmp = algp.processRouteTable(networkRoutes->begin(), networkRoutes->end(), [&](NetworkRoute* rt) {
				rt->printRoute();
				return true;
				});
			delete networkRoutes;
			networkRoutes = tmp;
		});
	main_menu.AddItem(&itemPrintSequence);

	this->main_menu.AddItem(new MenuActionItem("Load CSV", [&]()
		{
			this->main_menu.RunItem(&clearSequence);
			networkRoutes = new ImplicitSequence<NetworkRoute*>();
			Loader().load("C:\\Users\\potoc\\source\\repos\\BeardedCrackie\\DataStructures\\SemPr\\RT.csv", *networkRoutes);
		}
	));

	this->main_menu.AddItem(&clearSequence);


	// ========== level 1 ==========
	CliMenu level1("1 level");
	main_menu.AddItem(&level1);
	level1.AddItem(&itemPrintSequence);

	level1.AddItem(new MenuActionItem("matchWithAddress", [&]()
		{
			std::cout << "type ip address in format X.X.X.X" << endl;
			string ipAddr;
			std::cin >> ipAddr;
			std::bitset<32> compareRt = NetworkRoute::ipToBitset(ipAddr);
			ImplicitSequence<NetworkRoute*>* tmp = algp.processRouteTable(networkRoutes->begin(), networkRoutes->end(), [&](NetworkRoute* rt) {
				std::bitset<32> parent = rt->getNetworkAddress();
				for (int i = 0; i < rt->getPrefix(); i++)
				{
					if (parent[31 - i] != compareRt[31 - i]) {
						return false;
					}
				}
				return true;
				});
			delete networkRoutes;
			networkRoutes = tmp;
		}));

	level1.AddItem(new MenuActionItem("matchLifetime", [&]()
		{
			std::cout << "type lower ttl boundary" << endl;
			string lower;
			std::cin >> lower;
			std::cout << "type upper ttl boundary" << endl;
			string upper;
			std::cin >> upper;
			int lowerBorder = stoi(lower);
			int higherBorder = stoi(upper);

			ImplicitSequence<NetworkRoute*>* tmp = algp.processRouteTable(networkRoutes->begin(), networkRoutes->end(), [&](NetworkRoute* rt) {
				if (rt->getTtl() >= lowerBorder && rt->getTtl() <= higherBorder) {
					rt->printRoute();
					return true;
				}
				return false;
				});
			delete networkRoutes;
			networkRoutes = tmp;
		}));


	//settings_menu.AddItem(new MenuActionItem("Print time", &AuxFunctions::printTime));
	//settings_menu.AddItem(new MenuActionItem("Set time", &AuxFunctions::setTime));
	//settings_menu.AddItem(new MenuActionItem("LED test", &lc_testLed));

	// main menu

	//this->main_menu.AddItem(new MenuActionItem("Load CSV", &ConsoleApp::loadData(dataPath, networkRoutes)));
	//this->main_menu.AddItem(&settings_menu);

	// start

	this->main_menu.apply();
}