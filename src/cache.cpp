#include "../include/cache.h"

namespace memory
{
    Cache::Cache() : mCache(std::map<std::string, std::string>())
    {
    }

    void Cache::add(const std::string &request, const std::string &content)
    {
        mCache[request] = content;
    }
    
    bool Cache::contains(const std::string &request) const
    {
        return mCache.find(request) != mCache.end();
    }
    
    std::string Cache::retrieve(const std::string &request) const
    {
        return mCache.at(request);
    }
}
