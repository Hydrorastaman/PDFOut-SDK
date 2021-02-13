#include <graphics/Operators.h>

#include <InternalTools.h>
#include <object/ObjectArray.h>
#include <object/ObjectStream.h>
#include <object/ObjectUTF16BEString.h>

#include <iomanip>
#include <sstream>

using namespace pdfout;

namespace kernel{

  bool OperatorNx<float>::serializePrefix(kernel::ObjectStream *stream) const{
    uint16_t const operands = getOperands();
    char dest[MAX_FLOAT_LENGTH * 6 + 1] = {0};

    if (operands >= 1)
      floatToString(&dest[strlen(dest)], mOperand0);

    if (operands >= 2)
      floatToString(&dest[strlen(dest)], mOperand1);

    if (operands >= 3)
      floatToString(&dest[strlen(dest)], mOperand2);

    if (operands >= 4)
      floatToString(&dest[strlen(dest)], mOperand3);

    if (operands >= 5)
      floatToString(&dest[strlen(dest)],  mOperand4);

    if (operands >= 6)
      floatToString(&dest[strlen(dest)], mOperand5);

    stream->write(dest, sizeof(uint8_t), strlen(dest));
    return true;
  }

  bool OperatorNx<int32_t>::serializePrefix(kernel::ObjectStream *stream) const{
    uint16_t const operands = getOperands();
    char dest[MAX_ULONG_LENGTH * 6 + 1] = {0};

    if (operands == 1)
      sprintf(dest, "%i", mOperand0);
    else if (operands == 2)
      sprintf(dest, "%i %i", mOperand0, mOperand1);
    else if (operands == 3)
      sprintf(dest, "%i %i %i", mOperand0, mOperand1, mOperand2);
    else if (operands == 4)
      sprintf(dest, "%i %i %i %i", mOperand0, mOperand1, mOperand2, mOperand3);
    else if (operands == 5)
      sprintf(dest, "%i %i %i %i %i", mOperand0, mOperand1, mOperand2, mOperand3, mOperand4);
    else if (operands == 6)
      sprintf(dest, "%i %i %i %i %i %i", mOperand0, mOperand1, mOperand2, mOperand3, mOperand4, mOperand5);

    stream->write(dest, sizeof(uint8_t), strlen(dest));
    return true;
  }

  Operator::Operator(OperatorCode opCode, char const *cmd, uint16_t operands)
    : mOpCode(opCode),
    mCmd(cmd),
    mOperands(operands){
  }

  Operator::~Operator(void){
  }

  OperatorCode Operator::getOperatorCode(void) const{
    return mOpCode;
  }

  uint16_t Operator::getOperands(void) const{
    return mOperands;
  }

  void Operator::serialize(kernel::ObjectStream *stream) const{
    if (serializePrefix(stream))
      stream->write(" ", sizeof(uint8_t), 1);

    stream->write(mCmd.c_str(), sizeof(uint8_t), mCmd.length());
  }

  bool Operator::serializePrefix(kernel::ObjectStream *stream) const{
    return false;
  }

  OperatorD::OperatorD(OperatorCode opCode, char const *cmd, float const *dashPattern, uint32_t count, uint32_t phase)
    : Operator(opCode, cmd, count + 1),
    mDashPattern(dashPattern, dashPattern + count),
    mPhase(phase){
  }

  OperatorD::~OperatorD(void) {}

  bool OperatorD::serializePrefix(kernel::ObjectStream *stream) const{
    uint16_t const operands = getOperands();
    std::make_unique<ObjectArray>(mDashPattern, IndirectTypeNonIndirectable)->serialize(stream->getStream(), nullptr);
    std::make_unique<ObjectInteger>(mPhase)->serialize(stream->getStream(), nullptr);

    return true;
  }

  OperatorF::OperatorF(OperatorCode opCode, char const *cmd, char const *fontName, int32_t fontSize)
    : Operator(opCode, cmd, 2),
    mFontSize(fontSize),
    mFontName(fontName){
  }

  OperatorF::~OperatorF(void){
  }

