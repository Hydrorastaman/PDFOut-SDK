#pragma once

#include <stdint.h>
#include <unordered_map>

namespace pdfout{ namespace unicode{

  enum class BidiCategory {
    None,
    Unknown,
    L,    /* 0x00 strong: left-to-right (bc=L) */
    ON,   /* 0x01 neutral: Other Neutral (bc=ON) */
    R,    /* 0x02 strong: right-to-left (bc=R) */
    EN,   /* 0x03 weak: European Number (bc=EN) */
    ES,   /* 0x04 weak: European Number Separator (bc=ES) */
    ET,   /* 0x05 weak: European Number Terminator (bc=ET) */
    AN,   /* 0x06 weak: Arabic Number (bc=AN) */
    CS,   /* 0x07 weak: Common Number Separator (bc=CS) */
    B,    /* 0x08 neutral: Paragraph Separator (bc=B) */
    S,    /* 0x09 neutral: Segment Separator (bc=S) */
    WS,   /* 0x0A neutral: White Space (bc=WS) */
    AL,   /* 0x0B strong: right-to-left Arabic (bc=AL) */
    NSM,  /* 0x0C weak: non-spacing mark (bc=NSM) */
    BN,   /* 0x0D weak: boundary neutral (bc=BN) */
    PDF,  /* 0x0E format: pop directional formatting (bc=PDF) */
    LRE,  /* format: left-to-right embedding */
    LRO,  /* format: left-to-right override */
    RLE,  /* format: right-to-left embedding */
    RLO,  /* format: right-to-left override */
    LRI,  /* format: left-to-right isolate */
    RLI,  /* format: right-to-left isolate */
    FSI,  /* format: first strong isolate */
    PDI   /* format: pop directional isolate */
  };

  BidiCategory getBidiCategory(uint32_t symbol);
}}
