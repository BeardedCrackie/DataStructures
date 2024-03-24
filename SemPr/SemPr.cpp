#pragma once

#include <iostream>
#include <libds/amt/implicit_sequence.h>
#include <windows.h>
#include "ConsoleApp.h"

using namespace ds::amt;

int main()
{
    //initHeapMonitor();
    SetConsoleOutputCP(1250);
    SetConsoleCP(1250);

    //std::cout << "--load routes--" << std::endl;
    //ImplicitSequence<NetworkRoute*> routeSequence = ImplicitSequence<NetworkRoute*>();
    //
    //Loader routeLoader = Loader();
    //routeLoader.load("C:\\Users\\potoc\\source\\repos\\BeardedCrackie\\DataStructures\\SemPr\\RT.csv", routeSequence);
    //
    //std::cout << "--routes algorithm--" << std::endl;
    //
    //AlgorithmProcessor algp = AlgorithmProcessor();
    ////algp.processData(routeSequence.begin(), routeSequence.end(), [&](NetworkRoute* rt) {
    ////        rt->printRoute();
    ////        return true;
    ////    });
    //
    //std::bitset<32> compareRt = NetworkRoute::ipToBitset("1.0.16.0");
    //
    //std::cout << "--matchWithAddress--" << std::endl;
    //algp.processRouteTable(routeSequence.begin(), routeSequence.end(), [&](NetworkRoute* rt) {
    //    //matchWithAddress
    //    std::bitset<32> parent = rt->getNetworkAddress();
    //    for (int i = 0; i < rt->getPrefix(); i++)
    //    {
    //        if (parent[31 - i] != compareRt[31 - i]) {
    //            return false;
    //        }
    //    }
    //    rt->printRoute();
    //    return true;
    //});
    //
    //int lowerBorder = 100;
    //int higherBorder = 10000;
    //std::cout << "--matchLifetime--" << std::endl;
    //algp.processRouteTable(routeSequence.begin(), routeSequence.end(), [&](NetworkRoute* rt) {
    //    //matchLifetime
    //    if (rt->getTtl() >= 100 && rt->getTtl() <= higherBorder) {
    //        rt->printRoute();
    //        return true;
    //    }
    //    return false;
    //});
    //
    //std::cout << "Done";

    ConsoleApp().Start();

}   
    
    
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
