#pragma once

#include <unordered_map>
#include "UBAStack.h"
#include "UBAContext.h"

namespace pdfout{ namespace unicode{

  /*
   * Version enumeration for support of distinct versions
   * of UBA.
   *
   * This enumeration may be extended, if further distinctions
   * make sense. UBA62 is considered the base version.
   * UBA63 is a distinct version for
   * the purposes of separating the details of the
   * algorithm which have changed for UBA 6.3.0.
   * UBA70, UBA80, etc. extend for future version support.
   */
  enum class UBAVersion {
    UBAXX,  /* Undefined rule set */
    UBA62,  /* Base version. UBA 6.2.0 rule set */
    UBA63,  /* UBA 6.3.0 rule set */
    UBA70,  /* UBA 6.3.0 rule set; Unicode 7.0.0 repertoire */
    UBA80,  /* UBA 8.0.0 rule set */
    UBA90,  /* UBA 8.0.0 rule set; Unicode 9.0.0 repertoire */
    UBA100, /* UBA 8.0.0 rule set; Unicode 10.0.0 repertoire */
    UBA110, /* UBA 8.0.0 rule set; Unicode 11.0.0 repertoire */
    UBA120, /* UBA 8.0.0 rule set; Unicode 12.0.0 repertoire */
    UBACUR  /* Unspecified version: default to current UBA rules and
                use unversioned file names for data. */ 
  };

  class UBA {
  private:
    /*
     * Typedefs used in the bracket pairing algorithm in rule N0.
     */
    struct BRACKETSTACKELEMENT {
      uint32_t bracket;
      int pos;
    };

    struct BRACKETLISTELEMENT {
      int openingpos;
      int closingpos;
    };

  protected:
    UBA(UBAVersion version, std::size_t maximumDepth)
      : mVersion(version),
      mMaximumDepth(maximumDepth) {
    }

    UBAVersion GetUBAVersion() const { return mVersion;}
    std::size_t GetMaximumDepth() const { return mMaximumDepth;}

  public:
    virtual ~UBA() {}
    virtual std::vector<uint32_t> resolve(uint32_t const *source, uint32_t *transpositions, std::size_t length) const = 0;
    virtual std::vector<uint32_t> resolve(std::vector<uint32_t> const &source, std::vector<uint32_t> &transpositions) const = 0;

  protected:
    /*
     * br_UBA_DeleteFormatCharacters
     *
     * This function runs Rule X9.
     *
     * Characters are not actually deleted. Instead, their level
     * is set to NOLEVEL, which allows ignoring them in later steps.
     *
     * This setting could, of course, be done more efficiently by
     * also setting these levels as part of the processing of X2..X8,
     * but is pulled out separately here to make the impact of
     * X9 clearer for the didactic implementation.
     */
    void br_UBA_DeleteFormatCharacters(UBACONTEXT *ctxt) const {
      ctxt->dirtyBit = 0;

      BIDIUNIT *bdu = ctxt->theText.data();
      BIDIUNIT *endOfText = ctxt->theText.data() + ctxt->textLen;

      while (bdu < endOfText) {
        BidiCategory const bc = bdu->bc;

        switch (bc) {
          case BidiCategory::RLE:
          case BidiCategory::LRE:
          case BidiCategory::RLO:
          case BidiCategory::LRO:
          case BidiCategory::PDF:
          case BidiCategory::BN:
            bdu->level = BIDIUNIT::NOLEVEL;
            ctxt->dirtyBit = 1;
            break;
        }

        /*
         * Now reset the order positions.
         * Use -1 as the position for all "deleted" characters.
         * This is later checked when actually reordering levels.
         */
        if (bdu->level == BIDIUNIT::NOLEVEL) {
          bdu->order = -1;
        }

        /* Advance to the next character to process. */
        bdu++;
      }

      ctxt->state = AlgorithmState::X9Done;
    }

    /*
     * br_UBA_IdentifyRuns
     *
     * This function runs Rule X10.
     *
     * X10 is treated with a separate function.
     * It logically occurs after the completion of the application
     * of rules X2-X8 sequentionally through the input string
     * and after X9 has identified and tagged any NOLEVEL values.
     * All levels have to be set before the level runs can be
     * accurately identified.
     *
     * Instead of marking up the BIDIUNIT vector with more
     * character-by-character information, the runs are best
     * handled by a separate structure. A linked list of runs is hung
     * off the UBACONTEXT struct, with each run containing pointers
     * to its start and end BIDIUNITs. This list can then be
     * traversed by the subsequent rules which operate on a
     * run-by-run basis.
     *
     * Note that this code presupposes that the input vector is
     * at least one unit long. Tests for a zero-length vector
     * should occur before hitting this function.
     *
     * It is possible that an input vector may consist *entirely*
     * of units set to NOLEVEL. This would happen if all the
     * units were bc=BN or bidi embedding/override controls,
     * which would have been "deleted" by this point from the
     * input by setting their levels to NOLEVEL. In that case
     * the input vector will have no level spans at all.
     */
    bool br_UBA_IdentifyRuns(UBACONTEXT *ctxt) const {
      ctxt->dirtyBit = 0;

      /* Initialize for looping to extract spans. */
      int spanid = 1;
      int spanningDone = 0;
      int spanlevel;
      int spanlen;

      BIDIUNIT *bdu = ctxt->theText.data();                /* point to start of span */
      BIDIUNIT *bduend = bdu + (ctxt->textLen - 1); /* point to last BIDIUNIT in vector */
      BIDIUNIT *bdunext;
      BIDIRUN *brp;

      while (!spanningDone) {
        spanningDone = br_SpanOneRun(bdu, bduend, &bdunext, &spanlevel, &spanlen);

        /*
         * Process the extracted span. If the spanlevel was
         * set to NOLEVEL, skip this step, as there is no
         * span to process.
         *
         * The minimum length
         * of a span should be 1, in which case the start of
         * the span and the end of the span point at the same
         * BIDIUNIT.
         */
        if (spanlevel != BIDIUNIT::NOLEVEL) {
          if (spanningDone) {
            brp = br_ConstructBidiRun(spanid, spanlevel, bdu, bduend, spanlen);
          }
          else {
            brp = br_ConstructBidiRun(spanid, spanlevel, bdu, bdunext - 1, spanlen);
          }

          if (brp == nullptr) {
            return false;
          }

          /*
           * If we have a valid run, append it to the run list
           * in the context.
           */
          br_AppendBidiRun(ctxt, brp);
        }

        /* If that was the last span, exit the while loop. */
        if (!spanningDone) {
          /* Set bdu to the next span start, increment the span id and come around. */
          bdu = bdunext;
          spanid++;
        }
      }

      /*
       * Add text chains for each run, for uniform rule application.
       */
      if (!br_UBA_AddTextChainsForRuns(ctxt->theRuns))
        return false;

      /*
       * Now that we have identified the runs, calculate the sor and eor
       * values for each run.
       */
      br_UBA_CalculateSorEor(ctxt);

      /*
       * This rule always has an impact, so just always set the dirtyBit
       * on exit.
       */
      ctxt->dirtyBit = 1;

      if (GetUBAVersion() > UBAVersion::UBA62) {
        ctxt->state = AlgorithmState::RunsDone;
      }
      else {
        ctxt->state = AlgorithmState::X10Done;
      }

      return true;
    }

    /*
     * br_UBA_CalculateSorEor
     *
     * Process the run list, calculating sor and eor values for
     * each run. Those values default to BIDI_Unknown when the
     * runs are first identified. But each needs to be set to
     * either L or R.
     */
    void br_UBA_CalculateSorEor(UBACONTEXT *ctxt) const {
      BIDIRUN *brp = ctxt->theRuns;

      if (brp == nullptr) {
        /* No runs to process */
        return;
      }

      /*
       * Default the priorRunLevel for the first run to
       * the paragraph embedding level.
       */
      int priorRunLevel = ctxt->paragraphEmbeddingLevel;
      int nextRunLevel;
      int higherLevel;

      while (brp != nullptr) {
        /*
         * If we have reached the last run, set the nextRunLevel
         * to the paragraphEmbedding Level, otherwise set it
         * to the level of the next run.
         */
        if (brp->next == nullptr) {
          nextRunLevel = ctxt->paragraphEmbeddingLevel;
        }
        else {
          nextRunLevel = brp->next->level;
        }

        /*
         * Set sor based on the higher of the priorRunLevel and
         * the current level.
         */
        higherLevel = (priorRunLevel > brp->level) ? priorRunLevel : brp->level;
        brp->sor = (higherLevel % 2 == 1) ? BidiCategory::R : BidiCategory::L;

        /*
         * Set eor based on the higher of the nextRunLevel and
         * the current level.
         */
        higherLevel = (nextRunLevel > brp->level) ? nextRunLevel : brp->level;
        brp->eor = (higherLevel % 2 == 1) ? BidiCategory::R : BidiCategory::L;

        /*
         * Set priorRunLevel to the current level and
         * move to the next run in the list.
         */
        priorRunLevel = brp->level;
        brp = brp->next;
      }
    }

