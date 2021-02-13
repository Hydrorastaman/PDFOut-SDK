#pragma once

#include <AcroForm.h>

#include <memory>
#include <mutex>
#include <deque>

namespace pdfout{
  class OutputStream;
}

namespace kernel{
  struct SerializeParams;
  class ObjectIndirectReference;
  class ResourceManager;

  template <typename T>
  class UserObjectManager;

  namespace interactive{
    class AcroFormDictionary;
  }
}

namespace pdfout{ namespace impl{

  class AcroFormImpl : public AcroForm{
  public:
    AcroFormImpl(kernel::ResourceManager *resourceManager);
    ~AcroFormImpl(void);

    void serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const;
    std::unique_ptr<kernel::ObjectIndirectReference> getReference(void) const;

    //PushButton *createPushButton(struct Rectangle const &rect);
    //RadioButtons *createRadioButtons(pdfout::UString const &name, struct Rectangle const &rect);
    //CheckBoxes *createCheckBoxes(pdfout::UString const &name, struct Rectangle const &rect);
    //EditBox *createEditBox(pdfout::UString const &name, struct Rectangle const &rect);
    //ListBox *createListBox(pdfout::UString const &name, struct Rectangle const &rect);

  private:
    AcroFormImpl(AcroFormImpl const &) = delete;
    AcroFormImpl &operator=(AcroFormImpl const &) = delete;

  private:
    mutable std::mutex mAccessMutex;
    std::unique_ptr<kernel::ObjectIndirectReference> mRef;
    kernel::ResourceManager *mResourceManager;
    //std::deque<std::unique_ptr<Control>> mControlManager;
  };
}}
