#pragma once

#include <string>
#include <vector>
#include <memory>

#include <TransformationMatrix.h>

namespace kernel{
  class ObjectStream;
  class ObjectUTF16BEString;

  enum OperatorCode{
    // General graphics state
    OperatorCode_w,
    OperatorCode_J,
    OperatorCode_j,
    OperatorCode_M,
    OperatorCode_d,
    OperatorCode_ri,
    OperatorCode_i,
    OperatorCode_gs,

    // Special graphics state
    OperatorCode_q,
    OperatorCode_Q,
    OperatorCode_cm,

    // Path construction
    OperatorCode_m,
    OperatorCode_l,
    OperatorCode_c,
    OperatorCode_v,
    OperatorCode_y,
    OperatorCode_h,
    OperatorCode_re,

    // Path painting
    OperatorCode_S,
    OperatorCode_s,
    OperatorCode_f,
    OperatorCode_F,
    OperatorCode_f_ast,
    OperatorCode_B,
    OperatorCode_B_ast,
    OperatorCode_b,
    OperatorCode_b_ast,
    OperatorCode_n,

    // Clipping paths
    OperatorCode_W,
    OperatorCode_W_ast,

    // Text objects
    OperatorCode_BT,
    OperatorCode_ET,

    // Text state
    OperatorCode_Tc,
    OperatorCode_Tw,
    OperatorCode_Tz,
    OperatorCode_TL,
    OperatorCode_Tf,
    OperatorCode_Tr,
    OperatorCode_Ts,

    // Text positioning
    OperatorCode_Td,
    OperatorCode_TD,
    OperatorCode_Tm,
    OperatorCode_T_ast,

    // Text showing
    OperatorCode_Tj,
    OperatorCode_TJ,
    OperatorCode_quote,
    OperatorCode_dquote,


    // Type 3 fonts
    OperatorCode_d0,
    OperatorCode_d1,

    // Color
    OperatorCode_CS,
    OperatorCode_cs,
    OperatorCode_SC,
    OperatorCode_SCN,
    OperatorCode_sc,
    OperatorCode_scn,
    OperatorCode_G,
    OperatorCode_g,
    OperatorCode_RG,
    OperatorCode_rg,
    OperatorCode_K,
    OperatorCode_k,

    // Shading patterns
    OperatorCode_sh,

    // Inline images
    OperatorCode_BI,
    OperatorCode_ID,
    OperatorCode_EI,

    // XObjects
    OperatorCode_Do,

    // Marked content
    OperatorCode_MP,
    OperatorCode_DP,
    OperatorCode_BMC,
    OperatorCode_BDC,
    OperatorCode_EMC,

    // Compatibility
    OperatorCode_BX,
    OperatorCode_EX
  };

  /**
   * @brief For operators w/o parameters
   */
  class Operator{
  public:
    Operator(OperatorCode opCode, char const *cmd, uint16_t operands = 0);
    virtual ~Operator(void);
    OperatorCode getOperatorCode(void) const;
    uint16_t getOperands(void) const;

    void serialize(kernel::ObjectStream *stream) const;

  protected:
    virtual bool serializePrefix(kernel::ObjectStream *stream) const;

  private:
    Operator(Operator const &) = delete;
    Operator &operator=(Operator const &) = delete;

  private:
    OperatorCode mOpCode; /**< operation code */
    std::string mCmd;     /**< command */
    uint16_t mOperands;   /**< operands count */
  };

  /**
   * @brief For operators with 1-6 T parameters
   */
  template <typename T>
  class OperatorNx : public Operator{
  public:
    OperatorNx(OperatorCode opCode, char const *cmd, T operand0) : Operator(opCode, cmd, 1), mOperand0(operand0) {}
    OperatorNx(OperatorCode opCode, char const *cmd, T operand0, T operand1) : Operator(opCode, cmd, 2), mOperand0(operand0), mOperand1(operand1) {}
    OperatorNx(OperatorCode opCode, char const *cmd, T operand0, T operand1, T operand2) : Operator(opCode, cmd, 3), mOperand0(operand0), mOperand1(operand1), mOperand2(operand2) {}
    OperatorNx(OperatorCode opCode, char const *cmd, T operand0, T operand1, T operand2, T operand3) : Operator(opCode, cmd, 4), mOperand0(operand0), mOperand1(operand1), mOperand2(operand2), mOperand3(operand3) {}
    OperatorNx(OperatorCode opCode, char const *cmd, T operand0, T operand1, T operand2, T operand3, T operand4) : Operator(opCode, cmd, 5), mOperand0(operand0), mOperand1(operand1), mOperand2(operand2), mOperand3(operand3), mOperand4(operand4) {}
    OperatorNx(OperatorCode opCode, char const *cmd, T operand0, T operand1, T operand2, T operand3, T operand4, T operand5) : Operator(opCode, cmd, 6), mOperand0(operand0), mOperand1(operand1), mOperand2(operand2), mOperand3(operand3), mOperand4(operand4), mOperand5(operand5) {}

