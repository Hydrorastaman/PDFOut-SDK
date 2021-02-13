#pragma once

#include <mutex>
#include <deque>
#include <memory>

#include <Singletone.h>

namespace kernel{

  template <typename T>
  class UserObjectManager : public Singleton<UserObjectManager<T>>{
  public:
    UserObjectManager(void)
      : mAccessMutex(),
      mResources() {
    }

    ~UserObjectManager(void) {}

    T *insert(std::unique_ptr<T> resouce) {
      std::lock_guard<std::mutex> lock(mAccessMutex);
      T *obj = resouce.get();
      mResources.push_back(std::move(resouce));
      return obj;
    }

  private:
    UserObjectManager(UserObjectManager const &) = delete;
    UserObjectManager &operator=(UserObjectManager const &) = delete;

  private:
    std::mutex mAccessMutex;
    std::deque<std::unique_ptr<T>> mResources;
  };
}
