#pragma once

#include <libds/heap_monitor.h>
#include <iostream>
#include <libds/amt/implicit_sequence.h>
#include <windows.h>
#include "ConsoleApp.h"
#include "SemPr.h"


using namespace ds::amt;

int main()
{
    initHeapMonitor();
    SetConsoleOutputCP(1250);
    SetConsoleCP(1250);

	/*
	ImplicitSequence<NetworkRoute*>* networkRoutes = new ImplicitSequence<NetworkRoute*>();
	//AlgorithmProcessor algp = AlgorithmProcessor<NetworkRoute*>();
	Loader().load("C:\\Users\\potoc\\source\\repos\\BeardedCrackie\\DataStructures\\SemPr\\RT.csv", *networkRoutes);


	//networkRoutes->insertLast().data_ = new NetworkRoute();

	for (size_t i = 0; i < networkRoutes->size(); ++i) {
		delete networkRoutes->access(i)->data_;
	}

	networkRoutes->clear();

	delete networkRoutes;


	//delete networkRoutes;
	*/

    ConsoleApp().Start();
    //capp.Start();
    //capp.Stop();

}   
    