    /*
     * br_UBA_AddTextChainsForRuns
     *
     * Walk through a run list, allocating, initializing and
     * linking in a text chain for each run.
     */
    bool br_UBA_AddTextChainsForRuns(BIDIRUN *brp) const {
      if (brp == nullptr) {
        /* No runs to process */
        return true;
      }

      BIDIRUN *tbrp = brp;
      while (tbrp != nullptr) {
        /*
         * Each run has a len defined already. Use that value
         * to allocate a text chain.
         */
        int const len = tbrp->len;
        BIDIUNIT **tcp = (BIDIUNIT **) malloc(len * sizeof(BIDIUNIT *));
        if (tcp == nullptr) {
          /*
           * Let br_DropContext do any cleanup
           * needed for already allocated arrays.
           */
          return false;
        }

        /* Copy BIDIUNIT pointers for the level run into the text chain. */
        BIDIUNIT *bup = tbrp->first;
        BIDIUNIT **tcp2 = tcp;
        for (; bup <= tbrp->last; bup++, tcp2++) {
          *tcp2 = bup;
        }

        /* Attach the initialized text chain to the bidi run. */
        tbrp->textChain = tcp;
        tbrp = tbrp->next;
      }

      return true;
    }

    /*
     * br_UBA_AddTextChainsForSequences
     *
     * Walk through a sequence list, allocating, initializing and
     * linking in a text chain for each isolating run sequence.
     */
    bool br_UBA_AddTextChainsForSequences(IRSEQ *irp) const {
      if (irp == nullptr) {
        /* No sequences to process */
        return true;
      }

      IRSEQ *tirp = irp;
      while (tirp != nullptr) {
        /*
         * An isolating run sequence consists of a sequence of
         * runs, which may be discontiguous. To find the length
         * of text chain to allocate, we first need to traverse
         * the run list, accumulating the lengths of the runs.
         */
        int len = 0;
        BIDIRUNLIST *brlp = tirp->theRuns;
        while (brlp != nullptr) {
          len += brlp->run->len;
          brlp = brlp->next;
        }

        /*
         * Write the length value back into the sequence, to store
         * the length of the calculated text chain.
         */
        tirp->len = len;
        if (len == 0) {
          return false;
        }

        BIDIUNIT **tcp = (BIDIUNIT **) malloc(len * sizeof(BIDIUNIT *));
        if (tcp == nullptr) {
          /*
           * Let br_DropContext do any cleanup
           * needed for already allocated arrays.
           */
          return false;
        }

        /*
         * Copy BIDIUNIT pointers for the isolating run sequence into the
         * text chain.
         *
         * This differs from the initialization for the level runs, because
         * we have to read sequentially through each level run and
         * append the BIDIUNIT pointers to the allocated array.
         *
         * First initialize the tcp2 pointer to the allocated text chain.
         */
        BIDIUNIT **tcp2 = tcp;

        /*
         * Process the run list in order.
         */
        brlp = tirp->theRuns;
        while (brlp != nullptr) {
          BIDIRUN *brp = brlp->run;

          /* Append this run to the text chain. */
          for (BIDIUNIT *bup = brp->first; bup <= brp->last; bup++, tcp2++) {
            *tcp2 = bup;
          }

          brlp = brlp->next;
        }

        /* Attach the initialized text chain to the bidi run. */
        tirp->textChain = tcp;
        tirp = tirp->next;
      }

      return true;
    }

    /*
     * br_UBA_CalculateSosEos
     *
     * Process the isolating run sequence list, calculating sos and eos values for
     * each sequence. Those values default to BIDI_Unknown when the
     * sequences are first identified. But each needs to be set to
     * either L or R.
     *
     * Strategy: Instead of recalculating all the sos and eos values from
     * scratch, as specified in X10, we can take a shortcut here, because
     * we already have sor and eor values assigned to all the level runs.
     * For any isolating run sequence, simply assign sos to the value of
     * sor for the *first* run in that sequence, and assign eos to the
     * value of eor for the *last* run in that sequence. This provides
     * equivalent values, and is more straightforward to implement and
     * understand.
     *
     * This strategy has to be modified for defective isolating run sequences,
     * where the sequence ends with an LRI/RLI/FSI.
     * In those cases the eot needs to be calculated based on
     * the paragraph embedding level, rather than from the level run.
     * Note that this only applies when an isolating run sequence
     * terminating in an LRI/RLI/FSI but with no matching PDI.
     * An example would be:
     *
     *    R  RLI    R
     * <L-----R> <RR>
     * <L------[          <== eot would be L, not R
     *           <RR>
     *
     */
    void br_UBA_CalculateSosEos(UBACONTEXT *ctxt) const {
      IRSEQ *tirp = ctxt->theSequences;
      while (tirp != nullptr) {
        /*
         * First inherit the sos and eos values from the
         * first and last runs in the sequence.
         */
        tirp->sos = tirp->theRuns->run->sor;
        tirp->eos = tirp->lastRun->run->eor;

        /*
         * Next adjust for the special case when an isolating
         * run sequence terminates in an unmatched isolate
         * initiator.
         */
        if (tirp->lastRun->run->last->bc_isoinit) {
          int const nextRunLevel = ctxt->paragraphEmbeddingLevel;
          int const higherLevel = (nextRunLevel > tirp->level) ? nextRunLevel : tirp->level;
          tirp->eos = (higherLevel % 2 == 1) ? BidiCategory::R : BidiCategory::L;
        }

        tirp = tirp->next;
      }
    }

    /*
     * br_UBA_ResolveCombiningMarks
     *
     * This is the method for Rule W1.
     *
     * Resolve combining marks for a single text chain.
     *
     * For each character in the text chain, examine its
     * Bidi_Class. For characters of bc=NSM, change the Bidi_Class
     * value to that of the preceding character. Formatting characters
     * (Bidi_Class RLE, LRE, RLO, LRO, PDF) and boundary neutral (Bidi_Class BN)
     * are skipped over in this calculation, because they have been
     * "deleted" by Rule X9.
     *
     * If a bc=NSM character occurs at the start of a text chain, it is given
     * the Bidi_Class of sot (either R or L).
     */
    int br_UBA_ResolveCombiningMarks(BIDIRULECONTEXT *brcp) const {
      int dirtyBit = 0;

      /*
       * Default the priorbc to the sot value passed in.
       */
      BidiCategory priorbc = brcp->sot;

      /*
       * Process the text chain from first to last BIDIUNIT,
       * checking the Bidi_Class of each character.
       */
      BIDIUNIT **bupp = brcp->textChain;
      BIDIUNIT **buppend = bupp + brcp->len;

      while (bupp < buppend) {
        if ((*bupp)->bc == BidiCategory::NSM) {
          /* Reset any NSM to the Bidi_Class stored in priorbc. */
          (*bupp)->bc = priorbc;

          if ((priorbc == BidiCategory::AN) || (priorbc == BidiCategory::EN)) {
            (*bupp)->bc_numeric = 1;
          }

          dirtyBit = 1;
        }
        /* For a "deleted" BIDIUNIT, do nothing. */
        else if ((*bupp)->level != BIDIUNIT::NOLEVEL) {
          /* For all other Bidi_Class, set priorbc to the current value. */
          priorbc = (*bupp)->bc;
        }

        bupp++;
      }

      return dirtyBit;
    }

    /*
     * br_UBA_ResolveEuropeanNumbers
     *
     * This is the method for Rule W2.
     *
     * Resolve European numbers for a single text chain.
     *
     * For each character in the text chain, examine its
     * Bidi_Class. For characters of bc=EN, scan back to find the first
     * character of strong type (or sot). If the strong type is bc=AL,
     * change the Bidi_Class EN to AN. Formatting characters
     * (Bidi_Class RLE, LRE, RLO, LRO, PDF) and boundary neutral (Bidi_Class BN)
     * are skipped over in this calculation, because they have been
     * "deleted" by Rule X9.
     */
    int br_UBA_ResolveEuropeanNumbers(BIDIRULECONTEXT *brcp) const {
      int dirtyBit = 0;

      BIDIUNIT **buppfirst = brcp->textChain;
      BIDIUNIT **bupplast = buppfirst + brcp->len - 1;

      /*
       * Process the text chain in reverse from last to first BIDIUNIT,
       * checking the Bidi_Class of each character.
       */
      for (BIDIUNIT **bupp = bupplast; bupp >= buppfirst; bupp--) {
        if ((*bupp)->bc == BidiCategory::EN) { /* For any EN found, scan back in the run to find the first strong type */
          /* Default firststrongbc to sot */
          BidiCategory firststrongbc = brcp->sot;

          if (bupp > buppfirst) {
            for (BIDIUNIT **bupp2 = bupp - 1; bupp2 >= buppfirst; bupp2--) {
              if (((*bupp2)->bc == BidiCategory::L) || ((*bupp2)->bc == BidiCategory::R) || ((*bupp2)->bc == BidiCategory::AL)) {
                firststrongbc = (*bupp2)->bc;
                break;
              }
            }
          }

          /*
           * Check if the first strong type is AL. If so
           * reset this EN to AN. This change does not affect
           * the bc_numeric flag.
           */
          if (firststrongbc == BidiCategory::AL) {
            (*bupp)->bc = BidiCategory::AN;
            dirtyBit = 1;
          }
        }
      }

      return dirtyBit;
    }

