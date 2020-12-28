#include "DisplayMap.h"

namespace apa102 {

DisplayMap::DisplayMap(
    const std::shared_ptr<PointIndexMap> locationMap,
    const std::shared_ptr<DisplayWriter> writer) : locationMap(locationMap), writer(writer)
{
}

DisplayMap::DisplayMap(
    PointIndexMap *locationMap,
    DisplayWriter *writer) : locationMap(locationMap), writer(writer)
{
}

int DisplayMap::writePixel(const Point2D &location, const ColorRGB &color)
{

    if (locationMap->count(location))
    {
        return writer->writePixel(locationMap->at(location), color);
    }

    return 1;
}

int DisplayMap::blendPixel(const Point2D &location, const ColorRGBA &color)
{
    if (locationMap->count(location))
    {
        auto where = locationMap->at(location);

        return writer->writePixel(
            where,
            compositePixel(color, writer->readPixel(where)));
    }

    return 1;
}

uint8_t DisplayMap::compositeColor(const uint8_t paint, const uint8_t alpha, const uint8_t onto)
{
    return paint * alpha + onto * (1 - alpha);
}

ColorRGB DisplayMap::compositePixel(const ColorRGBA paint, const ColorRGB onto)
{
    return {
        .red = compositeColor(paint.red, paint.alpha, onto.red),
        .green = compositeColor(paint.green, paint.alpha, onto.green),
        .blue = compositeColor(paint.blue, paint.alpha, onto.blue),
    };
}

// namespace apa102
}