  bool OperatorF::serializePrefix(kernel::ObjectStream *stream) const{
    std::make_unique<ObjectName>(mFontName)->serialize(stream->getStream(), nullptr);
    stream->write(" ");
    std::make_unique<ObjectInteger>(mFontSize)->serialize(stream->getStream(), nullptr);
    return true;
  }

  OperatorT::OperatorT(OperatorCode opCode, char const *cmd, std::unique_ptr<kernel::ObjectUTF16BEString> text)
    : Operator(opCode, cmd, 1),
    mWordSpacing(0.f),
    mCharSpacing(0.f),
    mString(std::move(text)),
    mPositions(){
  }

  OperatorT::OperatorT(OperatorCode opCode, char const *cmd, std::unique_ptr<kernel::ObjectUTF16BEString> text, std::vector<int32_t> const &positions)
    : Operator(opCode, cmd, 1),
    mWordSpacing(0.f),
    mCharSpacing(0.f),
    mString(std::move(text)),
    mPositions(positions){
  }

  OperatorT::OperatorT(OperatorCode opCode, char const *cmd, float wordSpacing, float charSpacing, std::unique_ptr<kernel::ObjectUTF16BEString> text)
    : Operator(opCode, cmd, 3),
    mWordSpacing(wordSpacing),
    mCharSpacing(charSpacing),
    mString(std::move(text)),
    mPositions(){
  }

  OperatorT::~OperatorT(void){
  }

  bool OperatorT::serializePrefix(kernel::ObjectStream *stream) const{
    Endian const endian = getEndian();
    OperatorCode code = getOperatorCode();
    switch (code){
      case OperatorCode_dquote:{
        std::make_unique<ObjectNumber>(mWordSpacing)->serialize(stream->getStream(), nullptr);
        stream->write(" ");
        std::make_unique<ObjectNumber>(mCharSpacing)->serialize(stream->getStream(), nullptr);
        stream->write(" ");
        mString->serialize(stream->getStream(), nullptr);
        break;
      }

      case OperatorCode_TJ:{
        uint16_t const *source = (uint16_t *) mString->getData();
        std::size_t const textSize = mString->getSize();
        std::size_t const posSize = mPositions.size();

        std::stringstream ss;
        ss << "[";
        for (std::size_t i = 0; i < textSize; ++i){
          uint16_t value = source[i];
          if (endian == EndianLittle){
            swapByteOrder(value);
            ss << "<" << std::setfill('0') << std::setw(4) << std::hex << (int32_t) (value) << "> ";
          }
          else
            ss << "<" << std::setfill('0') << std::setw(4) << std::hex << (int32_t) (value) << "> ";

          if (i < posSize)
             ss << std::to_string(mPositions[i]) << " ";
        }
        ss << "]";
        std::string result = ss.str();
        stream->write(result.data(), sizeof(uint8_t), result.length());
        break;
      }

      default:
        mString->serialize(stream->getStream(), nullptr);
    }

    return true;
  }

  OperatorC::OperatorC(OperatorCode opCode, char const *cmd, char const *name)
    : Operator(opCode, cmd, 1),
    mName(name, name + strlen(name)),
    mColorants(){
  }

  OperatorC::OperatorC(OperatorCode opCode, char const *cmd, float *colorants, uint32_t count)
    : Operator(opCode, cmd, count),
    mName(),
    mColorants(colorants, colorants + count){
  }

  OperatorC::OperatorC(OperatorCode opCode, char const *cmd, float *colorants, uint32_t count, char const *name)
    : Operator(opCode, cmd, count + 1),
    mName(name, name + strlen(name)),
    mColorants(colorants, colorants + count){
  }

  OperatorC::~OperatorC(void){
  }

  bool OperatorC::serializePrefix(kernel::ObjectStream *stream) const{
    if (!mColorants.empty()){
      for (std::size_t i = 0, size = mColorants.size(); i < size; ++i){
        std::make_unique<ObjectNumber>(mColorants[i])->serialize(stream->getStream(), nullptr);
        stream->write(" ");
      }
    }

    if (!mName.empty())
      std::make_unique<ObjectName>(mName)->serialize(stream->getStream(), nullptr);

    return true;
  }

