#ifndef FILE_COLLECTION_HPP
#define FILE_COLLECTION_HPP

#include <filesystem>
#include <vector>
#include "configParsing.hpp"
#include <memory>
#include "border.hpp"
#include <cmath>
#include "File.hpp"

namespace fs = std::filesystem;

class FileCollection{
    public:
        FileCollection(std::shared_ptr<ConfigParser> configParser);
        void addFile(File file);
        void newFile(fs::path path);
        std::string getFormattedFiles(bool longMode=false, bool showBorder=false, bool showHiddenFiles=false);
        size_t getCnt();
    private:
        size_t maxFileNameCnt = 0;
        std::unique_ptr<Border> border = std::make_unique<Border>();
        std::vector<File> filesVector;
        ConfigParsing::ConfigMap iconNameMap;
        ConfigParsing::ConfigMap iconExtMap;

        size_t filesCnt = 0;

};

#endif
