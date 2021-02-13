#pragma once

#include <memory>

#include "UBA.h"
#include "UBAContext.h"
#include "UnicodeData.h"
#include "BidiBrackets.h"


namespace pdfout{ namespace unicode{

  class UBA63 : public UBA {
  public:
    UBA63()
      : UBA(UBAVersion::UBA63, 125) {
    }

    std::vector<uint32_t> resolve(uint32_t const *source, uint32_t *transpositions, std::size_t length) const {
      std::unique_ptr<UBACONTEXT> ctxtPtr = std::make_unique<UBACONTEXT>();
      UBACONTEXT *ctxt = ctxtPtr.get();

      ctxt->state = AlgorithmState::Initialized;
      ctxt->testId = 0;
      ctxt->dirtyBit = 0;
      ctxt->paragraphDirection = ParagraphDirection::LTR;
      ctxt->paragraphEmbeddingLevel = 0;
      ctxt->textLen = length;
      ctxt->theText = br_ConstructBidiUnitVector(source, length);
      ctxt->theRuns = nullptr;
      ctxt->lastRun = nullptr;
      ctxt->theSequences = nullptr;
      ctxt->lastSequence = nullptr;
      ctxt->expEmbeddingLevel = -1;
      ctxt->expOrder = nullptr;

      br_UBA63_ParagraphEmbeddingLevel(ctxt);
      br_UBA63_ExplicitEmbeddingLevels(ctxt);
      br_UBA_DeleteFormatCharacters(ctxt);

      if (!br_UBA_IdentifyRuns(ctxt))
        return {};

      if (!br_UBA63_IdentifyIsolatingRunSequences(ctxt))
        return {};

      /*
       * Next we start a stretch of rules which apply systematically
       * to the list of isolating run sequences (for UBA63).
       *
       * For compactness and abstraction, these are dispatched
       * through a central rule handler which handles rules of
       * this generic type.
       */

      if (!br_UBA_WRulesDispatch(ctxt))
        return {};

      if (!br_UBA_RuleDispatch(ctxt, BidiRuleType::N0))
        return {};

      if (!br_UBA_RuleDispatch(ctxt, BidiRuleType::N1))
        return {};

      if (!br_UBA_RuleDispatch(ctxt, BidiRuleType::N2))
        return {};

      br_UBA_ResolveImplicitLevels(ctxt);
      br_UBA63_ResetWhitespaceLevels(ctxt);
      br_UBA_ReverseLevels(ctxt);

      std::vector<uint32_t> result(ctxt->theText.size());
      for (std::size_t i = 0, size = ctxt->theText.size(); i < size; ++i) {
        auto const &unit = ctxt->theText[i];
        result[i] = unit.c;
        transpositions[unit.order] = i;
      }

      return result;
    }

    std::vector<uint32_t> resolve(std::vector<uint32_t> const &source, std::vector<uint32_t> &transpositions) const {
      if (source.size() != transpositions.size())
        return {};

      return resolve(source.data(), transpositions.data(), source.size());
    }

  private:
    void br_UBA63_ParagraphEmbeddingLevel(UBACONTEXT *ctxt) const;
    void br_UBA63_ExplicitEmbeddingLevels(UBACONTEXT *ctxt) const;
    bool br_UBA63_IdentifyIsolatingRunSequences(UBACONTEXT *ctxt) const;

