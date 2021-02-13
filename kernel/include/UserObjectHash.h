#pragma once

#include <unordered_map>
#include <mutex>

namespace kernel{

  template<typename T>
  class UserObjectHash{
  public:
    UserObjectHash(void) {}
    ~UserObjectHash(void) {}

    T *find(std::string const &sha256) const{
      std::lock_guard<std::mutex> lock(mAccessMutex);

      auto iter = mCache.find(sha256);
      if (iter != std::end(mCache))
        return iter->second;

      return nullptr;
    }

    void store(T *obj, std::string const &sha256){
      std::lock_guard<std::mutex> lock(mAccessMutex);
      mCache[sha256] = obj;
    }
    
  private:
    UserObjectHash(UserObjectHash const &) = delete;
    UserObjectHash &operator=(UserObjectHash const &) = delete;

  private:
    mutable std::mutex mAccessMutex;
    std::unordered_map<std::string, T *> mCache;
  };

}
