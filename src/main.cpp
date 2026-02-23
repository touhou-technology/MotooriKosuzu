#include <iostream>
#include <meta>
#include <print>

#include "core/core.hpp"


int main() {
    UseAPI Test;

    auto Obj = Test.ReflectionAPI();
    for(auto Str : Obj){
        std::cout << Str << std::endl;
    }
}
