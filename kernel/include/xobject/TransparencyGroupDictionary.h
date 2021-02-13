#pragma once

#include <xobject/GroupXObjectDictionary.h>

namespace kernel{ namespace xobject{

  enum TransparencyGroupDictionaryKey{
    TransparencyGroupDictionaryKeyCS, /**< (Sometimes required, as discussed below) The group color space, which is used for the following purposes:
                                            1) As the color space into which colors are converted when painted into the group
                                            2) As the blending color space in which objects are composited within the group
                                            3) As the color space of the group as a whole when it in turn is painted as an object onto its backdrop */
    TransparencyGroupDictionaryKeyI,  /**< (Optional) A flag specifying whether the transparency group is isolated */
    TransparencyGroupDictionaryKeyK   /**< (Optional) A flag specifying whether the transparency group is a knockout group */
  };

  class TransparencyGroupDictionary : public GroupXObjectDictionary{
  public:
    TransparencyGroupDictionary(void);
    ~TransparencyGroupDictionary(void);

    using GroupXObjectDictionary::addKey;
    void addKey(TransparencyGroupDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<TransparencyGroupDictionaryKey, std::pair<std::string, uint32_t>> mTransparencyGroupDictionaryMap;

  private:
    TransparencyGroupDictionary(TransparencyGroupDictionary const &) = delete;
    TransparencyGroupDictionary &operator=(TransparencyGroupDictionary const &) = delete;
  };

}}
