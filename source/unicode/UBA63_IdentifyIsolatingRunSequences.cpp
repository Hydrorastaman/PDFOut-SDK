#include "UBA63.h"

namespace pdfout{ namespace unicode{

  /*
   * br_UBA_IdentifyIsolatingRunSequences
   *
   * This function applies only to UBA63. Once the embedding
   * levels are identified, UBA63 requires further processing
   * to assign each of the level runs to an isolating run sequence.
   *
   * Each level run must be uniquely assigned to exactly one
   * isolating run sequence. Each isolating run sequence must
   * have at least one level run, but may have more.
   *
   * The exact details on how to match up isolating run sequences
   * with level runs are specified in BD13.
   *
   * The strategy taken here is to scan the level runs in order.
   *
   * If a level run is not yet assigned to an isolating run sequence,
   * its seqID will be zero. Create a new isolating run sequence
   * and add this level run to it.
   *
   * If the last BIDIUNIT of *this* level run is an isolate
   * initiator (LRI/RLI/FSI), then scan ahead in the list of
   * level runs seeking the next level run which meets the
   * following criteria:
   *   1. seqID = 0 (not yet assigned to an isolating run sequence)
   *   2. its level matches the level we are processing
   *   3. the first BIDIUNIT is a PDI
   * If all those conditions are met, assign that next level run
   * to this isolating run sequence (set its seqID, and append to
   * the list).
   *
   * Repeat until we hit a level run that doesn't terminate with
   * an isolate initiator or we hit the end of the list of level
   * runs.
   *
   * That terminates the definition of the isolating run sequence
   * we are working on. Append it to the list of isolating run
   * sequences in the UBACONTEXT.
   *
   * Then advance to the next level run which has not yet been
   * assigned to an isolating run sequence and repeat the process.
   *
   * Continue until all level runs have been assigned to an
   * isolating run sequence.
   */
  bool UBA63::br_UBA63_IdentifyIsolatingRunSequences(UBACONTEXT *ctxt) const {
    ctxt->dirtyBit = 0;

    int seqid = 0;
    int savelevel = -1;
    BIDIRUN *brp = ctxt->theRuns;

    while (brp != nullptr) {
      /*
       * Skip past any run which already has a seqID assigned
       * to it. Only process runs with seqID == 0.
       */
      if (brp->seqID == 0) {
        seqid++;

        IRSEQ *irp = br_ConstructIsolatingRunSequence(seqid, brp);
        if (irp == nullptr) {
          return false;
        }

        br_AppendIsolatingRunSequence(ctxt, irp);
        brp->seqID = seqid;

        /*
         * Next check whether this run ends in an isolate initiator.
         * If so, scan ahead looking for the run with the matching PDI.
         */
        if (brp->last->bc_isoinit) {
          /*
           * Use a temporary brp2 run pointer for this scan, so
           * the outer loop resumes correctly from where it left
           * off in the main scan through the runs.
           */
          BIDIRUN *brp2 = brp;
          savelevel = brp->level;

          while (brp2->next != nullptr) {
            brp2 = brp2->next;
            if ((brp2->seqID == 0) && (brp2->level == savelevel) && (br_FirstSignificantBC(brp2) == BidiCategory::PDI)) {
              /*
               * We matched the criteria for adding this run to the
               * sequence. Construct a BIDIRUNLISTELEMENT and append
               * it to the sequence.
               */
              BIDIRUNLIST *brlp = br_ConstructBidiRunListElement();
              if (brlp == nullptr) {
                /* Let the br_DropContext do the cleanup. Less messy. */
                return false;
              }

              /* Set the seq ID of the run to the seq ID of this sequence. */
              brp2->seqID = seqid;
              brlp->run = brp2;

              /* Append it to the seqeunce. */
              br_AppendBidiRunToSequence(irp, brlp);

              /*
               * Check is the last unit in *this* run is also
               * an isolate initiator. If not, we are done with
               * this sequence. If so, come around and scan for
               * another run with a matching PDI.
               */
              if (!(brp2->last->bc_isoinit)) {
                break;
              }
            }
          }
        }
      }

      /* Advance to the next run */
      brp = brp->next;
    }

    /*
     * Add text chains for each sequence, for uniform rule application.
     */
    if (!br_UBA_AddTextChainsForSequences(ctxt->theSequences))
      return false;

    /*
     * Now that we have identified the sequences, calculate the sos and eos
     * values for each sequence.
     */
    br_UBA_CalculateSosEos(ctxt);

    /*
     * This rule always has an impact, so just always set the dirtyBit
     * on exit.
     */
    ctxt->dirtyBit = 1;
    ctxt->state = AlgorithmState::X10Done;

    return true;
  }
}}
