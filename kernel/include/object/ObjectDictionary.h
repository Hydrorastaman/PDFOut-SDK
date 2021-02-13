#pragma once

#include <deque>
#include <memory>
#include <unordered_map>

#include <Exception.h>
#include <InternalTools.h>
#include <object/Object.h>
#include <object/ObjectName.h>
#include <object/ObjectIndirectReference.h>
#include <document/DocumentCrossReferenceTable.h>

namespace pdfout{

  class InputStream;
  class OutputStream;

}

namespace kernel{

  class ObjectDictionary : public Object{
  public:
    explicit ObjectDictionary(IndirectType indirectable = IndirectTypeIndirectable)
      : Object(ObjectTypeDictionary, indirectable),
      mDictionary(){
      mDictionary.reserve(32);
    }

    ~ObjectDictionary(void) {}

    std::size_t getSize(void) const {return mDictionary.size();}

    bool isEmpty(void) const {return mDictionary.empty();}

    void insert(std::string const &key, std::unique_ptr<Object> value) {mDictionary[key] = std::move(value);}

    void insert(ObjectName *key, std::unique_ptr<Object> value) {mDictionary[key->getValue()] = std::move(value);}

    void remove(std::string const &key) {mDictionary.erase(key);}

    void remove(ObjectName *key) {mDictionary.erase(key->getValue());}

    Object *find(std::string const &key) const {
      auto iter = mDictionary.find(key);
      if (iter != mDictionary.end())
        return iter->second.get();

      return nullptr;
    }

    Object *find(ObjectName *key) const {return find(key->getValue());}

    void serialize(pdfout::OutputStream *stream, SerializeParams *params) const;
    void serializeBody(pdfout::OutputStream *stream, SerializeParams *params, std::deque<Object *> &indirectObjects) const;
    void serializeIndirectObjects(pdfout::OutputStream *stream, SerializeParams *params, std::deque<Object *> const &indirectObjects) const;

    ObjectDictionary *clone(void) const{
      return new ObjectDictionary(*this);
    }

    template <typename T>
    void addKey(std::unordered_map<T, std::pair<std::string, uint32_t>> const &map, T key, std::unique_ptr<Object> value){
      if (!value)
        return;

      auto iter = map.find(key);
      if (iter == std::end(map))
        RUNTIME_EXCEPTION("Invalid KEY value");

      if (value->getType() & (iter->second.second | ObjectTypeIndirectReference)){
        ObjectDictionary::insert(iter->second.first, std::move(value));
        return;
      }

      RUNTIME_EXCEPTION("Invalid object type");
    }

    template <typename T>
    void removeKey(std::unordered_map<T, std::pair<std::string, uint32_t>> const &map, T key){
      auto iter = map.find(key);
      if (iter == std::end(map))
        return;

      ObjectDictionary::remove(iter->second.first);
    }

    template <typename T>
    Object const *findKey(std::unordered_map<T, std::pair<std::string, uint32_t>> const &map, T key) const{
      auto iter = map.find(key);
      if (iter == std::end(map))
        return nullptr;

      return find(iter->second.first);
    }

  protected:
    ObjectDictionary(ObjectDictionary const &obj)
      : Object(obj),
      mDictionary(){
      for (auto const &elem : obj.mDictionary)
        mDictionary[elem.first] = std::move(std::unique_ptr<Object>(elem.second->clone()));
    }

    void clear(void){
      mDictionary.clear();
    }

  private:
    ObjectDictionary &operator=(ObjectDictionary const &) = delete;

  private:
    std::unordered_map<std::string, std::unique_ptr<Object>> mDictionary;
  };
}