    /*
     * br_UBA_ResolveAL
     *
     * This is the method for Rule W3.
     *
     * Resolve Bidi_Class=AL for a single text chain.
     *
     * For each character in the text chain, examine its
     * Bidi_Class. For characters of bc=AL, change the Bidi_Class
     * value to R.
     */
    int br_UBA_ResolveAL(BIDIRULECONTEXT *brcp) const {
      int dirtyBit = 0;

      BIDIUNIT **buppfirst = brcp->textChain;
      BIDIUNIT **bupplast = buppfirst + brcp->len - 1;

      /*
       * Process the text chain from first to last BIDIUNIT,
       * checking the Bidi_Class of each character.
       */
      for (BIDIUNIT **bupp = buppfirst; bupp <= bupplast; bupp++) {
        if ((*bupp)->bc == BidiCategory::AL) { /* Reset any AL to the Bidi_Class R. */
          (*bupp)->bc = BidiCategory::R;
          dirtyBit = 1;
        }
      }

      return dirtyBit;
    }

    /*
     * br_UBA_ResolveSeparators
     *
     * This is the method for Rule W4.
     *
     * Resolve Bidi_Class=ES and CS for a single text chain.
     *
     * For each character in the text chain, examine its
     * Bidi_Class.
     *
     * For characters of bc=ES, check if they are *between* EN.
     * If so, change their Bidi_Class to EN.
     *
     * For characters of bc=CS, check if they are *between* EN
     * or between AN. If so, change their Bidi_Class to match.
     *
     * Update the bc_numeric flag for any ES or CS changed.
     */
    int br_UBA_ResolveSeparators(BIDIRULECONTEXT *brcp) const {
      int dirtyBit = 0;

      BIDIUNIT **buppfirst = brcp->textChain;
      BIDIUNIT **bupplast = buppfirst + brcp->len - 1;

      /*
       * Process the text chain from first to last BIDIUNIT,
       * checking the Bidi_Class of each character.
       */
      for (BIDIUNIT **bupp = buppfirst; bupp <= bupplast; bupp++) {
        if ((*bupp)->bc == BidiCategory::ES) { /* Check to see if ES is in context EN ES EN */
          if (br_IsPriorContext(buppfirst, bupp, BidiCategory::EN) && br_IsFollowingContext(bupplast, bupp, BidiCategory::EN)) {
            (*bupp)->bc = BidiCategory::EN;
            (*bupp)->bc_numeric = 1;
            dirtyBit = 1;
          }
        }
        else if ((*bupp)->bc == BidiCategory::CS) { /* Check to see if CS is in context EN CS EN  or AN CS AN */
          if (br_IsPriorContext(buppfirst, bupp, BidiCategory::EN) && br_IsFollowingContext(bupplast, bupp, BidiCategory::EN)) {
            (*bupp)->bc = BidiCategory::EN;
            (*bupp)->bc_numeric = 1;
            dirtyBit = 1;
          }
          else if (br_IsPriorContext(buppfirst, bupp, BidiCategory::AN) && br_IsFollowingContext(bupplast, bupp, BidiCategory::AN)) {
            (*bupp)->bc = BidiCategory::AN;
            (*bupp)->bc_numeric = 1;
            dirtyBit = 1;
          }
        }
      }

      return dirtyBit;
    }

    /*
     * br_UBA_ResolveTerminators
     *
     * This is the method for Rule W5.
     *
     * Resolve Bidi_Class=ET for a single text chain.
     *
     * For each character in the text chain, examine its
     * Bidi_Class.
     *
     * For characters of bc=ET, check if they are *next to* EN.
     * If so, change their Bidi_Class to EN. This includes
     * ET on either side of EN, so the context on both sides
     * needs to be checked.
     *
     * Because this rule applies to indefinite sequences of ET,
     * and because the context which triggers any change is
     * adjacency to EN, the strategy taken here is to seek for
     * EN first. If found, scan backwards, changing any eligible
     * ET to EN. Then scan forwards, changing any eligible ET
     * to EN. Then continue the search from the point of the
     * last ET changed (if any).
     * 
     * Update the bc_numeric flag for any ET changed.
     */
    int br_UBA_ResolveTerminators(BIDIRULECONTEXT *brcp) const {
      int dirtyBit = 0;

      BIDIUNIT **buppfirst = brcp->textChain;
      BIDIUNIT **bupplast = buppfirst + brcp->len - 1;
      BIDIUNIT **bupp2;

      /*
       * Process the run from first to last BIDIUNIT,
       * checking the Bidi_Class of each character.
       */
      BIDIUNIT **bupp = buppfirst;
      while (bupp <= bupplast) {
        if ((*bupp)->bc == BidiCategory::EN) { /* Check to see if there are any adjacent ET */
          /* First scan left for ET, skipping any NOLEVEL characters */
          if (bupp > buppfirst) {
            for (bupp2 = bupp - 1; bupp2 >= buppfirst; bupp2--) {
              if ((*bupp2)->bc == BidiCategory::ET) {
                (*bupp2)->bc = BidiCategory::EN;
                (*bupp2)->bc_numeric = 1;
                dirtyBit = 1;
              }
              else if ((*bupp2)->level != BIDIUNIT::NOLEVEL) {
                break;
              }
            }
          }

          /* Next scan right for ET, skipping any NOLEVEL characters */
          if (bupp < bupplast) {
            for (bupp2 = bupp + 1; bupp2 <= bupplast; bupp2++) {
              if ((*bupp2)->bc == BidiCategory::ET) {
                (*bupp2)->bc = BidiCategory::EN;
                (*bupp2)->bc_numeric = 1;
                dirtyBit = 1;
              }
              else if ((*bupp2)->level != BIDIUNIT::NOLEVEL) {
                break;
              }
            }

            /*
             * If we scanned ahead, reset bupp to bupp2, to prevent
             * reprocessing characters
             * that have already been checked and/or changed.
             */
            bupp = bupp2;
          }
          else {
            /*
             * Otherwise just increment bupp and come around.
             */
            bupp++;
          }
        }
        else {
          /* Increment bupp and come around. */
          bupp++;
        }
      }

      return dirtyBit;
    }

    /*
     * br_UBA_ResolveESCSET
     *
     * This is the method for Rule W6.
     *
     * Resolve remaining Bidi_Class=ES, CS, or ET for a single text chain.
     *
     * For each character in the text chain, examine its
     * Bidi_Class. For characters of bc=ES, bc=CS, or bc=ET, change
     * the Bidi_Class value to ON. This resolves any remaining
     * separators or terminators which were not already processed
     * by Rules W4 and W5.
     */
    int br_UBA_ResolveESCSET(BIDIRULECONTEXT *brcp) const {
      int dirtyBit = 0;

      BIDIUNIT **buppfirst = brcp->textChain;
      BIDIUNIT **bupplast = buppfirst + brcp->len - 1;

      /*
       * Process the text chain from first to last BIDIUNIT,
       * checking the Bidi_Class of each character.
       */
      for (BIDIUNIT **bupp = buppfirst; bupp <= bupplast; bupp++) {
        if (((*bupp)->bc == BidiCategory::ES) || ((*bupp)->bc == BidiCategory::CS) || ((*bupp)->bc == BidiCategory::ET)) {
          /* Reset any ES CS ET to the Bidi_Class ON. */
          (*bupp)->bc = BidiCategory::ON;
          dirtyBit = 1;
        }
      }

      return dirtyBit;
    }

    /*
     * br_UBA_ResolveEN
     *
     * This is the method for Rule W7.
     *
     * Resolve Bidi_Class=EN for a single level text chain.
     *
     * Process the text chain in reverse order. For each character in the text chain, examine its
     * Bidi_Class. For characters of bc=EN, scan back to find the first strong
     * directional type. If that type is L, change the Bidi_Class
     * value of the number to L.
     */
    int br_UBA_ResolveEN(BIDIRULECONTEXT *brcp) const {
      int dirtyBit = 0;

      BIDIUNIT **buppfirst = brcp->textChain;
      BIDIUNIT **bupplast = buppfirst + brcp->len - 1;
      /*
       * Process the text chain from last to first BIDIUNIT,
       * checking the Bidi_Class of each character.
       */
      for (BIDIUNIT **bupp = bupplast; bupp >= buppfirst; bupp--) {
        if ((*bupp)->bc == BidiCategory::EN) {
          /* Scan back to find the first strong type,
           * R, L, or sot.
           */
          BidiCategory bc = BidiCategory::None;
          if (bupp == buppfirst) {
            bc = brcp->sot;
          }
          else {
            BIDIUNIT **bupp2 = bupp - 1;
            while (bupp2 >= buppfirst) {
              bc = (*bupp2)->bc;
              if ((bc == BidiCategory::L) || (bc == BidiCategory::R)) {
                break;
              }

              if (bupp2 == buppfirst) {
                bc = brcp->sot;
                break;
              }

              bupp2--;
            }
          }

          /*
           * If the first strong type is L, reset the
           * bc of the number to L. Update the bc_numeric
           * flag accordingly.
           */
          if (bc == BidiCategory::L) {
            (*bupp)->bc = BidiCategory::L;
            (*bupp)->bc_numeric = 0;
            dirtyBit = 1;
          }
        }
      }

      return dirtyBit;
    }

