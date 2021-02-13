#pragma once

#include <atomic>
#include <string>
#include <unordered_map>

namespace kernel{

  enum ResourceType{
    ResourceTypeFont = 0,
    ResourceTypeColorSpace,
    ResourceTypeGS,
    ResourceTypeImage,
    ResourceTypeWidget
  };

  class ResourceUid{
  public:
    static std::string getResourceUid(ResourceType type);
  
  private:
    static std::atomic_uint32_t gFontCounter;
    static std::atomic_uint32_t gColorSpaceCounter;
    static std::atomic_uint32_t gGSCounter;
    static std::atomic_uint32_t gImageCounter;
    static std::atomic_uint32_t gWidgetCounter;
    static std::unordered_map<ResourceType, std::pair<std::string, std::atomic_uint32_t *>> gResourceMap;
  };
}
