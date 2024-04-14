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
	ConsoleApp() : main_menu("Main menu") {};
	void Start();
};

void ConsoleApp::Start() {

	// ========== initialization ==========
	networkRoutes = new ImplicitSequence<NetworkRoute*>();


	// === routes loading ===
	string dataPath = "C:\\Users\\potoc\\source\\repos\\BeardedCrackie\\DataStructures\\SemPr\\RT.csv";
	Loader().load(dataPath, *networkRoutes);


	// ========== main menu ==========
	AlgorithmProcessor algp = AlgorithmProcessor<NetworkRoute*>();

	main_menu.AddItem(new MenuActionItem("Print loaded networks", [&]()
		{
			ImplicitSequence<NetworkRoute*>* tmp = AlgorithmProcessor<ImplicitSequence<NetworkRoute*>>().processRouteTable(networkRoutes->begin(), networkRoutes->end(), [&](NetworkRoute* rt) {
				rt->printRoute();
				return true;
				});
		}));


	// ========== level 1 ==========
	CliMenu level1("1 level");
	main_menu.AddItem(&level1);

	level1.AddItem(new MenuActionItem("matchWithAddress", [&]()
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

			algp.processRouteTable(networkRoutes->begin(), networkRoutes->end(), [&](NetworkRoute* rt) {
				if (rt->getTtl() >= lowerBorder && rt->getTtl() <= higherBorder) {
					return true;
				}
				return false;
				});
		}));

	level1.AddItem(new MenuActionItem("print routes", [&]()
		{
			algp.printRoutes();
		}));

	level1.AddItem(new MenuActionItem("flush algorithm", [&]()
		{
			algp.flush();
		}));

	this->main_menu.apply();
}