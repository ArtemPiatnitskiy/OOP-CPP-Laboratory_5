#include <gtest/gtest.h>
#include "../include/doubly_linked_list.h"
#include "../include/fixed_block_memory_resource.h"
#include <string>
#include <sstream>

// Структура Animal для тестирования
struct Animal {
    std::string name;
    int age;
    double weight;
    
    Animal() : name(""), age(0), weight(0.0) {}
    
    Animal(const std::string& n, int a, double w)
        : name(n), age(a), weight(w) {}
    
    bool operator==(const Animal& other) const {
        return name == other.name && age == other.age && 
               std::abs(weight - other.weight) < 0.001;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Animal& animal) {
        os << "Animal{" << animal.name << ", " << animal.age 
           << " years, " << animal.weight << " kg}";
        return os;
    }
};

// Структура Car для тестирования
struct Car {
    std::string brand;
    std::string model;
    int year;
    double price;
    
    Car() : brand(""), model(""), year(0), price(0.0) {}
    
    Car(const std::string& b, const std::string& m, int y, double p)
        : brand(b), model(m), year(y), price(p) {}
    
    bool operator==(const Car& other) const {
        return brand == other.brand && model == other.model && 
               year == other.year && std::abs(price - other.price) < 0.001;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Car& car) {
        os << "Car{" << car.brand << " " << car.model << ", " 
           << car.year << ", $" << car.price << "}";
        return os;
    }
};

// Тесты с Animal
TEST(StructTest, AnimalBasic) {
    fixed_block_memory_resource mr(4096);
    doubly_linked_list<Animal> list(&mr);
    
    Animal cat("Whiskers", 3, 4.5);
    Animal dog("Rex", 5, 25.0);
    Animal bird("Tweety", 1, 0.05);
    
    list.push_back(cat);
    list.push_back(dog);
    list.push_back(bird);
    
    EXPECT_EQ(list.size(), 3);
}

TEST(StructTest, AnimalIteration) {
    fixed_block_memory_resource mr(4096);
    doubly_linked_list<Animal> list(&mr);
    
    list.push_back(Animal("Cat", 2, 3.5));
    list.push_back(Animal("Dog", 4, 20.0));
    list.push_back(Animal("Hamster", 1, 0.2));
    
    std::vector<Animal> expected = {
        Animal("Cat", 2, 3.5),
        Animal("Dog", 4, 20.0),
        Animal("Hamster", 1, 0.2)
    };
    
    int index = 0;
    for (const auto& animal : list) {
        EXPECT_EQ(animal, expected[index]);
        ++index;
    }
}

TEST(StructTest, AnimalArrowOperator) {
    fixed_block_memory_resource mr(4096);
    doubly_linked_list<Animal> list(&mr);
    
    list.push_back(Animal("Leo", 7, 190.0));
    
    auto it = list.begin();
    EXPECT_EQ(it->name, "Leo");
    EXPECT_EQ(it->age, 7);
    EXPECT_DOUBLE_EQ(it->weight, 190.0);
}

TEST(StructTest, AnimalClear) {
    fixed_block_memory_resource mr(4096);
    doubly_linked_list<Animal> list(&mr);
    
    for (int i = 0; i < 10; ++i) {
        list.push_back(Animal("Animal" + std::to_string(i), i, i * 1.5));
    }
    
    EXPECT_EQ(list.size(), 10);
    
    list.clear();
    EXPECT_EQ(list.size(), 0);
    EXPECT_TRUE(list.empty());
}

// Тесты с Car
TEST(StructTest, CarBasic) {
    fixed_block_memory_resource mr(4096);
    doubly_linked_list<Car> list(&mr);
    
    Car toyota("Toyota", "Camry", 2022, 25000.0);
    Car honda("Honda", "Accord", 2021, 24000.0);
    Car bmw("BMW", "X5", 2023, 60000.0);
    
    list.push_back(toyota);
    list.push_back(honda);
    list.push_back(bmw);
    
    EXPECT_EQ(list.size(), 3);
}

TEST(StructTest, CarIteration) {
    fixed_block_memory_resource mr(4096);
    doubly_linked_list<Car> list(&mr);
    
    list.push_back(Car("Tesla", "Model 3", 2023, 45000.0));
    list.push_back(Car("Ford", "Mustang", 2022, 35000.0));
    
    double total_price = 0.0;
    for (const auto& car : list) {
        total_price += car.price;
    }
    
    EXPECT_DOUBLE_EQ(total_price, 80000.0);
}

