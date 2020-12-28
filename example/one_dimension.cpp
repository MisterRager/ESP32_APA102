#include "APA102Chain.h"
#include <memory>
#include <cmath>

#define LED_COUNT 32
#define CLK 12
#define DAT 13

uint8_t wave(int index, int period)
{
    return round(fmax(0.0, sin(1.5 * index / period * M_PI)) * 255);
}

int main()
{
    auto lights = std::shared_ptr<apa102::APA102Chain>(apa102::APA102Chain::build(
        LED_COUNT,
        DAT,
        CLK));

    for (int k = 0; k < LED_COUNT; k++)
    {
        lights->setPixel(
            k,
            wave(
                k,
                LED_COUNT),
            wave(
                (k + LED_COUNT / 3) % LED_COUNT,
                LED_COUNT),
            wave(
                (k + LED_COUNT * 2 / 3) % LED_COUNT,
                LED_COUNT));
    }

    lights->flush();
}