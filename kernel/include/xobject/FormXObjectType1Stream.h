#pragma once

#include <memory>
#include <ContentStream.h>
#include <xobject/FormXObjectStream.h>

namespace kernel{ namespace xobject{

  enum FormXObjectType1DictionaryKey{
    FormXObjectType1DictionaryKeyBBox,          /**< (Required) An array of four numbers in the form coordinate system (see above), giving the coordinates of the left, bottom, right, and top edges, respectively, of the form XObject's bounding box. These boundaries are used to clip the form XObject and to determine its size for caching. */
    FormXObjectType1DictionaryKeyMatrix,        /**< (Optional) An array of six numbers specifying the form matrix, which maps form space into user space. */
    FormXObjectType1DictionaryKeyResources,     /**< (Optional but strongly recommended; PDF 1.2) A dictionary specifying any resources (such as fonts and images) required by the form XObject. */
    FormXObjectType1DictionaryKeyGroup,         /**< (Optional; PDF 1.4) A group attributes dictionary indicating that the contents of the form XObject are to be treated as a group and specifying the attributes of that group. */
    FormXObjectType1DictionaryKeyRef,           /**< (Optional; PDF 1.4) A reference dictionary identifying a page to be imported from another PDF file, and for which the form XObject serves as a proxy. */
    FormXObjectType1DictionaryKeyMetadata,      /**< (Optional; PDF 1.4) A metadata stream containing metadata for the form XObject. */
    FormXObjectType1DictionaryKeyPieceInfo,     /**< (Optional; PDF 1.3) A page-piece dictionary associated with the form XObject. */
    FormXObjectType1DictionaryKeyLastModified,  /**< (Required if PieceInfo is present; optional otherwise; PDF 1.3) The date and time when the form XObject's contents were most recently modified. */
    FormXObjectType1DictionaryKeyStructParent,  /**< (Required if the form XObject is a structural content item; PDF 1.3) The integer key of the form XObject's entry in the structural parent tree. */
    FormXObjectType1DictionaryKeyStructParents, /**< (Required if the form XObject contains marked-content sequences that are structural content items; PDF 1.3) The integer key of the form XObject's entry in the structural parent tree. */
    FormXObjectType1DictionaryKeyOPI,           /**< (Optional; PDF 1.2) An OPI version dictionary for the form XObject. */
    FormXObjectType1DictionaryKeyOC             /**< (Optional; PDF 1.5) An optional content group or optional content membership dictionary specifying the optional content properties for the form XObject. Before the form is processed, its visibility is determined based on this entry. If it is determined to be invisible, the entire form is skipped, as if there were no Do operator to invoke it. */
  };

  class FormXObjectType1Stream : public FormXObjectStream{
  public:
    FormXObjectType1Stream(void);
    ~FormXObjectType1Stream(void);

    using ObjectStream::addKey;
    void addKey(FormXObjectType1DictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<FormXObjectType1DictionaryKey, std::pair<std::string, uint32_t>> mFormXObjectType1StreamMap;

  private:
    FormXObjectType1Stream(FormXObjectType1Stream const &obj) = delete;
    FormXObjectType1Stream &operator=(FormXObjectType1Stream const &) = delete;
  };

}}
