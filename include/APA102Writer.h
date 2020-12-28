#ifndef delite_apa102_writer
#define delite_apa102_writer

#include "displaymap.h"
#include "APA102Chain.h"

#include <memory>

namespace apa102 {

class APA102Writer : public DisplayWriter
{
private:
    uint8_t brightness;
    std::shared_ptr<APA102Chain> apa102;

public:
    APA102Writer(
        uint8_t brightness,
        std::shared_ptr<APA102Chain> apa102
    );

    ~APA102Writer();

    // override
    int writePixel(const size_t index, const ColorRGB &color);
    ColorRGB readPixel(const size_t index);
};

// namespace apa102
}

//delite_apa102_writer
#endif
