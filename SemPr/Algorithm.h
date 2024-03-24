#pragma once

#include "NetworkRoute.h"
#include <libds/amt/implicit_sequence.h>

using namespace ds::amt;

class AlgorithmProcessor
{
public:
    template<typename Iterator>
    ImplicitSequence<NetworkRoute*>* processRouteTable(Iterator begin, Iterator end, std::function<boolean(NetworkRoute*)> processFunction);
};

template<typename Iterator>
inline ImplicitSequence<NetworkRoute*>* AlgorithmProcessor::processRouteTable(Iterator begin, Iterator end, std::function<boolean(NetworkRoute*)> processFunction)
{
    ImplicitSequence<NetworkRoute*>* processedSequence = new ImplicitSequence<NetworkRoute*>();
    auto current = begin;
    while (current != end)
    {
        if (processFunction(*current))
        {
            processedSequence->insertLast().data_ = *current;
        }
        ++current;
    }
    return processedSequence;
}
