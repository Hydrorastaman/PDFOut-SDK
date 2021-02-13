#pragma once

#include <stdint.h>
#include "UnicodeData.h"
#include "BidiBrackets.h"

namespace pdfout{ namespace unicode{

  /*
   * ALGORITHM_STATE is used to store how far along in
   * algorithm the data has been processed. This can be
   * used to help the debug display to show relevant
   * information.
   */
  enum class AlgorithmState {
    Unitialized, /* context allocated, but no data */
    Initialized, /* test case data read in and parsed */
    P3Done,      /* rules done through P3 */
    X8Done,      /* rules done through X8 */
    X9Done,      /* rules done through X9 */
    RunsDone,    /* rules done through X10 part 1: runs are identified */
    X10Done,     /* rules done through X10: runs & seqs are identified */
    W1Done,      /* rules done through W1:  combining marks are resolved */
    W2Done,      /* rules done through W2: */
    W3Done,      /* rules done through W3: */
    W4Done,      /* rules done through W4: */
    W5Done,      /* rules done through W5: */
    W6Done,      /* rules done through W6: */
    W7Done,      /* rules done through W7:  weak types are resolved */
    N0Done,      /* rules done through N0:  bracket pairs are resolved */
    N1Done,      /* rules done through N1: */
    N2Done,      /* rules done through N2:  neutral types are resolved */
    I2Done,      /* rules done through I2:  implicit levels are resolved */
    L1Done,      /* rules done through L1:  trailing whitespace resolved */
    L2Done,      /* rules done through L2:  reordering data available */
    Complete     /* finished application of rules: ready for checking */
  };

  enum class ParagraphDirection {
    LTR,
    RTL,
    Auto,
    Unknown
  };

  /*
   * BIDI_RULE_TYPE
   *
   * An enumerated type which facilitates lookup of
   * rule callbacks and other data by rule type.
   */
  enum class BidiRuleType {
    W1,
    W2,
    W3,
    W4,
    W5,
    W6,
    W7,
    N0,
    N1,
    N2
  };


  /*
   * BIDIUNIT
   *
   * This struct is the primitive manipulated by
   * the UBA reference implementation. It is used
   * to construct a vector in the UBACONTEXT,
   * containing the character data (stored as UTF-32),
   * the looked-up property data, so that the UBA,
   * which works primarily on the basis of the Bidi_Class
   * values, has the correct input, along with the
   * level and order information.
   *
   * Starting with Version 2.0 of bidiref, a number of
   * accelerator flags are added, to speed up checking of
   * common combinations of Bidi_Class values. Each flag
   * is conceptually a Boolean, and could be stored as a bit
   * in a bitfield, but in this reference implementation
   * is simply stored as another int field in the BIDIUNIT.
   * These values are all initialized when the BIDIUNIT
   * vector is initialized for a testcase string. Some need
   * to be reset, whenever a rule changes the current bc
   * value for a character.
   */
  struct BIDIUNIT {
    uint32_t c;           /* character value, stored as UTF-32 */
    int order;    /* store position for reordering */
    BidiCategory bc;      /* current Bidi_Class value */
    BidiCategory orig_bc; /* store original value for Bidi_Class */
    int bc_numeric;   /* accelerator for bc = (AN or EN) */
    int bc_isoinit;   /* accelerator for bc = (LRI or RLI or FSI) */
    uint32_t  bpb;    /* bidi paired bracket property */
    ParenthesisDirection bpt;      /* bidi paired bracket type property */
    int level;    /* current embedding level */
    int expLevel; /* store expected level for test case */

    static int const NOLEVEL = -1;
  };


  /*
   * BIDIRUN
   *
   * Once embedding levels are determined, the UBA
   * treats each contiguous sequence at the same level
   * as a distinct run.
   *
   * To simplify the processing of runs, a list of runs
   * is constructed as a linked list, which hangs off
   * the UBACONTEXT.
   *
   * Each BIDIRUN consists of pointers to the first
   * and last BIDIUNIT of the run, and then additional
   * information calculated during the X10 rule, when
   * the runs are identified.
   *
   * This may not be the most efficient approach to
   * an implementation, but it makes it much easier
   * to express the X10 rule and subsequent rules which
   * process the runs individually.
   *
   * The seqID is only used in processing run lists
   * into isolating run sequence lists in UBA63.
   * It is initialized to zero. During processing
   * to identify isolating run sequences, it is set
   * to the sequence id of the isolating run sequence
   * that a run is assigned to.
   */
  struct BIDIRUN {
    struct BIDIRUN *next; /* next run */
    BIDIUNIT *first;  /* first element of this run */
    BIDIUNIT *last;   /* last element of this run */
    int len;          /* explicit len, to simplify processing */
    int runID;        /* stamp run with id for debugging */
    int seqID;        /* isolating run sequence id */
    int level;        /* embedding level of this run */
    BidiCategory sor; /* direction of start of run: L or R */
    BidiCategory eor; /* direction of end of run: L or R */
    BIDIUNIT **textChain;  /* constructed text chain */
  };

