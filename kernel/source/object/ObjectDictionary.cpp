#include <object/ObjectDictionary.h>

namespace kernel{

  void ObjectDictionary::serialize(pdfout::OutputStream *stream, SerializeParams *params) const{
    std::deque<Object *> indirectObjects;
    serializeBody(stream, params, indirectObjects);
    serializeIndirectObjects(stream, params, indirectObjects);
  }

  void ObjectDictionary::serializeBody(pdfout::OutputStream *stream, SerializeParams *params, std::deque<Object *> &indirectObjects) const{
    std::unique_ptr<ObjectIndirectReference> ref = getReference();

    if (isIndirectable()){
      params->mXref->addRecord(ref->getNumber(), ref->getGeneration(), stream->tellp());
      writeHeader(stream, ref->getNumber(), ref->getGeneration());
      params->mRootObj = this;
    }

    stream->write("<<\r\n", sizeof(uint8_t), 4);

    for (auto const &iter : mDictionary){
      stream->write("/", sizeof(uint8_t), 1);
      stream->write(iter.first.c_str(), sizeof(uint8_t), iter.first.length());
      stream->write(" ", sizeof(uint8_t), 1);

      if (iter.second->isIndirectable()){
        indirectObjects.push_back(iter.second.get());
        iter.second->getReference()->serialize(stream, params);
      }
      else
        iter.second->serialize(stream, params);

      stream->write("\r\n", sizeof(uint8_t), 2);
    }

    stream->write(">>\r\n", sizeof(uint8_t), 4);

    if (isIndirectable())
      writeFooter(stream);
  }

  void ObjectDictionary::serializeIndirectObjects(pdfout::OutputStream *stream, SerializeParams *params, std::deque<Object *> const &indirectObjects) const{
    for (auto const &i : indirectObjects)
      i->serialize(stream, params);
  }

}
