#ifndef FILE_HPP
#define FILE_HPP

#include "AnsiUtils.hpp"
#include <filesystem>
#include <vector>
#include "configParsing.hpp"
#include <memory>
#include <unordered_map>
#include "border.hpp"

namespace fs = std::filesystem;

class File{
    public:
        File(std::string);
        File(fs::path);

        // getters
        std::string getFileName();
        std::string getFilePath();
        std::string getFormattedLine() const;
        size_t getLineLen() const;
        
        // setters
        void setIcon(const std::unordered_map<std::string, std::vector<std::string>> iconNameMap,
        const std::unordered_map<std::string, std::vector<std::string>> extMap);

    private:
        fs::path path;

        std::string fileIcon = "";
        AnsiUtils::Color fileIconColor;

        std::string fileName;
        AnsiUtils::Color fileNameColor;

        std::string symLinkIcon = "";
        AnsiUtils::Color symLinkIconColor;

        std::string symLinkPath;
        AnsiUtils::Color symLinkPathColor;

        bool isHidden = false;
        bool showSymLink = false;
        bool isSymLink = false;

        size_t lineLen = 0;

        // setters
        void setFileName();
        void setLineLen();
};

class FileCollection{
    public:
        FileCollection(std::shared_ptr<ConfigParser> configParser);
        void addFile(File file);
        void newFile(fs::path path);
        std::string getFormattedFiles(bool longMode=false, bool showBorder=false);
        size_t getCnt();
    private:
        size_t maxFileNameCnt = 0;
        std::unique_ptr<Border> border;
        std::vector<File> filesVector;
        ConfigParsing::configMap iconNameMap;
        ConfigParsing::configMap iconExtMap;

        size_t filesCnt = 0;

};

#endif
