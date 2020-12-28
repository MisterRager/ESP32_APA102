#ifndef delite_apa102_panelbuilder
#define delite_apa102_panelbuilder

#include "DisplayMap.h"
#include <cstddef>

namespace apa102 {

class PanelBuilder
{
private:
    const size_t subPanelSpan;
    const size_t subPanelHeight;
    const size_t subPanelPixels;

public:
    PanelBuilder(size_t span, size_t height);

    void addSnakes(
        PointIndexMap &addTo,
        const size_t firstIndex,
        const Point2D &deltas,
        const Point2D &offsets) const;
};

// namespace apa102
}

// delite_apa102_panelbuilder
#endif