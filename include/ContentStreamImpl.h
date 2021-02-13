#pragma once

#include <stack>
#include <memory>
#include <vector>

#include <ContentStream.h>
#include <Types.h>
#include <Color.h>
#include <Encoding.h>
#include <UString.h>
#include <TransformationMatrix.h>

namespace kernel{
  class Operator;
  class ObjectStream;
  class ResourceManager;
  struct GraphicsState;
}

namespace pdfout{ namespace impl{

  /**
   * @brief PDF content stream representation
   */
  class ContentStreamImpl : public ContentStream{
  public:
    explicit ContentStreamImpl(kernel::ResourceManager *resourceManger);
    ~ContentStreamImpl(void);

    void serialize(kernel::ObjectStream *stream) const;

    // Special graphics state
    void save(void);                                   /**< Save the current graphics state on the graphics state stack. */
    void restore(void);                                /**< Restore the graphics state by removing the most recently saved state from the stack and making it the current state. */
    void setCTM(TransformationMatrix const &ctm);      /**< Modify the current transformation matrix. */
    TransformationMatrix getCTM(void) const;

    // General graphics state
    void setLineWidth(float lineWidth);                /**< Set the line width in the graphics state. */
    void setLineCap(LineCap lineCap);                  /**< Set the line cap style in the graphics state. */
    void setLineJoin(LineJoin lineJoin);               /**< Set the line join style in the graphics state. */
    void setMiterLimit(float miterLimit);              /**< Set the miter limit in the graphics state. */
    void setDashArray(float const *dashPattern, std::size_t count, std::size_t phase); /**< Set the line dash pattern in the graphics state. */
    void setIntent(RenderingIntent renderingIntent);   /**< Set the color rendering intent in the graphics state. */
    void setFlatness(float flatness);                  /**< Set the flatness tolerance in the graphics state. Flatness is a number in the range 0 to 100;
                                                            a value of 0 specifies the output device's default flatness tolerance. */
    void setExtGState(ExtGState *extGState);           /**< Set the specified parameters in the ExtGState subdictionary of the current resource dictionary. */

    // Text objects
    void beginText(void); /**< Begin a text object, initializing the text matrix and the text line matrix to the identity matrix. */
    void endText(void);   /**< End a text object, discarding the text matrix. */

    // Text state
    void setTextCharSpacing(float charSpacing);     /**< Set the character spacing, which is a number expressed in unscaled text space units. */
    void setTextWordSpacing(float wordSpacing);     /**< Set the word spacing, which is a number expressed in unscaled text space units. */
    void setTextScale(float scale);                 /**< Set the horizontal scaling, scale is a number specifying the percentage of the normal width. */
    void setTextLeading(int32_t leading);           /**< Set the text leading, which is a number expressed in unscaled text space units. */
    void setTextFont(Font *font, int32_t fontSize); /**< Set the text font and the text font size, font is the name of a font resource in the Font subdictionary
                                                         of the current resource dictionary; size is a number representing a scale factor. */
    void setTextRender(TextRenderingMode render);   /**< Set the text rendering mode to render. */
    void setTextRise(int32_t rise);                 /**< Set the text rise, which is a number expressed in unscaled text space units. */

    // Text positioning
    void setTextPosition(float x, float y);                 /**< Move to the start of the next line, offset from the start of the current line by (x , y ). */
    void setTextAbsolutePosition(float x, float y);         /**< Set text absolute position */
    void setTextPositionAndLeading(float x, float y);       /**< Move to the start of the next line, offset from the start of the current line by (x , y ).
                                                                 As a side effect, this operator sets the leading parameter in the text state to y. */
    void setTextMatrix(TransformationMatrix const &textMatrix); /**< Set the text matrix. */
    void setTextNewLine(void);                                  /**< Move to the start of the next line. */

    // Text showing
    void showText(pdfout::UString const &text);          /**< Show a text string as the current text position. */
    void showTextAtNewLine(pdfout::UString const &text); /**< Move to the next line and show a text string. */
    void showTextAtNewLine(float wordSpacing, float charSpacing, pdfout::UString const &text);  /**< Move to the next line and show a text string, using wordSpace as the word spacing and charSpace as the character spacing */

    /**
     * @brief Shows formatted text in the specified rectangle
     * @param text [in] Text message
     * @param rect [in] Rectangle in which the text is to be formatted
     * @param alignment [in] The method of formatting the text. This parameter can be one or more of the ::Alignment values
     * @param distance [in] The array with specified distance between origins of adjacent glyphs in thousandths of a unit of text space
     * @param count [in] The count of elements in the distance array
     */
    void showText(pdfout::UString const &text, struct Rectangle const &rect, uint16_t alignment, uint16_t decoration);
    void showText(pdfout::UString const &text, float x, float y, uint16_t alignment, uint16_t decoration, int32_t *distance, std::size_t count);

