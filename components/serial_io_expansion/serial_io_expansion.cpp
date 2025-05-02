#include "serial_io_expansion.h"
#include "esphome/core/log.h"

namespace esphome {
namespace serial_io_expansion {

static const char *const TAG = "serial_io_expansion";

void SerialIOExpansion::loop() {
    while (this->available()) {
        this->read_byte(&this->last_read_);
        ESP_LOGVV(TAG, "Got data: 0x%02x", this->last_read_);
    }
}

bool SerialIOExpansion::digital_read(uint8_t pin_) {
    return this->last_read_ && (1<<pin_);
}

float SerialIOExpansion::get_setup_priority() const { return setup_priority::IO; }
void SerialIOExpansion::dump_config() { ESP_LOGCONFIG(TAG, "Serial io"); }
std::string SerialIOGPIOPin::dump_summary() const { return str_sprintf("SERIO%u via serial_io_expansion", pin_); }

} // namespace serial_io_expansion
} // namespace esphome

