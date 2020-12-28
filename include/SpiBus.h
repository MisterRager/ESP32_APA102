#ifndef APA102CHAIN_SPIBUS_H
#define APA102CHAIN_SPIBUS_H

#include "driver/spi_common.h"

namespace apa102 {

class SpiBus {
public:
    const spi_host_device_t spiDevice;

    SpiBus(
        const spi_host_device_t spiDevice,
        const spi_bus_config_t &config,
        const int dmaChannel);

    ~SpiBus();

    static spi_bus_config_t configure(int mosi, int sclk, int maxTransferSize);
};

// namespace apa102
}

#endif //APA102CHAIN_SPIBUS_H
