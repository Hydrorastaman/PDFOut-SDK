#pragma once

#include <memory>
#include <mutex>

namespace kernel{

  template<typename T>
  class Singleton{
  protected:
    Singleton() {}

  public:
    ~Singleton() {}

    static T &getInstance(){
      std::call_once(Singleton::gOnceFlag, []() {Singleton::gInstance.reset(new T()); });
      return *gInstance;
    }

  private:
    static std::unique_ptr<T> gInstance;
    static std::once_flag gOnceFlag;
  };

  template<typename T>
  std::unique_ptr<T>  Singleton<T>::gInstance(nullptr);

  template<typename T>
  std::once_flag Singleton<T>::gOnceFlag;

}