    /*
     * br_UBA_ResolvePairedBrackets
     *
     * This is the method for Rule N0. (New in UBA63)
     *
     * Resolve paired brackets for a single text chain.
     *
     * For each character in the text chain, examine its
     * Bidi_Class. For any character with the bpt value open or close,
     * scan its context seeking a matching paired bracket. If found,
     * resolve the type of both brackets to match the embedding
     * direction.
     *
     * For UBA63 (and unchanged in UBA70), the error handling for
     * a stack overflow was unspecified for this rule.
     *
     * Starting with UBA80, the exact stack size is specified (63),
     * and the specification declares that if a stack overflow
     * condition is encountered, the BD16 processing for this
     * particular isolating run ceases immediately. This condition
     * does not treated as a fatal error, however, so the rule
     * should not return an error code here, which would stop
     * all processing for *all* runs of the input string.
     *
     * For clarity in this reference implementation, this same
     * behavior is now also used when running UBA63 (or UBA70)
     * rules.
     */
    int br_UBA_ResolvePairedBrackets(BIDIRULECONTEXT *brcp) const {
      /*
       * Initialize the bracket stack and the pair list.
       */
      UBAStack<BRACKETSTACKELEMENT> bracketStack(63);
      std::list<BRACKETLISTELEMENT> bracketList;

      BIDIUNIT **buppfirst = brcp->textChain;
      BIDIUNIT **bupplast = buppfirst + brcp->len - 1;

      /*
       * Process the text chain from first to last BIDIUNIT,
       * checking the Bidi_Paired_Bracket_Type of each character.
       *
       * BD16 examples used 1-based indexing, but this implementation
       * used 0-based indexing, for consistency with other processing
       * of text chains.
       *
       * UBA80 provided a clarification that the testing for bracket
       * pairs only applies to brackets whose current Bidi_Class is
       * still BIDI_ON at this point in the algorithm. This testing
       * is now done. It can affect outcomes for complicated mixes
       * of explicit embedding controls and bracket pairs, but does
       * not seem to have ever been tested in conformance test cases
       * for earlier implementations. In this bidiref implementation,
       * this check is not assumed to apply to UBA63 (and UBA70),
       * in order to maintain backward compatibility with the bidiref
       * implementation for those versions. By the UBA80 rules,
       * any bracket that has been forced to R or L already
       * by the resolution of explicit embedding (see X6) is simply
       * passed over for bracket pair matching.
       */
      int pos = 0;
      BIDIUNIT **bupp = buppfirst;
      bool const testONisNotRequired = GetUBAVersion() < UBAVersion::UBA80;
      for (; bupp <= bupplast; bupp++, pos++) {
        if (((*bupp)->bpt != ParenthesisDirection::None) && ((*bupp)->bc == BidiCategory::ON) || testONisNotRequired) {
          if ((*bupp)->bpt == ParenthesisDirection::Open) {
            /* Process an opening bracket. Push on the stack. */
            BRACKETSTACKELEMENT const bracketData = {(*bupp)->bpb, pos};
            if (!bracketStack.push(bracketData)) {
              /*
               * Stack overflow. Stop processing this text chain.
               * Return 0 to indicate no change was made.
               */
              return 0;
            }
          }
          else {
            /*
             * Process a closing bracket.
             * br_SeekOpeningBracketMatch handles the search
             * through the stack and the pairslist building if
             * any match is found.
             */
            br_SeekOpeningBracketMatch((*bupp)->c, pos, bracketList, bracketStack);
          }
        }
      }

      if (bracketList.empty()) {
        /*
         * The pairList pointer will still be NULL if no paired brackets
         * were found. In this case, no further processing is
         * necessary. Just return 0 (no change to set the dirtyBit).
         */
        return 0;
      }

      /*
       * Do further processing on the calculated pair list.
       *
       * First sort the pair list.
       */
      bracketList.sort([](BRACKETLISTELEMENT const &first, BRACKETLISTELEMENT const &second) { return first.openingpos < second.openingpos; });

      /*
       * Next scan through the pair list, resolving the
       * embedding levels of each identified pair of brackets.
       */
      br_ResolvePairEmbeddingLevels(brcp, bracketList);

      return 1;
    }

    /*
     * br_ResolvePairEmbeddingLevels
     *
     * Scan through the pair list, resolving the embedding
     * levels of each pair of matched brackets.
     */
    void br_ResolvePairEmbeddingLevels(BIDIRULECONTEXT *brcp, std::list<BRACKETLISTELEMENT> const &bracketList) const {
      /*
       * Scan through the pair list and resolve each pair in order.
       */
      for (auto const &pair : bracketList) {
        /*
         * Now for each pair, we have the first and last position
         * of the substring in this isolating run sequence
         * enclosed by those brackets (inclusive
         * of the brackets). Resolve that individual pair.
         */
        br_ResolveOnePair(brcp, pair.openingpos, pair.closingpos);
      }

      return;
    }

    /*
     * br_ResolveOnePair
     *
     * Resolve the embedding levels of one pair of matched brackets.
     *
     * This determination is based on the embedding direction.
     * See BD3 in the UBA specification.
     *
     * If embedding level is even, embedding direction = L.
     * If embedding level is odd,  embedding direction = R.
     */
    void br_ResolveOnePair(BIDIRULECONTEXT *brcp, int firstpos, int lastpos) const {
      BIDIUNIT **buppfirst = brcp->textChain;
      BIDIUNIT **bupplast = buppfirst + brcp->len - 1;
      BIDIUNIT **buppopening = buppfirst + firstpos;
      BIDIUNIT **buppclosing = buppfirst + lastpos;

      /*
       * First establish the embedding direction, based on the embedding
       * level passed in as a paramter with the context for this text chain.
       */
      BidiCategory const embeddingdirection = (brcp->level % 2 == 1) ? BidiCategory::R : BidiCategory::L;
      BidiCategory const oppositedirection = (embeddingdirection == BidiCategory::L) ? BidiCategory::R : BidiCategory::L;
      int strongtypefound = 0;

      /*
       * Next check for a strong type (R or L) between firstpos and lastpos,
       * i.e., one between the matched brackets. If a strong type is found
       * which matches the embedding direction, then set the type of both
       * brackets to match the embedding direction, too.
       */
      if (firstpos < lastpos - 1) {
        for (BIDIUNIT **bupp = buppopening + 1; bupp <= buppclosing - 1; bupp++) {
          /*
           * For the purposes of this direction checking, any EN or AN
           * which have not been reset by the Weak rules count as
           * BidiCategory::R. Set up a temporary bc variable based on mapping
           * any EN or AN value to BidiCategory::R.
           */
          BidiCategory const tempbc = ((((*bupp)->bc) == BidiCategory::R) || ((*bupp)->bc_numeric))
              ? BidiCategory::R
              : (((*bupp)->bc) == BidiCategory::L)
                  ? BidiCategory::L
                  : BidiCategory::None;

          if (tempbc == embeddingdirection) {
            /* N0 Step b, */
            br_SetBracketPairBC(buppopening, buppclosing, bupplast, embeddingdirection);
            return;
          }
          else if (tempbc == oppositedirection) {
            strongtypefound = 1;
          }
        }
      }

      /*
       * Next branch on whether we found a strong type opposite the embedding
       * direction between the brackets or not.
       */

      if (strongtypefound) {
        /*
         * First attempt to resolve direction by checking the prior context for
         * a strong type matching the opposite direction. N0 Step c1.
         */
        if (((oppositedirection == BidiCategory::L) && (br_IsPriorContextL(buppfirst, buppopening, brcp->sot))) ||
            ((oppositedirection == BidiCategory::R) && (br_IsPriorContextRANEN(buppfirst, buppopening, brcp->sot)))) {
          br_SetBracketPairBC(buppopening, buppclosing, bupplast, oppositedirection);
        }
        else {
          /*
           * No strong type matching the oppositedirection was found either
           * before or after these brackets in this text chain. Resolve the
           * brackets based on the embedding direction. N0 Step c2.
           */
          br_SetBracketPairBC(buppopening, buppclosing, bupplast, embeddingdirection);
        }
      }
      else {
        /*
         * No strong type was found between the brackets. Leave
         * the brackets with unresolved direction.
         */
      }
    }

