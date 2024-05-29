#ifndef ARG_PARSING_H
#define ARG_PARSING_H

#include <iostream>
#include <unordered_map>

class ArgumentParser{
    // constructor
    public:
        ArgumentParser();
        void parseArgs(int argc, char* argv[]);
        // template <typename T>
        // T getArgVal();

        std::unordered_map<char, std::string> shortArgs; 

        // long args
        template <typename T>
        void addArgument(std::string name, T& var){
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
        void addArgument(char name, T& var){
            if(std::is_same<T, int>::value){
                std::cout << "is int" << std::endl;
            }
        }
};



#endif