  /*
   * The BidiRunListStruct abstracts the creation of
   * a list of bidi runs for attachment to the isolating
   * run lists. Instead of duplicating all the run information
   * into that list, the list consists just of pointers to
   * the already allocated basic list of runs.
   */
  struct BIDIRUNLIST {
    BIDIRUNLIST *next;
    BIDIRUN *run;
  };

  /*
   * ISOLATING_RUN_SEQUENCE
   *
   * This is a concept introduced in UBA63.
   *
   * Essentially it consists of an ordered sequence of
   * bidi runs, as defined in BD13.
   *
   * It is implemented here by attaching the list of runs
   * associated with this particular isolating run sequence.
   * The attached list just contains pointers to each of
   * the relevant BIDIRUN structs in the already constructed
   * sequential list of runs attached to the UBACONTEXT.
   *
   * All runs associated with a single isolating run sequence
   * are at the same level, so that level can be stored
   * in the isolating run sequence struct for ease of access.
   *
   * Each isolating run sequence has a start of sequence (sos)
   * and end of sequence (eos) directional value assigned.
   * These are calculated based on the sor and eor values
   * for the associated list of runs, but again, are stored
   * in the isolating run sequence struct for ease of access.
   */
  struct IRSEQ {
    struct IRSEQ *next;   /* next sequence */
    BIDIRUNLIST *theRuns; /* list of runs in this sequence */
    BIDIRUNLIST *lastRun; /* for list appending */
    int len;              /* explicit len, to simplify processing */
    int seqID;            /* stamp seq with id for debugging */
    int level;            /* embedding level of this seq */
    BidiCategory sos;     /* direction of start of seq: L or R */
    BidiCategory eos;     /* directino of end of seq: L or R */
    BIDIUNIT **textChain; /* constructed text chain */
  };

  /*
   * UBACONTEXT
   *
   * This struct is used to store all context associated
   * with the bidi reference UBA processing, including
   * input, expected test output,
   * and the constructed runs and other intermediate data.
   *
   * theText, testLen, paragraphDirection are input.
   * theRuns and paragraphEmbeddingLevel are calculated.
   * expEmbeddingLevel, expOrder are parsed
   *    from the testcase data and checked against
   *    calculated values.
   * For simplicity, the expected levels data parsed
   * from the test case are stored with theText.
   *
   * Starting from Version 2.0, theText pointer is
   * allocated statically, simply pointing to a static
   * buffer of BR_MAXINPUTLEN length, to cut down on
   * repeated dynamic allocations during long testcase
   * runs.
   */
  struct UBACONTEXT {
    AlgorithmState state; /* track state */
    int dirtyBit;         /* used for debug output control */
    int64_t testId;       /* 64-bit id used for tagging trace output */
    ParagraphDirection paragraphDirection; /* input */
    int paragraphEmbeddingLevel;           /* calculated */
    int textLen;                           /* input */
    std::vector<BIDIUNIT> theText;         /* input */
    BIDIRUN *theRuns;      /* calculated */
    BIDIRUN *lastRun;      /* for list appending */
    IRSEQ *theSequences;   /* calculated: UBA63 only */
    IRSEQ *lastSequence;   /* for list appending */
    int expEmbeddingLevel; /* expected test result */
    char *expOrder;        /* expected test result */
  };

  /*
   * RULE_CONTEXT
   *
   * A struct used to package up parameter information
   * for a class of rules, to make parameter passing
   * and function prototype neater.
   */
  struct BIDIRULECONTEXT {
    BIDIUNIT **textChain;
    int len;
    int level;
    BidiCategory sot;
    BidiCategory eot;
  };
}}
