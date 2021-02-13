#pragma once

#include "Function.h"
#include "PdfOutConfig.h"

namespace pdfout{

  enum Colorant{
    ColorantGray,
    ColorantRed,
    ColorantGreen,
    ColorantBlue,
    ColorantCyan,
    ColorantMagenta,
    ColorantYellow,
    ColorantBlack
  };

  class PDFOUT_EXPORTS Halftone{
  protected:
    Halftone(void) {}

  public:
    virtual ~Halftone(void) {}

    /**
     * Creates a Halftone type 1 dictionary
     * @param [in, optional] halftoneName The name of the halftone dictionary.
     * @param [in, required] frequency The screen frequency, measured in halftone cells per inch in device space.
     * @param [in, required] angle The screen angle, in degrees of rotation counterclockwise with respect to the device coordinate system.
     * @param [in, required] spotFunction A function object defining the order in which device pixels within a screen cell are adjusted for different gray levels.
     * @param [in, optional] accurateScreens A flag specifying whether to invoke a special halftone algorithm that is extremely precise but computationally expensive.
     * @param [in, optional] transferFunction A transfer function, which overrides the current transfer function in the graphics state for the same component.
     *                                        This entry is required if the dictionary is a component of a type 5 halftone and represents either a nonprimary or nonstandard primary color component.
     */
    static Halftone *createHalftoneType1(char const *halftoneName, float frequency, float angle, Function *spotFunction, bool accurateScreens, Function *transferFunction);

    /**
     * Creates a Halftone type 6 dictionary
     * @param [in, optional] halftoneName The name of the halftone dictionary.
     * @param [in, required] width The width of the threshold array, in device pixels.
     * @param [in, required] height The height of the threshold array, in device pixels.
     * @param [in, optional] transferFunction A transfer function, which overrides the current transfer function in the graphics state for the same component.
                                              This entry is required if the dictionary is a component of a type 5 halftone and represents either a nonprimary or nonstandard primary color component.
     */
    static Halftone *createHalftoneType6(char const *halftoneName, int width, int height, Function *transferFunction);

    /**
     * Creates a Halftone type 10 dictionary
     * @param [in, optional] halftoneName The name of the halftone dictionary.
     * @param [in, required] xSquare The side of square X, in device pixels.
     * @param [in, required] ySquare The side of square Y, in device pixels.
     * @param [in, optional] transferFunction A transfer function, which overrides the current transfer function in the graphics state for the same component.
                                              This entry is required if the dictionary is a component of a type 5 halftone and represents either a nonprimary or nonstandard primary color component.
     */
    static Halftone *createHalftoneType10(char const *halftoneName, int xSquare, int ySquare, Function *transferFunction);

    /**
     * Creates a Halftone type 16 dictionary
     * @param [in, optional] halftoneName The name of the halftone dictionary.
     * @param [in, required] width The width of the first (or only) rectangle in the threshold array, in device pixels.
     * @param [in, required] height The height of the first (or only) rectangle in the threshold array, in device pixels.
     * @param [in, optional] width2 The width of the optional second rectangle in the threshold array, in device pixels. Set to 0 for skipping.
     * @param [in, optional] height2 The height of the optional second rectangle in the threshold array, in device pixels. Set to 0 for skipping.
     * @param [in, optional] transferFunction A transfer function, which overrides the current transfer function in the graphics state for the same component.
                                              This entry is required if the dictionary is a component of a type 5 halftone and represents either a nonprimary or nonstandard primary color component.
     */
    static Halftone *createHalftoneType16(char const *halftoneName, int width, int height, int width2, int height2, Function *transferFunction);

    /**
     * Creates a Halftone type 5 dictionary
     * @param [in, optional] halftoneName The name of the halftone dictionary.
     * @param [in, required] colorants The set of colorants.
     * @param [in, required] halftones The halftone corresponding to the colorant or color component named by the key. The halftone may be of any type other than 5.
     * @param [in, required] colorantsSize Count of colorants.
     * @param [in, required] default A halftone to be used for any colorant or color component that does not have an entry of its own.
                                     The value may not be a type 5 halftone. If there are any nonprimary colorants, the default halftone must have a transfer function.
     */
    static Halftone *createHalftoneType5(char const *halftoneName, Colorant *colorants, Halftone **halftones, std::size_t colorantsSize, Halftone *default);

  private:
    Halftone(Halftone const &obj);
    Halftone &operator=(Halftone const &obj);
  };
}
