#pragma once

#include <cstddef>
#include "PdfOutConfig.h"

namespace pdfout{

  enum BitsPerSample{
    BitsPerSample1 = 1,
    BitsPerSample2 = 2,
    BitsPerSample4 = 4,
    BitsPerSample8 = 8,
    BitsPerSample12 = 12,
    BitsPerSample16 = 16,
    BitsPerSample24 = 24,
    BitsPerSample32 = 32
  };

  enum Interploation{
    InterpolationLinear = 1,
    InterpolationCubic = 3
  };

  enum SpotFunction{
    SpotFunctionSimpleDot,
    SpotFunctionInvertedSimpleDot,
    SpotFunctionDoubleDot,
    SpotFunctionInvertedDoubleDot,
    SpotFunctionCosineDot,
    SpotFunctionDouble,
    SpotFunctionInvertedDouble,
    SpotFunctionLine,
    SpotFunctionLineX,
    SpotFunctionLineY,
    SpotFunctionRound,
    SpotFunctionEllipse,
    SpotFunctionEllipseA,
    SpotFunctionInvertedEllipseA,
    SpotFunctionEllipseB,
    SpotFunctionEllipseC,
    SpotFunctionInvertedEllipseC,
    SpotFunctionSquare,
    SpotFunctionCross,
    SpotFunctionRhomboid,
    SpotFunctionDiamond
  };

  class PDFOUT_EXPORTS InputStream;
  class PDFOUT_EXPORTS OutputStream;

  /**
   * @brief Function object
   */
  class PDFOUT_EXPORTS Function{
  protected:
    Function(void) {}

  public:
    virtual ~Function(void) {}

    /**
      * Creates Type0 (Sampled) function.
      * @param [in, required] m
      * @param [in, required] n
      * @param [in, required] domain An array of 2 * m numbers, where m is the number of input values.
      * @param [in, required] range An array of 2 * n numbers, where n is the number of output values.
      * @param [in, required] size An array of m positive integers specifying the number of samples in each input dimension of the sample table.
      * @param [in, required] bps The number of bits used to represent each sample.
      * @param [in, optional] order The order of interpolation between samples.
      * @param [in, optional] encode An array of 2 * m numbers specifying the linear mapping of input values into the domain of the function's sample table.
      * @param [in, optional] decode An array of 2 * n numbers specifying the linear mapping of sample values into the range appropriate for the function's output values.
      * @param [in, optional] stream The sample values.
      */
    static Function *createFunctionType0(std::size_t m, std::size_t n, float *domain, float *range, uint32_t *size, BitsPerSample bps, Interploation order, float *encode, float *decode, InputStream *stream);

    /**
      * Creates Type2 (Exponential Interpolation) function.
      * @param [in, required] m
      * @param [in, required] n
      * @param [in, required] domain An array of 2 * m numbers, where m is the number of input values.
      * @param [in, optional] range An array of 2 * n numbers, where n is the number of output values.
      * @param [in, required] N The interpolation exponent.
      * @param [in, optional] C0 An array of n numbers defining the function result when x = 0.0.
      * @param [in, optional] C1 An array of n numbers defining the function result when x = 1.0.
      */
    static Function *createFunctionType2(std::size_t m, std::size_t n, float *domain, float *range, float N, float *C0, float *C1);

    /**
      * Creates Type3 (Stitching) function.
      * @param [in, required] m
      * @param [in, required] n
      * @param [in, required] k
      * @param [in, required] domain An array of 2 * m numbers, where m is the number of input values.
      * @param [in, optional] range An array of 2 * n numbers, where n is the number of output values.
      * @param [in, required] functions An array of k 1-input functions making up the stitching function.
      * @param [in, required] bounds An array of k - 1 numbers that, in combination with Domain, define the intervals to which each function from the Functions array applies.
      * @param [in, required] encode An array of 2 * k numbers that, taken in pairs, map each subset of the domain defined by Domain and the Bounds array to the domain of the corresponding function.
      */
    static Function *createFunctionType3(std::size_t m, std::size_t n, std::size_t k, float *domain, float *range, Function **functions, float *bounds, float *encode);

    /**
      * Creates Type4 (PostScript Calculator) function
      * @param [in, required] m
      * @param [in, required] n
      * @param [in, required] domain An array of 2 * m numbers, where m is the number of input values.
      * @param [in, required] range An array of 2 * n numbers, where n is the number of output values.
      * @param [in, optional] stream The sample values.
      */
    static Function *createFunctionType4(std::size_t m, std::size_t n, float *domain, float *range, InputStream *stream);

    /**
      * Creates a predefined Spot function
      * @param [in, required] spotFunction spot function type.
      */
    static Function *createFunctionSpot(SpotFunction spotFunction);

    /**
      * Creates an identity function
      */
    static Function *createFunctionIdentity(void);
  };
}