  private:
    OperatorNx(OperatorNx const &) = delete;
    OperatorNx &operator=(OperatorNx const &) = delete;

  protected:
    bool serializePrefix(kernel::ObjectStream *stream) const{
      std::string data;
      uint16_t operands = getOperands();

      if (getOperatorCode() == OperatorCode_d) // dash pattern
        data = "[ " + std::to_string(mOperand0) + " " + std::to_string(mOperand1) + " ] " + std::to_string(mOperand2);
      else if (operands == 1)
        data = std::to_string(mOperand0);
      else if (operands == 2)
        data = std::to_string(mOperand0) + " " + std::to_string(mOperand1);
      else if (operands == 3)
        data = std::to_string(mOperand0) + " " + std::to_string(mOperand1) + " " + std::to_string(mOperand2);
      else if (operands == 4)
        data = std::to_string(mOperand0) + " " + std::to_string(mOperand1) + " " + std::to_string(mOperand2) + " " + std::to_string(mOperand3);
      else if (operands == 5)
        data = std::to_string(mOperand0) + " " + std::to_string(mOperand1) + " " + std::to_string(mOperand2) + " " + std::to_string(mOperand3) + " " + std::to_string(mOperand4);
      else if (operands == 6)
        data = std::to_string(mOperand0) + " " + std::to_string(mOperand1) + " " + std::to_string(mOperand2) + " " + std::to_string(mOperand3) + " " + std::to_string(mOperand4) + " " + std::to_string(mOperand5);

      stream->write(data.c_str(), sizeof(uint8_t), data.length());
      return true;
    }

  public:
    T mOperand0;   /**< the first operand */
    T mOperand1;   /**< the second operand */
    T mOperand2;   /**< the third operand */
    T mOperand3;   /**< the fourth operand */
    T mOperand4;   /**< the fifth operand */
    T mOperand5;   /**< the sixth operand */
  };

  template <>
  class OperatorNx<float> : public Operator{
  public:
    OperatorNx(OperatorCode opCode, char const *cmd, float operand0) : Operator(opCode, cmd, 1), mOperand0(operand0) {}
    OperatorNx(OperatorCode opCode, char const *cmd, float operand0, float operand1) : Operator(opCode, cmd, 2), mOperand0(operand0), mOperand1(operand1) {}
    OperatorNx(OperatorCode opCode, char const *cmd, float operand0, float operand1, float operand2) : Operator(opCode, cmd, 3), mOperand0(operand0), mOperand1(operand1), mOperand2(operand2) {}
    OperatorNx(OperatorCode opCode, char const *cmd, float operand0, float operand1, float operand2, float operand3) : Operator(opCode, cmd, 4), mOperand0(operand0), mOperand1(operand1), mOperand2(operand2), mOperand3(operand3) {}
    OperatorNx(OperatorCode opCode, char const *cmd, float operand0, float operand1, float operand2, float operand3, float operand4) : Operator(opCode, cmd, 5), mOperand0(operand0), mOperand1(operand1), mOperand2(operand2), mOperand3(operand3), mOperand4(operand4) {}
    OperatorNx(OperatorCode opCode, char const *cmd, float operand0, float operand1, float operand2, float operand3, float operand4, float operand5) : Operator(opCode, cmd, 6), mOperand0(operand0), mOperand1(operand1), mOperand2(operand2), mOperand3(operand3), mOperand4(operand4), mOperand5(operand5) {}

  private:
    OperatorNx(OperatorNx const &) = delete;
    OperatorNx &operator=(OperatorNx const &) = delete;

  protected:
    bool serializePrefix(kernel::ObjectStream *stream) const;

