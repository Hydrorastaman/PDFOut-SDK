#include <ToString.h>

#include <Types.h>
#include <Filter.h>
#include <Function.h>
#include <Exception.h>
#include <ColorSpace.h>
#include <InternalTools.h>
#include <encryption/CryptFilter.h>

using namespace pdfout;

namespace kernel{

  std::string toString(pdfout::UString const &ustr){
    if (ustr.getEncoding() == Encoding_CHAR){
      char const *data = (char const *) ustr.getData();
      std::size_t const size = ustr.getSize();
      return std::string(data, data + size / sizeof(char));
    }
    else{
      UString const tmp = ustr.encode(Encoding_CHAR);
      char const *data = (char const *) tmp.getData();
      std::size_t const size = tmp.getSize();
      return std::string(data, data + size / sizeof(char));
    }

    return std::string("");
  }

  std::wstring toWString(pdfout::UString const &ustr){
    if (ustr.getEncoding() == Encoding_WCHAR_T){
      wchar_t const *data = (wchar_t const *) ustr.getData();
      std::size_t const size = ustr.getSize();
      return std::wstring(data, data + size / sizeof(wchar_t));
    }
    else{
      UString const tmp = ustr.encode(Encoding_WCHAR_T);
      wchar_t const *data = (wchar_t const *) tmp.getData();
      std::size_t const size = tmp.getSize();
      return std::wstring(data, data + size / sizeof(wchar_t));
    }

    return std::wstring(L"");
  }

  std::string toString(RenderingIntent ri){
    switch (ri){
      case RenderingIntentAbsoluteColorimetric: return "AbsoluteColorimetric";
      case RenderingIntentRelativeColorimetric: return "RelativeColorimetric";
      case RenderingIntentSaturation:           return "Saturation";
      case RenderingIntentPerceptual:           return "Perceptual";
    }

    RUNTIME_EXCEPTION("Invalid RenderingIntent value");
    return "";
  }

  std::string toString(BlendMode blendMode){
    switch (blendMode){
      case BlendModeNormal:     return "Normal";
      case BlendModeMultiply:   return "Multiply";
      case BlendModeScreen:     return "Screen";
      case BlendModeOverlay:    return "Overlay";
      case BlendModeDarken:     return "Darken";
      case BlendModeLighten:    return "Lighten";
      case BlendModeColorDodge: return "ColorDodge";
      case BlendModeColorBurn:  return "ColorBurn";
      case BlendModeHardLight:  return "HardLight";
      case BlendModeSoftLight:  return "SoftLight";
      case BlendModeDifference: return "Difference";
      case BlendModeExclusion:  return "Exclusion";
      case BlendModeHue:        return "Hue";
      case BlendModeSaturation: return "Saturation";
      case BlendModeColor:      return "Color";
      case BlendModeLuminosity: return "Luminosity";
    }

    RUNTIME_EXCEPTION("Invalid BlendMode value");
    return "";
  }

  std::string toString(kernel::encryption::CryptFilterCfm cfm){
    switch (cfm){
      case kernel::encryption::CryptFilterCfmNone:  return "None";
      case kernel::encryption::CryptFilterCfmV2:    return "V2";
      case kernel::encryption::CryptFilterCfmAESV2: return "AESV2";
      case kernel::encryption::CryptFilterCfmAESV3: return "AESV3";
    }

    RUNTIME_EXCEPTION("Invalid CryptFilterCfm value");
    return "";
  }

  std::string toString(kernel::encryption::CryptFilterAuthEvent authEvent){
    switch (authEvent){
      case kernel::encryption::CryptFilterAuthEventDocOpen: return "DocOpen";
      case kernel::encryption::CryptFilterAuthEventEFOpen:  return "EFOpen";
    }

    RUNTIME_EXCEPTION("Invalid CryptFilterAuthEvent value");
    return "";
  }

  std::string toString(ColorSpaceFamily colorSpace){
    switch (colorSpace){
      case ColorSpaceFamilyDeviceGray: return "DeviceGray";
      case ColorSpaceFamilyDeviceRGB:  return "DeviceRGB";
      case ColorSpaceFamilyDeviceCMYK: return "DeviceCMYK";
      case ColorSpaceFamilyCalGray:    return "CalGray";
      case ColorSpaceFamilyCalRGB:     return "CalRGB";
      case ColorSpaceFamilyLab:        return "Lab";
      case ColorSpaceFamilyICCBased:   return "ICCBased";
      case ColorSpaceFamilyPattern:    return "Pattern";
      case ColorSpaceFamilyIndexed:    return "Indexed";
      case ColorSpaceFamilySeparation: return "Separation";
      case ColorSpaceFamilyDeviceN:    return "DeviceN";
    }

    RUNTIME_EXCEPTION("Invalid ColorSpaceFamily value");
    return "";
  }

  std::string toString(FilterName filterName){
    switch (filterName){
      case FilterNameASCIIHex:  return "ASCIIHexDecode";
      case FilterNameASCII85:   return "ASCII85Decode";
      case FilterNameLZW:       return "LZWDecode";
      case FilterNameFlate:     return "FlateDecode";
      case FilterNameRunLength: return "RunLengthDecode";
      case FilterNameCCITTFax:  return "CCITTFaxDecode";
      case FilterNameJBIG2:     return "JBIG2Decode";
      case FilterNameDCT:       return "DCTDecode";
      case FilterNameJPX:       return "JPXDecode";
    }

    RUNTIME_EXCEPTION("Invalid FilterName value");
    return "";
  }

  std::string toString(SpotFunction spotFunction){
    switch (spotFunction){
      case SpotFunctionSimpleDot: return "SimpleDot";
      case SpotFunctionInvertedSimpleDot: return "InvertedSimpleDot";
      case SpotFunctionDoubleDot: return "DoubleDot";
      case SpotFunctionInvertedDoubleDot: return "InvertedDoubleDot";
      case SpotFunctionCosineDot: return "CosineDot";
      case SpotFunctionDouble: return "Double";
      case SpotFunctionInvertedDouble: return "InvertedDouble";
      case SpotFunctionLine: return "Line";
      case SpotFunctionLineX: return "LineX";
      case SpotFunctionLineY: return "LineY";
      case SpotFunctionRound: return "Round";
      case SpotFunctionEllipse: return "Ellipse";
      case SpotFunctionEllipseA: return "EllipseA";
      case SpotFunctionInvertedEllipseA: return "InvertedEllipseA";
      case SpotFunctionEllipseB: return "EllipseB";
      case SpotFunctionEllipseC: return "EllipseC";
      case SpotFunctionInvertedEllipseC: return "InvertedEllipseC";
      case SpotFunctionSquare: return "Square";
      case SpotFunctionCross: return "Cross";
      case SpotFunctionRhomboid: return "Rhomboid";
      case SpotFunctionDiamond: return "Diamond";
    }

    RUNTIME_EXCEPTION("Invalid SpotFunction value");
    return "";
  }

  std::string toString(pdfout::DocumentVersion version){
    switch (version){
      case DocumentVersion10: return "%PDF-1.0\r\n";
      case DocumentVersion11: return "%PDF-1.1\r\n";
      case DocumentVersion12: return "%PDF-1.2\r\n";
      case DocumentVersion13: return "%PDF-1.3\r\n";
      case DocumentVersion14: return "%PDF-1.4\r\n";
      case DocumentVersion15: return "%PDF-1.5\r\n";
      case DocumentVersion16: return "%PDF-1.6\r\n";
      case DocumentVersion17: return "%PDF-1.7\r\n";
    }

    RUNTIME_EXCEPTION("Invalid DocumentVersion value");
    return "";
  }

}
