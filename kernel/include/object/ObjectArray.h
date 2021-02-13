#pragma once

#include <vector>
#include <deque>
#include <string>

#include <memory>
#include <object/Object.h>
#include <object/ObjectNumber.h>
#include <object/ObjectInteger.h>
#include <object/ObjectIndirectReference.h>
#include <document/DocumentCrossReferenceTable.h>

namespace kernel{

  class ObjectArray : public Object{
  public:
    explicit ObjectArray(IndirectType indirectable = IndirectTypeIndirectable) : Object(ObjectTypeArray, indirectable), mArray() {};

    ObjectArray(float *values, std::size_t valuesSize, IndirectType indirectable = IndirectTypeIndirectable)
      : Object(ObjectTypeArray, indirectable),
      mArray(valuesSize){

      for (std::size_t i = 0; i < valuesSize; ++i)
        mArray[i] = std::make_unique<ObjectNumber>(values[i]);
    }

    ObjectArray(std::vector<float> const &values, IndirectType indirectable = IndirectTypeIndirectable)
      : Object(ObjectTypeArray, indirectable),
      mArray(values.size()){

      for (std::size_t i = 0, size = values.size(); i < size; ++i)
        mArray[i] = std::make_unique<ObjectNumber>(values[i]);
    }

    ObjectArray(int *values, std::size_t valuesSize, IndirectType indirectable = IndirectTypeIndirectable)
      : Object(ObjectTypeArray, indirectable),
      mArray(valuesSize){

      for (std::size_t i = 0; i < valuesSize; ++i)
        mArray[i] = std::make_unique<ObjectInteger>(values[i]);
    }

    ObjectArray(uint32_t *values, std::size_t valuesSize, IndirectType indirectable = IndirectTypeIndirectable)
      : Object(ObjectTypeArray, indirectable),
      mArray(valuesSize){

      for (std::size_t i = 0; i < valuesSize; ++i)
        mArray[i] = std::make_unique<ObjectInteger>(values[i]);
    }

    ~ObjectArray(void) {}

    std::size_t getSize(void) const {return mArray.size();}

    bool isEmpty(void) const {return mArray.empty();}

    Object *get(std::size_t index) const {return mArray[index].get();}

    void insert(std::unique_ptr<Object> object) {mArray.push_back(std::move(object));}

    void serialize(pdfout::OutputStream *stream, SerializeParams *params) const{
      std::unique_ptr<ObjectIndirectReference> ref = getReference();

      if (isIndirectable()){
        params->mXref->addRecord(ref->getNumber(), ref->getGeneration(), stream->tellp());
        writeHeader(stream, ref->getNumber(), ref->getGeneration());
        params->mRootObj = this;
      }

      stream->write("[", sizeof(uint8_t), 1);

      std::deque<std::size_t> indirectObjects;

      for (std::size_t i = 0, size = mArray.size(); i < size; ++i){
        if (mArray[i]->isIndirectable()){
          indirectObjects.push_back(i);
          mArray[i]->getReference()->serialize(stream, params);
        }
        else
          mArray[i]->serialize(stream, params);

        if (i != size - 1)
          stream->write(" ", sizeof(uint8_t), 1);
      }

      stream->write("]", sizeof(uint8_t), 1);

      if (isIndirectable()){
        stream->write("\r\n", sizeof(uint8_t), 2);
        writeFooter(stream);
      }

      for (std::size_t i = 0, size = indirectObjects.size(); i < size; ++i)
        mArray[i]->serialize(stream, params);
    }

    ObjectArray *clone(void) const{
      return new ObjectArray(*this);
    }

  protected:
    ObjectArray(ObjectArray const &obj)
      : Object(obj),
      mArray(obj.mArray.size()){
        for (std::size_t i = 0, size = obj.mArray.size(); i < size; ++i)
          mArray[i].reset(obj.mArray[i]->clone());
    }

  private:
    ObjectArray &operator=(ObjectArray const &) = delete;

  private:
    std::vector<std::unique_ptr<Object>> mArray;
  };
}