  public:
    float mOperand0;   /**< the first operand */
    float mOperand1;   /**< the second operand */
    float mOperand2;   /**< the third operand */
    float mOperand3;   /**< the fourth operand */
    float mOperand4;   /**< the fifth operand */
    float mOperand5;   /**< the sixth operand */
  };

  template <>
  class OperatorNx<int32_t> : public Operator{
  public:
    OperatorNx(OperatorCode opCode, char const *cmd, int32_t operand0) : Operator(opCode, cmd, 1), mOperand0(operand0) {}
    OperatorNx(OperatorCode opCode, char const *cmd, int32_t operand0, int32_t operand1) : Operator(opCode, cmd, 2), mOperand0(operand0), mOperand1(operand1) {}
    OperatorNx(OperatorCode opCode, char const *cmd, int32_t operand0, int32_t operand1, int32_t operand2) : Operator(opCode, cmd, 3), mOperand0(operand0), mOperand1(operand1), mOperand2(operand2) {}
    OperatorNx(OperatorCode opCode, char const *cmd, int32_t operand0, int32_t operand1, int32_t operand2, int32_t operand3) : Operator(opCode, cmd, 4), mOperand0(operand0), mOperand1(operand1), mOperand2(operand2), mOperand3(operand3) {}
    OperatorNx(OperatorCode opCode, char const *cmd, int32_t operand0, int32_t operand1, int32_t operand2, int32_t operand3, int32_t operand4) : Operator(opCode, cmd, 5), mOperand0(operand0), mOperand1(operand1), mOperand2(operand2), mOperand3(operand3), mOperand4(operand4) {}
    OperatorNx(OperatorCode opCode, char const *cmd, int32_t operand0, int32_t operand1, int32_t operand2, int32_t operand3, int32_t operand4, int32_t operand5) : Operator(opCode, cmd, 6), mOperand0(operand0), mOperand1(operand1), mOperand2(operand2), mOperand3(operand3), mOperand4(operand4), mOperand5(operand5) {}

  private:
    OperatorNx(OperatorNx const &) = delete;
    OperatorNx &operator=(OperatorNx const &) = delete;

  protected:
    bool serializePrefix(kernel::ObjectStream *stream) const;

  public:
    int32_t mOperand0;   /**< the first operand */
    int32_t mOperand1;   /**< the second operand */
    int32_t mOperand2;   /**< the third operand */
    int32_t mOperand3;   /**< the fourth operand */
    int32_t mOperand4;   /**< the fifth operand */
    int32_t mOperand5;   /**< the sixth operand */
  };

  /**
   * @brief For operators with 1-6 float parameters
   */
  typedef OperatorNx<float> OperatorNf;

  /**
   * @brief For operators with 1-6 integer parameters
   */
  typedef OperatorNx<int32_t> OperatorNi;

  /**
   * @brief For dash pattern
   * dashArray dashPhase d
   */
  class OperatorD : public Operator{
  public:
    OperatorD(OperatorCode opCode, char const *cmd, float const *dashPattern, uint32_t count, uint32_t phase);
    ~OperatorD(void);

  protected:
    bool serializePrefix(kernel::ObjectStream *stream) const;

  private:
    std::vector<float> mDashPattern;
    uint32_t mPhase;
  };

  /**
   * @brief For text font selection
   * font size Tf
   */
  class OperatorF : public Operator{
  public:
    OperatorF(OperatorCode opCode, char const *cmd, char const *fontName, int32_t fontSize);
    ~OperatorF(void);

  protected:
    bool serializePrefix(kernel::ObjectStream *stream) const;

  public:
    int32_t mFontSize;     /**< font size for Tf operator OperatorCode_Tf */
    std::string mFontName; /**< font name for OperatorCode_Tf  */
  };

  /**
   * @brief For text showing operators
   * wordSpacing charSpacing string "
   * string '
   * string Tj
   */
  class OperatorT : public Operator{
  public:
    OperatorT(OperatorCode opCode, char const *cmd, std::unique_ptr<kernel::ObjectUTF16BEString> text);
    OperatorT(OperatorCode opCode, char const *cmd, std::unique_ptr<kernel::ObjectUTF16BEString> text, std::vector<int32_t> const &positions);
    OperatorT(OperatorCode opCode, char const *cmd, float wordSpacing, float charSpacing, std::unique_ptr<kernel::ObjectUTF16BEString> text);
    ~OperatorT(void);

