#include "../include/fixed_block_memory_resource.h"
#include <cassert>
#include <stdexcept>
#include <iostream>


fixed_block_memory_resource::fixed_block_memory_resource(size_t size) : pool_size(size), block_size(0) {
    memory_pool = new char[pool_size];  // Выделяем большой блок
}

fixed_block_memory_resource::~fixed_block_memory_resource() {
    delete[] memory_pool;  // Освобождаем пул
    // std::list освободится сам
}

void* fixed_block_memory_resource::do_allocate(size_t bytes, size_t alignment) {
    // Поиск свободного места в пуле
    for (auto it = blocks.begin(); it != blocks.end(); ++it) {
        if (it->is_free && it->size >= bytes) {
            // Проверяем выровнивание
            uintptr_t addr = reinterpret_cast<uintptr_t>(it->ptr);
            if (addr % alignment == 0) {
                it->is_free = false;
                return it->ptr;
            }
        }
    }
    // Если свободного блока нет, выделяем из "хвоста" пула
    uintptr_t current_addr = reinterpret_cast<uintptr_t>(memory_pool + used_bytes);
    uintptr_t aligned_addr = (current_addr + alignment - 1) & ~(alignment - 1);
    size_t padding = aligned_addr - current_addr; // Вычисляем отступ для выравнивания
    
    // Проверяем, хватает ли места в пуле
    if (used_bytes + padding + bytes > pool_size) {
        throw std::bad_alloc(); // Недостаточно памяти
    }

    // Выделяем память
    void* ptr = reinterpret_cast<void*>(aligned_addr);
    blocks.push_back({ptr, bytes, false});
    used_bytes += padding + bytes;

    return ptr;
}

void fixed_block_memory_resource::do_deallocate(void* p, size_t bytes, size_t alignment) {
    // Находим блок и помечаем его как свободный
    for (auto it = blocks.begin(); it != blocks.end(); ++it) {
        if (it->ptr == p) {
            assert(it->size == bytes && "Deallocating block with incorrect size");
            it->is_free = true;
            return;
        }
    }
    // Если блок не найден, это ошибка
    throw std::invalid_argument("Pointer not allocated by this memory resource");
}

bool fixed_block_memory_resource::do_is_equal(const std::pmr::memory_resource& other) const noexcept {
    return this == &other; // Сравнение по адресу
}

size_t fixed_block_memory_resource::get_used_memory() const {
    return used_bytes;
}

size_t fixed_block_memory_resource::get_free_memory() const {
    return pool_size - used_bytes;
}

void fixed_block_memory_resource::print_allocated_blocks() const {
    size_t index = 0;
    for (const auto& block : blocks) {
        std::cout << "Block " << index++
                  << ": Address=" << block.ptr
                  << ", Size=" << block.size
                  << ", " << (block.is_free ? "Free" : "Allocated") << "\n";
    }
}