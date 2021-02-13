#pragma once

#include <string>
#include <UString.h>

namespace pdfout{
  enum Encoding;
  enum BlendMode;
  enum FilterName;
  enum SpotFunction;
  enum DocumentVersion;
  enum RenderingIntent;
  enum ColorSpaceFamily;
}

namespace kernel{

  namespace encryption{
    enum CryptFilterCfm;
    enum CryptFilterAuthEvent;
  }

  std::string toString(pdfout::UString const &ustr);
  std::wstring toWString(pdfout::UString const &ustr);

  std::string toString(pdfout::RenderingIntent ri);
  std::string toString(pdfout::BlendMode blendMode);
  std::string toString(kernel::encryption::CryptFilterCfm cfm);
  std::string toString(kernel::encryption::CryptFilterAuthEvent authEvent);
  std::string toString(pdfout::ColorSpaceFamily colorSpace);
  std::string toString(pdfout::FilterName filterName);
  char const *toString(pdfout::Encoding encoding);
  std::string toString(pdfout::SpotFunction spotFunction);
  std::string toString(pdfout::DocumentVersion version);

}
