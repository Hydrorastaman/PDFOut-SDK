#include <graphics/color/ColorSpaceCIEBased.h>

#include <memory>

#include <ToString.h>
#include <ResourceUid.h>
#include <object/ObjectNumber.h>
#include <object/ObjectInteger.h>
#include <object/ObjectArray.h>
#include <object/ObjectDictionary.h>

namespace{
  std::string const keyWhitePoint = "WhitePoint";
  std::string const keyBlackPoint = "BlackPoint";
  std::string const keyGamma = "Gamma";
  std::string const keyMatrix = "Matrix";
  std::string const keyRange = "Range";
}


namespace kernel{ namespace graphics{ namespace color{

  ColorSpaceCalGray::ColorSpaceCalGray(ColorWhitePoint const &whitePoint)
    : ColorSpaceImpl(ColorSpaceFamilyCalGray),
    mWhitePoint(whitePoint),
    mBlackPoint(),
    mGamma(1.0){
  }

  ColorSpaceCalGray::ColorSpaceCalGray(ColorWhitePoint const &whitePoint, ColorBlackPoint const &blackPoint, float gamma)
    : ColorSpaceImpl(ColorSpaceFamilyCalGray),
    mWhitePoint(whitePoint),
    mBlackPoint(blackPoint),
    mGamma(gamma){
  }

  std::unique_ptr<kernel::ObjectIndirectReference> ColorSpaceCalGray::serialize(pdfout::OutputStream *stream, SerializeParams *params) const{
    std::unique_ptr<ObjectDictionary> dictionary(std::make_unique<ObjectDictionary>(IndirectTypeNonIndirectable));

    std::unique_ptr<ObjectArray> whitePoint(std::make_unique<ObjectArray>(IndirectTypeNonIndirectable));
    whitePoint->insert(std::make_unique<ObjectNumber>(mWhitePoint.mX));
    whitePoint->insert(std::make_unique<ObjectNumber>(mWhitePoint.mY));
    whitePoint->insert(std::make_unique<ObjectNumber>(mWhitePoint.mZ));

    std::unique_ptr<ObjectArray> blackPoint(std::make_unique<ObjectArray>(IndirectTypeNonIndirectable));
    blackPoint->insert(std::make_unique<ObjectNumber>(mBlackPoint.mX));
    blackPoint->insert(std::make_unique<ObjectNumber>(mBlackPoint.mY));
    blackPoint->insert(std::make_unique<ObjectNumber>(mBlackPoint.mZ));

    dictionary->insert(keyWhitePoint, std::move(whitePoint));
    dictionary->insert(keyBlackPoint, std::move(blackPoint));
    dictionary->insert(keyGamma, std::make_unique<ObjectNumber>(mGamma));

    std::unique_ptr<ObjectArray> array(std::make_unique<ObjectArray>());
    array->insert(std::make_unique<ObjectName>(toString(ColorSpaceFamilyCalGray)));
    array->insert(std::move(dictionary));

    array->serialize(stream, params);
    std::unique_ptr<ObjectIndirectReference> ref = array->getReference();
    return ref;
  }

  ColorSpaceCalRGB::ColorSpaceCalRGB(ColorWhitePoint const &whitePoint)
    : ColorSpaceImpl(ColorSpaceFamilyCalRGB),
    mWhitePoint(whitePoint),
    mBlackPoint(),
    mGamma(),
    mMatrix(){
  }

  ColorSpaceCalRGB::ColorSpaceCalRGB(ColorWhitePoint const &whitePoint, ColorBlackPoint const &blackPoint, ColorGamma const &gamma, ColorCalMatrix const &matrix)
    : ColorSpaceImpl(ColorSpaceFamilyCalRGB),
    mWhitePoint(whitePoint),
    mBlackPoint(blackPoint),
    mGamma(gamma),
    mMatrix(matrix){
  }

