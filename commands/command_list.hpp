#include <iostream>
#include <string>
#include <unordered_map>

// Define the function pointer type (e.g., functions taking no parameters and returning void)
using FunctionPtr = void (*)();
// using FunctionPtr = void	(*fct_write)(env* , int);

// Example functions to be stored in the map
void functionA() {
    std::cout << "Function A called!" << std::endl;
}

void functionB() {
    std::cout << "Function B called!" << std::endl;
}

void functionC() {
    std::cout << "Function C called!" << std::endl;
}

// Constant global map with string keys and function pointers as values
static const std::unordered_map<std::string, FunctionPtr> functionMap = {
    {"A", functionA},
    {"B", functionB},
    {"C", functionC}
};

// Function to call a function by name using the map
void callFunction(const std::string& key) {
    auto it = functionMap.find(key);
    if (it != functionMap.end()) {
        // Call the function pointer
        it->second();
    } else {
        std::cout << "Function not found for key: " << key << std::endl;
    }
}
