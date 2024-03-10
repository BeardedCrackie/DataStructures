#pragma once

#include <libds/mm/memory_manager.h>
#include <libds/mm/memory_omanip.h>
#include <libds/constants.h>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <ostream>
#include <utility>

namespace ds::mm {

    template<typename BlockType>
    class CompactMemoryManager : public MemoryManager<BlockType> {
    public:
        CompactMemoryManager();
        CompactMemoryManager(size_t size);
        CompactMemoryManager(const CompactMemoryManager<BlockType>& other);
        ~CompactMemoryManager() override;

        BlockType* allocateMemory() override;
        BlockType* allocateMemoryAt(size_t index);
        void releaseMemory(BlockType* pointer) override;
        void releaseMemoryAt(size_t index);
        void releaseMemory();

        size_t getCapacity() const;

        CompactMemoryManager<BlockType>& assign(const CompactMemoryManager<BlockType>& other);
        void changeCapacity(size_t newCapacity);
        void shrinkMemory();
        void clear();
        bool equals(const CompactMemoryManager<BlockType>& other) const;
        void* calculateAddress(const BlockType& data);
        size_t calculateIndex(const BlockType& data);
        BlockType& getBlockAt(size_t index);
        void swap(size_t index1, size_t index2);

        void print(std::ostream& os);

    private:
        size_t getAllocatedBlocksSize() const;
        size_t getAllocatedCapacitySize() const;

    private:
        BlockType* base_;
        BlockType* end_;
        BlockType* limit_;

        static const size_t INIT_SIZE = 4;
    };

    template<typename BlockType>
    CompactMemoryManager<BlockType>::CompactMemoryManager() :
        CompactMemoryManager(INIT_SIZE)
    {
    }

    template<typename BlockType>
    CompactMemoryManager<BlockType>::CompactMemoryManager(size_t size) :
        base_(static_cast<BlockType*>(std::calloc(size, sizeof(BlockType)))),
        end_(base_),
        limit_(base_ + size)
    {
    }

    template<typename BlockType>
    CompactMemoryManager<BlockType>::CompactMemoryManager(const CompactMemoryManager<BlockType>& other) :
        CompactMemoryManager(other.getAllocatedBlockCount())
    {
        this->assign(other);
    }

    template<typename BlockType>
    CompactMemoryManager<BlockType>::~CompactMemoryManager()
    {
        this->releaseMemory(this->base_);
        std::free(this->base_);
        this->base_ = nullptr;
        this->end_ = nullptr;
        this->limit_ = nullptr;
    }

    template<typename BlockType>
    BlockType* CompactMemoryManager<BlockType>::allocateMemory()
    {
        return this->allocateMemoryAt(this->end_ - this->base_);
    }

    template<typename BlockType>
    BlockType* CompactMemoryManager<BlockType>::allocateMemoryAt(size_t index)
    {
        if (this->end_ == this->limit_) {
            this->changeCapacity(this->allocatedBlockCount_ == 0 ? 10 : 2 * this->allocatedBlockCount_);
        }
        BlockType* adr = this->base_ + index;
        if (adr != this->end_) {
            std::memmove(
                adr + 1, 
                adr, 
                (this->end_ - adr) * sizeof(BlockType)
            );
        }
        placement_new(adr);
        ++this->end_;
        ++this->allocatedBlockCount_;
        return adr;
    }

    template<typename BlockType>
    void CompactMemoryManager<BlockType>::releaseMemory(BlockType* pointer)
    {
        BlockType* cur = pointer;
        while (cur != this->end_) {
            destroy(cur);
            ++cur;
        }
        this->end_ = pointer;
        this->allocatedBlockCount_ = this->end_ - this->base_;
    }

    template<typename BlockType>
    void CompactMemoryManager<BlockType>::releaseMemoryAt(size_t index)
    {
        BlockType* adr = this->base_ + index;
        adr->~BlockType();
        if (adr + 1 < this->end_) {
            std::memmove(adr, adr + 1, (this->end_ - adr - 1) * sizeof(BlockType));
        }
        --this->end_;
        --this->allocatedBlockCount_;
    }

    template<typename BlockType>
    void CompactMemoryManager<BlockType>::releaseMemory()
    {
        this->releaseMemoryAt(this->end_ - this->base_ - 1);
    }

