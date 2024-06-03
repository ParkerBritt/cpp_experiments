#include "ArgParsing.hpp"
#include "AnsiUtils.hpp"

ArgumentParser::ArgumentParser(){
    std::cout << "constructor successful" << std::endl;
}


void ArgumentParser::addArgument(const char name, Type type){
    // Type type = Bool;
    shortArgTypeMap[name] = type;
    std::cout << "adding: " << name << " as type: " << type << std::endl;
    for (const auto& pair : shortArgValMap) {
            std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
    }
}

void ArgumentParser::addArgument(const std::string name, Type type){
    longArgTypeMap[name] = type;
    std::cout << "adding: " << name << " as type: " << type << std::endl;
}

bool ArgumentParser::parseArgs(int argc, char* argv[]){
    int optind = 1;
    int column_cnt = -1;
    int row_cnt = -1;
    for(optind; optind<argc; optind++){
        const std::string opt = argv[optind];
        if(opt[0]!='-'){ // break if no flag detected
            // handle positional arguments
            if(longArgTypeMap.find(opt)==longArgTypeMap.end()){
                continue;
            }
            Type type = longArgTypeMap[opt];
            if(type!=Positional){ continue; }
            // do positional argument handling here
            // add each positional argument to longArgValMap if the previous positional entry has been filled
            
        }

        size_t optSize= opt.size();
        if(optSize<2){ // guard against short arg with no char
            continue;
        }
        // -- start short op handling --
        if(opt[0]=='-' && opt[1]!='-'){
            char shortArg;
            // handle boolean shortArgValMap
            for(size_t shortArgInd=1; shortArgInd<optSize; shortArgInd++){
                shortArg = opt[shortArgInd];
                // if flag cannot be found in typemap then don't save it's value
                // not in typeMap means invalid flag
                if(shortArgTypeMap.find(shortArg)==shortArgTypeMap.end()){
                    unkownArg(shortArg);
                    return false;
                }
                Type argType = shortArgTypeMap[shortArg];

                // if flag is bool set arg value to true
                if(argType == Bool){
                    std::cout << "setting: " << shortArg << " to true" << std::endl;
                    shortArgBoolMap[shortArg] = true;
                    continue;
                }

                // for other types check if there is a token
                if(shortArgInd==optSize-1){ // if last char
                    // if there is an argument after the current one and that argumen is a token
                    if(optind<argc-1 && argv[optind+1][0]!='-'){
                        shortArgValMap[shortArg] = argv[optind+1]; // set value to the token if present
                    }
                    else{ // if no token is present set raise error
                        errNoTokenFound(shortArg, argType);
                        return false;
                    }
                }
            }
        }
        // -- start long arg handling --

        std::cout << "SHORT ARG VALUES" << std::endl;
        for (const auto& pair : shortArgValMap) {
                std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
        }
    }
    return true;
}

void ArgumentParser::unkownArg(const char name){
    std::cerr << AnsiUtils::color(255,0,0) << "UnkownArg: " << AnsiUtils::bold() << name << std::endl;
}
void ArgumentParser::unkownArg(const std::string name){
    std::cerr << "UnkownArg: " << name << std::endl;
}

void ArgumentParser::errNoTokenFound(const char argName, const ArgumentParser::Type tokenType){
    std::string typeString;
    switch(tokenType){
        case(2):
            typeString = "string";
            break;
        default:
            typeString = "unkown";
            break;
    }
    std::cerr << AnsiUtils::color(255,0,0) << "Error: No token found. Expected token of type: " << typeString << " for arg: " << AnsiUtils::bold() << argName << std::endl;
}
