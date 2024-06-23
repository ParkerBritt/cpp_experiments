#include "ArgParsing.hpp"
#include <catch2/catch.hpp>

TEST_CASE("Arg Parser: Bool"){
    ArgumentParser argParser = ArgumentParser();
    argParser.addArgument('a', argParser.Bool);
    argParser.addArgument('b', argParser.Bool);
    argParser.addArgument('c', argParser.Bool);
    argParser.addArgument("foo", argParser.Bool);

    const char* argv[] = {"executable", "-a", "-c", "--foo"};
    int argc = std::size(argv);
    bool didPassArgs = argParser.parseArgs(argc, const_cast<char**>(argv));
    
    // check the arg parser finished successfuly
    REQUIRE(didPassArgs == true);

    std::optional<bool> flagA = argParser.getArgVal<bool>('a');
    std::optional<bool> flagB = argParser.getArgVal<bool>('b');
    std::optional<bool> flagC = argParser.getArgVal<bool>('c');
    std::optional<bool> flagFoo = argParser.getArgVal<bool>('c');

    // check bool flags
    REQUIRE(*flagA==true);
    REQUIRE(*flagB==false);
    REQUIRE(*flagC==true);
    REQUIRE(*flagFoo==true);
}

TEST_CASE("Arg Parser: Positional"){
    ArgumentParser argParser = ArgumentParser();
    argParser.addArgument('a', argParser.Positional);
    argParser.addArgument("foo", argParser.Positional);
    argParser.addArgument("bar", argParser.Positional);
    const char* argv[] = {"executable", "-a", "hello", "--foo", "world"};
    int argc = std::size(argv);
    bool didPassArgs = argParser.parseArgs(argc, const_cast<char**>(argv));
    
    // check the arg parser finished successfuly
    REQUIRE(didPassArgs == true);

    std::optional<std::string> flagA = argParser.getArgVal<std::string>('a');
    std::optional<std::string> flagFoo = argParser.getArgVal<std::string>("foo");
    std::optional<std::string> flagBar = argParser.getArgVal<std::string>("bar");

    // check positional args 
    REQUIRE(flagA.has_value()==true);
    REQUIRE(*flagA=="hello");
    REQUIRE(flagFoo.has_value()==true);
    REQUIRE(*flagFoo=="world");
    REQUIRE(flagBar.has_value()==false);
}

TEST_CASE("Arg Parser: missing positional token"){
    ArgumentParser argParser = ArgumentParser();
    argParser.addArgument('a', argParser.Positional);
    const char* argv[] = {"executable", "-a"};
    int argc = std::size(argv);
    bool didPassArgs = argParser.parseArgs(argc, const_cast<char**>(argv));
    
    // check the arg parser failed
    REQUIRE(didPassArgs == false);
}
