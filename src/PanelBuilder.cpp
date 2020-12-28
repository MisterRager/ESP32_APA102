#include "PanelBuilder.h"
#include "esp_log.h"

namespace apa102 {

#define IS_EVEN(n) (n % 2 == 0)
#define TAG "panel_builder"
PanelBuilder::PanelBuilder(
    size_t span,
    size_t height) : subPanelSpan(span), subPanelHeight(height), subPanelPixels(span * height)
{
}

void PanelBuilder::addSnakes(
    PointIndexMap &addTo,
    const size_t firstIndex,
    const Point2D &deltas,
    const Point2D &offsets) const
{
    Point2D location;
    for (size_t n = firstIndex,
             row = deltas.y > 0 ? 0 : subPanelHeight,
             maxRow = (subPanelPixels + subPanelSpan / 2) / subPanelSpan;
         row < maxRow;
         row += deltas.y)
    {
        for (size_t col = deltas.x > 0 ? 0 : subPanelSpan;
             col < subPanelSpan;
             col += deltas.x, n++)
        {
            location.x = (IS_EVEN(row) ? col : subPanelSpan - col - 1) + offsets.x;
            location.y = row + offsets.y;

            addTo.emplace(location, n);
            ESP_LOGV(TAG, "Added: [%d] at [%d, %d]", n, location.x, location.y);
        }
    }
}

// namespace apa102
}
