#pragma once

#include <libds/heap_monitor.h>
#include "NetworkRoute.h"
#include <libds/amt/implicit_sequence.h>
#include "simpleLogger.h"

using namespace ds::amt;

template<typename T>
class Comparator
{
public:
    bool comparePrefix(NetworkRoute firstRoute, NetworkRoute secondRoute);
    bool compareTime(NetworkRoute firstRoute, NetworkRoute secondRoute);
};

template<typename T>
inline bool AlgorithmSorter<T>::comparePrefix(NetworkRoute firstRoute, NetworkRoute secondRoute)
{
    return false;
}

template<typename T>
inline bool AlgorithmSorter<T>::compareTime(NetworkRoute firstRoute, NetworkRoute secondRoute)
{
    return false;
}
