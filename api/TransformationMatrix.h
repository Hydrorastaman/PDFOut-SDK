#pragma once

#include "PdfOutConfig.h"

namespace pdfout{

  /**
   * @brief Transformation matrix structure
   * {e11, e12, 0}
   * {e21, e22, 0}
   * {e31, e32, 1}
   * In PDF matrix multiplication is left-multiply, A by B means BA.
   */
  struct PDFOUT_EXPORTS TransformationMatrix{
    /**
     * Creates identity transformation matrix
     */
    TransformationMatrix(void)
      : mA(1.0f), mB(0.0f), mC(0.0f), mD(1.0f), mE(0.0f), mF(0.0f){
    }

    /**
     * Creates transfromation matrix
     */
    TransformationMatrix(float a, float b, float c, float d, float e, float f)
      : mA(a), mB(b), mC(c), mD(d), mE(e), mF(f){
    }

    /**
     * Creates transfromation matrix from an array of float, size must be greater or equal to 6
     */
    TransformationMatrix(float const *data)
      : mA(data[0]), mB(data[1]), mC(data[2]), mD(data[3]), mE(data[4]), mF(data[5]){
    }

    /**
     * Translation to position (x, y)
     */
    static TransformationMatrix createTranslationMatrix(float x, float y);

    /**
     * Scaling current matrix
     */
    static TransformationMatrix createScalingMatrix(float scaleX, float scaleY);

    /**
     * Rotation to angle radiant about point (x, y)
     */
    static TransformationMatrix createRotationMatrix(float angle, float x = 0.f, float y = 0.f);

    /**
     * Skewing to a and b radiant
     */
    static TransformationMatrix createSkewingMatrix(float a, float b);

    /**
     * Inverse matrix
     */
    TransformationMatrix inverse(void) const;

    /**
     * Returns determinant of the matrix
     */
    float getDeterminant(void) const;

    /**
     * Result is rhs * this
     */
    TransformationMatrix &operator*=(TransformationMatrix const &rhs);

    float mA;
    float mB;
    float mC;
    float mD;
    float mE;
    float mF;
  };

  /**
   * Result is rhs * lhs
   */
  PDFOUT_EXPORTS TransformationMatrix operator*(TransformationMatrix const &lhs, TransformationMatrix const &rhs);

  /**
   * @brief 2D vector structure {x, y, 1}
   */
  struct PDFOUT_EXPORTS Vector{
    Vector(float x, float y) : mX(x), mY(y) {}
    Vector(float *data) : mX(data[0]), mY(data[1]) {}

    Vector &operator*=(TransformationMatrix const &rhs);

    float mX;
    float mY;
  };

  PDFOUT_EXPORTS Vector operator*(Vector const &lhs, TransformationMatrix const &rhs);
}
