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
    template<typename Iterator>
    ImplicitSequence<NetworkRoute*>* processHierarchy(Iterator begin, Iterator end, std::function<boolean(NetworkHierarchyBlock&)> processFunction);
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
    SimpleLogger::log(LOG_INFO, "called: AlgorithmProcessor<T>::process");

    for (auto current = begin; current != end; ++current) {
        if (processFunction(*current))
        {
            networkRoutes->insertLast().data_ = *current;
        }
        //++current;
    }
    return networkRoutes;
}


template<typename T>
template<typename Iterator>
inline ImplicitSequence<NetworkRoute*>* AlgorithmProcessor<T>::processHierarchy(Iterator begin, Iterator end, std::function<boolean(NetworkHierarchyBlock&)> processFunction)
{
    SimpleLogger::log(LOG_INFO, "called: AlgorithmProcessor<MultiWayExplicitHierarchy<NetworkHierarchyBlock>>::process");
        NetworkHierarchyBlock network = static_cast<NetworkHierarchyBlock>(*current);
        if (network.octetValue < 256 && network.route != nullptr && processFunction(*current))
        {
            networkRoutes->insertLast().data_ =network.route;
        }
//        ++current;
    }
    return networkRoutes;
}

template<typename T>
inline void AlgorithmProcessor<T>::printRoutes()
{
    if (networkRoutes->size() > 0) {
        for (auto current = networkRoutes->begin(); current != networkRoutes->end(); ++current) {
            NetworkRoute* rt = *current;
            rt->printRoute();
        }
    }
    else {
        std::cout << "List is Empty" << std::endl;
    }
}

template<typename T>
inline void AlgorithmProcessor<T>::flush()
{
    SimpleLogger::log(LOG_DEBUG, "called: AlgorithmProcessor<T>::flush");
    networkRoutes->clear();
}


