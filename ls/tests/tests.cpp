#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <iostream>

TEST_CASE("Basic Tests"){
    std::cout << "---------------" << std::endl;
    std::cout << "Beggining tests" << std::endl;
    std::cout << "---------------" << std::endl;
    REQUIRE(1==1);
}


