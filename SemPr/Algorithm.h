#pragma once

#include "NetworkRoute.h"
#include <libds/amt/implicit_sequence.h>

using namespace ds::amt;

template<typename T>
class AlgorithmProcessor
{
public:
    AlgorithmProcessor();
    template<typename Iterator>
    ImplicitSequence<NetworkRoute*>* processRouteTable(Iterator begin, Iterator end, std::function<boolean(NetworkRoute*)> processFunction);
    void printRoutes();
    void flush();
private:
    ImplicitSequence<NetworkRoute*>* networkRoutes;
};

template<typename T>
inline AlgorithmProcessor<T>::AlgorithmProcessor()
{
    networkRoutes = new ImplicitSequence<NetworkRoute*>();
}

template<typename T>
template<typename Iterator>
inline ImplicitSequence<NetworkRoute*>* AlgorithmProcessor<T>::processRouteTable(Iterator begin, Iterator end, std::function<boolean(NetworkRoute*)> processFunction)
{
    ImplicitSequence<NetworkRoute*>* processedSequence = new ImplicitSequence<NetworkRoute*>();
    auto current = begin;
    while (current != end)
    {
        if (processFunction(*current))
        {
            networkRoutes->insertLast().data_ = *current;
        }
        ++current;
    }
    return networkRoutes;
}

template<typename T>
inline void AlgorithmProcessor<T>::printRoutes()
{
    /*
    this.processRouteTable(networkRoutes->begin(), networkRoutes->end(), [&](NetworkRoute* rt) {
        rt->printRoute();
        return true;
        });
    */

    for (auto current = networkRoutes->begin(); current != networkRoutes->end(); ++current) {
        NetworkRoute* rt = *current;
        rt->printRoute();
    }
}

template<typename T>
inline void AlgorithmProcessor<T>::flush()
{
    networkRoutes->clear();
}


