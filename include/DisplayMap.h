#ifndef delite_displaymap
#define delite_displaymap

#include <cstddef>
#include <cstdint>
#include <map>
#include <vector>
#include <memory>

namespace apa102 {

typedef struct
{
    size_t x;
    size_t y;
} Point2D;

struct compare_Point2D
{
    bool operator()(const Point2D &left, const Point2D &right) const
    {
        return (left.y > right.y) ||
               ((left.y == right.y) && (left.x > right.x));
    }
};

typedef struct
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} ColorRGB;

typedef struct
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
} ColorRGBA;

class DisplayWriter
{
public:
    virtual ~DisplayWriter() { };
    virtual int writePixel(const size_t index, const ColorRGB &color) = 0;
    virtual ColorRGB readPixel(const size_t index) = 0;
};

using PointIndexMap = std::map<Point2D, int, compare_Point2D>;

class DisplayMap
{
private:
    const std::shared_ptr<PointIndexMap> locationMap;
    const std::shared_ptr<DisplayWriter> writer;

    static uint8_t compositeColor(const uint8_t paint, const uint8_t alpha, const uint8_t onto);
    static ColorRGB compositePixel(const ColorRGBA paint, const ColorRGB onto);

public:
    DisplayMap(
        const std::shared_ptr<PointIndexMap> locationMap,
        const std::shared_ptr<DisplayWriter> writer);

    DisplayMap(
        PointIndexMap * locationMap,
        DisplayWriter * writer);

    int writePixel(const Point2D &location, const ColorRGB &color);
    int blendPixel(const Point2D &location, const ColorRGBA &color);
};

// namespace apa102
}

//delite_displaymap
#endif