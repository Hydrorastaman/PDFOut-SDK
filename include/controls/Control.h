#pragma once
#ifdef __NOT_IMPLEMENTED___

#include <memory>
#include <string>
#include <map>
#include <vector>

#include <Types.h>

class OutputStream;
class FormXObject;

enum AppearanceState{
  AppearanceStateOn,
  AppearanceStateOff
};

namespace kernel{
  class ObjectIndirectReference;
  struct SerializeParams;
  class ResourceManager;

  namespace interactive{
    class WidgetAnnotationDictionary;
  }
}

class PushButton;
class RadioButtons;
class CheckBoxes;
class EditBox;
class ListBox;

class Control{
protected:
  Control(kernel::ResourceManager *resourceManager, struct Rectangle const &rect);

public:
  virtual ~Control(void);
  std::string const &getUID(void) const;

  std::unique_ptr<kernel::ObjectIndirectReference> getReference(void) const;
  virtual std::unique_ptr<kernel::ObjectIndirectReference> serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const = 0;

  struct Rectangle getRect(void) const;

  FormXObject *createAppearance(void);
  void setNormalAppearance(FormXObject *xObj, std::string const &name);
  void setRolloverAppearance(FormXObject *xObj, std::string const &name);
  void setDownAppearance(FormXObject *xObj, std::string const &name);
  void setDefaultApperance(std::string const &name);

private:
  Control(Control const &) = delete;
  Control &operator=(Control const &) = delete;

protected:
  std::string mUID;
  std::unique_ptr<kernel::ObjectIndirectReference> mWidgetRef;
  struct Rectangle mRect;
  kernel::ResourceManager *mResourceManager;
  std::string mDefaultApperance;
  std::vector<std::unique_ptr<FormXObject>> mFormXObjects;
  std::map<std::string, FormXObject *> mNormalAppearance;
  std::map<std::string, FormXObject *> mRolloverAppearance;
  std::map<std::string, FormXObject *> mDownAppearance;
};
#endif
