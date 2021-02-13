#ifdef __NOT_IMPLEMENTED___

#include <annotations/TextAnnotation.h>

#include <unordered_map>

#include <object/ObjectName.h>
#include <object/ObjectBoolean.h>
#include <object/ObjectUTF16BETextString.h>
#include <interactive/annotation/TextAnnotationDictionary.h>

using namespace kernel;
using namespace kernel::interactive;

namespace{

  std::unordered_map<TextAnnotationIcon, std::string> const textIconToString{
    {TextAnnotationIconComment,      "Comment"},
    {TextAnnotationIconKey,          "Key"},
    {TextAnnotationIconNote,         "Note"},
    {TextAnnotationIconHelp,         "Help"},
    {TextAnnotationIconNewParagraph, "NewParagraph"},
    {TextAnnotationIconParagraph,    "Paragraph"},
    {TextAnnotationIconInsert,       "Insert"}
  };

  std::unordered_map<TextAnnotationState, std::pair<std::string, std::string>> const textStateToString{
    {TextAnnotationStateMarked,    {"Marked", "Marked"}},
    {TextAnnotationStateUnmarked,  {"Marked", "Unmarked"}},
    {TextAnnotationStateAccepted,  {"Review", "Accepted"}},
    {TextAnnotationStateRejected,  {"Review", "Rejected"}},
    {TextAnnotationStateCancelled, {"Review", "Cancelled"}},
    {TextAnnotationStateCompleted, {"Review", "Completed"}},
    {TextAnnotationStateNone,      {"Review", "None"}}
  };

}

TextAnnotation::TextAnnotation(kernel::ResourceManager *resourceManager, struct Rectangle const &rect, pdfout::UString const &name)
  : MarkupAnnotation(resourceManager, rect, name),
  mInitialState(false),
  mIconName(TextAnnotationIconNone),
  mState(TextAnnotationStateNone){
}

TextAnnotation::~TextAnnotation(void) {}

std::unique_ptr<kernel::ObjectIndirectReference> TextAnnotation::serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const{
  std::unique_ptr<TextAnnotationDictionary> annot = std::make_unique<TextAnnotationDictionary>();

  MarkupAnnotation::serialize(annot.get());

  annot->addKey(TextAnnotationDictionaryKeyOpen, std::make_unique<ObjectBoolean>(mInitialState));

  auto iconIter = textIconToString.find(mIconName);
  if (iconIter != std::end(textIconToString))
    annot->addKey(TextAnnotationDictionaryKeyName, std::make_unique<ObjectName>(iconIter->second));

  auto stateIter = textStateToString.find(mState);
  if (stateIter != std::end(textStateToString)){
    annot->addKey(TextAnnotationDictionaryKeyStateModel, std::make_unique<ObjectUTF16BETextString>(UString(stateIter->second.first.c_str(), Encoding_CHAR)));
    annot->addKey(TextAnnotationDictionaryKeyState, std::make_unique<ObjectUTF16BETextString>(UString(stateIter->second.second.c_str(), Encoding_CHAR)));
  }

  annot->serialize(stream, params);

  return annot->getReference();
}

void TextAnnotation::setInitialState(bool isOpen){
  mInitialState = isOpen;
}

void TextAnnotation::setIconName(TextAnnotationIcon iconName){
  mIconName = iconName;
}

void TextAnnotation::setState(TextAnnotationState state){
  mState = state;
}

#endif
