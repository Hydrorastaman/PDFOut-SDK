#pragma once

#include "Font.h"
#include "Image.h"
#include "Types.h"
#include "Color.h"
#include "UString.h"
#include "ExtGState.h"
#include "ColorSpace.h"
#include "PdfOutConfig.h"
#include "TransformationMatrix.h"

namespace pdfout{

  class PDFOUT_EXPORTS ContentStream{
  protected:
    ContentStream(void) {}
    virtual ~ContentStream(void) {}

  public:
    static float const Pi;

    // Special graphics state
    virtual void save(void) = 0;                                   /**< Save the current graphics state on the graphics state stack. */
    virtual void restore(void) = 0;                                /**< Restore the graphics state by removing the most recently saved state from the stack and making it the current state. */
    virtual void setCTM(TransformationMatrix const &ctm) = 0;      /**< Modify the current transformation matrix. */
    virtual TransformationMatrix getCTM(void) const = 0;

    // General graphics state
    virtual void setLineWidth(float lineWidth) = 0;                /**< Set the line width in the graphics state. */
    virtual void setLineCap(LineCap lineCap) = 0;                  /**< Set the line cap style in the graphics state. */
    virtual void setLineJoin(LineJoin lineJoin) = 0;               /**< Set the line join style in the graphics state. */
    virtual void setMiterLimit(float miterLimit) = 0;              /**< Set the miter limit in the graphics state. */
    virtual void setDashArray(float const *dashPattern, std::size_t count, std::size_t phase) = 0; /**< Set the line dash pattern in the graphics state. */
    virtual void setIntent(RenderingIntent renderingIntent) = 0;   /**< Set the color rendering intent in the graphics state. */
    virtual void setFlatness(float flatness) = 0;                  /**< Set the flatness tolerance in the graphics state. Flatness is a number in the range 0 to 100;
                                                                        a value of 0 specifies the output device's default flatness tolerance. */
    virtual void setExtGState(ExtGState *extGState) = 0;           /**< Set the specified parameters in the ExtGState subdictionary of the current resource dictionary. */

    // Text objects
    virtual void beginText(void) = 0; /**< Begin a text object, initializing the text matrix and the text line matrix to the identity matrix. */
    virtual void endText(void) = 0;   /**< End a text object, discarding the text matrix. */

    // Text state
    virtual void setTextCharSpacing(float charSpacing) = 0;     /**< Set the character spacing, which is a number expressed in unscaled text space units. */
    virtual void setTextWordSpacing(float wordSpacing) = 0;     /**< Set the word spacing, which is a number expressed in unscaled text space units. */
    virtual void setTextScale(float scale) = 0;                 /**< Set the horizontal scaling, scale is a number specifying the percentage of the normal width. */
    virtual void setTextLeading(int32_t leading) = 0;           /**< Set the text leading, which is a number expressed in unscaled text space units. */
    virtual void setTextFont(Font *font, int32_t fontSize) = 0; /**< Set the text font and the text font size, font is the name of a font resource in the Font subdictionary
                                                                     of the current resource dictionary; size is a number representing a scale factor. */
    virtual void setTextRender(TextRenderingMode render) = 0;   /**< Set the text rendering mode to render. */
    virtual void setTextRise(int32_t rise) = 0;                 /**< Set the text rise, which is a number expressed in unscaled text space units. */

    // Text positioning
    virtual void setTextPosition(float x, float y) = 0;                 /**< Move to the start of the next line, offset from the start of the current line by (x , y ). */
    virtual void setTextAbsolutePosition(float x, float y) = 0;         /**< Set text absolute position */
    virtual void setTextPositionAndLeading(float x, float y) = 0;       /**< Move to the start of the next line, offset from the start of the current line by (x , y ).
                                                                             As a side effect, this operator sets the leading parameter in the text state to y. */
    virtual void setTextMatrix(TransformationMatrix const &textMatrix) = 0; /**< Set the text matrix. */
    virtual void setTextNewLine(void) = 0;                                  /**< Move to the start of the next line. */

    // Text showing
    virtual void showText(pdfout::UString const &text) = 0;          /**< Show a text string as the current text position. */
    virtual void showTextAtNewLine(pdfout::UString const &text) = 0; /**< Move to the next line and show a text string. */
    virtual void showTextAtNewLine(float wordSpacing, float charSpacing, pdfout::UString const &text) = 0;  /**< Move to the next line and show a text string, using wordSpace as the word spacing and charSpace as the character spacing */

