#pragma once

#include <memory>
#include <cstdint>

#include <InternalTools.h>

namespace pdfout{

  class OutputStream;

}

namespace kernel{ namespace font{ namespace truetype{

  enum OutlineFlag{
    OutlineFlagOnCurve = 0x1,                /**< If set, the point is on the curve; Otherwise, it is off the curve. */
    OutlineFlagXShortVector = 0x2,           /**< If set, the corresponding x-coordinate is 1 byte long; Otherwise, the corresponding x-coordinate is 2 bytes long */
    OutlineFlagYShortVector = 0x4,           /**< If set, the corresponding y-coordinate is 1 byte long; Otherwise, the corresponding y-coordinate is 2 bytes long */
    OutlineFlagRepeat = 0x8,                 /**< If set, the next byte specifies the number of additional times this set of flags is to be repeated.
                                                  In this way, the number of flags listed can be smaller than the number of points in a character. */
    OutlineFlagThisXIsSame = 0x10,           /**< This flag has one of two meanings, depending on how the x-Short Vector flag is set.
                                                  If the x-Short Vector bit is set, this bit describes the sign of the value, with a value of 1 equalling positive and a zero value negative.
                                                  If the x-short Vector bit is not set, and this bit is set, then the current x-coordinate is the same as the previous x-coordinate.
                                                  If the x-short Vector bit is not set, and this bit is not set, the current x-coordinate is a signed 16-bit delta vector. In this case, the delta vector is the change in x. */
    OutlineFlagThisYIsSame = 0x20            /**< This flag has one of two meanings, depending on how the y-Short Vector flag is set.
                                                  If the y-Short Vector bit is set, this bit describes the sign of the value, with a value of 1 equalling positive and a zero value negative.
                                                  If the y-short Vector bit is not set, and this bit is set, then the current y-coordinate is the same as the previous y-coordinate.
                                                  If the y-short Vector bit is not set, and this bit is not set, the current y-coordinate is a signed 16-bit delta vector. In this case, the delta vector is the change in y. */
  };

  enum ComponentFlag{
    ComponentFlagArg1And2AreWords = 0x1,          /**< If set, the arguments are words. If not set, they are bytes. */
    ComponentFlagArgsAreXYValues = 0x2,           /**< If set, the arguments are xy values. If not set, they are points. */
    ComponentFlagroundXYToGrid = 0x4,             /**< If set, round the xy values to grid. If not set do not round xy values to grid (relevant only to bit 1 is set) */
    ComponentFlagWeHaveAScale = 0x8,              /**< If set, there is a simple scale for the component. If not set, scale is 1.0. */
    ComponentFlagMoreComponents = 0x20,           /**< If set, at least one additional glyph follows this one. */
    ComponentFlagWeHaveAnXAndYScale = 0x40,       /**< If set the x direction will use a different scale than the y direction. */
    ComponentFlagWeHaveATwoByTwo = 0x80,          /**< If set there is a 2-by-2 transformation that will be used to scale the component. */
    ComponentFlagWeHaveInstructions = 0x100,      /**< If set, instructions for the component character follow the last component. */
    ComponentFlagUseMyMetrics = 0x200,            /**< Use metrics from this component for the compound glyph. */
    ComponentFlagOverlapCompound = 0x400,         /**< If set, the components of this compound glyph overlap. */
    ComponentFlagScaledComponentOffset = 0x800,   /**< Composite designed to have the component offset scaled (designed for Apple rasterizer). */
    ComponentFlagUnscaledComponentOffset = 0x1000 /**< Composite designed not to have the component offset scaled (designed for the Microsoft TrueType rasterizer). */
  };

  PACK(struct GlyphDescription{
    int16_t mNumberOfContours; /**< If the number of contours is positive or zero, it is a single glyph; If the number of contours less than zero, the glyph is compound */
    int16_t mXMin;             /**< Minimum x for coordinate data */
    int16_t mYMin;             /**< Minimum y for coordinate data */
    int16_t mXMax;             /**< Maximum x for coordinate data */
    int16_t mYMax;             /**< Maximum y for coordinate data */
  });

  enum GlyphType{
    GlyphTypeSimple = 0,
    GlyphTypeCompound
  };

  class Glyph{
  public:
    Glyph(GlyphType type);
    virtual ~Glyph(void);

    GlyphType getType(void) const;

    virtual void save(pdfout::OutputStream *stream) const = 0;
    virtual std::unique_ptr<Glyph> clone(std::vector<uint16_t> const &gids) const = 0;

    virtual GlyphDescription const &getGlyphDescription(void) const = 0;

  protected:
    Glyph(Glyph const &obj);

  private:
    Glyph &operator=(Glyph const &) = delete;

  private:
    GlyphType mType;
  };

}}}
