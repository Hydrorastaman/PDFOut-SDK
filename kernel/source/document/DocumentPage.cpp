#include <document/DocumentPage.h>

namespace{
  std::string const keyType = "Type";
  std::string const valueType = "Page";

  std::string const keyParent = "Parent";
  std::string const keyLastModified = "LastModified";
  std::string const keyResources = "Resources";
  std::string const keyMediaBox = "MediaBox";
  std::string const keyCropBox = "CropBox";
  std::string const keyBleedBox = "BleedBox";
  std::string const keyTrimBox = "TrimBox";
  std::string const keyArtBox = "ArtBox";
  std::string const keyBoxColorInfo = "BoxColorInfo";
  std::string const keyContents = "Contents";
  std::string const keyRotate = "Rotate";
  std::string const keyGroup = "Group";
  std::string const keyThumb = "Thumb";
  std::string const keyB = "B";
  std::string const keyDur = "Dur";
  std::string const keyTrans = "Trans";
  std::string const keyAnnots = "Annots";
  std::string const keyAA = "AA";
  std::string const keyMetadata = "Metadata";
  std::string const keyPieceInfo = "PieceInfo";
  std::string const keyStructParents = "StructParents";
  std::string const keyID = "ID";
  std::string const keyPZ = "PZ";
  std::string const keySeparationInfo = "SeparationInfo";
  std::string const keyTabs = "Tabs";
  std::string const keyTemplateInstantiated = "TemplateInstantiated";
  std::string const keyPresSteps = "PresSteps";
  std::string const keyUserUnit = "UserUnit";
  std::string const keyVP = "VP";
}

namespace kernel{

  std::unordered_map<DocumentPageKey, std::pair<std::string, uint32_t>> DocumentPage::mDocumentPageMap{
    {DocumentPageKeyParent,               {keyParent,               ObjectTypeDictionary}},
    {DocumentPageKeyLastModified,         {keyLastModified,         ObjectTypeDate}},
    {DocumentPageKeyResources,            {keyResources,            ObjectTypeDictionary}},
    {DocumentPageKeyMediaBox,             {keyMediaBox,             ObjectTypeRectangle}},
    {DocumentPageKeyCropBox,              {keyCropBox,              ObjectTypeRectangle}},
    {DocumentPageKeyBleedBox,             {keyBleedBox,             ObjectTypeRectangle}},
    {DocumentPageKeyTrimBox,              {keyTrimBox,              ObjectTypeRectangle}},
    {DocumentPageKeyArtBox,               {keyArtBox,               ObjectTypeRectangle}},
    {DocumentPageKeyBoxColorInfo,         {keyBoxColorInfo,         ObjectTypeDictionary}},
    {DocumentPageKeyContents,             {keyContents,             ObjectTypeDictionary | ObjectTypeStream}},
    {DocumentPageKeyRotate,               {keyRotate,               ObjectTypeInteger}},
    {DocumentPageKeyGroup,                {keyGroup,                ObjectTypeDictionary}},
    {DocumentPageKeyThumb,                {keyThumb,                ObjectTypeStream}},
    {DocumentPageKeyB,                    {keyB,                    ObjectTypeArray}},
    {DocumentPageKeyDur,                  {keyDur,                  ObjectTypeNumber}},
    {DocumentPageKeyTrans,                {keyTrans,                ObjectTypeDictionary}},
    {DocumentPageKeyAnnots,               {keyAnnots,               ObjectTypeArray}},
    {DocumentPageKeyAA,                   {keyAA,                   ObjectTypeDictionary}},
    {DocumentPageKeyMetadata,             {keyMetadata,             ObjectTypeStream}},
    {DocumentPageKeyPieceInfo,            {keyPieceInfo,            ObjectTypeDictionary}},
    {DocumentPageKeyStructParents,        {keyStructParents,        ObjectTypeInteger}},
    {DocumentPageKeyID,                   {keyID,                   ObjectTypeByteString}},
    {DocumentPageKeyPZ,                   {keyPZ,                   ObjectTypeNumber}},
    {DocumentPageKeySeparationInfo,       {keySeparationInfo,       ObjectTypeDictionary}},
    {DocumentPageKeyTabs,                 {keyTabs,                 ObjectTypeName}},
    {DocumentPageKeyTemplateInstantiated, {keyTemplateInstantiated, ObjectTypeName}},
    {DocumentPageKeyPresSteps,            {keyPresSteps,            ObjectTypeDictionary}},
    {DocumentPageKeyUserUnit,             {keyUserUnit,             ObjectTypeNumber}},
    {DocumentPageKeyVP,                   {keyVP,                   ObjectTypeDictionary}},
  };

  DocumentPage::DocumentPage(void)
    : ObjectDictionary(){
    ObjectDictionary::insert(keyType, std::make_unique<ObjectName>(valueType));
  }

  DocumentPage::~DocumentPage(void) {}

  void DocumentPage::addKey(DocumentPageKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mDocumentPageMap, key, std::move(value));
  }

  Object *DocumentPage::find(DocumentPageKey key) const{
    auto iter = mDocumentPageMap.find(key);
    if (iter != std::end(mDocumentPageMap))
      return ObjectDictionary::find(iter->second.first);

    return nullptr;
  }
}
