#include "ArgParsing.hpp"

ArgumentParser::ArgumentParser(){
    std::cout << "constructor successful" << std::endl;
}



void ArgumentParser::parseArgs(int argc, char* argv[]){
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
            // if(optSize>2){
            //     for(int i=1; i<optSize; i++){
                    // const char flag = opt[i];
            //     }
            // }
            const char flag = opt[1];
            if(optind<argc-1){
                shortArgs[flag] = argv[optind+1];
            }
        }
        std::cout << "SHORT ARG VALUES" << std::endl;
        for (const auto& pair : shortArgs) {
                std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
        }
    }
}
