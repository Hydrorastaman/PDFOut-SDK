#include <graphics/GraphicsStateParameterDictionary.h>

#include <string>

namespace{
  std::string const keyType = "Type";
  std::string const valueType = "ExtGState";

  std::string const keyLW = "LW";
  std::string const keyLC = "LC";
  std::string const keyLJ = "LJ";
  std::string const keyML = "ML";
  std::string const keyD = "D";
  std::string const keyRI = "RI";
  std::string const keyOP = "OP";
  std::string const keyop = "op";
  std::string const keyOPM = "OPM";
  std::string const keyFont = "Font";
  std::string const keyBG = "BG";
  std::string const keyBG2 = "BG2";
  std::string const keyUCR = "UCR";
  std::string const keyUCR2 = "UCR2";
  std::string const keyTR = "TR";
  std::string const keyTR2 = "TR2";
  std::string const keyHT = "HT";
  std::string const keyFL = "FL";
  std::string const keySM = "SM";
  std::string const keySA = "SA";
  std::string const keyBM = "BM";
  std::string const keySMask = "SMask";
  std::string const keyCA = "CA";
  std::string const keyca = "ca";
  std::string const keyAIS = "AIS";
  std::string const keyTK = "TK";
}

namespace kernel{

  std::unordered_map<GraphicsStateParameterDictionaryKey, std::pair<std::string, uint32_t>> GraphicsStateParameterDictionary::mGraphicsStateParameterDictionaryMap{
    {GraphicsStateParameterDictionaryKeyLW,    {keyLW,    ObjectTypeNumber}},
    {GraphicsStateParameterDictionaryKeyLC,    {keyLC,    ObjectTypeInteger}},
    {GraphicsStateParameterDictionaryKeyLJ,    {keyLJ,    ObjectTypeInteger}},
    {GraphicsStateParameterDictionaryKeyML,    {keyML,    ObjectTypeNumber}},
    {GraphicsStateParameterDictionaryKeyD,     {keyD,     ObjectTypeArray}},
    {GraphicsStateParameterDictionaryKeyRI,    {keyRI,    ObjectTypeName}},
    {GraphicsStateParameterDictionaryKeyOP,    {keyOP,    ObjectTypeBoolean}},
    {GraphicsStateParameterDictionaryKeyop,    {keyop,    ObjectTypeBoolean}},
    {GraphicsStateParameterDictionaryKeyOPM,   {keyOPM,   ObjectTypeInteger}},
    {GraphicsStateParameterDictionaryKeyFont,  {keyFont,  ObjectTypeArray}},
    {GraphicsStateParameterDictionaryKeyBG,    {keyBG,    ObjectTypeFunction}},
    {GraphicsStateParameterDictionaryKeyBG2,   {keyBG2,   ObjectTypeFunction | ObjectTypeName}},
    {GraphicsStateParameterDictionaryKeyUCR,   {keyUCR,   ObjectTypeFunction}},
    {GraphicsStateParameterDictionaryKeyUCR2,  {keyUCR2,  ObjectTypeFunction | ObjectTypeName}},
    {GraphicsStateParameterDictionaryKeyTR,    {keyTR,    ObjectTypeFunction | ObjectTypeArray | ObjectTypeName}},
    {GraphicsStateParameterDictionaryKeyTR2,   {keyTR2,   ObjectTypeFunction | ObjectTypeArray | ObjectTypeName}},
    {GraphicsStateParameterDictionaryKeyHT,    {keyHT,    ObjectTypeDictionary | ObjectTypeStream | ObjectTypeName}},
    {GraphicsStateParameterDictionaryKeyFL,    {keyFL,    ObjectTypeNumber}},
    {GraphicsStateParameterDictionaryKeySM,    {keySM,    ObjectTypeNumber}},
    {GraphicsStateParameterDictionaryKeySA,    {keySA,    ObjectTypeBoolean}},
    {GraphicsStateParameterDictionaryKeyBM,    {keyBM,    ObjectTypeName | ObjectTypeArray}},
    {GraphicsStateParameterDictionaryKeySMask, {keySMask, ObjectTypeDictionary | ObjectTypeName}},
    {GraphicsStateParameterDictionaryKeyCA,    {keyCA,    ObjectTypeNumber}},
    {GraphicsStateParameterDictionaryKeyca,    {keyca,    ObjectTypeNumber}},
    {GraphicsStateParameterDictionaryKeyAIS,   {keyAIS,   ObjectTypeBoolean}},
    {GraphicsStateParameterDictionaryKeyTK,    {keyTK,    ObjectTypeBoolean}}
  };

  GraphicsStateParameterDictionary::GraphicsStateParameterDictionary(void)
    : ObjectDictionary(){
    ObjectDictionary::insert(keyType, std::make_unique<ObjectName>(valueType));
  }

  void GraphicsStateParameterDictionary::addKey(GraphicsStateParameterDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mGraphicsStateParameterDictionaryMap, key, std::move(value));
  }

  void GraphicsStateParameterDictionary::removeKey(GraphicsStateParameterDictionaryKey key){
    ObjectDictionary::removeKey(mGraphicsStateParameterDictionaryMap, key);
  }

  Object const *GraphicsStateParameterDictionary::findKey(GraphicsStateParameterDictionaryKey key) const{
    return ObjectDictionary::findKey(mGraphicsStateParameterDictionaryMap, key);
  }

}
