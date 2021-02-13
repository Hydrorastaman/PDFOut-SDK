#include <ResourceUid.h>

namespace kernel{

  std::atomic_uint32_t ResourceUid::gFontCounter = 0;
  std::atomic_uint32_t ResourceUid::gColorSpaceCounter = 0;
  std::atomic_uint32_t ResourceUid::gGSCounter = 0;
  std::atomic_uint32_t ResourceUid::gImageCounter = 0;
  std::atomic_uint32_t ResourceUid::gWidgetCounter = 0;

  std::unordered_map<ResourceType, std::pair<std::string, std::atomic_uint32_t *>> ResourceUid::gResourceMap{
    {ResourceTypeFont,        {"Font"  , &gFontCounter}},
    {ResourceTypeColorSpace,  {"CS"    , &gColorSpaceCounter}},
    {ResourceTypeGS,          {"GS"    , &gGSCounter}},
    {ResourceTypeImage,       {"Image" , &gImageCounter}},
    {ResourceTypeWidget,      {"Widget", &gWidgetCounter}}
  };

  std::string ResourceUid::getResourceUid(ResourceType type){
    std::string const uid = gResourceMap[type].first + std::to_string(gResourceMap[type].second->fetch_add(1));
    return uid;
  }
}
