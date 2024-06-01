#include "ArgParsing.hpp"

ArgumentParser::ArgumentParser(){
    std::cout << "constructor successful" << std::endl;
}



bool ArgumentParser::parseArgs(int argc, char* argv[]){
    std::cout << "PARSTING ARGS" << std::endl;
    int optind = 1;
    int column_cnt = -1;
    int row_cnt = -1;
    for(optind; optind<argc; optind++){
        const std::string opt = argv[optind];
        if(opt[0]!='-'){ // break if no flag detected
            // std::cout << "break " << argc << opt << std::endl;
            // std::cout << opt.substr(1,0) << std::endl;
            continue;
        }

        // -- start short op handling --
        unsigned int optSize= opt.size();
        if(optSize<2){ // guard against short arg with no char
            continue;
        }
        if(opt[0]=='-' && opt[1]!='-'){
            size_t shortArgInd = 1;
            char shortArg;
            if(optSize>2){
                // handle boolean shortArgValMap
                for(shortArgInd; shortArgInd<optSize-1; shortArgInd++){
                    shortArg = opt[shortArgInd];
                    // if flag cannot be found in typemap then don't save it's value
                    // not in typeMap means invalid flag
                    if(shortArgTypeMap.find(shortArg)==shortArgTypeMap.end()){
                        unkownArg(shortArg);
                        return false;
                    }
                    shortArgValMap[shortArg] = "true";
                }
            }
            shortArg = opt[shortArgInd];
            // handle setable shortArgValMap
            if(optind<argc-1 && argv[optind+1][0]!='-'){
                shortArgValMap[shortArg] = argv[optind+1]; 
            }
            else{
                shortArgValMap[shortArg] = "true";
            }
        }
        std::cout << "SHORT ARG VALUES" << std::endl;
        for (const auto& pair : shortArgValMap) {
                std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
        }
    }
    return true;
}

void ArgumentParser::unkownArg(const char name){
    std::cerr << "UnkownArg: " << name << std::endl;
}
void ArgumentParser::unkownArg(const std::string name){
    std::cerr << "UnkownArg: " << name << std::endl;
}