  OperatorM::OperatorM(OperatorCode opCode, char const *cmd, char const *name)
    : Operator(opCode, cmd, 1),
    mTag(name, name + strlen(name)),
    mProperties(){
  }

  OperatorM::OperatorM(OperatorCode opCode, char const *cmd, char const *name, void *properties, uint32_t size)
    : Operator(opCode, cmd, 2),
    mTag(name,  name + strlen(name)),
    mProperties((uint8_t *) properties, (uint8_t *) properties + size){
  }

  OperatorM::OperatorM(OperatorCode opCode, char const *cmd, char const *name, char const *properties)
    : Operator(opCode, cmd, 2),
    mTag(name,  name + strlen(name)),
    mProperties(properties, properties + strlen(properties)){
  }

  OperatorM::~OperatorM(void){
  }

  bool OperatorM::serializePrefix(kernel::ObjectStream *stream) const{
    stream->write(mTag.data(), sizeof(uint8_t), mTag.size());
    if (!mProperties.empty()){
      stream->write(" ", sizeof(uint8_t), 1);
      stream->write(mProperties.data(), sizeof(uint8_t), mProperties.size());
    }

    return true;
  }

  OperatorS::OperatorS(OperatorCode opCode, char const *cmd, char const *name)
    : Operator(opCode, cmd, 1),
    mName(name, name + strlen(name)){
  }

  OperatorS::~OperatorS(void){
  }

  bool OperatorS::serializePrefix(kernel::ObjectStream *stream) const{
    stream->write("/", sizeof(uint8_t), 1);
    stream->write(mName.c_str(), sizeof(uint8_t), mName.length());
    return true;
  }

  Operator *makeOperator_w(float lineWidth) {return new OperatorNf(OperatorCode_w, "w", lineWidth);}
  Operator *makeOperator_J(int32_t lineCap) {return new OperatorNi(OperatorCode_J, "J", lineCap);}
  Operator *makeOperator_j(int32_t lineJoin) {return new OperatorNi(OperatorCode_j, "j", lineJoin);}
  Operator *makeOperator_M(float miterLimit) {return new OperatorNf(OperatorCode_M, "M", miterLimit);}
  Operator *makeOperator_d(float const *dashPattern, uint32_t count, uint32_t phase) {return new OperatorD(OperatorCode_d, "d", dashPattern, count, phase);}
  Operator *makeOperator_ri(char const *ri) {return new OperatorS(OperatorCode_ri, "ri", ri);}
  Operator *makeOperator_i(uint32_t flatness) {return new OperatorNi(OperatorCode_i, "i", flatness);}
  Operator *makeOperator_gs(char const *extGState) {return new OperatorS(OperatorCode_gs, "gs", extGState);}

  Operator *makeOperator_q(void) {return new Operator(OperatorCode_q, "q");}
  Operator *makeOperator_Q(void) {return new Operator(OperatorCode_Q, "Q");}
  Operator *makeOperator_cm(TransformationMatrix const &ctm) {return new OperatorNf(OperatorCode_cm, "cm", ctm.mA, ctm.mB, ctm.mC, ctm.mD, ctm.mE, ctm.mF);}

  Operator *makeOperator_c(float x1, float y1, float x2, float y2, float x3, float y3) {return new OperatorNf(OperatorCode_c, "c", x1, y1, x2, y2, x3, y3);}
  Operator *makeOperator_h(void) {return new Operator(OperatorCode_h, "h");}
  Operator *makeOperator_l(float x, float y) {return new OperatorNf(OperatorCode_l, "l", x, y);}
  Operator *makeOperator_m(float x, float y) {return new OperatorNf(OperatorCode_m, "m", x, y);}
  Operator *makeOperator_re(float x, float y, float width, float height) {return new OperatorNf(OperatorCode_re, "re", x, y, width, height);}
  Operator *makeOperator_v(float x2, float y2, float x3, float y3) {return new OperatorNf(OperatorCode_v, "v", x2, y2, x3, y3);}
  Operator *makeOperator_y(float x1, float y1, float x3, float y3) {return new OperatorNf(OperatorCode_y, "y", x1, y1, x3, y3);}

