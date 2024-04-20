#pragma once

#include <libds/heap_monitor.h>
#include "NetworkRoute.h"
#include <libds/amt/implicit_sequence.h>

using namespace ds::amt;

template<typename T>
class AlgorithmProcessor
{
public:
    AlgorithmProcessor();
    ~AlgorithmProcessor();
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
inline AlgorithmProcessor<T>::~AlgorithmProcessor()
{
    delete networkRoutes;
    networkRoutes = nullptr;
}

template<typename T>
template<typename Iterator>
inline ImplicitSequence<NetworkRoute*>* AlgorithmProcessor<T>::processRouteTable(Iterator begin, Iterator end, std::function<boolean(NetworkRoute*)> processFunction)
{
    for (auto current = begin; current != end; ++current) {
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