  std::unique_ptr<kernel::ObjectIndirectReference> ColorSpaceCalRGB::serialize(pdfout::OutputStream *stream, SerializeParams *params) const{
    std::unique_ptr<ObjectDictionary> dictionary(std::make_unique<ObjectDictionary>(IndirectTypeNonIndirectable));

    std::unique_ptr<ObjectArray> whitePoint(std::make_unique<ObjectArray>(IndirectTypeNonIndirectable));
    whitePoint->insert(std::make_unique<ObjectNumber>(mWhitePoint.mX));
    whitePoint->insert(std::make_unique<ObjectNumber>(mWhitePoint.mY));
    whitePoint->insert(std::make_unique<ObjectNumber>(mWhitePoint.mZ));

    std::unique_ptr<ObjectArray> blackPoint(std::make_unique<ObjectArray>(IndirectTypeNonIndirectable));
    blackPoint->insert(std::make_unique<ObjectNumber>(mBlackPoint.mX));
    blackPoint->insert(std::make_unique<ObjectNumber>(mBlackPoint.mY));
    blackPoint->insert(std::make_unique<ObjectNumber>(mBlackPoint.mZ));

    std::unique_ptr<ObjectArray> gamma(std::make_unique<ObjectArray>(IndirectTypeNonIndirectable));
    gamma->insert(std::make_unique<ObjectNumber>(mGamma.mX));
    gamma->insert(std::make_unique<ObjectNumber>(mGamma.mY));
    gamma->insert(std::make_unique<ObjectNumber>(mGamma.mZ));

    std::unique_ptr<ObjectArray> matrix(std::make_unique<ObjectArray>(IndirectTypeNonIndirectable));
    matrix->insert(std::make_unique<ObjectNumber>(mMatrix.xA));
    matrix->insert(std::make_unique<ObjectNumber>(mMatrix.xB));
    matrix->insert(std::make_unique<ObjectNumber>(mMatrix.xC));

    matrix->insert(std::make_unique<ObjectNumber>(mMatrix.yA));
    matrix->insert(std::make_unique<ObjectNumber>(mMatrix.yB));
    matrix->insert(std::make_unique<ObjectNumber>(mMatrix.yC));

    matrix->insert(std::make_unique<ObjectNumber>(mMatrix.zA));
    matrix->insert(std::make_unique<ObjectNumber>(mMatrix.zB));
    matrix->insert(std::make_unique<ObjectNumber>(mMatrix.zC));

    dictionary->insert(keyWhitePoint, std::move(whitePoint));
    dictionary->insert(keyBlackPoint, std::move(blackPoint));
    dictionary->insert(keyGamma, std::move(gamma));
    dictionary->insert(keyMatrix, std::move(matrix));

    std::unique_ptr<ObjectArray> array(std::make_unique<ObjectArray>());
    array->insert(std::make_unique<ObjectName>(toString(ColorSpaceFamilyCalRGB)));
    array->insert(std::move(dictionary));

    array->serialize(stream, params);
    std::unique_ptr<ObjectIndirectReference> ref = array->getReference();
    return ref;
  }

  ColorSpaceLab::ColorSpaceLab(ColorWhitePoint const &whitePoint)
    : ColorSpaceImpl(ColorSpaceFamilyLab),
    mWhitePoint(whitePoint),
    mBlackPoint(),
    mRange(){
  }

  ColorSpaceLab::ColorSpaceLab(ColorWhitePoint const &whitePoint, ColorBlackPoint const &blackPoint, ColorLabRange const &range)
    : ColorSpaceImpl(ColorSpaceFamilyLab),
    mWhitePoint(whitePoint),
    mBlackPoint(blackPoint),
    mRange(range){
  }

  std::unique_ptr<kernel::ObjectIndirectReference> ColorSpaceLab::serialize(pdfout::OutputStream *stream, SerializeParams *params) const{
    std::unique_ptr<ObjectDictionary> dictionary(std::make_unique<ObjectDictionary>(IndirectTypeNonIndirectable));

    std::unique_ptr<ObjectArray> whitePoint(std::make_unique<ObjectArray>(IndirectTypeNonIndirectable));
    whitePoint->insert(std::make_unique<ObjectNumber>(mWhitePoint.mX));
    whitePoint->insert(std::make_unique<ObjectNumber>(mWhitePoint.mY));
    whitePoint->insert(std::make_unique<ObjectNumber>(mWhitePoint.mZ));

    std::unique_ptr<ObjectArray> blackPoint(std::make_unique<ObjectArray>(IndirectTypeNonIndirectable));
    blackPoint->insert(std::make_unique<ObjectNumber>(mBlackPoint.mX));
    blackPoint->insert(std::make_unique<ObjectNumber>(mBlackPoint.mY));
    blackPoint->insert(std::make_unique<ObjectNumber>(mBlackPoint.mZ));

    std::unique_ptr<ObjectArray> range(std::make_unique<ObjectArray>(IndirectTypeNonIndirectable));
    range->insert(std::make_unique<ObjectNumber>(mRange.aMin));
    range->insert(std::make_unique<ObjectNumber>(mRange.aMax));
    range->insert(std::make_unique<ObjectNumber>(mRange.bMin));
    range->insert(std::make_unique<ObjectNumber>(mRange.bMax));

    dictionary->insert(keyWhitePoint, std::move(whitePoint));
    dictionary->insert(keyBlackPoint, std::move(blackPoint));
    dictionary->insert(keyRange, std::move(range));

    std::unique_ptr<ObjectArray> array(std::make_unique<ObjectArray>());
    array->insert(std::make_unique<ObjectName>(toString(ColorSpaceFamilyLab)));
    array->insert(std::move(dictionary));

    array->serialize(stream, params);
    std::unique_ptr<ObjectIndirectReference> ref = array->getReference();
    return ref;
  }

  ColorSpaceICC::ColorSpaceICC(ColorICCComponents components)
    : ColorSpaceImpl(ColorSpaceFamilyICCBased),
    mStream(std::make_unique<ColorICCStream>()){

    mStream->addKey(ICCStreamDictionaryKeyN, std::make_unique<ObjectInteger>(components));
  }

  std::unique_ptr<kernel::ObjectIndirectReference> ColorSpaceICC::serialize(pdfout::OutputStream *stream, SerializeParams *params) const{
    std::unique_ptr<ObjectArray> array(std::make_unique<ObjectArray>(IndirectTypeIndirectable));
    array->insert(std::make_unique<ObjectName>(toString(ColorSpaceFamilyICCBased)));
    array->insert(mStream->getReference());
    array->serialize(stream, params);

    mStream->serialize(stream, params);
    std::unique_ptr<ObjectIndirectReference> ref = mStream->getReference();
    return ref;
  }

}}}