    // Image showing
    /**
     * Show image
     * @param image [in] An image object
     * @param x [in] The x-coordinate of the bottom-left corner of the destination rectangle
     * @param y [in] The y-coordinate of the bottom-left corner of the destination rectangle
     * @param width [in] The width of the destination rectangle
     * @param height [in] The height of the destination rectagnle
     * @param rotate [in] Angle in radiant for counterclockwise rotation
     */
    void showImage(Image *image, float x, float y, float width, float height, float rotate);
    void showImage(Image *image, TransformationMatrix const &tm);

    // ColorSpace
    void setColorSpace(ColorSpace *colorSpace);
    void setStrokingColorSpace(ColorSpace *colorSpace);

    // Color
    void setColorGray(float gray);
    void setColorGray(Color const &color);
    void setColorIndex(uint32_t index);

    void setColorRGB(float red, float green, float blue);
    void setColorRGB(Color const &color);
    void setColorRGB(ColorName colorName);

    void setColorCMYK(float cyan, float magenta, float yellow, float black);
    void setColorCMYK(Color const &color);
    void setColorCMYK(ColorName colorName);

    void setColor(Color const &color);

    void setStrokingColorGray(float gray);
    void setStrokingColorGray(Color const &color);
    void setStrokingColorIndex(uint32_t index);

    void setStrokingColorRGB(float red, float green, float blue);
    void setStrokingColorRGB(Color const &color);
    void setStrokingColorRGB(ColorName colorName);

    void setStrokingColorCMYK(float cyan, float magenta, float yellow, float black);
    void setStrokingColorCMYK(Color const &color);
    void setStrokingColorCMYK(ColorName colorName);

    void setStrokingColor(Color const & color);

    // Path construction
    /**
     * Begin a new path/subpath by moving the current point to coordinates (x, y), omitting any connecting line segment.
     */
    void beginPath(float x, float y);

    /**
     * Close the current path by appending a straight line segment from the current point to the starting point of the subpath.
     * @param clippingRule [in, optional] Clipping rule.
     */
    void closePath(PathPaintingOp paintingOp = PathPaintingOpCloseFillStrokeNW);

    /**
     * Close the current subpath by appending a straight line segment from the current point to the starting point of the subpath. If the current subpath is already closed, does nothing.
     */
    void closeSubPath(void);

    /**
     * Append a straight line segment from the current point to the point (x, y). The new current point is (x, y).
     */
    void addLine(float x, float y);

    /**
     * Append a rectangle to the current path as a complete subpath, with lower-left corner (x, y) and dimensions width and height in user space.
     */
    void addRectangle(float x, float y, float width, float height);
    void addRectangle(struct Rectangle const &rect);

    /**
     * Append an ellipse to the current path as a complete subpath, with bounded rectangle in user space.
     */
    void addEllipse(struct Rectangle const &rect);

    /**
     * Append an elliptical arc to the current path as a complete subpath.
     */
    void addArc(struct Rectangle const &rect, float angFrom, float angTo, Point &start, Point &end);

    /**
     * Append a cubic Bezier curve to the current path. The curve extends from the current point to
     * the point (x3 , y3 ), using (x1 , y1 ) and (x2 , y2 ) as the Bezier control points.
     * The new current point is (x3 , y3 ).
     */
    void addBezierCurve12(float x1, float y1, float x2, float y2, float x3, float y3); 

    /**
     * Append a cubic Bezier curve to the current path. The curve extends from the current point to the point (x3 , y3 ), 
     * using the current point and (x2 , y2 ) as the Bezier control points. The new current point is (x3 , y3 ).
     */
    void addBezierCurve02(float x2, float y2, float x3, float y3);

    /**
     * Append a cubic Bezier curve to the current path. The curve extends from the current point to the point (x3 , y3 ),
     * using (x1 , y1 ) and (x3 , y3 ) as the Bezier control points. The new current point is (x3 , y3 ).
     */
    void addBezierCurve13(float x1, float y1, float x3, float y3);

    // XObject
    /**
     * Shows XObject 
     */
    void showXObject(char const *name);

  private:
    ContentStreamImpl(ContentStreamImpl const &) = delete;
    ContentStreamImpl &operator=(ContentStreamImpl const &) = delete;

  private:
    void push(kernel::Operator *cmd);
    void forceTextUnderline(float textWidth, float winDescent, float lineWidth, TransformationMatrix const& tm);

  private:
    std::unique_ptr<kernel::GraphicsState> mCurrentGraphicsState;
    std::stack<std::unique_ptr<kernel::GraphicsState>> mGraphicsStateStack;
    std::deque<std::unique_ptr<kernel::Operator>> mOperators;
    kernel::ResourceManager *mResourceManager;
  };

}}