    /*
     * br_UBA_ResolveNeutralsByContext
     *
     * This is the method for Rule N1.
     *
     * Resolve neutrals by context for a single text chain.
     *
     * For each character in the text chain, examine its
     * Bidi_Class. For any character of neutral type, examine its
     * context.
     *
     * L N L --> L L L
     * R N R --> R R R [note that AN and EN count as R for this rule]
     *
     * Here "N" stands for "any sequence of neutrals", so the neutral
     * does not have to be immediately adjacent to a strong type
     * to be resolved this way.
     */
    int br_UBA_ResolveNeutralsByContext(BIDIRULECONTEXT *brcp) const {
      int dirtyBit = 0;
      BIDIUNIT **buppfirst = brcp->textChain;
      BIDIUNIT **bupplast = buppfirst + brcp->len - 1;

      /*
       * Process the text chain from first to last BIDIUNIT,
       * checking the Bidi_Class of each character.
       */
      for (BIDIUNIT **bupp = buppfirst; bupp <= bupplast; bupp++) {
        if (br_IsNeutralType((*bupp)->bc)) {
          /* Check to see if N is in context L N L */
          if (br_IsPriorContextL(buppfirst, bupp, brcp->sot) && br_IsFollowingContextL(bupplast, bupp, brcp->eot)) {
            (*bupp)->bc = BidiCategory::L;
            dirtyBit = 1;
          }
          /* Check to see if N is in context R N R */
          else if (br_IsPriorContextRANEN(buppfirst, bupp, brcp->sot) && br_IsFollowingContextRANEN(bupplast, bupp, brcp->eot)) {
            (*bupp)->bc = BidiCategory::R;
            dirtyBit = 1;
          }
        }
      }

      return dirtyBit;
    }

    /*
     * br_UBA_ResolveNeutralsByLevel
     *
     * This is the method for Rule N2.
     *
     * Resolve neutrals by level for a single text chain.
     *
     * For each character in the text chain, examine its
     * Bidi_Class. For any character of neutral type, examine its
     * embedding level and resolve accordingly.
     *
     * N --> e
     * where e = L for an even level, R for an odd level
     */
    int br_UBA_ResolveNeutralsByLevel(BIDIRULECONTEXT *brcp) const {
      int dirtyBit = 0;
      BIDIUNIT **buppfirst = brcp->textChain;
      BIDIUNIT **bupplast = buppfirst + brcp->len - 1;

      /*
       * Process the text chain from first to last BIDIUNIT,
       * checking the Bidi_Class of each character.
       */
      for (BIDIUNIT **bupp = buppfirst; bupp <= bupplast; bupp++) {
        if (br_IsNeutralType((*bupp)->bc)) {
          /* Check to see if N is in even embedding level */
          if ((*bupp)->level % 2 == 0) {
            (*bupp)->bc = BidiCategory::L;
          }
          else {
            (*bupp)->bc = BidiCategory::R;
          }

          dirtyBit = 1;
        }
      }

      return dirtyBit;
    }

    /*
     * br_UBA_ResolveImplicitLevels
     *
     * This function runs Rules I1 and I2 together.
     */
    void br_UBA_ResolveImplicitLevels(UBACONTEXT *ctxt) const {
      ctxt->dirtyBit = 0;
      BIDIUNIT *bdu = ctxt->theText.data();
      BIDIUNIT *endOfText = ctxt->theText.data() + ctxt->textLen;
      while (bdu < endOfText) {
        if (bdu->level != BIDIUNIT::NOLEVEL) {
          int const oddlevel = (bdu->level % 2 == 1);
          if (oddlevel) {
            if ((bdu->bc == BidiCategory::L) || (bdu->bc_numeric)) {
              bdu->level += 1;
              ctxt->dirtyBit = 1;
            }
          }
          else {
            if (bdu->bc == BidiCategory::R) {
              bdu->level += 1;
              ctxt->dirtyBit = 1;
            }
            else if (bdu->bc_numeric) {
              bdu->level += 2;
              ctxt->dirtyBit = 1;
            }
          }
        }

        /* Advance to the next character to process. */
        bdu++;
      }

      ctxt->state = AlgorithmState::I2Done;
    }

    /*
     * br_IsFollowingContextL
     *
     * Scan forwards in a text chain, checking if the first non-neutral character is an "L" type.
     * Skip over any "deleted" controls, which have NOLEVEL, as well as any neutral types.
     */
    bool br_IsFollowingContextL(BIDIUNIT **bupplast, BIDIUNIT **bupp, BidiCategory eot) const {
      if (bupp == bupplast) {
        return eot == BidiCategory::L;
      }

      BIDIUNIT **tbupp = bupp + 1;
      while (tbupp <= bupplast) {
        if ((*tbupp)->bc == BidiCategory::L) {
          return true;
        }
        else if ((*tbupp)->level == BIDIUNIT::NOLEVEL) {
          tbupp++;
        }
        else if (br_IsNeutralType((*tbupp)->bc)) {
          tbupp++;
        }
        else {
          return false;
        }
      }

      return eot == BidiCategory::L;
    }

    /*
     * br_IsPriorContextL
     *
     * Scan backwards in a text chain, checking if the first non-neutral character is an "L" type.
     * Skip over any "deleted" controls, which have NOLEVEL, as well as any neutral types.
     */
    bool br_IsPriorContextL(BIDIUNIT **buppfirst, BIDIUNIT **bupp, BidiCategory sot) const {
      if (bupp == buppfirst)
        return sot == BidiCategory::L;

      BIDIUNIT **tbupp = bupp - 1;
      while (tbupp >= buppfirst) {
        if ((*tbupp)->bc == BidiCategory::L) {
          return 1;
        }
        else if ((*tbupp)->level == BIDIUNIT::NOLEVEL) {
          tbupp--;
        }
        else if (br_IsNeutralType((*tbupp)->bc)) {
          tbupp--;
        }
        else {
          return false;
        }
      }

      return sot == BidiCategory::L;
    }

    /*
     * br_IsNeutralType
     *
     * Encapsulate the checking for Bidi_Class values of neutral
     * type (B, S, WS, ON).
     *
     * Note that BIDI_B is kept at the end of each "paragraph" to
     * be run through the algorithm, so it *can* occur as the very
     * last element of the paragraph and has to be checked here.
     *
     * To keep the expression of the rules fairly simple, the
     * extension of "neutral" to "NI" in UBA63, including all
     * the isolate format controls, is implemented in this rule
     * with a test on UBA version.
     *
     * Note that optimized implementations can speed up this kind
     * of checking by keeping precomputed boolean or bit arrays indexed by
     * Bidi_Class values and returning these kinds of True/False
     * queries by a single array lookup instead of using chains of
     * individual Bidi_Class equality tests.
     */
    bool br_IsNeutralType(BidiCategory bc) const {
      if ((bc == BidiCategory::ON) || (bc == BidiCategory::WS) || (bc == BidiCategory::S) || (bc == BidiCategory::B))
        return true;
      else if ((GetUBAVersion() > UBAVersion::UBA62) && (br_IsIsolateControl(bc)))
        return true;

      return false;
    }

    /*
     * br_IsIsolateControl
     *
     * Encapsulate the checking for Bidi_Class values of isolate
     * initiator types (LRI, RLI, FSI) or PDI.
     */
    bool br_IsIsolateControl(BidiCategory bc) const {
      if ((bc == BidiCategory::LRI) || (bc == BidiCategory::RLI) || (bc == BidiCategory::FSI) || (bc == BidiCategory::PDI))
        return true;

      return false;
    }

    /*
     * br_IsFollowingContextRANEN
     *
     * Used by Rule N1.
     *
     * Scan forwards in a text chain, checking if the first non-neutral character is an "R" type.
     * (BIDI_R, BIDI_AN, BIDI_EN) Skip over any "deleted" controls, which
     * have NOLEVEL, as well as any neutral types.
     */
    bool br_IsFollowingContextRANEN(BIDIUNIT **bupplast, BIDIUNIT **bupp, BidiCategory eot) const {
      if (bupp == bupplast){
        return eot == BidiCategory::R;
      }

      BIDIUNIT **tbupp = bupp + 1;
      while (tbupp <= bupplast) {
        if (((*tbupp)->bc == BidiCategory::R) || ((*tbupp)->bc_numeric)) {
          return true;
        }
        else if ((*tbupp)->level == BIDIUNIT::NOLEVEL) {
          tbupp++;
        }
        else if (br_IsNeutralType((*tbupp)->bc)) {
          tbupp++;
        }
        else {
          return false;
        }
      }

      return eot == BidiCategory::R;
    }

