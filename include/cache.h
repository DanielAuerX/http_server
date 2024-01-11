#pragma once

#include <map>
#include <string>

namespace memory{
    class Cache {
        public:
        Cache();
        void add(const std::string &request, const std::string &content);
        bool contains(const std::string& request) const;
        std::string retrieve(const std::string& request) const;

        private:
        std::map<std::string, std::string> mCache;
    };
}