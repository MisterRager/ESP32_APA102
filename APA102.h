#include "spi_master.h"

class APA102 {
    public:
        APA102(uint8_t mosiPin, uint8_t clockPin, uint8_t ledsCount);
        ~APA102();
    private:
        spi_bus_config_t busConfig;
        spi_device_interface_config_t deviceConfig;
        spi_device_handle_t deviceHandle;
        bool hasInit = 0;
        void init();
};