  protected:
    bool serializePrefix(kernel::ObjectStream *stream) const;

  public:
    float mWordSpacing;           /**< word spacing for " operator OperatorCode_dquote */
    float mCharSpacing;           /**< char spacing for " operator OperatorCode_dquote */
    std::unique_ptr<kernel::ObjectUTF16BEString> mString;  /**< pointer to string representation, can be (Text) */
    std::vector<int32_t> mPositions; /**< text positions for operator TJ */
  };

  /**
   * @brief For color operators
   * name SC, sc, Do or sh
   * c1 ... cN SC or sc
   * c1 ... cN SCN or scn
   * c1 ... cN name SCN or scn
   */
  class OperatorC : public Operator{
  public:
    OperatorC(OperatorCode opCode, char const *cmd, char const *name);
    OperatorC(OperatorCode opCode, char const *cmd, float *colorants, uint32_t count);
    OperatorC(OperatorCode opCode, char const *cmd, float *colorants, uint32_t count, char const *name);
    ~OperatorC(void);

  protected:
    bool serializePrefix(kernel::ObjectStream *stream) const;

  public:
    std::vector<uint8_t> mName;     /**< color space name */
    std::vector<float> mColorants; /**< array of colorants */
  };

  /**
   * @brief For Marked-content operators
   */
  class OperatorM : public Operator{
  public:
    OperatorM(OperatorCode opCode, char const *cmd, char const *name);
    OperatorM(OperatorCode opCode, char const *cmd, char const *name, void *properties, uint32_t size);
    OperatorM(OperatorCode opCode, char const *cmd, char const *name, char const *properties);
    ~OperatorM(void);
  
  protected:
    bool serializePrefix(kernel::ObjectStream *stream) const;

  public:
    std::vector<uint8_t> mTag;        /**< tag name */
    std::vector<uint8_t> mProperties; /**< properties name or inline dictionary */
  };

  /**
   * @brief For one-string operators
   * ri (Rendering Intent)
   * gs (ExtGState)
   */
  class OperatorS : public Operator{
  public:
    OperatorS(OperatorCode opCode, char const *cmd, char const *name);
    ~OperatorS(void);

  private:
    bool serializePrefix(kernel::ObjectStream *stream) const;

  public:
    std::string mName;
  };

  // General graphics state
  Operator *makeOperator_w(float lineWidth);   /**< Set line width 4.7 219 */
  Operator *makeOperator_J(int32_t lineCap);   /**< Set line cap style 4.7 219 */
  Operator *makeOperator_j(int32_t lineJoin);  /**< Set line join style 4.7 219 */
  Operator *makeOperator_M(float miterLimit);  /**< Set miter limit 4.7 219 */
  Operator *makeOperator_d(float const *dashPattern, uint32_t count, uint32_t phase);  /**< Set line dash pattern, 4.7 219 */
  Operator *makeOperator_ri(char const *ri);   /**< Set color rendering intent 4.7 219 */
  Operator *makeOperator_i(uint32_t flatness); /**< Set flatness tolerance 4.7 219 */
  Operator *makeOperator_gs(char const *name); /**< (PDF 1.2) Set parameters from graphics state parameter dictionary 4.7 219 */

  // Special graphics state
  Operator *makeOperator_q(void);  /**< Save graphics state 4.7 219 */
  Operator *makeOperator_Q(void);  /**< Restore graphics state 4.7 219 */
  Operator *makeOperator_cm(pdfout::TransformationMatrix const &ctm); /**< Concatenate matrix to current transformation matrix, 4.7 219 */

  // Path construction
  Operator *makeOperator_c(float x1, float y1, float x2, float y2, float x3, float y3);  /**< Append curved segment to path (three control points), 4.9 226 */
  Operator *makeOperator_h(void);                /**< Close subpath 4.9 227 */
  Operator *makeOperator_l(float x, float y);  /**< Append straight line segment to path 4.9 226 */
  Operator *makeOperator_m(float x, float y);  /**< Begin new subpath 4.9 226 */
  Operator *makeOperator_re(float x, float y, float width, float height); /**< Append rectangle to path 4.9 227 */
  Operator *makeOperator_v(float x2, float y2, float x3, float y3);       /**< Append curved segment to path (initial point replicated) 4.9 226 */
  Operator *makeOperator_y(float x1, float y1, float x3, float y3);       /**< Append curved segment to path (final point replicated) 4.9 226 */

