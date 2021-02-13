#pragma once

#include <Windows.h>

namespace emf2pdf{ namespace impl{

  typedef struct{
    EMR emr;
    POINTL ptlReference;
    DWORD nChars;
    DWORD fOptions;
    DWORD iGraphicsMode;
    FLOAT exScale;
    FLOAT eyScale;
    RECTL rcClip;
    char Text[1];
  } EMRSMALLTEXTOUTCLIPA;

  typedef struct{
    EMR emr;
    POINTL ptlReference;
    DWORD nChars;
    DWORD fOptions;
    DWORD iGraphicsMode;
    FLOAT exScale;
    FLOAT eyScale;
    RECTL rcClip;
    WCHAR Text[1];
  } EMRSMALLTEXTOUTCLIPW;

  typedef struct{
    EMR emr;
    POINTL ptlReference;
    DWORD nChars;
    DWORD fOptions;
    DWORD iGraphicsMode;
    FLOAT exScale;
    FLOAT eyScale;
    char Text[1];
  } EMRSMALLTEXTOUTA, EMRSMALLTEXTOUT;

  typedef struct{
    EMR emr;
    POINTL ptlReference;
    DWORD nChars;
    DWORD fOptions;
    DWORD iGraphicsMode;
    FLOAT exScale;
    FLOAT eyScale;
    WCHAR Text[1];
  } EMRSMALLTEXTOUTW;

  typedef struct{
    EMR emr;
    uint32_t checksum;
    uint32_t index;
  } EMR_Reserved_109;

  typedef struct{
    EMR emf;
    DWORD cbData;
    DWORD cbCommentId;
    DWORD cbEmfSpoolRecordId;
    BYTE Data[1];
  } EMR_CommentEmfSpool;

  enum EmrGdiCommentType{
    EmrGdiCommentTypeTextURL = 1
  };

  typedef struct{
    EmrGdiCommentType mType;
    DWORD mSize;
  } EMRCOMMENT;

  typedef struct{
    EMRCOMMENT mEmrComment;
    EMREXTTEXTOUTW mText;
    DWORD mUrlLenght;
    WCHAR mUrlText[1];
  } EMREXTTEXTOUTURLW;

  typedef struct{ 
    DWORD ulID;   // Type of record
    DWORD cjSize; // Size, in bytes, of the data attached to the record
  } EMRI_GenericRecord;

  typedef struct{
    EMRI_GenericRecord generic;
    BYTE UniversalFontId[8]; // An EMFUniversalFontId object ([MS-EMF] section 2.2.27) that identifies the font
    BYTE data[];             // Data that contains the definitions of glyphs in the font
  } EMRI_DesignVectorRecord, EMRI_SubsetFontRecord, EMRI_DeltaFontRecord;

  typedef struct{
    EMRI_GenericRecord generic;
    DWORD Type1ID;  // The value MUST be 0x00000000, to indicate a TrueType
    DWORD NumFiles; // Number of files attached to this record
    BYTE data[];
  } EMRI_EngineFontRecord, EMRI_Type1FontRecord;

  DWORD const ETO_NO_RECT = 0x100;
  DWORD const ETO_SMALL_CHARS = 0x200;

}}
