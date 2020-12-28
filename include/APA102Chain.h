#ifndef APA102_APA102CHAIN
#define APA102_APA102CHAIN

#include "SpiDevice.h"
#include <memory>
#include "driver/spi_common.h"

#define APA102_PREAMBLE_BYTES 4

namespace apa102 {

typedef struct __attribute__ ((packed)) {
    uint8_t brightness;
    uint8_t blue;
    uint8_t green;
    uint8_t red;
} APA102Pixel;

class APA102Chain {
private:
    std::shared_ptr<SpiDevice> spi;
    const uint8_t padding;
    const size_t pixelCount;
    const size_t byteCount;
    
    uint8_t * buffer;
    APA102Pixel * pixelBuffer;

public:

    APA102Chain(
        std::shared_ptr<SpiDevice> device,
        size_t length);

    ~APA102Chain();

    /**
     * Queue output buffer for display.
     */
    esp_err_t flush();

    /**
     * Convenience factory method for APA102Chain
     */
    static APA102Chain * build(
        int length,
        uint8_t mosi,
        uint8_t sclk,
        spi_host_device_t spiDevice = SPI2_HOST,
        int baudRate = 1000000);

    /**
     * Doesn't actually write to the lights - update the state of the output buffer.
     */
    int setPixel(
        size_t index,
        uint8_t brightness,
        uint8_t blue,
        uint8_t green,
        uint8_t red);

    /**
     * For reading/writing the buffer as individual RGB units, provide an index
     * and recive an APA102Pixel pointer.
     */
    APA102Pixel * getPixel(size_t index);

    /**
     * More convenient syntax for #getPixel
     */
    APA102Pixel * operator[](size_t index);
};

// namespace apa102
}
#endif //APA102_APA102CHAIN