  // Path painting
  Operator *makeOperator_b(void);     /**< Close, fill, and stroke path using nonzero winding number rule, 4.10 230 */
  Operator *makeOperator_B(void);     /**< Fill and stroke path using nonzero winding number rule, 4.10 230 */
  Operator *makeOperator_b_ast(void); /**< b*, Close, fill, and stroke path using even-odd rule, 4.10 230 */
  Operator *makeOperator_B_ast(void); /**< B*, Fill and stroke path using even-odd rule, 4.10 230 */
  Operator *makeOperator_f(void);     /**< Fill path using nonzero winding number rule, 4.10 230 */
  Operator *makeOperator_F(void);     /**< Fill path using nonzero winding number rule (obsolete) 4.10 230 */
  Operator *makeOperator_f_ast(void); /**< f*, Fill path using even-odd rule 4.10 230 */
  Operator *makeOperator_n(void);     /**< End path without filling or stroking 4.10 230 */
  Operator *makeOperator_s(void);     /**< Close and stroke path 4.10 230 */
  Operator *makeOperator_S(void);     /**< Stroke path 4.10 230 */

  // Clipping paths
  Operator *makeOperator_W(void);     /**< Set clipping path using nonzero winding number rule 4.11 235 */
  Operator *makeOperator_W_ast(void); /**< W*, Set clipping path using even-odd rule 4.11 235 */

  // Text objects
  Operator *makeOperator_BT(void); /**< Begin text object, 5.4 405 */
  Operator *makeOperator_ET(void); /**< End text object, 5.4 405 */

  // Text state
  Operator *makeOperator_Tc(float charSpacing); /**< Set character spacing 5.2 398 */
  Operator *makeOperator_Tf(char const *font, float size); /**< Set text font and size 5.2 398 */
  Operator *makeOperator_TL(int32_t leading);   /**< Set text leading 5.2 398 */
  Operator *makeOperator_Tr(int32_t render);    /**< Set text rendering mode 5.2 398 */
  Operator *makeOperator_Ts(int32_t rise);      /**< Set text rise 5.2 398 */
  Operator *makeOperator_Tw(float wordSpacing); /**< Set word spacing 5.2 398 */
  Operator *makeOperator_Tz(float scale);     /**< Set horizontal text scaling 5.2 398 */

  // Text positioning
  Operator *makeOperator_Td(float tx, float ty);                 /**< Move text position 5.5 406 */
  Operator *makeOperator_TD(float tx, float ty);                 /**< Move text position and set leading 5.5 406 */
  Operator *makeOperator_Tm(pdfout::TransformationMatrix const &textMatrix); /**< Set text matrix and text line matrix 5.5 406 */
  Operator *makeOperator_T_ast(void);                                /**< T*, Move to start of next text line 5.5 406 */

  // Text showing
  Operator *makeOperator_Tj(std::unique_ptr<kernel::ObjectUTF16BEString> text);    /**< Show text 5.6 407 */
  Operator *makeOperator_TJ(std::unique_ptr<kernel::ObjectUTF16BEString> text, std::vector<int32_t> const &positions);     /**< Show text, allowing individual glyph positioning 5.6 408 */
  Operator *makeOperator_quote(std::unique_ptr<kernel::ObjectUTF16BEString> text); /**< ', Move to next line and show text 5.6 407 */
  Operator *makeOperator_dquote(float wordSpacing, float charSpacing, std::unique_ptr<kernel::ObjectUTF16BEString> text);  /**< Set word and character spacing, move to next line, and show text 5.6 407 */

  // Type3 fonts
  Operator *makeOperator_d0(float wx, float wy); /**< Set glyph width in Type3 font, 5.10 423 */
  Operator *makeOperator_d1(float wx, float wy, float llx, float lly, float urx, float ury); /**< Set glyph width and bounding box in Type 3 font, 5.10 423 */

  // Color
  Operator *makeOperator_CS(char const *name);  /**< (PDF 1.1) Set color space for stroking operations, 4.24 287 */
  Operator *makeOperator_cs(char const *name);  /**< (PDF 1.1) Set color space for nonstroking operations, 4.24 287 */