  Operator *makeOperator_b(void) {return new Operator(OperatorCode_b, "b");}
  Operator *makeOperator_B(void) {return new Operator(OperatorCode_B, "B");}
  Operator *makeOperator_b_ast(void) {return new Operator(OperatorCode_b_ast, "b*");}
  Operator *makeOperator_B_ast(void) {return new Operator(OperatorCode_B_ast, "B*");}
  Operator *makeOperator_f(void) {return new Operator(OperatorCode_f, "f");}
  Operator *makeOperator_F(void) {return new Operator(OperatorCode_F, "F");}
  Operator *makeOperator_f_ast(void) {return new Operator(OperatorCode_f_ast, "f*");}
  Operator *makeOperator_n(void) {return new Operator(OperatorCode_n, "n");}
  Operator *makeOperator_s(void) {return new Operator(OperatorCode_s, "s");}
  Operator *makeOperator_S(void) {return new Operator(OperatorCode_S, "S");}

  Operator *makeOperator_W(void) {return new Operator(OperatorCode_W, "W");}
  Operator *makeOperator_W_ast(void) {return new Operator(OperatorCode_W_ast, "W*");}

  Operator *makeOperator_BT(void) {return new Operator(OperatorCode_BT, "BT");}
  Operator *makeOperator_ET(void) {return new Operator(OperatorCode_ET, "ET");}

  Operator *makeOperator_Tc(float charSpacing) {return new OperatorNf(OperatorCode_Tc, "Tc", charSpacing);}
  Operator *makeOperator_Tf(char const *font, float size) {return new OperatorF(OperatorCode_Tf, "Tf", font, size);}
  Operator *makeOperator_TL(int32_t leading) {return new OperatorNi(OperatorCode_TL, "TL", leading);}
  Operator *makeOperator_Tr(int32_t render) {return new OperatorNi(OperatorCode_Tr, "Tr", render);}
  Operator *makeOperator_Ts(int32_t rise) {return new OperatorNi(OperatorCode_Ts, "Ts", rise);}
  Operator *makeOperator_Tw(float wordSpacing) {return new OperatorNf(OperatorCode_Tw, "Tw", wordSpacing);}
  Operator *makeOperator_Tz(float scale) {return new OperatorNf(OperatorCode_Tz, "Tz", scale);}

  Operator *makeOperator_Td(float tx, float ty) {return new OperatorNi(OperatorCode_Td, "Td", tx, ty);}
  Operator *makeOperator_TD(float tx, float ty) {return new OperatorNi(OperatorCode_TD, "TD", tx, ty);}
  Operator *makeOperator_Tm(TransformationMatrix const &textMatrix) {return new OperatorNf(OperatorCode_Tm, "Tm", textMatrix.mA, textMatrix.mB, textMatrix.mC, textMatrix.mD, textMatrix.mE, textMatrix.mF);}
  Operator *makeOperator_T_ast(void) {return new Operator(OperatorCode_T_ast, "T*");}

  Operator *makeOperator_Tj(std::unique_ptr<kernel::ObjectUTF16BEString> text) {return new OperatorT(OperatorCode_Tj, "Tj", std::move(text));}
  Operator *makeOperator_TJ(std::unique_ptr<kernel::ObjectUTF16BEString> text, std::vector<int32_t> const &positions) {return new OperatorT(OperatorCode_TJ, "TJ", std::move(text), positions);}
  Operator *makeOperator_quote(std::unique_ptr<kernel::ObjectUTF16BEString> text) {return new OperatorT(OperatorCode_quote, "'", std::move(text));}
  Operator *makeOperator_dquote(float wordSpacing, float charSpacing, std::unique_ptr<kernel::ObjectUTF16BEString> text) {return new OperatorT(OperatorCode_dquote, "\"", wordSpacing, charSpacing, std::move(text));}

  Operator *makeOperator_d0(float wx, float wy) {return new OperatorNf(OperatorCode_d0, "d0", wx, wy);}
  Operator *makeOperator_d1(float wx, float wy, float llx, float lly, float urx, float ury) {return new OperatorNf(OperatorCode_d1, "d1", wx, wy, llx, lly, urx, ury);}