    /**
     * @brief Shows formatted text in the specified rectangle
     * @param text [in] Text message
     * @param rect [in] Rectangle in which the text is to be formatted
     * @param alignment [in] The method of formatting the text. This parameter can be one or more of the ::Alignment values
     * @param distance [in] The array with specified distance between origins of adjacent glyphs in thousandths of a unit of text space
     * @param count [in] The count of elements in the distance array
     */
    virtual void showText(pdfout::UString const &text, struct Rectangle const &rect, uint16_t alignment, uint16_t decoration = 0) = 0;
    virtual void showText(pdfout::UString const &text, float x, float y, uint16_t alignment, uint16_t decoration = 0, int32_t *distance = nullptr, std::size_t count = 0) = 0;

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
    virtual void showImage(Image *image, float x, float y, float width, float height, float rotate) = 0;
    virtual void showImage(Image *image, TransformationMatrix const &tm) = 0;

    // ColorSpace
    virtual void setColorSpace(ColorSpace *colorSpace) = 0;
    virtual void setStrokingColorSpace(ColorSpace *colorSpace) = 0;

    // Color
    virtual void setColorGray(float gray) = 0;
    virtual void setColorGray(Color const &color) = 0;
    virtual void setColorIndex(uint32_t index) = 0;

    virtual void setColorRGB(float red, float green, float blue) = 0;
    virtual void setColorRGB(Color const &color) = 0;
    virtual void setColorRGB(ColorName colorName) = 0;

    virtual void setColorCMYK(float cyan, float magenta, float yellow, float black) = 0;
    virtual void setColorCMYK(Color const &color) = 0;
    virtual void setColorCMYK(ColorName colorName) = 0;

    virtual void setColor(Color const &color) = 0;

    virtual void setStrokingColorGray(float gray) = 0;
    virtual void setStrokingColorGray(Color const &color) = 0;
    virtual void setStrokingColorIndex(uint32_t index) = 0;

    virtual void setStrokingColorRGB(float red, float green, float blue) = 0;
    virtual void setStrokingColorRGB(Color const &color) = 0;
    virtual void setStrokingColorRGB(ColorName colorName) = 0;

    virtual void setStrokingColorCMYK(float cyan, float magenta, float yellow, float black) = 0;
    virtual void setStrokingColorCMYK(Color const &color) = 0;
    virtual void setStrokingColorCMYK(ColorName colorName) = 0;

    virtual void setStrokingColor(Color const & color) = 0;

    // Path construction
    /**
     * Begin a new path/subpath by moving the current point to coordinates (x, y), omitting any connecting line segment.
     */
    virtual void beginPath(float x, float y) = 0;

    /**
     * Close the current path by appending a straight line segment from the current point to the starting point of the subpath.
     * @param clippingRule [in, optional] Clipping rule.
     */
    virtual void closePath(PathPaintingOp paintingOp = PathPaintingOpCloseFillStrokeNW) = 0;

    /**
     * Close the current subpath by appending a straight line segment from the current point to the starting point of the subpath. If the current subpath is already closed, does nothing.
     */
    virtual void closeSubPath(void) = 0;

    /**
     * Append a straight line segment from the current point to the point (x, y). The new current point is (x, y).
     */
    virtual void addLine(float x, float y) = 0;

    /**
     * Append a rectangle to the current path as a complete subpath, with lower-left corner (x, y) and dimensions width and height in user space.
     */
    virtual void addRectangle(float x, float y, float width, float height) = 0;
    virtual void addRectangle(struct Rectangle const &rect) = 0;

    /**
     * Append an ellipse to the current path as a complete subpath, with bounded rectangle in user space.
     */
    virtual void addEllipse(struct Rectangle const &rect) = 0;

    /**
     * Append an elliptical arc to the current path as a complete subpath.
     */
    virtual void addArc(struct Rectangle const &rect, float angFrom, float angTo, Point &start, Point &end) = 0;

    /**
     * Append a cubic Bezier curve to the current path. The curve extends from the current point to
     * the point (x3 , y3 ), using (x1 , y1 ) and (x2 , y2 ) as the Bezier control points.
     * The new current point is (x3 , y3 ).
     */
    virtual void addBezierCurve12(float x1, float y1, float x2, float y2, float x3, float y3) = 0; 

    /**
     * Append a cubic Bezier curve to the current path. The curve extends from the current point to the point (x3 , y3 ), 
     * using the current point and (x2 , y2 ) as the Bezier control points. The new current point is (x3 , y3 ).
     */
    virtual void addBezierCurve02(float x2, float y2, float x3, float y3) = 0;

    /**
     * Append a cubic Bezier curve to the current path. The curve extends from the current point to the point (x3 , y3 ),
     * using (x1 , y1 ) and (x3 , y3 ) as the Bezier control points. The new current point is (x3 , y3 ).
     */
    virtual void addBezierCurve13(float x1, float y1, float x3, float y3) = 0;

    // XObject
    /**
     * Shows XObject 
     */
    virtual void showXObject(char const *name) = 0;
  };

}

