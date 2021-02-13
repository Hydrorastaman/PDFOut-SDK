#include "UBA63.h"
#include "UBAStack.h"

namespace {
  enum class OverrideStatus {
    Neutral,
    LTR,
    RTL
  };

  /*
   * A single status stack definition is shared by both the
   * UBA62 and UBA63 reference implementations. The
   * UBA62 implementation makes no use of the isolate_status
   * field.
   *
   * The status stack is used by rules X1-X8.
   */
  struct STATUSSTACKELEMENT {
    int embedding_level;
    OverrideStatus override_status; /* direction */
    bool isolate_status; /* boolean */
  };

  /*
   * br_AssembleStackElement_63
   *
   * Assign values to a stack element per UBA 6.3.
   */
  void br_AssembleStackElement_63(STATUSSTACKELEMENT *sptr, int level, OverrideStatus ors, int isos) {
    sptr->embedding_level = level;
    sptr->override_status = ors;
    sptr->isolate_status = isos;
  }
}

namespace pdfout{ namespace unicode{

  /*
   * br_UBA63_ExplicitEmbeddingLevels
   *
   * This function runs Rules X1 through X8.
   *
   * The paragraph embedding level defaults to zero.
   *
   * The UBA63 version of these rules is considerably
   * more complex than for UBA62. It takes into account
   * explicit embedding and override levels and *also*
   * handles explicit isolate sequences.
   *
   * Some state information is stored on the top element
   * of the stack, so a PeekStack function which examines
   * those values without popping the stack is required.
   */
  void UBA63::br_UBA63_ExplicitEmbeddingLevels(UBACONTEXT *ctxt) const {
    std::size_t const maximum_depth = 125;
    UBAStack<STATUSSTACKELEMENT> statusStack(maximum_depth + 2);
    UBAVersion const version = GetUBAVersion();

    ctxt->dirtyBit = 0;
    /*
     * X1:
     * Initialize the stack and the other variables.
     *
     * Because rules X2 through X8, which use the stack,
     * are handled here in a single pass, the required values can be
     * stored in local variables, and do not need to be saved
     * in the context.
     */

    /*
     * Initialize the stack each time into this function, before
     * running the X2..X8 rules.
     */
    STATUSSTACKELEMENT stack_element;
    br_AssembleStackElement_63(&stack_element, ctxt->paragraphEmbeddingLevel, OverrideStatus::Neutral, 0);
    statusStack.push(stack_element);

    int overflowIsolateCount = 0;
    int overflowEmbeddingCount = 0;
    int validIsolateCount = 0;

    /*
     * X2..X8:
     *
     * Process each character in the input, setting embedding levels
     * and override status.
     */
    BIDIUNIT *bdu = ctxt->theText.data();
    BIDIUNIT *endOfText = ctxt->theText.data() + ctxt->textLen;
    while (bdu < endOfText) {
      BidiCategory const bc = bdu->bc;

      switch (bc) {
        case BidiCategory::RLE: { /* X2 */
          statusStack.peek(stack_element);
          int const templevel = br_leastGreaterOddLevel(stack_element.embedding_level, maximum_depth);
          if ((templevel != -1) && (overflowIsolateCount == 0) && (overflowEmbeddingCount == 0)) {
            br_AssembleStackElement_63(&stack_element, templevel, OverrideStatus::Neutral, 0);
            statusStack.push(stack_element);
          }
          else {
            if (overflowIsolateCount == 0) {
              overflowEmbeddingCount++;
            }
          }

          break;
        }

        case BidiCategory::LRE: { /* X3 */
          statusStack.peek(stack_element);
          int const templevel = br_leastGreaterEvenLevel(stack_element.embedding_level, maximum_depth);
          if ((templevel != -1) && (overflowIsolateCount == 0) && (overflowEmbeddingCount == 0)) {
            br_AssembleStackElement_63(&stack_element, templevel, OverrideStatus::Neutral, 0);
            statusStack.push(stack_element);
          }
          else {
            if (overflowIsolateCount == 0) {
              overflowEmbeddingCount++;
            }
          }

          break;
        }

        case BidiCategory::RLO: { /* X4 */
          statusStack.peek(stack_element);
          int const templevel = br_leastGreaterOddLevel(stack_element.embedding_level, maximum_depth);
          if ((templevel != -1) && (overflowIsolateCount == 0) && (overflowEmbeddingCount == 0)) {
            br_AssembleStackElement_63(&stack_element, templevel, OverrideStatus::RTL, 0);
            statusStack.push(stack_element);
          }
          else {
            if (overflowIsolateCount == 0) {
              overflowEmbeddingCount++;
            }
          }

          break;
        }

        case BidiCategory::LRO: { /* X5 */
          statusStack.peek(stack_element);
          int const templevel = br_leastGreaterEvenLevel(stack_element.embedding_level, maximum_depth);
          if ((templevel != -1) && (overflowIsolateCount == 0) && (overflowEmbeddingCount == 0)) {
            br_AssembleStackElement_63(&stack_element, templevel, OverrideStatus::LTR, 0);
            statusStack.push(stack_element);
          }
          else {
            if (overflowIsolateCount == 0) {
              overflowEmbeddingCount++;
            }
          }

          break;
        }

        case BidiCategory::RLI: { /* X5a */
          statusStack.peek(stack_element);
          bdu->level = stack_element.embedding_level;

          /*
           * For UBA80 and later, if the directional override status of the last
           * entry is not neutral, change the current bc of the RLI explicitly to
           * either L or R, accordingly.
           */
          if (version >= UBAVersion::UBA80) {
            if (stack_element.override_status != OverrideStatus::Neutral) {
              bdu->bc = (stack_element.override_status == OverrideStatus::LTR) ? BidiCategory::L : BidiCategory::R;
            }
          }

          ctxt->dirtyBit = 1;

          int const templevel = br_leastGreaterOddLevel(stack_element.embedding_level, maximum_depth);
          if ((templevel != -1) && (overflowIsolateCount == 0) && (overflowEmbeddingCount == 0)) {
            validIsolateCount++;
            br_AssembleStackElement_63(&stack_element, templevel, OverrideStatus::Neutral, 1);
            statusStack.push(stack_element);
          }
          else {
            overflowIsolateCount++;
          }

          break;
        }

        case BidiCategory::LRI: { /* X5b */
          statusStack.peek(stack_element);
          bdu->level = stack_element.embedding_level;

          /*
           * For UBA80 and later, if the directional override status of the last
           * entry is not neutral, change the current bc of the LRI explicitly to
           * either L or R, accordingly.
           */
          if (version >= UBAVersion::UBA80) {
            if (stack_element.override_status != OverrideStatus::Neutral) {
              bdu->bc = (stack_element.override_status == OverrideStatus::LTR) ? BidiCategory::L : BidiCategory::R;
            }
          }

          ctxt->dirtyBit = 1;
          int const templevel = br_leastGreaterEvenLevel(stack_element.embedding_level, maximum_depth);
          if ((templevel != -1) && (overflowIsolateCount == 0) && (overflowEmbeddingCount == 0)) {
            validIsolateCount++;
            br_AssembleStackElement_63(&stack_element, templevel, OverrideStatus::Neutral, 1);
            statusStack.push(stack_element);
          }
          else {
            overflowIsolateCount++;
          }

          break;
        }

        case BidiCategory::FSI: { /* X5c This is a complicated mix of X5a/X5b */
          BIDIUNIT* pdiPtr = nullptr;
          int tmpParaEmbedLevel = 0;

          /* Check if there is a matching PDI */
          bool const hasPDIMatch = br_HasMatchingPDI(bdu, endOfText, &pdiPtr);
          if (hasPDIMatch) {
            tmpParaEmbedLevel = br_DecideParaLevel(bdu + 1, pdiPtr);
          }
          else {
            tmpParaEmbedLevel = br_DecideParaLevel(bdu + 1, endOfText);
          }

          statusStack.peek(stack_element);
          bdu->level = stack_element.embedding_level;

          /*
           * For UBA80 and later, if the directional override status of the last
           * entry is not neutral, change the current bc of the FSI explicitly to
           * either L or R, accordingly.
           */
          if (version >= UBAVersion::UBA80) {
            if (stack_element.override_status != OverrideStatus::Neutral)
              bdu->bc = (stack_element.override_status == OverrideStatus::LTR) ? BidiCategory::L : BidiCategory::R;
          }

          ctxt->dirtyBit = 1;

          /*
           * If the calculated paragraph embedding level is 1, treat
           * this FSI as an RLI. Otherwise, treat it as an LRI.
           */
          int templevel = 0;
          if (tmpParaEmbedLevel == 1) {
            templevel = br_leastGreaterOddLevel(stack_element.embedding_level, maximum_depth);
          }
          else {
            templevel = br_leastGreaterEvenLevel(stack_element.embedding_level, maximum_depth);
          }

          if ((templevel != -1) && (overflowIsolateCount == 0) && (overflowEmbeddingCount == 0)) {
            validIsolateCount++;
            br_AssembleStackElement_63(&stack_element, templevel, OverrideStatus::Neutral, 1);
            statusStack.push(stack_element);
          }
          else {
            overflowIsolateCount++;
          }

          break;
        }

        case BidiCategory::PDI: { /* X6a */
          if (overflowIsolateCount > 0) {
            overflowIsolateCount--;
          }
          else if (validIsolateCount == 0) {
            /* do nothing */
          }
          else {
            overflowEmbeddingCount = 0;
            int continuepopping = 1;

            while (continuepopping) {
              statusStack.peek(stack_element);
              if (stack_element.isolate_status == 0) {
                statusStack.pop(stack_element);
              }
              else {
                continuepopping = 0;
              }
            }

            statusStack.pop(stack_element);
            validIsolateCount--;
          }

          statusStack.peek(stack_element);
          bdu->level = stack_element.embedding_level;

          /*
           * For UBA80 and later, if the directional override status of the last
           * entry is not neutral, change the current bc of the PDI explicitly to
           * either L or R, accordingly.
           */
          if (version >= UBAVersion::UBA80) {
            if (stack_element.override_status != OverrideStatus::Neutral) {
              bdu->bc = (stack_element.override_status == OverrideStatus::LTR) ? BidiCategory::L : BidiCategory::R;
            }
          }

          ctxt->dirtyBit = 1;

          break;
        }

        case BidiCategory::PDF: { /* X7 */
          if (overflowIsolateCount > 0) {
            /* do nothing */
          }
          else if (overflowEmbeddingCount > 0) {
            overflowEmbeddingCount--;
          }
          else {
            statusStack.peek(stack_element);

            if (stack_element.isolate_status == 0) {
              if (statusStack.size() >= 2) {
                statusStack.pop(stack_element);
              }
            }
          }

          break;
        }

        case BidiCategory::BN:
          break;

        case BidiCategory::B: {  /* X8 */
          /*
           * A paragraph break terminates all embedding contexts.
           * Just set the level back to the paragraph embedding level.
           * A BIDI_B should only be encountered as the very last element
           * in a paragraph. If not, the paragraph chunking was not
           * done correctly.
           */
          bdu->level = ctxt->paragraphEmbeddingLevel;
          ctxt->dirtyBit = 1;

          break;
        }

        default: { /* X6 */
          statusStack.peek(stack_element);
          bdu->level = stack_element.embedding_level;

          if (stack_element.override_status == OverrideStatus::RTL) {
            bdu->bc = BidiCategory::R;
            bdu->bc_numeric = 0;
          }
          else if (stack_element.override_status == OverrideStatus::LTR) {
            bdu->bc = BidiCategory::L;
            bdu->bc_numeric = 0;
          }

          ctxt->dirtyBit = 1;

          break;
        }
      }

      /* Advance to the next character to process. */
      bdu++;
    }

    ctxt->state = AlgorithmState::X8Done;
  }
}}
