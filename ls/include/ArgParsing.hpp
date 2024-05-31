#ifndef ARG_PARSING_H
#define ARG_PARSING_H

#include <iostream>
#include <unordered_map>
#include <vector>

class ArgumentParser{
    // constructor
    public:
        ArgumentParser();
        void parseArgs(int argc, char* argv[]);
        // template <typename T>
        // T getArgVal();

        std::unordered_map<char, std::string> shortArgValMap; 
        std::unordered_map<std::string, std::string> longArgValMap; 

        enum Type{
            Int,
            Bool,
            String,
            Float
        };
        std::unordered_map<char, Type> shortArgTypeMap;
    

        void addArgument(const char name, Type type){
            // Type type = Bool;
            shortArgTypeMap[name] = type;
            std::cout << "adding: " << name << " as type: " << type << std::endl;
            for (const auto& pair : shortArgValMap) {
                    std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
            }
        }

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
            if(std::is_same<T, int>::value){
                std::cout << "is int" << std::endl;
            }
            else if(std::is_same<T, bool>::value){
                std::cout << "fetching bool value for getArgument" << std::endl;
                if(shortArgValMap.find(name)!=shortArgValMap.end()){ // if value is present
                    std::cout << "passed" << std::endl;
                    var = shortArgValMap[name]=="true";
                }
            }
        }

};



#endif
