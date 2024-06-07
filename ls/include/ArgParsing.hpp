#ifndef ARG_PARSING_H
#define ARG_PARSING_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include "AnsiUtils.hpp"
#include <optional>

class ArgumentParser{
    // constructor
    public:
        enum Type{
            Int,
            Bool,
            String,
            Float,
            Positional
        };

        ArgumentParser();
        bool parseArgs(int argc, char* argv[]);
        // template <typename T>
        // T getArgVal();

        std::unordered_map<char, std::string> shortArgValMap; 
        std::unordered_map<char, bool> shortArgBoolMap; 
        std::unordered_map<std::string, bool> longArgBoolMap; 
        std::unordered_map<std::string, std::string> longArgValMap; 
        std::vector<std::string> positionalArgs;

        std::unordered_map<char, Type> shortArgTypeMap;
        std::unordered_map<std::string, Type> longArgTypeMap;
    

        void addArgument(const char name, Type type);
        void addArgument(const std::string name, Type type);

        // long args
        template <typename T>
        void getArgument(std::string name, T& var){
            if(std::is_same<T, int>::value){
                std::cout << "is int" << std::endl;
            }
            else if(std::is_same<T, std::string>::value){
                std::cout << "is string" << std::endl;
                var = "VAL SET";
            }
        }

        // short args
        template <typename T>
        void getArgument(char name, T& var){
            std::cout << "\ngetting argument val for: " << name << std::endl;
            Type argType = shortArgTypeMap[name];
            switch (argType) {
                case(Type::Bool):
                    std::cout << "type is bool" << std::endl;
                    break;
                default:
                    std::cout << "unkown type: " << argType << std::endl;
                    break;
            }

            // if(std::is_same<T, int>::value){
            //     std::cout << "is int" << std::endl;
            // }
            // // handle bool types
            // else if(shortArgValMap[name]==Bool){
            //     std::cout << "is bool" << std::endl;
            //     if(shortArgValMap.find(name)!=shortArgValMap.end()){ // if value is present
            //         var = shortArgValMap[name]=="true"; // convert string to bool
            //         return;
            //     }
            // }
        }

    // bool operator[](const std::string& flagName) const {
    //     std::string argVal = shortArgValMap[name];
    //     return argVal=="true";
    // }
    template <typename T>
    std::optional<T> getArgVal(const char flagName);

    template <typename T>
    std::optional<T> getArgVal(const char flagName, T defaultVal);

    template <typename T>
    std::optional<T> getArgVal(const std::string);

    template <typename T>
    std::optional<T> getArgVal(const std::string, T defaultVal);


    private:
        void unkownArg(const char name);
        void unkownArg(const std::string name);
        void errNoTokenFound(const char argName, const ArgumentParser::Type tokenType);
        void errNoTokenFound(const std::string argName, const ArgumentParser::Type tokenType);
};



#endif
