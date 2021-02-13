#pragma once
#ifdef __NOT_IMPLEMENTED___

#include <controls/Control.h>
#include <Color.h>
#include <UString.h>

namespace kernel{ namespace interactive{
  class WidgetAnnotationMKDictionary;
}}

class PushButton : public Control{
  friend class AcroForm;

protected:
  PushButton(kernel::ResourceManager *resourceManager, struct Rectangle const &rect);

public:
  ~PushButton(void);

  std::unique_ptr<kernel::ObjectIndirectReference> serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const;

  void setBorderColor(Color const &color);
  void setBkColor(Color const &color);
  void setNCaption(pdfout::UString const &caption);
  void setRCaption(pdfout::UString const &caption);
  void setDCaption(pdfout::UString const &caption);

private:
  PushButton(PushButton const &) = delete;
  PushButton &operator=(PushButton const &) = delete;

private:
  std::unique_ptr<kernel::interactive::WidgetAnnotationMKDictionary> mMkDictionary;
};
#endif
