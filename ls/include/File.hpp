#ifndef FILE_HPP
#define FILE_HPP

#include "AnsiUtils.hpp"
#include <filesystem>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <optional>

namespace fs = std::filesystem;

class File{
    public:
        File(std::string);
        File(fs::path);

        // getters
        std::string getFileName();
        std::string getCompFileName();
        std::string getFilePath();
        std::string getFormattedLine() const;
        size_t getLineLen() const;
        
        // setters
        void setIcon(const std::unordered_map<std::string, std::vector<std::string>> iconNameMap,
        const std::unordered_map<std::string, std::vector<std::string>> extMap);

    private:
        std::optional<std::tuple<std::string, std::string>> parseConfigIconMap(const std::unordered_map<std::string, std::vector<std::string>> configMap, std::string key);

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

#endif
