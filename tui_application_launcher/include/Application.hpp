#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <string>
#include <boost/filesystem.hpp>

namespace bfs = boost::filesystem;

namespace launcher{
class Application{
public:
    Application(bfs::path desktopPath);

    // getters
    std::string getAppName() const;
    std::string getExecCommand() const;

    // operators
    bool operator<(const Application& rhs) const;

private:
    bfs::path desktopPath;
    std::string appName;
    std::string execCommand;

};
}


#endif