  Operator *makeOperator_SC(float c1);                                /**< (PDF 1.1) Set color for stroking operations 4.24 287 */
  Operator *makeOperator_SC(float c1, float c2, float c3);            /**< n == 1 for DeviceGray, CalGray and Indexed, n == 3 for DeviceRGB, CalRGB and Lab, */
  Operator *makeOperator_SC(float c1, float c2, float c3, float c4);  /**< n == 4 for DeviceCMYK */

  Operator *makeOperator_sc(float c1);                                /**< (PDF 1.1) Set color for nonstroking operations 4.24 288 */
  Operator *makeOperator_sc(float c1, float c2, float c3);            /**< n == 1 for DeviceGray, CalGray and Indexed, n == 3 for DeviceRGB, CalRGB and Lab, */
  Operator *makeOperator_sc(float c1, float c2, float c3, float c4);  /**< n == 4 for DeviceCMYK */


  Operator *makeOperator_SCN(float *colorants, uint32_t count);                   /**< (PDF 1.2) Set color for stroking operations (ICCBased and special color spaces) 4.24 288 */
  Operator *makeOperator_SCN(float *colorants, uint32_t count, char const *name); /**< (PDF 1.2) Set color for stroking operations (ICCBased and special color spaces) 4.24 288 */
  Operator *makeOperator_scn(float *colorants, uint32_t count);                   /**< (PDF 1.2) Set color for nonstroking operations (ICCBased and special color spaces) 4.24 288 */
  Operator *makeOperator_scn(float *colorants, uint32_t count, char const *name); /**< (PDF 1.2) Set color for nonstroking operations (ICCBased and special color spaces) 4.24 288 */
  Operator *makeOperator_G(float gray);   /**< Set gray level for stroking operations 4.24 288 */
  Operator *makeOperator_g(float gray);   /**< Set gray level for nonstroking operations 4.24 288 */
  Operator *makeOperator_RG(float r, float g, float b);  /**< Set RGB color for stroking operations 4.24 288 */
  Operator *makeOperator_rg(float r, float g, float b);  /**< Set RGB color for nonstroking operations 4.24 288 */
  Operator *makeOperator_K(float c, float m, float y, float k);   /**< Set CMYK color for stroking operations 4.24 288 */
  Operator *makeOperator_k(float c, float m, float y, float k);   /**< Set CMYK color for nonstroking operations 4.24 288 */

  // Shading patterns
  Operator *makeOperator_sh(char const *name); /**< (PDF 1.3) Paint area defined by shading pattern 4.27 303 */

  // Inline images
  Operator *makeOperator_BI(void); /**< Begin inline image object, 4.42 352 */
  Operator *makeOperator_EI(void); /**< End inline image object, 4.42 352 */
  Operator *makeOperator_ID(void); /**< Begin inline image data 4.42 352 */

  // XObjects
  Operator *makeOperator_Do(char const *name); /**< Invoke named XObject, 4.37 332 */

  // Marked content
  /**
   * Creates BDC operator
   * @param tag [in, required] is a name object indicating the role or significance of the point
   * @param properties [in, required] is either an inline dictionary containing the property list or
                           a name object associated with it in the Properties subdictionary of the current resource dictionary
   * @param size [in, required] size of properties in bytes
   */
  Operator *makeOperator_BDC(char const *tag, void *properties, uint32_t size); /**< (PDF 1.2) Begin marked-content sequence with property list, 10.7 851 */
  Operator *makeOperator_BMC(char const *tag, char const *properties); /**< (PDF 1.2) Begin marked-content sequence, 10.7 851 */

  /**
   * Creates DP operator
   * @param tag [in, required] is a name object indicating the role or significance of the point
   * @param properties [in, required] is either an inline dictionary containing the property list or
                           a name object associated with it in the Properties subdictionary of the current resource dictionary
   * @param size [in, required] size of properties in bytes
   */
  Operator *makeOperator_DP(char const *tag, void *properties, uint32_t size);  /**< (PDF 1.2) Define marked-content point with property list, 10.7 851 */
  Operator *makeOperator_EMC(void); /**< (PDF 1.2) End marked-content sequence, 10.7 851 */
  Operator *makeOperator_MP(char const *tag, char const *properties);  /**< (PDF 1.2) Define marked-content point 10.7 851 */

  // Compatibility
  Operator *makeOperator_BX(void); /**< (PDF 1.1) Begin compatibility section, 1.29 152 */
  Operator *makeOperator_EX(void); /**< (PDF 1.1) End compatibility section, 3.29 152 */
}
