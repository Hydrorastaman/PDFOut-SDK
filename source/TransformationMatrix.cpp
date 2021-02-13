#include <TransformationMatrix.h>

#include <cmath>

namespace pdfout{

  TransformationMatrix TransformationMatrix::createTranslationMatrix(float x, float y){
    TransformationMatrix translation(1.f, 0, 0, 1.f, x, y);
    return translation;
  }

  TransformationMatrix TransformationMatrix::createScalingMatrix(float scaleX, float scaleY){
    TransformationMatrix scaling(scaleX, 0, 0, scaleY, 0, 0);
    return scaling;
  }

  TransformationMatrix TransformationMatrix::createRotationMatrix(float angle, float x, float y){
    float const cos = std::cos(angle);
    float const sin = std::sin(angle);
    TransformationMatrix rotation(cos, sin, -sin, cos, x - cos * x + sin * y , y - cos * y - sin * x);
    return rotation;
  }

  TransformationMatrix TransformationMatrix::createSkewingMatrix(float a, float b){
    TransformationMatrix skewing(1.f, std::tan(a), std::tan(b), 1.f, 0, 0);
    return skewing;
  }

  TransformationMatrix TransformationMatrix::inverse(void) const{
    float const D = getDeterminant();
    return TransformationMatrix(mD / D, -mB / D, -mC / D, mA / D, (mC * mF - mE * mD) / D, (mE * mB - mA * mF) / D);
  }

  float TransformationMatrix::getDeterminant(void) const{
    return mA * mD - mC * mB;
  }

  TransformationMatrix &TransformationMatrix::operator*=(TransformationMatrix const &rhs){
    float const a = rhs.mA * mA + rhs.mB * mC;
    float const b = rhs.mA * mB + rhs.mB * mD;
    float const c = rhs.mC * mA + rhs.mD * mC;
    float const d = rhs.mC * mB + rhs.mD * mD;
    float const e = rhs.mE * mA + rhs.mF * mC + mE;
    float const f = rhs.mE * mB + rhs.mF * mD + mF;

    mA = a;
    mB = b;
    mC = c;
    mD = d;
    mE = e;
    mF = f;

    return *this;
  }

  TransformationMatrix operator*(TransformationMatrix const &lhs, TransformationMatrix const &rhs){
    TransformationMatrix result = lhs;
    result *= rhs;
    return result;
  }

  Vector &Vector::operator*=(TransformationMatrix const &rhs){
    float const x = mX * rhs.mA + mY * rhs.mC + rhs.mE;
    float const y = mX * rhs.mB + mY * rhs.mD + rhs.mF;

    mX = x;
    mY = y;

    return *this;
  }

  Vector operator*(Vector const &lhs, TransformationMatrix const &rhs){
    Vector result = lhs;
    result *= rhs;
    return result;
  }
}
