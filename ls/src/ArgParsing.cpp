#include "ArgParsing.hpp"
#include "AnsiUtils.hpp"

ArgumentParser::ArgumentParser(){
    // std::cout << "constructor successful" << std::endl;
}


void ArgumentParser::addArgument(const char name, Type type){
    shortArgTypeMap[name] = type;
    // std::cout << "adding: " << name << " as type: " << type << std::endl;
}

void ArgumentParser::addArgument(const std::string name, Type type){
    longArgTypeMap[name] = type;

    // add to positional argument vector
    if(type == ArgumentParser::Positional){
        positionalArgs.push_back(name);
    }
    // std::cout << "adding: " << name << " as type: " << type << std::endl;
}

bool ArgumentParser::parseArgs(int argc, char* argv[]){
    int positionalIndex = 0;
    for(size_t optind = 1; optind<argc; optind++){
        std::cout << "FOO" << std::endl;
        const std::string opt = argv[optind];
        std::cout << "parsing arg: " << opt << std::endl;
        if(opt[0]!='-'){ // break if no flag detected
            std::string argName = positionalArgs[positionalIndex];
            longArgValMap[argName] = opt;
            if(positionalIndex<positionalArgs.size()-1){
                positionalIndex++;
            }
            
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
                    // std::cout << "setting: " << shortArg << " to true" << std::endl;
                    shortArgBoolMap[shortArg] = true;
                    continue;
                }

                // for other types check if there is a token
                if(shortArgInd==optSize-1){ // if last char
                    // if there is an argument after the current one and that argumen is a token
                    if(optind<argc-1 && argv[optind+1][0]!='-'){
                        shortArgValMap[shortArg] = argv[optind+1]; // set value to the token if present
                        optind++;
                    }
                    else{ // if no token is present set raise error
                        errNoTokenFound(shortArg, argType);
                        return false;
                    }
                }
            }
        }

        // -- start long arg handling --
        if(opt.substr(0,2)=="--"){
            std::string longArg = opt.substr(2,optSize-2);
            // std::cout << "parsing longarg: " << longArg << std::endl;


            // check if arg has been registed with addArgument
            if(longArgTypeMap.find(longArg)==longArgTypeMap.end()){
                unkownArg(longArg);
                return false;
            }
            Type argType = longArgTypeMap[longArg];

            if(argType == Bool){
                std::cout << "setting: " << longArg << " to true" << std::endl;
                longArgBoolMap[longArg] = true;
                continue;
            }

            if(optind<argc-1 && argv[optind+1][0]!='-'){
                longArgValMap[longArg] = argv[optind+1]; // set value to the token if present
                optind++;
            }
            else{ // if no token is present set raise error
                errNoTokenFound(longArg, argType);
                return false;
            }
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

template <>
std::optional<bool> ArgumentParser::getArgVal(const char flagName, bool defaultVal){
    bool argVal = shortArgBoolMap[flagName];
    if(argVal){
        return argVal;
    }
    else{
        return defaultVal;
    }
}

template <>
std::optional<bool> ArgumentParser::getArgVal<bool>(const char flagName){
    return getArgVal<bool>(flagName, false);
}

template <>
std::optional<bool> ArgumentParser::getArgVal<bool>(const std::string flagName, bool defaultVal){
    return longArgBoolMap[flagName];
}

template <>
std::optional<std::string> ArgumentParser::getArgVal<std::string>(const char flagName){
    if(shortArgValMap.find(flagName)!=shortArgValMap.end()){
        return shortArgValMap[flagName];
    }else{
        return std::nullopt;
    }
}

template <>
std::optional<bool> ArgumentParser::getArgVal<bool>(const std::string flagName){
    return getArgVal<bool>(flagName, false);
}

template <>
std::optional<std::string> ArgumentParser::getArgVal<std::string>(const std::string flagName){
    if(longArgValMap.find(flagName)!=longArgValMap.end()){
        return longArgValMap[flagName];
    }else{
        return std::nullopt;
    }
}

void ArgumentParser::errNoTokenFound(const char argName, const ArgumentParser::Type tokenType){
    std::string typeString;
    switch(tokenType){
        case(2):
            typeString = "string";
            break;
        default:
            typeString = "unknown";
            break;
    }
    std::cerr << AnsiUtils::color(255,0,0) << "Error: No token found. Expected token of type: " << typeString << " for arg: " << AnsiUtils::bold() << argName << std::endl;
}
void ArgumentParser::errNoTokenFound(const std::string argName, const ArgumentParser::Type tokenType){
    std::string typeString;
    switch(tokenType){
        case(2):
            typeString = "string";
            break;
        default:
            typeString = "unknown";
            break;
    }
    std::cerr << AnsiUtils::color(255,0,0) << "Error: No token found. Expected token of type: " << typeString << " for arg: " << AnsiUtils::bold() << argName << std::endl;
}