  Operator *makeOperator_CS(char const *name) {return new OperatorC(OperatorCode_CS, "CS", name);}
  Operator *makeOperator_cs(char const *name) {return new OperatorC(OperatorCode_cs, "cs", name);}
  Operator *makeOperator_SC(float c1) {return new OperatorNf(OperatorCode_SC, "SC", c1);}
  Operator *makeOperator_SC(float c1, float c2, float c3) {return new OperatorNf(OperatorCode_SC, "SC", c1, c2, c3);}
  Operator *makeOperator_SC(float c1, float c2, float c3, float c4) {return new OperatorNf(OperatorCode_SC, "SC", c1, c2, c3, c4);}
  Operator *makeOperator_sc(float c1) {return new OperatorNf(OperatorCode_sc, "sc", c1);}
  Operator *makeOperator_sc(float c1, float c2, float c3) {return new OperatorNf(OperatorCode_sc, "sc", c1, c2, c3);}
  Operator *makeOperator_sc(float c1, float c2, float c3, float c4) {return new OperatorNf(OperatorCode_sc, "sc", c1, c2, c3, c4);}
  Operator *makeOperator_SCN(float *colorants, uint32_t count) {return new OperatorC(OperatorCode_SCN, "SCN", colorants, count);}
  Operator *makeOperator_SCN(float *colorants, uint32_t count, char const *name) {return new OperatorC(OperatorCode_SCN, "SCN", colorants, count, name);}
  Operator *makeOperator_scn(float *colorants, uint32_t count) {return new OperatorC(OperatorCode_scn, "scn", colorants, count);}
  Operator *makeOperator_scn(float *colorants, uint32_t count, char const *name) {return new OperatorC(OperatorCode_scn, "scn", colorants, count, name);}
  Operator *makeOperator_G(float gray) {return new OperatorNf(OperatorCode_G, "G", gray);}
  Operator *makeOperator_g(float gray) {return new OperatorNf(OperatorCode_g, "g", gray);}
  Operator *makeOperator_RG(float r, float g, float b) {return new OperatorNf(OperatorCode_RG, "RG", r, g, b);}
  Operator *makeOperator_rg(float r, float g, float b) {return new OperatorNf(OperatorCode_rg, "rg", r, g, b);}
  Operator *makeOperator_K(float c, float m, float y, float k) {return new OperatorNf(OperatorCode_K, "K", c, m, y, k);}
  Operator *makeOperator_k(float c, float m, float y, float k) {return new OperatorNf(OperatorCode_k, "k", c, m, y, k);}

  Operator *makeOperator_sh(char const *name) {return new OperatorC(OperatorCode_sh, "sh", name);}

  Operator *makeOperator_BI(void) {return new Operator(OperatorCode_BI, "BI");}
  Operator *makeOperator_EI(void) {return new Operator(OperatorCode_EI, "EI");}
  Operator *makeOperator_ID(void) {return new Operator(OperatorCode_ID, "ID");}

  Operator *makeOperator_Do(char const *name) {return new OperatorC(OperatorCode_Do, "Do", name);}

  Operator *makeOperator_BDC(char const *tag, void *properties, uint32_t size) {return new OperatorM(OperatorCode_BDC, "BDC", tag, properties, size);}
  Operator *makeOperator_BMC(char const *tag, char const *properties) {return new OperatorM(OperatorCode_BMC, "BMC", tag, properties);}
  Operator *makeOperator_DP(char const *tag, void *properties, uint32_t size) {return new OperatorM(OperatorCode_DP, "DP", tag, properties, size);}
  Operator *makeOperator_EMC(void) {return new Operator(OperatorCode_EMC, "EMC");}
  Operator *makeOperator_MP(char const *tag, char const *properties) {return new OperatorM(OperatorCode_MP, "MP", tag, properties);}

  Operator *makeOperator_BX(void) {return new Operator(OperatorCode_BX, "BX");}
  Operator *makeOperator_EX(void) {return new Operator(OperatorCode_EX, "EX");}
}
