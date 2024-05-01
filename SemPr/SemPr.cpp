#pragma once

#include <libds/heap_monitor.h>
#include <iostream>
#include <libds/amt/implicit_sequence.h>
#include <windows.h>
#include "ConsoleApp.h"
#include "simpleLogger.h"


using namespace ds::amt;

int main()
{
    LOGLEVEL = LOG_DEBUG;

    initHeapMonitor();
    SetConsoleOutputCP(1250);
    SetConsoleCP(1250);

    ConsoleApp().Start();
    //capp.Start();
    //capp.Stop();
	
	//just for initHeapMonitor testing
	//NetworkRoute* rt = new NetworkRoute();

}   
    