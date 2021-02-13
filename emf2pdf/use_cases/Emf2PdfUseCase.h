#pragma once

#ifndef EMF2PDF_API_EXPORTS

#include <gtest/gtest.h>
#include <memory>

#include <Document.h>
#include <Page.h>

#include "Emf2PdfEngine.h"

using namespace emf2pdf;

class Emf2PdfUseCase : public ::testing::Test{
public:
  static std::wstring emfRootDir;
  static std::string emfRootDirA;

public:
  static void SetUpTestCase(void);
  static void TearDownTestCase(void);
  void SetUp(void);
  void TearDown(void);

  void playEmfFile(std::wstring const &emfFile, std::wstring const &pdfFile) const;
  uint32_t getGdiObjectsCount(void) const;

public:
  static pdfout::Document mDocument;
  static emf2pdf::FontStorage mFontStorage;
  pdfout::ColorSpaceFamily mColorSpaceFamily;
  uint8_t mJpegQuality;
  //static Emf2PdfEngine *mEngine;
  //pdfout::Page *mPage;
  //pdfout::ContentStream *mContentStream;
};

#endif
