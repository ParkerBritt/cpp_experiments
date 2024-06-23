#include <catch2/catch.hpp>
#include "File.hpp"
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

TEST_CASE("create file object"){
    SECTION("create file object with valid path"){
        REQUIRE_NOTHROW(File(std::string("/")));
    }
    SECTION("create file object with invalid path"){
        REQUIRE_THROWS_AS(File(std::string("this path doesn't exist")), fs::filesystem_error);
    }
}

TEST_CASE("file operations"){
    std::string file_path = "/home/parker/evidence_reel.txt";
    File file(file_path);

    SECTION("line len"){
        REQUIRE(file.getFileName() == "hello");
    }
}
