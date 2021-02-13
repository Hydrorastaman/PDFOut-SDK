#pragma once

#include <map>
#include <mutex>
#include <memory>
#include <string>
#include <unordered_set>

#include <FontImpl.h>
#include <ImageImpl.h>
#include <Serializable.h>
#include <ExtGStateImpl.h>
#include <InternalTools.h>
#include <ColorSpaceImpl.h>

namespace kernel{

  class DocumentResourceDictionary;
  class ObjectIndirectReference;

  using pdfout::impl::FontImpl;
  using pdfout::impl::ImageImpl;
  using pdfout::impl::ExtGStateImpl;
  using pdfout::impl::ColorSpaceImpl;

  enum ProcSet{
    ProcSetPDF    = 0x01, /**< Painting and graphics state */
    ProcSetText   = 0x02, /**< Text */
    ProcSetImageB = 0x04, /**< Grayscale images or image masks */
    ProcSetImageC = 0x08, /**< Color images */
    ProcSetImageI = 0x10  /**< Indexed (color-table) images */
  };

  class ResourceManager : public Serializable{
  public:
    ResourceManager(void);
    ~ResourceManager(void);

    void serialize(pdfout::OutputStream *stream, SerializeParams *params) const;
    std::unique_ptr<ObjectIndirectReference> getReference(void) const;

    void updateUsedCIDs(FontImpl *font, uint16_t const *cids, std::size_t size, Endian cidEndian = EndianBig);

    void addFont(FontImpl *font);
    void addProcSet(ProcSet procSet);
    void addImage(ImageImpl *image);
    void addColorSpace(ColorSpaceImpl *colorSpace);
    void addExtGState(ExtGStateImpl *extGState);

  private:
    ResourceManager(ResourceManager const &) = delete;
    ResourceManager &operator=(ResourceManager const &) = delete;

  private:
    mutable std::mutex mAccessMutex;
    std::map<std::string, FontImpl *> mFontResource;
    std::map<std::string, ImageImpl *> mImageResource;
    std::map<std::string, ColorSpaceImpl *> mColorSpaceResource;
    std::map<std::string, ExtGStateImpl *> mExtGStateResource;
    uint16_t mProcSet;
    std::unique_ptr<kernel::DocumentResourceDictionary> mResourceDictionary;
    std::map<std::string, std::unordered_set<uint16_t>> mUsedCIDs; // Used cids in the font, <font UID, CIDS>
  };

}
