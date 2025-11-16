#include <memory_resource>
#include <list>
#pragma once

class fixed_block_memory_resource : public std::pmr::memory_resource {
    private:
        struct MemoryBlock {
            void* ptr{nullptr};
            size_t size{0};
            bool is_free{true};
        };

        static constexpr size_t BUFFER_SIZE{1024 * 1024}; // 1 MB
        char* memory_pool;
        size_t pool_size; // Размер всего пула
        size_t used_bytes{0}; // Количество использованных байт
        std::list<MemoryBlock> blocks;
        size_t block_size;

    protected:
        // Аллкатор вызывает эти методы внутри себя
        // do_allocate выделяет память из memory_pool под заданные размеры и выравнивание
        void* do_allocate(size_t bytes, size_t alignment) override;
        // do_deallocate освобождает память, возвращая её в memory_pool, а не удаляя её
        void do_deallocate(void* p, size_t bytes, size_t alignment) override;
        // do_is_equal - сравнивает два аллокатора между собой
        bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override;
    
    public:
        // Конструктор и деструктор
        explicit fixed_block_memory_resource(size_t size = BUFFER_SIZE);
        ~fixed_block_memory_resource();

        // Запрет копирования
        fixed_block_memory_resource(const fixed_block_memory_resource&) = delete;
        fixed_block_memory_resource& operator=(const fixed_block_memory_resource&) = delete;

        // Статистика для отладки
        size_t get_used_memory() const;
        size_t get_free_memory() const;

        void print_allocated_blocks() const;
};