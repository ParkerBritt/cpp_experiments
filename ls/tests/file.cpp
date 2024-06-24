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
    std::string file_path = "/dev/null";
    File file(file_path);

    SECTION("set icon"){
        std::unordered_map<std::string, std::vector<std::string>> iconNameMap;
        std::unordered_map<std::string, std::vector<std::string>> extMap;

        std::vector<std::string> v = {""};
        iconNameMap["null"] = v;
        file.setIcon(iconNameMap, extMap);
        REQUIRE(file.getFormattedLine() == "[38;2;255;255;255m [38;2;255;255;255mnull");
    }

    SECTION("get line len"){
        REQUIRE(file.getLineLen() == 6);
    }

    SECTION("file name"){
        REQUIRE(file.getFileName() == "null");
    }
}