    /*
     * br_IsPriorContextRANEN
     *
     * Used by Rule N1.
     *
     * Scan backwards in a text chain, checking if the first non-neutral character is an "R" type.
     * (BIDI_R, BIDI_AN, BIDI_EN) Skip over any "deleted" controls, which
     * have NOLEVEL, as well as any neutral types.
     */
    bool br_IsPriorContextRANEN(BIDIUNIT **buppfirst, BIDIUNIT **bupp, BidiCategory sot) const {
      if (bupp == buppfirst)
        return ((sot == BidiCategory::R) ? 1 : 0);

      BIDIUNIT **tbupp = bupp - 1;
      while (tbupp >= buppfirst) {
        if (((*tbupp)->bc == BidiCategory::R) || ((*tbupp)->bc_numeric)) {
          return true;
        }
        else if ((*tbupp)->level == BIDIUNIT::NOLEVEL) {
          tbupp--;
        }
        else if (br_IsNeutralType((*tbupp)->bc)) {
          tbupp--;
        }
        else {
          return false;
        }
      }

      return sot == BidiCategory::R;
    }

    /*
    * br_SetBracketPairBC
    *
    * Set the Bidi_Class of a bracket pair, based on the
    * direction determined by the N0 rule processing in
    * br_ResolveOnePair().
    *
    * The direction passed in will either be BIDI_R or BIDI_L.
    *
    * This setting is abstracted in a function here, rather than
    * simply being done inline in br_ResolveOnePair, because of
    * an edge case added to rule N0 as of UBA80. For UBA63 (and
    * UBA70), no special handling of combining marks following
    * either of the brackets is done. However, starting with UBA80,
    * there is an edge case fix-up done which echoes the processing
    * of rule W1. The text run needs to be scanned to find any
    * combining marks (orig_bc=NSM) following a bracket which has
    * its Bidi_Class changed by N0. Then those combining marks
    * can again be adjusted to match the Bidi_Class of the
    * bracket they apply to. This is an odd edge case, as combining
    * marks do not typically occur with brackets, but the UBA80
    * specification is now explicit about requiring this fix-up
    * to be done.
    */
    void br_SetBracketPairBC(BIDIUNIT **buppopening, BIDIUNIT **buppclosing, BIDIUNIT **bupplast, BidiCategory direction) const {
      (*buppopening)->bc = direction;
      (*buppclosing)->bc = direction;

      if (GetUBAVersion() >= UBAVersion::UBA80) {
        /*
         * Here is the tricky part.
         *
         * First scan from the opening bracket for any subsequent
         * character whose *original* Bidi_Class was NSM, and set
         * the current bc for it to direction also, to match the bracket.
         * Break out of the loop at the first character with any other
         * original Bidi_Class, so that this change only impacts
         * actual combining mark sequences.
         *
         * Then repeat the process for the matching closing bracket.
         *
         * The processing for the opening bracket is bounded to the
         * right by the position of the matching closing bracket.
         * The processing for the closing bracket is bounded to the
         * right by the end of the text run.
         */
        for (BIDIUNIT **bupp = buppopening + 1; bupp <= buppclosing - 1; bupp++) {
          if ((*bupp)->orig_bc == BidiCategory::NSM) {
            (*bupp)->bc = direction;
          }
          else {
            break;
          }
        }

        for (BIDIUNIT **bupp = buppclosing + 1; bupp <= bupplast; bupp++) {
          if ((*bupp)->orig_bc == BidiCategory::NSM) {
            (*bupp)->bc = direction;
          }
          else {
            break;
          }
        }
      }
    }

    /*
     * br_UBA_RuleDispatch
     *
     * The rule dispatcher abstracts the version-specific extraction
     * of text chains from the UBACONTEXT and dispatches text chains,
     * one at a time, to the appropriate rule method.
     *
     * This enables better encapsulation of the distinction between
     * UBA62 rules, which apply to lists of level runs, and
     * UBA63 rules, which apply to isolating run sequences (which
     * in turn consist of sets of level runs).
     *
     * The logic for the rule itself, which is applied on a per text chain
     * basis, can be abstracted to the ruleMethod, which often does
     * not need to be made version-specific.
     *
     * The return value from the ruleMethod is used to set the
     * dirtyBit in the UBACONTEXT.
     *
     * br_UBA_RuleDispatch is only used for rules which can be
     * analyzed as applying independently to a single text chain, and
     * which are not version-specific in their application to the runs.
     * (Currently the W rules and the N rules.)
     *
     * br_UBA_RuleDispatch also handles printing out an error message,
     * if any, and printing the debug display diagnostics, to cut
     * down on repetitive code in the main UBA call routines.
     */
    bool br_UBA_RuleDispatch(UBACONTEXT *ctxt, BidiRuleType rule) const {
      static std::unordered_map<BidiRuleType, std::pair<int (UBA::*)(BIDIRULECONTEXT *) const, AlgorithmState>> const states = {
        {BidiRuleType::W1, {&UBA::br_UBA_ResolveCombiningMarks, AlgorithmState::W1Done}},
        {BidiRuleType::W2, {&UBA::br_UBA_ResolveEuropeanNumbers, AlgorithmState::W2Done}},
        {BidiRuleType::W3, {&UBA::br_UBA_ResolveAL, AlgorithmState::W3Done}},
        {BidiRuleType::W4, {&UBA::br_UBA_ResolveSeparators, AlgorithmState::W4Done}},
        {BidiRuleType::W5, {&UBA::br_UBA_ResolveTerminators, AlgorithmState::W5Done}},
        {BidiRuleType::W6, {&UBA::br_UBA_ResolveESCSET, AlgorithmState::W6Done}},
        {BidiRuleType::W7, {&UBA::br_UBA_ResolveEN, AlgorithmState::W7Done}},
        {BidiRuleType::N0, {&UBA::br_UBA_ResolvePairedBrackets, AlgorithmState::N0Done}},
        {BidiRuleType::N1, {&UBA::br_UBA_ResolveNeutralsByContext, AlgorithmState::N1Done}},
        {BidiRuleType::N2, {&UBA::br_UBA_ResolveNeutralsByLevel, AlgorithmState::N2Done}}
      };

      auto iter = states.find(rule);
      if (iter == std::end(states))
          return false;

      auto callback = iter->second.first;
      auto state = iter->second.second;

      ctxt->dirtyBit = 0;

      int rc = 0;
      BIDIRULECONTEXT brc;

      if (GetUBAVersion() == UBAVersion::UBA62) {
        BIDIRUN *brp = ctxt->theRuns;

        while (brp != nullptr) {
          /* Initialize the BIDI_RULE_CONTEXT. */
          brc.textChain = brp->textChain;
          brc.len = brp->len;
          brc.level = brp->level;
          brc.sot = brp->sor;
          brc.eot = brp->eor;

          /*
           * Invoke the appropriate rule method for this rule,
           * applying it to the text chain for this run.
           */
          rc = (this->*callback)(&brc);
          if (rc == 1) {
            ctxt->dirtyBit = 1;
          }
          else if (rc == -1) {
            break;
          }

          /* Advance to the next run */
          brp = brp->next;
        }
      }
      else {
        IRSEQ *irp = ctxt->theSequences;

        while (irp != nullptr) {
          /* Initialize the BIDI_RULE_CONTEXT. */
          brc.textChain = irp->textChain;
          brc.len = irp->len;
          brc.level = irp->level;
          brc.sot = irp->sos;
          brc.eot = irp->eos;

          /*
           * Invoke the appropriate rule method for this rule,
           * applying it to the text chain for this run.
           */

          rc = (this->*callback)(&brc);
          if (rc == 1) {
            ctxt->dirtyBit = 1;
          }
          else if (rc == -1) {
            break;
          }

          /* Advance to the next run */
          irp = irp->next;
        }
      }

      ctxt->state = state;

      return ((rc == -1) ? false : true);
    }

    /*
     * br_UBA_WRulesDispatch
     *
     * Dispatch rules W1-W7.
     *
     * The dispatch and order of application is the same for UBA62
     * and for UBA63.
     */
    bool br_UBA_WRulesDispatch(UBACONTEXT *ctxt) const {
      if (!br_UBA_RuleDispatch(ctxt, BidiRuleType::W1))
        return false;

      if (!br_UBA_RuleDispatch(ctxt, BidiRuleType::W2))
        return false;

      if (!br_UBA_RuleDispatch(ctxt, BidiRuleType::W3))
        return false;

      if (!br_UBA_RuleDispatch(ctxt, BidiRuleType::W4))
        return false;

      if (!br_UBA_RuleDispatch(ctxt, BidiRuleType::W5))
        return false;

      if (!br_UBA_RuleDispatch(ctxt, BidiRuleType::W6))
        return false;

      if (!br_UBA_RuleDispatch(ctxt, BidiRuleType::W7))
        return false;

      return true;
    }

