#include <Function.h>

#include <object/ObjectArray.h>
#include <object/ObjectNumber.h>
#include <object/ObjectInteger.h>
#include <object/ObjectFunctionType0.h>
#include <object/ObjectFunctionType2.h>
#include <object/ObjectFunctionType3.h>
#include <object/ObjectFunctionType4.h>

#include <ToString.h>
#include <FunctionImpl.h>
#include <UserObjectManager.h>

namespace kernel{
  typedef UserObjectManager<pdfout::impl::FunctionImpl> FunctionManager;
}

namespace{
  char const *streamSimpleDot = "{ dup mul exch dup mul add 1 exch sub }";
  char const *streamInvertedSimpleDot = "{ dup mul exch dup mul add 1 sub }";
  char const *streamDoubleDot = "{ 360 mul sin 2 div exch 360 mul sin 2 div add }";
  char const *streamInvertedDoubleDot = "{ 360 mul sin 2 div exch 360 mul sin 2 div add neg }";
  char const *streamCosineDot = "{ 180 mul cos exch 180 mul cos add 2 div }";
  char const *streamDouble = "{ 360 mul sin 2 div exch 2 div 360 mul sin 2 div add }";
  char const *streamInvertedDouble = "{ 360 mul sin 2 div exch 2 div 360 mul sin 2 div add neg }";
  char const *streamLine = "{ exch pop abs neg }";
  char const *streamLineX = "{ pop }";
  char const *streamLineY = "{ exch pop }";

  char const *streamRound = "{ abs exch abs\r\n"
                            "  2 copy add 1 le\r\n"
                            "        { dup mul exch dup mul add 1 exch sub }\r\n"
                            "        { 1 sub dup mul exch 1 sub dup mul add 1 sub }\r\n"
                            "    ifelse }";

  char const *streamEllipse = "{ abs exch abs 2 copy 3 mul exch 4 mul add 3 sub dup 0 lt\r\n"
                              "        { pop dup mul exch 0.75 div dup mul add\r\n"
                              "          4 div 1 exch sub }\r\n"
                              "        { dup 1 gt\r\n"
                              "                { pop 1 exch sub dup mul\r\n"
                              "                     exch 1 exch sub 0.75 div dup mul add\r\n"
                              "                     4 div 1 sub }\r\n"
                              "                { 0.5 exch sub exch pop exch pop }\r\n"
                              "            ifelse }\r\n"
                              "    ifelse }";

  char const *streamEllipseA = "{ dup mul 0.9 mul exch dup mul add 1 exch sub }";
  char const *streamInvertedEllipseA = "{ dup mul 0.9 mul exch dup mul add 1 sub }";

  char const *streamEllipseB = "{ dup 5 mul 8 div mul exch dup mul exch add sqrt\r\n"
                               "  1 exch sub }";

  char const *streamEllipseC = "{ dup mul exch dup mul 0.9 mul add 1 exch sub }";
  char const *streamInvertedEllipseC = "{ dup mul exch dup mul 0.9 mul add 1 sub }";

  char const *streamSquare = "{ abs exch abs 2 copy lt\r\n"
                             "        { exch }\r\n"
                             "    if\r\n"
                             "  pop neg }";

  char const *streamCross = "{ abs exch abs 2 copy gt\r\n"
                            "        { exch }\r\n"
                            "    if\r\n"
                            "  pop neg }";

  char const *streamRhomboid = "{ abs exch abs 0.9 mul add 2 div }";

  char const *streamDiamond = "{ abs exch abs 2 copy add 0.75 le\r\n"
                              "        { dup mul exch dup mul add 1 exch sub }\r\n"
                              "        { 2 copy add 1.23 le\r\n"
                              "                { 0.85 mul add 1 exch sub }\r\n"
                              "                { 1 sub dup mul exch 1 sub dup mul add 1 sub }\r\n"
                              "            ifelse }\r\n"
                              "    ifelse }";

  char const *getSpotFunctionBody(pdfout::SpotFunction spotFunction);
}

using namespace kernel;
using namespace pdfout::impl;

namespace pdfout{

  Function *Function::createFunctionType0(std::size_t m, std::size_t n, float *domain, float *range, uint32_t *size, BitsPerSample bps, Interploation order, float *encode, float *decode, InputStream *stream){
    std::unique_ptr<ObjectFunctionType0> func = std::make_unique<ObjectFunctionType0>(stream);
    func->addKey(FunctionType0StreamKeyDomain, std::make_unique<ObjectArray>(domain, m * 2, IndirectTypeNonIndirectable));
    func->addKey(FunctionType0StreamKeyRange, std::make_unique<ObjectArray>(range, n * 2, IndirectTypeNonIndirectable));
    func->addKey(FunctionType0StreamKeySize, std::make_unique<ObjectArray>(size, m, IndirectTypeNonIndirectable));
    func->addKey(FunctionType0StreamKeyBitsPerSample, std::make_unique<ObjectInteger>(bps));
    func->addKey(FunctionType0StreamKeyOrder, std::make_unique<ObjectInteger>(order));

    if (encode)
      func->addKey(FunctionType0StreamKeyEncode, std::make_unique<ObjectArray>(encode, m * 2, IndirectTypeNonIndirectable));

    if(decode)
      func->addKey(FunctionType0StreamKeyDecode, std::make_unique<ObjectArray>(decode, n* 2, IndirectTypeNonIndirectable));

    return FunctionManager::getInstance().insert(std::unique_ptr<FunctionImpl>(new FunctionImpl(func.release())));
  }

