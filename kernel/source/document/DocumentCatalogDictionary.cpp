#include <document/DocumentCatalogDictionary.h>

#include <string>

namespace{
  std::string const keyType = "Type";
  std::string const valueType = "Catalog";

  std::string const keyVersion = "Version";
  std::string const keyPages = "Pages";
  std::string const keyPageLabels = "PageLabels";
  std::string const keyNames = "Names";
  std::string const keyDests = "Dests";
  std::string const keyViewerPreferences = "ViewerPreferences";

  std::string const keyPageLayout = "PageLayout";
  std::string const valuePageLayoutSinglePage = "SinglePage";
  std::string const valuePageLayoutOneColumn = "OneColumn";
  std::string const valuePageLayoutTwoColumnLeft = "TwoColumnLeft";
  std::string const valuePageLayoutTwoColumnRight = "TwoColumnRight";
  std::string const valuePageLayoutTwoPageLeft = "TwoPageLeft";
  std::string const valuePageLayoutTwoPageRight = "TwoPageRight";

  std::string const keyPageMode = "PageMode";
  std::string const valuePageModeUseNone = "UseNone";
  std::string const valuePageModeUseOutlines = "UseOutlines";
  std::string const valuePageModeUseThumbs = "UseThumbs";
  std::string const valuePageModeFullScreen = "FullScreen";
  std::string const valuePageModeUseOC = "UseOC";
  std::string const valuePageModeUseAttachments = "UseAttachments";

  std::string const keyOutlines = "Outlines";
  std::string const keyThreads = "Threads";
  std::string const keyOpenAction = "OpenAction";
  std::string const keyAA = "AA";
  std::string const keyURI = "URI";
  std::string const keyAcroForm = "AcroForm";
  std::string const keyMetadata = "Metadata";
  std::string const keyStructTreeRoot = "StructTreeRoot";
  std::string const keyMarkInfo = "MarkInfo";
  std::string const keyLang = "Lang";
  std::string const keySpiderInfo = "SpiderInfo";
  std::string const keyOutputIntents = "OutputIntents";
  std::string const keyPieceInfo = "PieceInfo";
  std::string const keyOCProperties = "OCProperties";
  std::string const keyPerms = "Perms";
  std::string const keyLegal = "Legal";
  std::string const keyRequirements = "Requirements";
  std::string const keyCollection = "Collection";
  std::string const keyNeedsRendering = "NeedsRendering";
}

namespace kernel{

  std::unordered_map<CatalogDictionaryKey, std::pair<std::string, uint32_t>> DocumentCatalogDictionary::mCatalogDictionaryMap{
    {CatalogDictionaryKeyVersion,        {keyVersion,           ObjectTypeName}},
    {CatalogDictionaryKeyPages,          {keyPages,             ObjectTypeDictionary}},
    {CatalogDictionaryKeyPageLabels,     {keyPageLabels,        ObjectTypeNumberTree}},
    {CatalogDictionaryKeyNames,          {keyNames,             ObjectTypeDictionary}},
    {CatalogDictionaryKeyDests,          {keyDests,             ObjectTypeDictionary}},
    {CatalogDictionaryKeyViewerPreferen, {keyViewerPreferences, ObjectTypeDictionary}},
    {CatalogDictionaryKeyPageLayout,     {keyPageLayout,        ObjectTypeName}},
    {CatalogDictionaryKeyPageMode,       {keyPageMode,          ObjectTypeName}},
    {CatalogDictionaryKeyOutlines,       {keyOutlines,          ObjectTypeDictionary}},
    {CatalogDictionaryKeyThreads,        {keyThreads,           ObjectTypeArray}},
    {CatalogDictionaryKeyOpenAction,     {keyOpenAction,        ObjectTypeArray | ObjectTypeDictionary}},
    {CatalogDictionaryKeyAA,             {keyAA,                ObjectTypeDictionary}},
    {CatalogDictionaryKeyURI,            {keyURI,               ObjectTypeDictionary}},
    {CatalogDictionaryKeyAcroForm,       {keyAcroForm,          ObjectTypeDictionary}},
    {CatalogDictionaryKeyMetadata,       {keyMetadata,          ObjectTypeStream}},
    {CatalogDictionaryKeyStructTreeRoot, {keyStructTreeRoot,    ObjectTypeDictionary}},
    {CatalogDictionaryKeyMarkInfo,       {keyMarkInfo,          ObjectTypeDictionary}},
    {CatalogDictionaryKeyLang,           {keyLang,              OBJECT_TYPE_TEXT_STRING}},
    {CatalogDictionaryKeySpiderInfo,     {keySpiderInfo,        ObjectTypeDictionary}},
    {CatalogDictionaryKeyOutputIntents,  {keyOutputIntents,     ObjectTypeArray}},
    {CatalogDictionaryKeyPieceInfo,      {keyPieceInfo,         ObjectTypeDictionary}},
    {CatalogDictionaryKeyOCProperties,   {keyOCProperties,      ObjectTypeDictionary}},
    {CatalogDictionaryKeyPerms,          {keyPerms,             ObjectTypeDictionary}},
    {CatalogDictionaryKeyLegal,          {keyLegal,             ObjectTypeDictionary}},
    {CatalogDictionaryKeyRequirements,   {keyRequirements,      ObjectTypeArray}},
    {CatalogDictionaryKeyCollection,     {keyCollection,        ObjectTypeDictionary}},
    {CatalogDictionaryKeyNeedsRendering, {keyNeedsRendering,    ObjectTypeBoolean}}
  };

  DocumentCatalogDictionary::DocumentCatalogDictionary(void)
    : ObjectDictionary(){
    ObjectDictionary::insert(keyType, std::make_unique<ObjectName>(valueType));
  }

  void DocumentCatalogDictionary::addKey(CatalogDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mCatalogDictionaryMap, key, std::move(value));
  }
}
