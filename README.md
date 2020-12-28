# APA102 Library For ESP-IDF on ESP32

This is a quick lib for controlling groups of daisy-chained APA102 lights on ESP32 inside of the ESP-IDF framework. I haven't seen one that covers that, so here it is.



## Version 1.0 Update!

A rewrite dropped in from another project has been given version `1.0.0`. It features more correct handling of the underlying esp-idf API's, a namespace and an attempt at cleaning up system resources when done. The code is incompatible with previous versions, though.



## Controlling a Simple Strip

A factory method, `APA102Chain::build` will construct an object that will give full 1-dimensional control of an APA102-like chain of lights, taking care of building the required `SpiBus` and `SpiDevice` objects needed. Unless you plan on connecting more than one strip of lights, this should be sufficient.

```
#include "APA102Chain.h"

#define LED_COUNT 32
#define CLK 12
#define DAT 13

auto lights = std::shared_ptr<apa102::APA102Chain>(apa102::APA102Chain::build(
    LED_COUNT,
    DAT,
    CLK));
```



## More Complex Arrays
    
A rough 2d addressing scheme is built on top with `DisplayMap`.

```
#define DAT 13
#define CLK 12
#define BRIGHTNESS 31

auto lights = std::shared_ptr<APA102Chain>(
    APA102Chain::build(LED_COUNT, DAT, CLK));

PointIndexMap screenMap;
DisplayMap screen(&screenMap, new APA102Writer(BRIGHTNESS, lights));
```



## For "Snaked" 2D Arrays

The type `PointIndexMap` is an alias of `std::map<Point2D, int, compare_Point2D>` - it maps from locations in 2d to points in 1d. A helper class, `PanelBuilder`, will add points to this map for a panel that alternates row order in "snake" fashion:

```
#define SPAN 8

PanelBuilder empaneler(SPAN, SPAN);
PointPixelMap screenMap;

empaneler.addSnakes(
    screenMap,
    0, // start with offset 0
    {
        .x = 1, // As index increases, so does X
        .y = 1, // As index increases, so does Y
    },
    {
        .x = 0, // Start with the origin at the first pixel
        .y = 0, // Start with the origin at the first pixel
    });


empaneler.addSnakes(
    screenMap,
    SPAN * SPAN, // next panel, start with SPAN * SPAN offset
    {
        .x = 1, // As index increases, so does X
        .y = 1, // As index increases, so does Y
    },
    {
        .x = SPAN, // Second panel to the right of the First, offset by SPAN
        .y = 0,    // Second panel tot he right of the First, same height
    });
```
