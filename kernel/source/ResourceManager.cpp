#include <ResourceManager.h>
#include <Font.h>
#include <Image.h>
#include <ColorSpace.h>
#include <ExtGState.h>
#include <document/DocumentResourceDictionary.h>
#include <object/ObjectDictionary.h>
#include <object/ObjectArray.h>
#include <font/FontDescriptorDictionary.h>

namespace{
  struct{
    kernel::ProcSet mProcSet;
    char const *mValue;
  } procedureSet[] = {
    {kernel::ProcSetPDF, "PDF"},
    {kernel::ProcSetText, "Text"},
    {kernel::ProcSetImageB, "ImageB"},
    {kernel::ProcSetImageC, "ImageC"},
    {kernel::ProcSetImageI, "ImageI"}
  };

}

//#ifdef _DEBUG
//#include <stream/SimpleOutputFileStream.h>
//int i = 0;
//#endif

namespace kernel{

  ResourceManager::ResourceManager(void)
    : mAccessMutex(),
    mFontResource(),
    mImageResource(),
    mColorSpaceResource(),
    mExtGStateResource(),
    mProcSet(0),
    mResourceDictionary(std::make_unique<DocumentResourceDictionary>()){
  }

  ResourceManager::~ResourceManager(void) {}

  void ResourceManager::addFont(FontImpl *font){
    std::lock_guard<std::mutex> lock(mAccessMutex);
    std::string const uid = font->getUID();
    auto iter = mFontResource.find(uid);
    if (iter == std::end(mFontResource))
      mFontResource[uid] = font;
    else{
      if (font->getVersion() > iter->second->getVersion())
        mFontResource[uid] = font;
    }
  }

  void ResourceManager::addProcSet(ProcSet procSet){
    mProcSet |= (uint16_t) procSet;
  }

  void ResourceManager::addImage(ImageImpl *image){
    std::lock_guard<std::mutex> lock(mAccessMutex);
    std::string const uid = image->getUID();
    auto iter = mImageResource.find(uid);
    if (iter == std::end(mImageResource))
      mImageResource.insert(std::make_pair(uid, image));
  }

  void ResourceManager::addColorSpace(ColorSpaceImpl *colorSpace){
    std::lock_guard<std::mutex> lock(mAccessMutex);
    std::string const uid = colorSpace->getUID();
    auto iter = mColorSpaceResource.find(uid);
    if (iter == std::end(mColorSpaceResource))
      mColorSpaceResource.insert(std::make_pair(uid, colorSpace));
  }

  void ResourceManager::addExtGState(ExtGStateImpl *extGState){
    std::lock_guard<std::mutex> lock(mAccessMutex);
    std::string const uid = extGState->getUID();
    auto iter = mExtGStateResource.find(uid);
    if (iter == std::end(mExtGStateResource))
      mExtGStateResource.insert(std::make_pair(uid, extGState));
  }

  void ResourceManager::serialize(pdfout::OutputStream *stream, SerializeParams *params) const{
    std::lock_guard<std::mutex> lock(mAccessMutex);

    mResourceDictionary->clear();

    // Font dictionary
    std::unique_ptr<ObjectDictionary> fontDictionary = std::make_unique<ObjectDictionary>();
    mResourceDictionary->addKey(ResourceDictionaryKeyFont, fontDictionary->getReference());

    for (auto const &font: mFontResource){
//#ifdef _DEBUG
//std::string path = "c:/tmp/font/subset/font_" + std::to_string(i) + ".ttf";
//std::string path2 = "c:/tmp/font/subset/subset_" + std::to_string(i) + ".ttf";
//++i;
//
//std::unique_ptr<pdfout::OutputStream> fs(std::make_unique<pdfout::SimpleOutputFileStream>(path.c_str()));
//font.second->saveToStream(fs.get());
////std::unique_ptr<pdfout::OutputStream> fs2(std::make_unique<pdfout::SimpleOutputFileStream>(path2.c_str()));
////subset->saveToStream(fs2.get());
//#endif

      if (params->mIsFontSubsetting){
        auto iter = mUsedCIDs.find(font.second->getUID());
        if (iter != std::end(mUsedCIDs) && !iter->second.empty()){
          std::vector<uint32_t> cids;
          cids.reserve(iter->second.size());
          for (auto const &elem : iter->second)
            cids.push_back(elem);

          std::unique_ptr<FontImpl> subset(font.second->createSubset(cids));
          std::unique_ptr<ObjectIndirectReference> ref(subset->serialize(stream, params));
          fontDictionary->insert(font.first, std::move(ref));
        }
      }
      else{
        std::unique_ptr<ObjectIndirectReference> ref(font.second->serialize(stream, params));
        fontDictionary->insert(font.first, std::move(ref));
      }
    }

    // ProcSet array
    std::unique_ptr<ObjectArray> procSet = std::make_unique<ObjectArray>();
    mResourceDictionary->addKey(ResourceDictionaryKeyProcSet, procSet->getReference());
    for (auto const &proc : procedureSet)
      if (mProcSet & (uint16_t) proc.mProcSet)
        procSet->insert(std::make_unique<ObjectName>(proc.mValue));

    // XObject dictionary
    std::unique_ptr<ObjectDictionary> xObjectDictionary = std::make_unique<ObjectDictionary>();
    mResourceDictionary->addKey(ResourceDictionaryKeyXObject, xObjectDictionary->getReference());

    for (auto const image: mImageResource){
      std::unique_ptr<ObjectIndirectReference> ref(image.second->serialize(stream, params));
      xObjectDictionary->insert(image.first, std::move(ref));
    }

    // ColorSpace dictionary
    std::unique_ptr<ObjectDictionary> csDictionary = std::make_unique<ObjectDictionary>();
    mResourceDictionary->addKey(ResourceDictionaryKeyColorSpace, csDictionary->getReference());

    for (auto const cs: mColorSpaceResource){
      std::unique_ptr<ObjectIndirectReference> ref(cs.second->serialize(stream, params));
      csDictionary->insert(cs.first, std::move(ref));
    }

    // ExtGState dictionary
    std::unique_ptr<ObjectDictionary> gsDictionary = std::make_unique<ObjectDictionary>();
    mResourceDictionary->addKey(ResourceDictionaryKeyExtGState, gsDictionary->getReference());

    for (auto const gs: mExtGStateResource){
      std::unique_ptr<ObjectIndirectReference> ref(gs.second->serialize(stream, params));
      gsDictionary->insert(gs.first, std::move(ref));
    }

    // TODO: add other resources to resourceDictionary

    mResourceDictionary->serialize(stream, params);
    fontDictionary->serialize(stream, params);
    procSet->serialize(stream, params);
    xObjectDictionary->serialize(stream, params);
    csDictionary->serialize(stream, params);
    gsDictionary->serialize(stream, params);
  }

  std::unique_ptr<ObjectIndirectReference> ResourceManager::getReference(void) const{
    return mResourceDictionary->getReference();
  }

  void ResourceManager::updateUsedCIDs(FontImpl *font, uint16_t const *cids, std::size_t size, Endian cidEndian){
    Endian const endian = getEndian();
    std::lock_guard<std::mutex> lock(mAccessMutex);
    if (!font || !cids || !size)
      return;

    std::string const uid = font->getUID();
    std::unordered_set<uint16_t> &set = mUsedCIDs[uid];

    if (endian == cidEndian){
      for (std::size_t i = 0; i < size; ++i)
        set.insert(cids[i]);
    }
    else{
      for (std::size_t i = 0; i < size; ++i){
        uint16_t value = cids[i];
        swapByteOrder(value);
        set.insert(value);
      }
    }
  }

}
