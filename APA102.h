#include "spi_master.h"
#include "esp_err.h"
#include "esp_heap_caps.h"

typedef struct PixelInfo {
    uint8_t brightness;
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} PixelInfo;

class APA102 {
    public:
        APA102(gpio_num_t mosiPin, gpio_num_t clockPin, size_t ledsCount);
        ~APA102();
        void flush();
        void setPixel(uint8_t index, PixelInfo pixel);
    private:
        uint8_t count;

        spi_bus_config_t busConfig;
        spi_device_interface_config_t deviceConfig;
        spi_device_handle_t deviceHandle;
        spi_transaction_t transaction;

        bool hasInit = 0;
        uint8_t bufferSize;
        uint8_t * outputBuffer;
};
