#include <gtest/gtest.h>
#include "../include/doubly_linked_list.h"
#include "../include/fixed_block_memory_resource.h"
#include <iterator>
#include <algorithm>
#include <numeric>

// Тест 1: Типы итератора
TEST(IteratorTest, TypeTraits) {
    fixed_block_memory_resource mr(1024);
    doubly_linked_list<int> list(&mr);
    
    using iterator = doubly_linked_list<int>::iterator;
    
    // Проверяем, что iterator_category правильный
    bool is_forward = std::is_same<
        typename std::iterator_traits<iterator>::iterator_category,
        std::forward_iterator_tag
    >::value;
    
    EXPECT_TRUE(is_forward);
}

// Тест 2: Разыменование
TEST(IteratorTest, Dereference) {
    fixed_block_memory_resource mr(1024);
    doubly_linked_list<int> list(&mr);
    
    list.push_back(42);
    
    auto it = list.begin();
    EXPECT_EQ(*it, 42);
    
    // Изменяем через итератор
    *it = 100;
    EXPECT_EQ(*it, 100);
}

// Тест 3: Оператор стрелки
TEST(IteratorTest, ArrowOperator) {
    fixed_block_memory_resource mr(1024);
    
    struct Point {
        int x, y;
        Point(int x_, int y_) : x(x_), y(y_) {}
    };
    
    doubly_linked_list<Point> list(&mr);
    list.push_back(Point(10, 20));
    
    auto it = list.begin();
    EXPECT_EQ(it->x, 10);
    EXPECT_EQ(it->y, 20);
    
    // Изменяем через стрелку
    it->x = 30;
    EXPECT_EQ(it->x, 30);
}

// Тест 4: Префиксный инкремент
TEST(IteratorTest, PrefixIncrement) {
    fixed_block_memory_resource mr(1024);
    doubly_linked_list<int> list(&mr);
    
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    
    auto it = list.begin();
    EXPECT_EQ(*it, 1);
    
    ++it;
    EXPECT_EQ(*it, 2);
    
    ++it;
    EXPECT_EQ(*it, 3);
    
    ++it;
    EXPECT_EQ(it, list.end());
}

// Тест 5: Постфиксный инкремент
TEST(IteratorTest, PostfixIncrement) {
    fixed_block_memory_resource mr(1024);
    doubly_linked_list<int> list(&mr);
    
    list.push_back(10);
    list.push_back(20);
    
    auto it = list.begin();
    auto old_it = it++;
    
    EXPECT_EQ(*old_it, 10); // Старый итератор указывает на 10
    EXPECT_EQ(*it, 20);     // Новый итератор указывает на 20
}

// Тест 6: Сравнение итераторов
TEST(IteratorTest, Comparison) {
    fixed_block_memory_resource mr(1024);
    doubly_linked_list<int> list(&mr);
    
    list.push_back(1);
    list.push_back(2);
    
    auto it1 = list.begin();
    auto it2 = list.begin();
    
    EXPECT_TRUE(it1 == it2);
    EXPECT_FALSE(it1 != it2);
    
    ++it2;
    EXPECT_FALSE(it1 == it2);
    EXPECT_TRUE(it1 != it2);
}

// Тест 7: Пустой список
TEST(IteratorTest, EmptyList) {
    fixed_block_memory_resource mr(1024);
    doubly_linked_list<int> list(&mr);
    
    EXPECT_EQ(list.begin(), list.end());
    
    // Итерация по пустому списку не должна выполниться
    int count = 0;
    for (auto it = list.begin(); it != list.end(); ++it) {
        ++count;
    }
    EXPECT_EQ(count, 0);
}

// Тест 8: Один элемент
TEST(IteratorTest, SingleElement) {
    fixed_block_memory_resource mr(1024);
    doubly_linked_list<int> list(&mr);
    
    list.push_back(42);
    
    auto it = list.begin();
    EXPECT_NE(it, list.end());
    EXPECT_EQ(*it, 42);
    
    ++it;
    EXPECT_EQ(it, list.end());
}