    /*
     * br_UBA_ReverseLevels
     *
     * This function runs Rule L2.
     *
     * Find the highest level among the resolved levels.
     * Then from that highest level down to the lowest odd
     * level, reverse any contiguous runs at that level or higher.
     */
    void br_UBA_ReverseLevels(UBACONTEXT *ctxt) const {
      ctxt->dirtyBit = 0;

      /*
       * First scan the text to determine the highest level and
       * the lowest odd level.
       */
      BIDIUNIT *bdu = ctxt->theText.data();
      BIDIUNIT *endOfText = ctxt->theText.data() + ctxt->textLen;
      int highestlevel = 0;
      int lowestoddlevel = (int) GetMaximumDepth() + 1;
      bool nolevels = true;

      while (bdu < endOfText) {
        if (bdu->level != BIDIUNIT::NOLEVEL) {
          /* Found something other than NOLEVEL. */
          nolevels = false;
          if (bdu->level > highestlevel) {
            highestlevel = bdu->level;
          }

          if ((bdu->level % 2 == 1) && (bdu->level < lowestoddlevel)) {
            lowestoddlevel = bdu->level;
          }
        }
        /* Advance to the next character to process. */

        bdu++;
      }

      /* If there are no levels set, don't bother with reordering anything. */
      if (nolevels){
        ctxt->state = AlgorithmState::L2Done;
        return;
      }

      /*
       * Next reverse contiguous runs at each level (or higher),
       * starting with the highest level and decrementing to
       * the lowest odd level.
       */
      for (int level = highestlevel; level >= lowestoddlevel; level--) {
        /*
         * For each relevant level, scan the text to find
         * contiguous ranges at that level (or higher).
         * A *significant* contiguous range is figured as
         * one that contains at least two characters with
         * an explicit level. Don't bother reversing ranges
         * which contain only one character with an explicit
         * level and then one or more trailing NOLEVEL "deleted"
         * characters.
         */
        int ix = 0;
        bdu = ctxt->theText.data();
        bool inrange = false;
        int significantrange = 0;
        int firstpos = -1;
        int lastpos = -1;

        while (ix < ctxt->textLen) {
          if (bdu->level >= level) {
            if (!inrange) {
              inrange = true;
              firstpos = ix;
            }
            else {
              /* Hit a second explicit level character. */
              significantrange = 1;
              lastpos = ix;
            }
          }
          else if (bdu->level == BIDIUNIT::NOLEVEL) {
            /* don't break ranges for "deleted" controls */
            if (inrange) {
              lastpos = ix;
            }
          }
          else {
            /* End of a range. Reset the range flag and reverse the range. */
            inrange = false;
            if ((lastpos > firstpos) && significantrange) {
              br_ReverseRange(ctxt, firstpos, lastpos);
              ctxt->dirtyBit = 1;
            }

            firstpos = -1;
            lastpos = -1;
          }

          /* Advance to the next character to process. */
          ix++;
          bdu++;
        }

        /*
         * If we reached the end of the input while the inrange flag is
         * set, then we need to do a reversal before exiting this level.
         */
        if (inrange && (lastpos > firstpos) && significantrange) {
          br_ReverseRange(ctxt, firstpos, lastpos);
          ctxt->dirtyBit = 1;
        }
      }

      ctxt->state = AlgorithmState::L2Done;
    }

    /*
     * br_ReverseRange
     *
     * For a specified range firstpos to lastpos, invert the position
     * values stored in the order field in the array of BIDIUNITs.
     *
     * Rather than reordering in place, which requires fancier manipulation
     * of indices, just use a spare order array in the BIDIUNIT vector.
     * Write the values into the range in reverse order, then copy them
     * back into the main order array in the reversed order.
     * This is a simple and easy to understand approach.
     */
    void br_ReverseRange(UBACONTEXT *ctxt, int firstpos, int lastpos) const {
      for (int ix = firstpos, newpos = lastpos; ix < newpos; ix++, newpos--) {
        std::swap(ctxt->theText[ix], ctxt->theText[newpos]);
      }
    }

    /*
     * br_HasMatchingPDI
     *
     * Scan forwards from the current pointer, looking for a matching
     * PDI for an isolate initiator.
     *
     * This scan needs to check for multiple isolate sequences, and find the
     * PDI which matches this isolate initiator.
     *
     * The strategy is to create a dumb
     * level counter, starting at 1 and increment or decrement it
     * for each PDI or isolate initiator encountered. A match condition
     * consists of first encounter of a PDI (while scanning
     * forwards) when the dumb level counter is set to zero.
     *
     * Return 1 if a match is found, 0 if no match is found.
     */
    bool br_HasMatchingPDI(BIDIUNIT* current, BIDIUNIT* endOfText, BIDIUNIT** pdiPtr) const {
      int dumblevelctr = 1;
      BIDIUNIT* bdu = current + 1;

      while (bdu < endOfText) {
        /* Check the Bidi_Class */
        BidiCategory const bc = bdu->bc;

        /* If we hit a PDI, decrement the level counter */
        if (bc == BidiCategory::PDI) {
          dumblevelctr--;
          /*
           * If the level counter has decremented back to zero, we have a match.
           * Set a pointer to the PDI we have found.
           */
          if (dumblevelctr == 0) {
            *pdiPtr = bdu;
            return true;
          }
        }
        /* If we hit another isolate initiator, increment the level counter */
        else if (bdu->bc_isoinit) {
          dumblevelctr++;
        }

        /* Increment the unit pointer */
        bdu++;
      }

      /* Fell through without a match. Return 0. */
      return false;
    }

    /*
     * Encapsulate the calculation of least greater odd or
     * even embedding levels.
     *
     * These functions return -1 if the resulting odd or even
     * embedding level would not be valid (exceeds the maximum
     * allowable level).
     */
    int br_leastGreaterOddLevel(int level, int maximum_depth) const {
      int const templevel = (level % 2 == 1) ? level + 2 : level + 1;
      return (templevel > maximum_depth) ? -1 : templevel;
    }

    int br_leastGreaterEvenLevel(int level, int maximum_depth) const {
      int const templevel = (level % 2 == 1) ? level + 2 : level + 1;
      return (templevel > maximum_depth - 1) ? -1 : templevel;
    }

    /*
     * br_FirstSignificantBC
     *
     * Return the first Bidi_Class value in the run for a non-deleted
     * element of the run (or Bidi_None, if all elements of the run
     * are deleted).
     *
     * This function is used to identify a matching PDI in a discontiguous
     * run for a sequence, when the first element (or elements) of the run is actually
     * a deleted format character. This accounts for matching across such
     * deletions, when brp->first->bc results in the wrong value.
     */
    BidiCategory br_FirstSignificantBC(BIDIRUN *brp) const {
      BIDIUNIT *bdu = brp->first;

      while (bdu <= brp->last) {
        if (bdu->level != BIDIUNIT::NOLEVEL) {
          return (bdu->bc);
        }

        bdu++;
      }

      return BidiCategory::None;
    }

    /*
     * br_DecideParaLevel
     *
     * Run rules P2 and P3 on the range start to endOfText, and return
     * a paragraph level value.
     */
    int br_DecideParaLevel(BIDIUNIT* start, BIDIUNIT* endOfText) const {
      BIDIUNIT* bdu = start;
      BIDIUNIT* pdiPtr = nullptr;

      while (bdu < endOfText) {
        BidiCategory const bc = bdu->bc;
        if (bc == BidiCategory::L)
          return 0;
        else if ((bc == BidiCategory::R) || (bc == BidiCategory::AL))
          return 1;
        else if (bdu->bc_isoinit) {
          bool const hasPDIMatch = br_HasMatchingPDI(bdu, endOfText, &pdiPtr);
          if (hasPDIMatch) {
            /*
             * Set bdu past the PDI which marks the end of
             * the isolated sequence.
             */
            bdu = pdiPtr + 1;
            continue;
          }
          else
          {
            /*
             * If there is no matching PDI, return 0.
             */
            return 0;
          }
        }

        /* Dropped through without finding a strong type yet. */
        bdu++;
      }

      return 0;
    }