TEST(StructTest, CarArrowOperator) {
    fixed_block_memory_resource mr(4096);
    doubly_linked_list<Car> list(&mr);
    
    list.push_back(Car("Audi", "A4", 2023, 40000.0));
    
    auto it = list.begin();
    EXPECT_EQ(it->brand, "Audi");
    EXPECT_EQ(it->model, "A4");
    EXPECT_EQ(it->year, 2023);
    EXPECT_DOUBLE_EQ(it->price, 40000.0);
}

TEST(StructTest, CarPopOperations) {
    fixed_block_memory_resource mr(4096);
    doubly_linked_list<Car> list(&mr);
    
    list.push_back(Car("Car1", "Model1", 2020, 10000.0));
    list.push_back(Car("Car2", "Model2", 2021, 20000.0));
    list.push_back(Car("Car3", "Model3", 2022, 30000.0));
    
    list.pop_back();
    EXPECT_EQ(list.size(), 2);
    
    list.pop_front();
    EXPECT_EQ(list.size(), 1);
    
    auto it = list.begin();
    EXPECT_EQ(it->brand, "Car2");
}

// Тест со смешанными операциями
TEST(StructTest, MixedOperations) {
    fixed_block_memory_resource mr(8192);
    doubly_linked_list<Animal> zoo(&mr);
    
    // Добавляем животных
    zoo.push_back(Animal("Lion", 8, 190.0));
    zoo.push_front(Animal("Elephant", 15, 5000.0));
    zoo.push_back(Animal("Monkey", 5, 15.0));
    
    EXPECT_EQ(zoo.size(), 3);
    
    // Проверяем порядок
    auto it = zoo.begin();
    EXPECT_EQ(it->name, "Elephant");
    ++it;
    EXPECT_EQ(it->name, "Lion");
    ++it;
    EXPECT_EQ(it->name, "Monkey");
    
    // Удаляем
    zoo.pop_front();
    EXPECT_EQ(zoo.size(), 2);
    
    it = zoo.begin();
    EXPECT_EQ(it->name, "Lion");
}

// Тест на большое количество структур
TEST(StructTest, LargeStructList) {
    fixed_block_memory_resource mr(1024 * 1024); // 1 MB
    doubly_linked_list<Car> dealership(&mr);
    
    const int N = 100;
    for (int i = 0; i < N; ++i) {
        dealership.push_back(Car(
            "Brand" + std::to_string(i),
            "Model" + std::to_string(i),
            2000 + i,
            10000.0 + i * 100.0
        ));
    }
    
    EXPECT_EQ(dealership.size(), N);
    
    int count = 0;
    for (const auto& car : dealership) {
        EXPECT_EQ(car.brand, "Brand" + std::to_string(count));
        EXPECT_EQ(car.year, 2000 + count);
        ++count;
    }
    EXPECT_EQ(count, N);
}

// Тест переиспользования памяти со структурами
TEST(StructTest, MemoryReuseWithStructs) {
    fixed_block_memory_resource mr(8192);
    doubly_linked_list<Animal> list(&mr);
    
    // Первый цикл
    for (int i = 0; i < 5; ++i) {
        list.push_back(Animal("Animal" + std::to_string(i), i, i * 2.5));
    }
    
    size_t used_after_first = mr.get_used_memory();
    
    list.clear();
    
    // Второй цикл - память должна переиспользоваться
    for (int i = 0; i < 5; ++i) {
        list.push_back(Animal("NewAnimal" + std::to_string(i), i + 10, i * 3.5));
    }
    
    size_t used_after_second = mr.get_used_memory();
    
    // Память не должна сильно вырасти
    EXPECT_LE(used_after_second, used_after_first + 200);
}

// Тест копирования структур
TEST(StructTest, StructCopy) {
    fixed_block_memory_resource mr(4096);
    doubly_linked_list<Car> list(&mr);
    
    Car original("Original", "Model", 2023, 30000.0);
    list.push_back(original);
    
    // Изменяем оригинал
    original.price = 35000.0;
    
    // В списке должна быть копия с оригинальной ценой
    auto it = list.begin();
    EXPECT_DOUBLE_EQ(it->price, 30000.0);
}
