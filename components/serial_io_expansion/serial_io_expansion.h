#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/core/defines.h"
#include "esphome/core/hal.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace serial_io_expansion {
class SerialIOExpansion : public Component, public uart::UARTDevice {
  public:

    void setup() override {};
    void loop() override;

    float get_setup_priority() const override;
    void dump_config() override;
    bool digital_read(uint8_t pin);


    void pin_mode(uint8_t pin, gpio::Flags flags);
  protected:
    uint8_t last_read_{0};

};

class SerialIOGPIOPin: public GPIOPin, public Parented<SerialIOExpansion> {
  public:
    void setup() override {};
    void set_pin(uint8_t pin) { pin_ = pin; }
    void set_inverted(bool inverted) { inverted_ = inverted; }
    bool digital_read() override { return this->parent_->digital_read(this->pin_) ^ this->inverted_; }
    gpio::Flags get_flags() const override { return gpio::Flags::FLAG_INPUT; }
    void digital_write(bool value) override {};
    std::string dump_summary() const override;
    void pin_mode(gpio::Flags flags) override {};

  protected:
    uint8_t pin_{};
    bool inverted_;
};

}  // namespace serial_io_expansion
}  // namespace esphome
