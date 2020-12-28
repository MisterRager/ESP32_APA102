#include "APA102Chain.h"

#include <cstring>
#include <esp_log.h>

#define _APA102_PADDING(length) (length > 64 ? ((length + 4) / 8) : 4)
#define _MIN(A, B) ((A < B) ? (A) : (B))
#define TAG "apa102_chain"

namespace apa102 {

APA102Chain::APA102Chain(
    const std::shared_ptr<SpiDevice> device,
    size_t length
) :
    spi(device),
    padding(_APA102_PADDING(length)),
    pixelCount(length)
{
    size_t bufferBytes = APA102_PREAMBLE_BYTES + length * 4 + padding;
    buffer = static_cast<uint8_t *>(
            heap_caps_malloc(sizeof(uint8_t) * bufferBytes, MALLOC_CAP_DMA));
    
    pixelBuffer = reinterpret_cast<APA102Pixel *>(&buffer[APA102_PREAMBLE_BYTES]);
    std::memset(buffer, 0, bufferBytes);
    std::memset(&pixelBuffer[length * 4], 0xFF, padding);

    ESP_LOGV(
        TAG,
        "APA102 Chain of %d length with preamble %d and %d padding totaling to %d bytes",
        pixelCount,
        APA102_PREAMBLE_BYTES,
        padding,
        bufferBytes);
}

APA102Chain::~APA102Chain() {
    free((void *) buffer);
}

esp_err_t APA102Chain::flush() {
    return spi->sendData(APA102_PREAMBLE_BYTES + pixelCount * 4 + padding, buffer);
}

APA102Chain *APA102Chain::build(
    int length,
    uint8_t mosi,
    uint8_t sclk,
    spi_host_device_t spiDevice,
    int baudRate)
{
    return new APA102Chain(
        std::make_shared<SpiDevice>(
            std::make_shared<SpiBus>(
                spiDevice,
                SpiBus::configure(
                    mosi,
                    sclk,
                    (APA102_PREAMBLE_BYTES + length * 4 + _APA102_PADDING(length)) * 8),
                2),
            SpiDevice::configure(baudRate)),
        length);
}

int APA102Chain::setPixel(
    size_t index,
    uint8_t brightness,
    uint8_t blue,
    uint8_t green,
    uint8_t red)
{
    if (index > 0 && index < pixelCount)
    {
        // Brightness is 5 bits of freedom with three bits of 1 right before
        uint8_t brightnessBit = 0b11100000u | _MIN(brightness, 0b11111u);
        ESP_LOGI(TAG, "Brightness %u from %u", brightnessBit, brightness);

        pixelBuffer[index] = {
            .brightness = brightnessBit,
            .blue = blue,
            .green = green,
            .red = red,
        };

        return 0;
    }
    return 1;
}

APA102Pixel *APA102Chain::getPixel(size_t index)
{
    return &pixelBuffer[index];
}

APA102Pixel *APA102Chain::operator[](size_t index)
{
    return getPixel(index);
}

// namespace apa102
}