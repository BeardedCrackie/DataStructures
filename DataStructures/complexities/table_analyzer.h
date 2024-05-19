#pragma once

#include <complexities/complexity_analyzer.h>
#include <random>
#include <libds/adt/table.h>

namespace ds::utils
{
    /**
     * @brief Common base for table analyzers.
     */
    template<class Table>
    class TableAnalyzer : public ComplexityAnalyzer<Table>
    {
    protected:
        explicit TableAnalyzer(const std::string& name);

    protected:
        void growToSize(Table& structure, size_t size) override;

        size_t getRandomIndex() const;
        int getRandomData() const;

        int index_;
        int data_;

    private:
        std::default_random_engine rngData_;
        std::default_random_engine rngKey_;
        
        //todo vytvorit sekvenciu klucov

    };

    /**
     * @brief Analyzes complexity of an insertion at the beginning.
     */
    template<class Table>
    class TableInsertAnalyzer : public TableAnalyzer<Table>
    {
    public:
        explicit TableInsertAnalyzer(const std::string& name);

    protected:
        void executeOperation(Table& structure) override;
    };


    /**
     * @brief Container for all table analyzers.
     */
    class TablesAnalyzer : public CompositeAnalyzer
    {
    public:
        TablesAnalyzer();
    };

    //----------

    template<class Table>
    TableAnalyzer<Table>::TableAnalyzer(const std::string& name) :
        ComplexityAnalyzer<Table>(name),
        rngData_(144),
        rngKey_(144),
        index_(0),
        data_(0)
    {
        this->registerBeforeOperation(
            [this](Table& structure)
            {
                std::uniform_int_distribution dist(1, 10000000);
                this->key_ = dist(this->rngKey_);
                this->data_ = this->rngData_();
                while (structure.contains(key)) {
                    this->key_ = dist(this->rngKey_);
                }
            }
        )
    }

    template <class Table>
    void TableAnalyzer<Table>::growToSize(Table& structure, size_t size)
    {
        size_t count = size - structure.size();
        std::uniform_int_distribution dist(1, 10000000);
        for (size_t i = 0; i < count; i++)
        {
            int key = dist(this->rngKey_);
            int data = this->rngData_();
            
            while (structure.contains(key)) {
                key = dist(this->rngKey_);
            }
            structure.insert(key, data);
        }
    }

    template<class Table>
    size_t TableAnalyzer<Table>::getRandomIndex() const
    {
        return index_;
    }

    template<class Table>
    int TableAnalyzer<Table>::getRandomData() const
    {
        return data_;
    }

    //----------

    template <class Table>
    TableInsertAnalyzer<Table>::TableInsertAnalyzer(const std::string& name) :
        TableAnalyzer<Table>(name)
    {
    }

    template <class Table>
    void TableInsertAnalyzer<Table>::executeOperation(Table& structure)
    {
        structure.insert(structure.begin(), this->getRandomData());
    }


    //----------

    inline TablesAnalyzer::TablesAnalyzer() :
        CompositeAnalyzer("Tables")
    {
        this->addAnalyzer(std::make_unique<TableInsertAnalyzer<ds::adt::SortedSequenceTable<int,int>>>("SST-insert"));
        this->addAnalyzer(std::make_unique<TableInsertAnalyzer<ds::adt::BinarySearchTree<int,int>>>("BST-insert"));
    }
