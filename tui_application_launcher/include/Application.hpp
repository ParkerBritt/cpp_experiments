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
            std::string getAppName();
            std::string getExecCommand();

        private:
            bfs::path desktopPath;
            std::string appName;
            std::string execCommand;

    };
}


#endif
