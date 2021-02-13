#pragma once

#include <Windows.h>

#include <Color.h>
#include <Types.h>
#include <FontStorage.h>
#include <TransformationMatrix.h>

namespace emf2pdf{ namespace impl{

  struct DeviceState{
    DeviceState(void)
      : mTextColor(RGB(0, 0, 0)),
      mBkColor(RGB(255, 255, 255)),
      mBkMode(OPAQUE),
      mTextAlignment(pdfout::AlignmentLeft | pdfout::AlignmentTop),
      mUpdateCP(false),
      mUfi(FontStorage::unknownUfi),
      mCurrentPos({0, 0}),
      mMapMode(MM_TEXT),
      mPolyfillMode(ALTERNATE),
      mWorldTransform(),
      mIsClipped(false),
      mIsNullBrush(false),
      mIsNullPen(false),
      mClippingPathPoints(),
      mClippingPathTypes(){
    }

    DeviceState *clone(void){
      return new DeviceState(*this);
    }

    COLORREF mTextColor;
    COLORREF mBkColor;
    int mBkMode;
    uint16_t mTextAlignment;
    bool mUpdateCP;
    uint32_t mUfi;
    pdfout::Point mCurrentPos; /**< Current position in logical coordinates, use LPtoDP before */
    int mMapMode;
    int mPolyfillMode;
    pdfout::TransformationMatrix mWorldTransform;
    bool mIsClipped;
    bool mIsNullBrush;
    bool mIsNullPen;
    std::vector<POINT> mClippingPathPoints;
    std::vector<uint8_t> mClippingPathTypes;
  };
}}
