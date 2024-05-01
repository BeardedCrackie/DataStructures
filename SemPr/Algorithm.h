#pragma once

#include <libds/heap_monitor.h>
#include "NetworkRoute.h"
#include <libds/amt/implicit_sequence.h>
#include <libds/amt/explicit_hierarchy.h>
#include "simpleLogger.h"

using namespace ds::amt;

template<typename T>
class AlgorithmProcessor
{
public:
    AlgorithmProcessor();
    ~AlgorithmProcessor();
    template<typename Iterator>
    ImplicitSequence<NetworkRoute*>* process(Iterator begin, Iterator end, std::function<boolean(NetworkRoute*)> processFunction);
    //template<typename Iterator>
    //ImplicitSequence<NetworkRoute*>* process(Iterator begin, Iterator end, std::function<boolean(NetworkRoute*&)> processFunction);

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
inline ImplicitSequence<NetworkRoute*>* AlgorithmProcessor<T>::process(Iterator begin, Iterator end, std::function<boolean(NetworkRoute*)> processFunction)
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

template<>
template<typename Iterator>
inline ImplicitSequence<NetworkRoute*>* AlgorithmProcessor<MultiWayExplicitHierarchy<NetworkHierarchyBlock>>::process(Iterator begin, Iterator end, std::function<boolean(NetworkRoute*)> processFunction)
{
    SimpleLogger::log(LOG_INFO, "called: AlgorithmProcessor<MultiWayExplicitHierarchy<NetworkHierarchyBlock>>::process");

    NetworkHierarchyBlock item;

    for (auto current = begin; current != end; ++current) {
        item = *current;
        if (item.route != nullptr && processFunction(item.route))
        {
            networkRoutes->insertLast().data_ = item.route;
        }
//        ++current;
    }
    return networkRoutes;
}

/*
template<typename T>
inline void AlgorithmProcessor<T>::printRoutes()
{
    SimpleLogger::log(LOG_INFO, "called: AlgorithmProcessor<T>::printRoutes");

    this->process(this->networkRoutes->begin(), this->networkRoutes->end(), [](NetworkRoute* rt) {
        rt->printRoute();
        return true;
        });

    
    /*
    if (networkRoutes->size() > 0) {
        for (auto current = networkRoutes->begin(); current != networkRoutes->end(); ++current) {
            T* rt = *current;
            rt->printRoute();
        }
    }
    else {
        std::cout << "List is Empty" << std::endl;
    }*/
/*
}
*/

template<typename T>
inline void AlgorithmProcessor<T>::flush()
{
    SimpleLogger::log(LOG_DEBUG, "called: AlgorithmProcessor<T>::flush");
    networkRoutes->clear();
}


class Predicate
{
public:
    static boolean matchWithAddress(std::bitset<32> compareRtFrom, NetworkRoute*& compareRtTo, bool print = true) {
        std::bitset<32> parent = compareRtTo->getNetworkAddress();
        for (size_t i = 0; i < compareRtTo->getPrefix(); i++)
        {
            if (parent[31 - i] != compareRtFrom[31 - i]) {
                return false;
            }
        }
        if (print) {
            compareRtTo->printRoute();
        }
        return true;
    }

    static boolean matchLifetime(int lowerTtl, int upperTtl, NetworkRoute*& compareRtTo, bool print = true) {
        if (compareRtTo->getTtl() >= lowerTtl && compareRtTo->getTtl() <= upperTtl) {
            if (print) {
                compareRtTo->printRoute();
            }
            return true;
        }
        return false;
    }

    static boolean print(NetworkRoute*& route) {
        route->printRoute();
        return true;
    }

};
