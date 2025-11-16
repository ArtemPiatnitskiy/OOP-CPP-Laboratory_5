#include "include/fixed_block_memory_resource.h"
#include "include/doubly_linked_list.h"
#include <iostream>
#include <string>

struct color {
    std::string name;
    int r, g, b;

    color(const std::string& n, int red, int green, int blue)
        : name(n), r(red), g(green), b(blue) {}
};

int main() {
    fixed_block_memory_resource mr(4096);

       
    
    doubly_linked_list<int> list(&mr);
    
    list.push_back(10);
    list.pop_back();
    
    mr.print_allocated_blocks();
    
    std::cout << "\n==============================================\n";
    
    doubly_linked_list<color> colors(&mr);

    colors.push_back(color("Red", 255, 0, 0));
    colors.push_back(color("Green", 0, 255, 0));
    colors.push_back(color("Blue", 0, 0, 255));
    colors.push_back(color("White", 255, 255, 255));
    colors.push_back(color("Black", 0, 0, 0));

    std::cout << "Colors in the list:\n";
    for (const auto& col : colors) {
        std::cout << "  " << col.name << " (RGB: " << col.r << ", " << col.g << ", " << col.b << ")\n";
    }

    fixed_block_memory_resource mr_2{};

    std::cout << "\n==============================================\n";

    mr_2.print_allocated_blocks();

    doubly_linked_list<void*> list_2(&mr_2);

    for (int i = 0; i < 100; ++i) {
        list_2.push_back((void*)new color("Color" + std::to_string(i), i, i, i));
        list_2.push_back((void*)new int(i));
    }

    std::cout << "\nList 2 contents:\n";

    list_2.print_list();

    std::cout << "\n==============================================\n";

    mr_2.print_allocated_blocks();
    
    std::cout << "\n==============================================\n";

    list_2.clear();
    mr_2.print_allocated_blocks();

    return 0;
}