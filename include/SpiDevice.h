#ifndef APA102CHAIN_SPIDEVICE_H
#define APA102CHAIN_SPIDEVICE_H

#include <mutex>
#include <vector>
#include <memory>

#include "driver/spi_master.h"
#include "SpiBus.h"

#include "esp_attr.h"

namespace apa102 {

class SpiDevice {
private:
public:
    spi_device_handle_t spi;
    static void IRAM_ATTR postTransactionHook(spi_transaction_t * transaction);
    std::shared_ptr<SpiBus> bus;

    SpiDevice(
        std::shared_ptr<SpiBus> bus,
        const spi_device_interface_config_t &config);

    ~SpiDevice();

    static spi_device_interface_config_t configure(int speed = 1000000);

    esp_err_t sendData(size_t length, const uint8_t * buffer);
};

// namespace apa102
}

#endif //APA102CHAIN_SPIDEV_H