    template<typename BlockType>
    size_t CompactMemoryManager<BlockType>::getCapacity() const
    {
        return this->limit_ - this->base_;
    }

    template<typename BlockType>
    CompactMemoryManager<BlockType>& CompactMemoryManager<BlockType>::assign
    (const CompactMemoryManager<BlockType>& other)
    {
        // TODO 02 assign //na doma
        // po implementacii vymazte vyhodenie vynimky!
        throw std::runtime_error("Not implemented yet");
    }

    template<typename BlockType>
    void CompactMemoryManager<BlockType>::shrinkMemory()
    {
        size_t newCapacity = static_cast<size_t>(end_ - base_);

        if (newCapacity < CompactMemoryManager<BlockType>::INIT_SIZE)
        {
            newCapacity = CompactMemoryManager<BlockType>::INIT_SIZE;
        }

        this->changeCapacity(newCapacity);
    }

    template<typename BlockType>
    void CompactMemoryManager<BlockType>::changeCapacity(size_t newCapacity)
    {
        if (newCapacity == this->getCapacity()) {
            return;
        }
        BlockType* tmp = nullptr;
        if (newCapacity > this->getCapacity()) {
            tmp = static_cast<BlockType*>(std::realloc(this->base_, newCapacity * sizeof(BlockType)));
        }
        else {
            if (newCapacity < this->allocatedBlockCount_) {
                this->releaseMemory(this->base_ + newCapacity);
            }
            tmp = static_cast<BlockType*>(std::realloc(this->base_, newCapacity * sizeof(BlockType)));
        }
        if (tmp != this->base_) {
            this->base_ = tmp;
            this->end_ = this->base_ + this->allocatedBlockCount_;
        }
        this->limit_ = this->base_ + newCapacity;
    }

    template<typename BlockType>
    void CompactMemoryManager<BlockType>::clear()
    {
        this->releaseMemory(this->base_);
    }

    template<typename BlockType>
    bool CompactMemoryManager<BlockType>::equals(const CompactMemoryManager<BlockType>& other) const
    {
        return this == &other ||
            (this->allocatedBlockCount_ == other.allocatedBlockCount_ &&
                memcmp(this->base_, other.base_, this->allocatedBlockCount_ * sizeof(BlockType)));
    }

    template<typename BlockType>
    void* CompactMemoryManager<BlockType>::calculateAddress(const BlockType& data)
    {
        BlockType* cur = this->base_;
        while (cur != this->end_)
        {
            if (cur == &data) {
                return cur;
            } 
            ++cur;
        }
        return nullptr;
    }

    template<typename BlockType>
    size_t CompactMemoryManager<BlockType>::calculateIndex(const BlockType& data)
    {
        if (&data >= this->base_ && &data < this->end_) {
            return &data - this->base_;
        }
        return INVALID_INDEX;
    }

    template<typename BlockType>
    BlockType& CompactMemoryManager<BlockType>::getBlockAt(size_t index)
    {
        return *(this->base_ + index);
    }

    template<typename BlockType>
    void CompactMemoryManager<BlockType>::swap(size_t index1, size_t index2)
    {
        std::swap(this->getBlockAt(index1), this->getBlockAt(index2));
    }

    template<typename BlockType>
    size_t CompactMemoryManager<BlockType>::getAllocatedBlocksSize() const
    {
        return this->allocatedBlockCount_ * sizeof(BlockType);
    }

    template<typename BlockType>
    size_t CompactMemoryManager<BlockType>::getAllocatedCapacitySize() const
    {
        return this->getCapacity() * sizeof(BlockType);
    }

    template<typename BlockType>
    void CompactMemoryManager<BlockType>::print(std::ostream& os)
    {
        os << "first = " << base_ << std::endl;
        os << "last = " << end_ << std::endl;
        os << "limit = " << limit_ << std::endl;
        os << "block size = " << sizeof(BlockType) << "B" << std::endl;

        BlockType* ptr = base_;
        while (ptr != limit_)
        {
            std::cout << ptr;
            os << PtrPrintBin<BlockType>(ptr);

            if (ptr == base_) {
                os << "<- first";
            }
            else if (ptr == end_) {
                os << "<- last";
            }
            os << std::endl;
            ++ptr;
        }

        os << limit_ << "|<- limit" << std::endl;
    }

}