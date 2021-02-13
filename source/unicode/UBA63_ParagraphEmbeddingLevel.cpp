#include "UBA63.h"

namespace pdfout{ namespace unicode{

  /*
   * br_UBA63_ParagraphEmbeddingLevel
   *
   * This function runs Rules P2 and P3.
   *
   * The paragraph embedding level defaults to zero.
   *
   * If the directionality is Dir_RTL, that signals an
   * override of rules P2 and P3. Set the paragraph
   * embedding level to 1.
   *
   * If the directionality is Dir_Auto (default), scan
   * the text, and if the *first* character with strong
   * directionality is bc=R or bc=AL, set the paragraph
   * embedding level to 1.
   *
   * The difference between this rule for UBA62 and UBA63 is
   * that the UBA63 version needs to ignore any characters
   * between an isolate initiator and a matching PDI.
   */
  void UBA63::br_UBA63_ParagraphEmbeddingLevel(UBACONTEXT *ctxt) const {
    if (ctxt->paragraphDirection == ParagraphDirection::RTL) {
      ctxt->paragraphEmbeddingLevel = 1;
    }
    else if (ctxt->paragraphDirection == ParagraphDirection::Auto) {
      BIDIUNIT *bdu = ctxt->theText.data();
      BIDIUNIT *endOfText = ctxt->theText.data() + ctxt->textLen;

      while (bdu < endOfText) {
        BidiCategory const bc = bdu->bc;
        if (bc == BidiCategory::L) {
          break;
        }
        else if ((bc == BidiCategory::R) || (bc == BidiCategory::AL)) {
          ctxt->paragraphEmbeddingLevel = 1;
          break;
        }
        else if (bdu->bc_isoinit) {
          BIDIUNIT *pdiPtr = nullptr;
          bool const hasPDIMatch = br_HasMatchingPDI(bdu, endOfText, &pdiPtr);

          if (hasPDIMatch) {
            /*
             * Set bdu past the PDI which marks the end of
             * the isolated sequence.
             */
            bdu = pdiPtr + 1;
          }
          else {
            /*
             * If there is no matching PDI, leave
             * the embedding level at 0 and return.
             */
            break;
          }
        }
        /* Dropped through without finding a strong type yet. */
        else {
          bdu++;
        }
      }
    }

    ctxt->dirtyBit = 1;
    ctxt->state = AlgorithmState::P3Done;
  }
}}