    /*
     * br_UBA63_ResetWhitespaceLevels
     *
     * This function runs Rule L1.
     *
     * As for br_UBA_ResetWhitespaceLevels, except that for
     * UBA63, the scanback context which is reset to the
     * paragraph embedding level includes all contiguous
     * sequences of whitespace *and* any isolate format
     * controls. The test for the Bidi_Class of the isolate
     * format controls could be done on either the current
     * or original Bidi_Class, as the preceding rules do not
     * change the Bidi_Class of isolate format controls, but
     * all tests are done here on the original Bidi_Class
     * values in the BIDIUNIT, for consistency.
     */
    void br_UBA63_ResetWhitespaceLevels(UBACONTEXT *ctxt) const {
      ctxt->dirtyBit = 0;
      BIDIUNIT *bdu = ctxt->theText.data();
      BIDIUNIT *endOfText = ctxt->theText.data() + ctxt->textLen;
      while (bdu < endOfText) {
        if ((bdu->orig_bc == BidiCategory::S) || (bdu->orig_bc == BidiCategory::B)) {
          if (bdu->level != ctxt->paragraphEmbeddingLevel) {
            /* Only set the dirtyBit if the level has actually changed. */
            bdu->level = ctxt->paragraphEmbeddingLevel;
            ctxt->dirtyBit = 1;
          }

          /* scan back looking for contiguous whitespace */
          if (bdu > ctxt->theText.data()) {
            BIDIUNIT *bdu2 = bdu - 1;
            while (bdu2 >= ctxt->theText.data()) {
              if (bdu2->orig_bc == BidiCategory::WS) {
                bdu2->level = ctxt->paragraphEmbeddingLevel;
              }
              else if (br_IsIsolateControl(bdu2->orig_bc)) {
                bdu2->level = ctxt->paragraphEmbeddingLevel;
              }
              else if (bdu2->level == BIDIUNIT::NOLEVEL) {
                /* skip over *deleted* controls */
              }
              else {
                /* break out of loop for any other character */
                break;
              }

              bdu2--;
            }
          }
        }

        /* If at end of string, scan back checking for terminal whitespace */
        if (bdu == endOfText - 1) {
          BIDIUNIT *bdu2 = bdu;
          while (bdu2 >= ctxt->theText.data()) {
            if (bdu2->orig_bc == BidiCategory::WS) {
              bdu2->level = ctxt->paragraphEmbeddingLevel;
              ctxt->dirtyBit = 1;
            }
            else if (br_IsIsolateControl(bdu2->orig_bc)) {
              bdu2->level = ctxt->paragraphEmbeddingLevel;
              ctxt->dirtyBit = 1;
            }
            else if (bdu2->level == BIDIUNIT::NOLEVEL) {
              /* skip over *deleted* controls */
            }
            else {
              /* break out of loop for any other character */
              break;
            }

            bdu2--;
          }
        }

        /* Advance to the next character to process. */

        bdu++;
      }

      ctxt->state = AlgorithmState::L1Done;
    }

    /*
     * br_ConstructBidiUnitVector()
     *
     * Initialize the text values from some source.
     * Look up GC and BC values for each character,
     * and initialize all the levels to zero.
     */
    std::vector<BIDIUNIT> br_ConstructBidiUnitVector(uint32_t const *text, std::size_t length) const {
      std::vector<BIDIUNIT> result(length);

      for (std::size_t i = 0; i < length; ++i) {
        uint32_t const tc = text[i];
        BIDIUNIT &tbp = result[i];

        tbp.c = tc;
        tbp.bc = getBidiCategory(tc);

        auto const parenthesisInfo = getParethesisInfo(tc);
        tbp.bpb = parenthesisInfo.mInPair;// br_GetBPB(tc);
        tbp.bpt = parenthesisInfo.mDirection;// br_GetBPT(tc);

        /*
         * Save the original Bidi_Class value.
         */
        tbp.orig_bc = tbp.bc;

        /*
         * Set all accelerator flags.
         */
        tbp.bc_numeric = 0;
        tbp.bc_isoinit = 0;
        if ((tbp.bc == BidiCategory::AN) || (tbp.bc == BidiCategory::EN))
          tbp.bc_numeric = 1;
        else if ((tbp.bc == BidiCategory::LRI) || (tbp.bc == BidiCategory::RLI) || (tbp.bc == BidiCategory::FSI))
          tbp.bc_isoinit = 1;

        /*
         * Initialize all the levels to zero.
         */
        tbp.level = 0;
        tbp.expLevel = 0;

        /*
         * Initialize the order fields based on index.
         */
        tbp.order = i;
      }

      return result;
    }

  };
}}
