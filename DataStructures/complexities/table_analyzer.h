#pragma once

#include <complexities/complexity_analyzer.h>
#include <random>
#include <libds/adt/table.h>
#include <libds/adt/queue.h>

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


        size_t getRandomKey() const;
        size_t getRandomData() const;

        size_t key_;
        size_t data_;
        ds::adt::ImplicitStack<size_t> keys_;
        size_t lastKey_;

    private:
        std::default_random_engine rngData_;
        std::default_random_engine rngKey_;
        
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
     * @brief Analyzes complexity of an find.
     */
    template<class Table>
    class TableFindAnalyzer : public TableAnalyzer<Table>
    {
    public:
        explicit TableFindAnalyzer(const std::string& name);

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
        key_(0),
        data_(0),
        keys_()
    {
        lastKey_ = 0;
        this->registerBeforeOperation(
            [this](Table& structure)
            {
                std::uniform_int_distribution dist(1, 10000000);
                this->key_ = dist(this->rngKey_);
                this->data_ = this->rngData_();

                this->lastKey_ = this->keys_.pop();
            }
        );
    }

    template <class Table>
    void TableAnalyzer<Table>::growToSize(Table& structure, size_t size)
    {
        size_t count = size - structure.size();
        std::uniform_int_distribution dist(1, 1000000000);
        for (size_t i = 0; i < count; i++)
        {
            size_t key = dist(this->rngKey_);
            size_t data = this->rngData_();

            while (structure.contains(key)) {
                key = dist(this->rngKey_);
            }
            structure.insert(key, data);
            this->keys_.push(key);
        }
    }

    template<class Table>
    size_t TableAnalyzer<Table>::getRandomKey() const
    {
        return key_;
    }

    template<class Table>
    size_t TableAnalyzer<Table>::getRandomData() const
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
        auto data = this->getRandomData();
        auto key = this->getRandomKey();
        structure.insert(key,data);
    }


    template <class Table>
    TableFindAnalyzer<Table>::TableFindAnalyzer(const std::string& name) :
        TableAnalyzer<Table>(name)
    {
    }

    template <class Table>
    void TableFindAnalyzer<Table>::executeOperation(Table& structure)
    {
        structure.find(this->lastKey_);
    }

    //----------

    inline TablesAnalyzer::TablesAnalyzer() :
        CompositeAnalyzer("Tables")
    {
        this->addAnalyzer(std::make_unique<TableInsertAnalyzer<ds::adt::SortedSequenceTable<size_t, size_t>>>("SST-insert"));
        this->addAnalyzer(std::make_unique<TableInsertAnalyzer<ds::adt::BinarySearchTree<size_t, size_t>>>("BST-insert"));
        
        this->addAnalyzer(std::make_unique<TableFindAnalyzer<ds::adt::SortedSequenceTable<size_t, size_t>>>("SST-access"));

    }

}
