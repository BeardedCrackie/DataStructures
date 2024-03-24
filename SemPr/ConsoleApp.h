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

	// ========== main menu ==========
	string dataPath = "C:\\Users\\potoc\\source\\repos\\BeardedCrackie\\DataStructures\\SemPr\\RT.csv";
	ImplicitSequence<NetworkRoute*>* networkRoutes = new ImplicitSequence<NetworkRoute*>();
	AlgorithmProcessor algp = AlgorithmProcessor();

	MenuActionItem itemPrintSequence = MenuActionItem("Print Sequence", [&]()
		{
			algp.processRouteTable(networkRoutes->begin(), networkRoutes->end(), [&](NetworkRoute* rt) {
				rt->printRoute();
				return true;
				});
		});
	main_menu.AddItem(&itemPrintSequence);

	this->main_menu.AddItem(new MenuActionItem("Load CSV", [&]()
		{
			networkRoutes->clear();
			Loader routeLoader = Loader();
			routeLoader.load("C:\\Users\\potoc\\source\\repos\\BeardedCrackie\\DataStructures\\SemPr\\RT.csv", *networkRoutes);
		}
	));

	this->main_menu.AddItem(new MenuActionItem("Clear Sequence", [&]()
		{
			networkRoutes->clear();
		}
	));


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
			networkRoutes = algp.processRouteTable(networkRoutes->begin(), networkRoutes->end(), [&](NetworkRoute* rt) {
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

			networkRoutes = algp.processRouteTable(networkRoutes->begin(), networkRoutes->end(), [&](NetworkRoute* rt) {
				if (rt->getTtl() >= lowerBorder && rt->getTtl() <= higherBorder) {
					return true;
				}
				return false;
				});
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