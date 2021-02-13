#pragma once

#include <vector>

namespace pdfout{ namespace unicode{

  template <typename T>
  class UBAStack {
  public:
    UBAStack(std::size_t const maxSize)
      : mStack(maxSize, T()),
      mStackPtr(&mStack[0]),
      mStackTop(&mStack[0]),
      mStackMax(&mStack[maxSize - 1]){
    }

    bool push(T const &value) {
      if (mStackTop < mStackMax) {
        ++mStackTop;
        *mStackTop = value;
        return true;
      }

      return false;
    }

    bool pop(T &value) {
      if (mStackTop > mStackPtr) {
        value = *mStackTop;
        --mStackTop;
        return true;
      }

      return false;
    }

    void pop(std::size_t n) {
      std::size_t const stackDepth = (std::size_t) (mStackTop - mStackPtr);
      if (stackDepth > n)
        mStackTop -= n;
      else
        mStackTop = mStackPtr;
    }

    bool peek(T &value) const {
      if (mStackTop > mStackPtr) {
        value = *mStackTop;
        return true;
      }

      return false;
    }

    bool peek(T &value, std::size_t depth) const {
      auto tsptr = mStackTop - depth + 1;
      if (tsptr > mStackPtr) {
        value = *tsptr;
        return true;
      }

      return false;
    }

    std::size_t size() const {
      return (std::size_t)(mStackTop - mStackPtr);
    }

    std::size_t maxDepth() const {
      return mStack.size();
    }

  private:
    std::vector<T> mStack;
    T *mStackPtr;
    T *mStackTop;
    T *mStackMax;
  };
}}