  Function *Function::createFunctionType2(std::size_t m, std::size_t n, float *domain, float *range, float N, float *C0, float *C1){
    std::unique_ptr<ObjectFunctionType2> func = std::make_unique<ObjectFunctionType2>();

    func->addKey(FunctionType2DictionaryKeyDomain, std::make_unique<ObjectArray>(domain, m * 2, IndirectTypeNonIndirectable));

    if (range)
      func->addKey(FunctionType2DictionaryKeyRange, std::make_unique<ObjectArray>(range, n * 2, IndirectTypeNonIndirectable));

    func->addKey(FunctionType2DictionaryKeyN, std::make_unique<ObjectNumber>(N));

    if (C0)
      func->addKey(FunctionType2DictionaryKeyC0, std::make_unique<ObjectArray>(C0, n, IndirectTypeNonIndirectable));

    if (C1)
      func->addKey(FunctionType2DictionaryKeyC1, std::make_unique<ObjectArray>(C1, n, IndirectTypeNonIndirectable));

    return FunctionManager::getInstance().insert(std::unique_ptr<FunctionImpl>(new FunctionImpl(func.release())));
  }

  Function *Function::createFunctionType3(std::size_t m, std::size_t n, std::size_t k, float *domain, float *range, Function **functions, float *bounds, float *encode){
    std::unique_ptr<ObjectFunctionType3> func = std::make_unique<ObjectFunctionType3>();

    func->addKey(FunctionType3DictionaryKeyDomain, std::make_unique<ObjectArray>(domain, m * 2, IndirectTypeNonIndirectable));

    if (range)
      func->addKey(FunctionType3DictionaryKeyRange, std::make_unique<ObjectArray>(range, n * 2, IndirectTypeNonIndirectable));

    std::unique_ptr<ObjectArray> funcArray(std::make_unique<ObjectArray>(IndirectTypeIndirectable));
    for (std::size_t i = 0; i < k; ++i)
      funcArray->insert(std::unique_ptr<Object>(((FunctionImpl *) functions[i])->getObject()->clone()));

    func->addKey(FunctionType3DictionaryKeyFunctions, std::move(funcArray));
    func->addKey(FunctionType3DictionaryKeyBounds, std::make_unique<ObjectArray>(bounds, k - 1, IndirectTypeNonIndirectable));
    func->addKey(FunctionType3DictionaryKeyEncode, std::make_unique<ObjectArray>(encode, k * 2, IndirectTypeNonIndirectable));

    return FunctionManager::getInstance().insert(std::unique_ptr<FunctionImpl>(new FunctionImpl(func.release())));
  }

  Function *Function::createFunctionType4(std::size_t m, std::size_t n, float *domain, float *range, InputStream *stream){
    std::unique_ptr<ObjectFunctionType4> func = std::make_unique<ObjectFunctionType4>(stream);

    func->addKey(FunctionType4StreamKeyDomain, std::make_unique<ObjectArray>(domain, m * 2, IndirectTypeNonIndirectable));
    func->addKey(FunctionType4StreamKeyRange, std::make_unique<ObjectArray>(range, n * 2, IndirectTypeNonIndirectable));

    return FunctionManager::getInstance().insert(std::unique_ptr<FunctionImpl>(new FunctionImpl(func.release())));
  }

  Function *Function::createFunctionSpot(SpotFunction spotFunction){
    std::unique_ptr<ObjectName> func = std::make_unique<ObjectName>(toString(spotFunction));
    return FunctionManager::getInstance().insert(std::unique_ptr<FunctionImpl>(new FunctionImpl(func.release())));
  }

  Function *Function::createFunctionIdentity(void){
    std::unique_ptr<ObjectName> func = std::make_unique<ObjectName>("Identity");
    return FunctionManager::getInstance().insert(std::unique_ptr<FunctionImpl>(new FunctionImpl(func.release())));
  }
}

namespace {

  using namespace pdfout;

  char const *getSpotFunctionBody(pdfout::SpotFunction spotFunction){
    switch (spotFunction){
      case SpotFunctionSimpleDot: return streamSimpleDot;
      case SpotFunctionInvertedSimpleDot: return streamInvertedSimpleDot;
      case SpotFunctionDoubleDot: return streamDoubleDot;
      case SpotFunctionInvertedDoubleDot: return streamInvertedDoubleDot;
      case SpotFunctionCosineDot: return streamCosineDot;
      case SpotFunctionDouble: return streamDouble;
      case SpotFunctionInvertedDouble: return streamInvertedDouble;
      case SpotFunctionLine: return streamLine;
      case SpotFunctionLineX: return streamLineX;
      case SpotFunctionLineY: return streamLineY;
      case SpotFunctionRound: return streamRound;
      case SpotFunctionEllipse: return streamEllipse;
      case SpotFunctionEllipseA: return streamEllipseA;
      case SpotFunctionInvertedEllipseA: return streamInvertedEllipseA;
      case SpotFunctionEllipseB: return streamEllipseB;
      case SpotFunctionEllipseC: return streamEllipseC;
      case SpotFunctionInvertedEllipseC: return streamInvertedEllipseC;
      case SpotFunctionSquare: return streamSquare;
      case SpotFunctionCross: return streamCross;
      case SpotFunctionRhomboid: return streamRhomboid;
      case SpotFunctionDiamond: return streamDiamond;
    }

    RUNTIME_EXCEPTION("Invalid SpotFunction value");
    return "";
  }
}