// Тест 9: Использование в std::distance
TEST(IteratorTest, StdDistance) {
    fixed_block_memory_resource mr(1024);
    doubly_linked_list<int> list(&mr);
    
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.push_back(4);
    list.push_back(5);
    
    auto dist = std::distance(list.begin(), list.end());
    EXPECT_EQ(dist, 5);
}

// Тест 10: Использование в std::find
TEST(IteratorTest, StdFind) {
    fixed_block_memory_resource mr(1024);
    doubly_linked_list<int> list(&mr);
    
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);
    list.push_back(40);
    
    auto it = std::find(list.begin(), list.end(), 30);
    EXPECT_NE(it, list.end());
    EXPECT_EQ(*it, 30);
    
    auto it_not_found = std::find(list.begin(), list.end(), 999);
    EXPECT_EQ(it_not_found, list.end());
}

// Тест 11: Использование в std::count
TEST(IteratorTest, StdCount) {
    fixed_block_memory_resource mr(1024);
    doubly_linked_list<int> list(&mr);
    
    list.push_back(1);
    list.push_back(2);
    list.push_back(2);
    list.push_back(3);
    list.push_back(2);
    
    auto count = std::count(list.begin(), list.end(), 2);
    EXPECT_EQ(count, 3);
}

// Тест 12: Использование в std::accumulate
TEST(IteratorTest, StdAccumulate) {
    fixed_block_memory_resource mr(1024);
    doubly_linked_list<int> list(&mr);
    
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.push_back(4);
    list.push_back(5);
    
    int sum = std::accumulate(list.begin(), list.end(), 0);
    EXPECT_EQ(sum, 15);
}

// Тест 13: Модификация через итератор
TEST(IteratorTest, ModificationThroughIterator) {
    fixed_block_memory_resource mr(1024);
    doubly_linked_list<int> list(&mr);
    
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    
    // Умножаем каждый элемент на 2
    for (auto it = list.begin(); it != list.end(); ++it) {
        *it *= 2;
    }
    
    auto it = list.begin();
    EXPECT_EQ(*it, 2);
    ++it;
    EXPECT_EQ(*it, 4);
    ++it;
    EXPECT_EQ(*it, 6);
}

// Тест 14: Range-based for с модификацией
TEST(IteratorTest, RangeBasedForModification) {
    fixed_block_memory_resource mr(1024);
    doubly_linked_list<int> list(&mr);
    
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);
    
    // Увеличиваем каждый элемент на 5
    for (int& value : list) {
        value += 5;
    }
    
    std::vector<int> expected = {15, 25, 35};
    int index = 0;
    for (const int& value : list) {
        EXPECT_EQ(value, expected[index++]);
    }
}

// Тест 15: Const reference для списка
TEST(IteratorTest, ConstIteration) {
    fixed_block_memory_resource mr(1024);
    doubly_linked_list<int> list(&mr);
    
    list.push_back(100);
    list.push_back(200);
    
    // Используем обычный range-based for
    int sum = 0;
    for (const auto& value : list) {
        sum += value;
    }
    
    EXPECT_EQ(sum, 300);
}

// Тест 16: Копирование итератора
TEST(IteratorTest, IteratorCopy) {
    fixed_block_memory_resource mr(1024);
    doubly_linked_list<int> list(&mr);
    
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    
    auto it1 = list.begin();
    auto it2 = it1; // Копирование
    
    EXPECT_EQ(it1, it2);
    EXPECT_EQ(*it1, *it2);
    
    ++it1;
    EXPECT_NE(it1, it2);
    EXPECT_EQ(*it2, 1); // it2 не изменился
}

// Тест 17: Несколько итераторов одновременно
TEST(IteratorTest, MultipleIterators) {
    fixed_block_memory_resource mr(1024);
    doubly_linked_list<int> list(&mr);
    
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.push_back(4);
    
    auto it1 = list.begin();
    auto it2 = list.begin();
    ++it2;
    ++it2; // it2 на третьем элементе
    
    EXPECT_EQ(*it1, 1);
    EXPECT_EQ(*it2, 3);
    
    ++it1;
    EXPECT_EQ(*it1, 2);
    EXPECT_EQ(*it2, 3); // it2 не изменился
}
