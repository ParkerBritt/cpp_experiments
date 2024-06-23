#include <catch2/catch.hpp>
#include "File.hpp"
#include <filesystem>

namespace fs = std::filesystem;

TEST_CASE("create file object"){
    SECTION("create file object with valid path"){
        REQUIRE_NOTHROW(File(std::string("/")));
    }
    SECTION("create file object with invalid path"){
        REQUIRE_THROWS_AS(File(std::string("this path doesn't exist")), fs::filesystem_error);
    }

}