    /*
     * br_SpanOneRun
     *
     * Take two pointers to a source BIDIUNIT vector.
     * Extract the first run containing characters all with
     * the same level value (or NOLEVEL).
     *
     * This spanning has to be tweaked a bit to work for
     * UBA63, because an isolate initiator needs to terminate
     * a level run:
     *
     *     R  RLI  PDF    R
     * <R-------[ <R-----R>  <== correct
     *
     * <R-----------R> <RR>  <== incorrect
     *
     * This tweak is a no-op for UBA62, which does not deal
     * with isolate format controls.
     *
     * Returns True if the spanning is done, False otherwise.
     */
    bool br_SpanOneRun(BIDIUNIT *first, BIDIUNIT *last, BIDIUNIT **next, int *newlevel, int *spanlen) const {
      int level;
      int isolateInitiatorFound;
      BIDIUNIT *bdu = first;
      int spanlevel = BIDIUNIT::NOLEVEL;
      UBAVersion const version = GetUBAVersion();

      while (bdu <= last) {
        level = bdu->level;
        isolateInitiatorFound = 0;

        /* skip past "deleted" format characters marked with no level */
        if (level != BIDIUNIT::NOLEVEL) {
          if ((version > UBAVersion::UBA62) && bdu->bc_isoinit) {
            isolateInitiatorFound = 1;
          }

          /* the first time a valid level is hit, set spanlevel */
          if (spanlevel == BIDIUNIT::NOLEVEL) {
            spanlevel = level;
          }
          /* when level changes, break from the while loop */
          else if (level != spanlevel) {
            break;
          }
        }

        bdu++;
        if (isolateInitiatorFound) {
          /*
           * Found an isolate initiator in UBA63 processing.
           * Terminate the level run here, including the isolate
           * initiator.
           */
          break;
        }
      }

      /*
       * Set the newlevel. Note that this could be NOLEVEL if the
       * entire vector consists of BN or bidi embedding controls.
       */
      *newlevel = spanlevel;
      *spanlen = (int)(bdu - first);

      /* Now check whether we are at the end of the vector */
      if (bdu > last) {
        /* ran off the end of the vector while in a span */
        *next = nullptr;
        return true; /* spanning done */
      }
      else {
        /* Change of level terminated span. Set next pointer. */
        *next = bdu;
        return false; /* spanning not done */
      }
    }

    /*
     * br_ConstructBidiRun
     *
     * Allocate and initialize a BIDIRUN. These structs are used to construct
     * a linked list of runs.
     */
    BIDIRUN *br_ConstructBidiRun(int id, int level, BIDIUNIT *first, BIDIUNIT *last, int len) const {
      BIDIRUN *brp = (BIDIRUN *) malloc(sizeof(BIDIRUN));

      if (brp == nullptr) {
        return nullptr;
      }

      brp->runID = id;
      brp->seqID = 0;
      brp->level = level;
      brp->first = first;
      brp->last = last;
      brp->len = len;
      brp->sor = BidiCategory::Unknown;
      brp->eor = BidiCategory::Unknown;
      brp->textChain = nullptr;
      brp->next = nullptr;

      return brp;
    }

    /*
     * br_ConstructIsolatingRunSequence
     *
     * Allocate and initialize an ISOLATING_RUN_SEQUENCE. These structs are used to construct
     * a linked list of isolating run sequences.
     *
     * Start the list of runs associated with the sequence and assign brp to
     * the head of that list.
     */
    IRSEQ *br_ConstructIsolatingRunSequence(int id, BIDIRUN *brp) const {
      IRSEQ *irp = (IRSEQ *) malloc(sizeof(IRSEQ));
      if (irp == nullptr) {
        return nullptr;
      }

      BIDIRUNLIST *brlp = br_ConstructBidiRunListElement();
      if (brlp == nullptr) {
        free(irp);
        return nullptr;
      }

      brlp->run = brp;

      irp->seqID = id;
      irp->level = brp->level;
      irp->theRuns = brlp;
      irp->lastRun = brlp;
      irp->sos = BidiCategory::Unknown;
      irp->eos = BidiCategory::Unknown;
      irp->textChain = nullptr;
      irp->next = nullptr;

      return irp;
    }

    /*
     * br_ConstructBidiRunListElement
     *
     * Allocate and initialize a BIDIRUNLISTELEMENT.
     */
    BIDIRUNLIST *br_ConstructBidiRunListElement(void) const {
      BIDIRUNLIST *brlp = (BIDIRUNLIST *) malloc(sizeof(BIDIRUNLIST));
      if (brlp == nullptr) {
        return nullptr;
      }

      brlp->run = nullptr;
      brlp->next = nullptr;

      return brlp;
    }

    /*
     * br_AppendBidiRun
     *
     * Append an allocated and initialized BIDIRUN to the linked list of
     * runs in the context.
     *
     * Maintain the lastRun pointer in the context to make this appending
     * easy for the linked list.
     */
    void br_AppendBidiRun(UBACONTEXT *ctxt, BIDIRUN *brp) const {
      if (ctxt->theRuns == nullptr) {
        ctxt->theRuns = brp;
      }
      else {
        ctxt->lastRun->next = brp;
      }

      ctxt->lastRun = brp;
    }

    /*
     * br_AppendIsolatingRunSequence
     *
     * Append an allocated and initialized ISOLATING_RUN_SEQUENCE to the linked list of
     * sequences in the context.
     *
     * Maintain the lastSequence pointer in the context to make this appending
     * easy for the linked list.
     */
    void br_AppendIsolatingRunSequence(UBACONTEXT *ctxt, IRSEQ *irp) const {
      if (ctxt->theSequences == nullptr) {
        ctxt->theSequences = irp;
      }
      else {
        ctxt->lastSequence->next = irp;
      }

      ctxt->lastSequence = irp;
    }

    /*
     * br_AppendBidiRunToSequence
     *
     * Append an allocated and initialized BIDIRUNLISTPTR (which contains
     * a pointer to a BIDIRUN) to the linked list of
     * runs in the isolating run sequence.
     */
    void br_AppendBidiRunToSequence(IRSEQ *irp, BIDIRUNLIST *brlp) const {
      if (irp->theRuns == nullptr) {
        irp->theRuns = brlp;
      }
      else {
        irp->lastRun->next = brlp;
      }

      irp->lastRun = brlp;
    }

    /*
     * br_IsPriorContext
     *
     * Scan backwards in a text chain, checking if the first prior character matches
     * the bc value passed in. Skip over any "deleted" controls, which
     * have NOLEVEL.
     */ 
    bool br_IsPriorContext(BIDIUNIT **buppfirst, BIDIUNIT **bupp, BidiCategory bc) const {
      if (bupp == buppfirst) {
        return false;
      }

      BIDIUNIT **tbupp = bupp - 1;
      while (tbupp >= buppfirst) {
        if ((*tbupp)->bc == bc) {
          return true;
        }
        else if ((*tbupp)->level == BIDIUNIT::NOLEVEL) {
          tbupp--;
        }
        else {
          return false;
        }
      }

      return false;
    }

    /*
     * br_IsFollowingContext
     *
     * Scan forwards in a text chain, checking if the first subsequent character matches
     * the bc value passed in. Skip over any "deleted" controls, which
     * have NOLEVEL.
     */ 
    bool br_IsFollowingContext(BIDIUNIT **bupplast, BIDIUNIT **bupp, BidiCategory bc) const {
      if (bupp == bupplast) {
        return false;
      }

      BIDIUNIT **tbupp = bupp + 1;
      while (tbupp <= bupplast) {
        if ((*tbupp)->bc == bc) {
          return true;
        }
        else if ((*tbupp)->level == BIDIUNIT::NOLEVEL) {
          tbupp++;
        }
        else {
          return false;
        }
      }

      return false;
    }

    /*
     * br_SeekOpeningBracketMatch
     *
     * Seek an opening bracket pair for the closing bracket
     * passed in.
     *
     * This is a stack based search.
     * Start with the top element in the stack and search
     * downwards until we either find a match or reach the
     * bottom of the stack.
     *
     * If we find a match, construct and append the bracket
     * pair to the pairList. Then pop the stack for all the
     * levels down to the level where we found the match.
     * (This approach is designed to discard pairs that
     * are not cleanly nested.)
     *
     * If we search all the way to the bottom of the stack
     * without finding a match, just return without changing
     * state. This represents a closing bracket with no
     * opening bracket to match it. Just discard and move on.
     */
    bool br_SeekOpeningBracketMatch(uint32_t closingcp, int pos, std::list<BRACKETLISTELEMENT> &list, UBAStack<BRACKETSTACKELEMENT> &stack) const {
      BRACKETSTACKELEMENT bracketData;

      int depth = 1;
      while (depth <= stack.maxDepth()) {
        if (!stack.peek(bracketData, depth)) {
          /*
           * Either the bracket stack is empty or the value of
           * depth exceeds the current depth of the stack.
           * Return no match.
           */
          return false;
        }

        /*
         * The basic test is for the closingcp equal to the bpb value
         * stored in the bracketData. But to account for the canonical
         * equivalences for U+2329 and U+232A, tack on extra checks here
         * for the asymmetrical matches. This hard-coded check avoids
         * having to require full normalization of all the bracket code
         * points before checking. It is highly unlikely that additional
         * canonical singletons for bracket pairs will be added to future
         * versions of the UCD.
         */
        if ((bracketData.bracket == closingcp) ||
          ((bracketData.bracket == 0x232A) && (closingcp == 0x3009)) ||
          ((bracketData.bracket == 0x3009) && (closingcp == 0x232A))) {
          /*
           * This is a match. Construct a pair and append
           * it to the pair list.
           */
          list.push_back({bracketData.pos, pos});
          stack.pop(depth);
          return true;
        }

        depth++;
      }

      /* Not reached, but return no match */
      return false;
   }

  private:
    UBAVersion mVersion;
    std::size_t mMaximumDepth;
  };

}}
