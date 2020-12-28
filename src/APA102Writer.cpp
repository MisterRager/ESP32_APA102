#include "APA102Writer.h"
#include "esp_log.h"

#define TAG "apa102_writer"

namespace apa102 {

APA102Writer::APA102Writer(
    uint8_t brightness,
    std::shared_ptr<APA102Chain> apa102) : brightness(brightness), apa102(apa102)
{
}

APA102Writer::~APA102Writer()
{
}

int APA102Writer::writePixel(const size_t index, const ColorRGB &color)
{
    return apa102->setPixel(
        index,
        brightness,
        color.blue,
        color.green,
        color.red
    );
}

ColorRGB APA102Writer::readPixel(const size_t index)
{
    auto pixel = apa102->operator[](index);

    return {
        .red = pixel->red,
        .green = pixel->green,
        .blue = pixel->blue,
    };
}

// namespace apa102
}