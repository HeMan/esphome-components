from esphome import pins
from esphome.components import uart
import esphome.codegen as cg

from esphome.const import CONF_ID, CONF_INPUT, CONF_NUMBER, CONF_INVERTED

import esphome.config_validation as cv

CONF_SERIAL_IO = "serial_io_expansion"


serial_io_expansion_ns = cg.esphome_ns.namespace("serial_io_expansion")
Serial_io_expansionComponent = serial_io_expansion_ns.class_(
    "SerialIOExpansion", cg.Component, uart.UARTDevice
)
SerialIOGPIOPin = serial_io_expansion_ns.class_(
    "SerialIOGPIOPin", cg.GPIOPin, cg.Parented.template(Serial_io_expansionComponent)
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(Serial_io_expansionComponent),
    }
).extend(uart.UART_DEVICE_SCHEMA)


def _validate_input_mode(value):
    if value.get(CONF_INPUT) is not True:
        raise cv.Invalid("Only input mode is supported")
    return value


SERIAL_IO_PIN_SCHEMA = pins.gpio_base_schema(
    SerialIOGPIOPin,
    cv.int_range(min=0, max=7),
    modes=[CONF_INPUT],
    invertable=True,
    mode_validator=_validate_input_mode,
).extend(
    {
        cv.Required(CONF_SERIAL_IO): cv.use_id(Serial_io_expansionComponent),
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)


@pins.PIN_SCHEMA_REGISTRY.register(CONF_SERIAL_IO, SERIAL_IO_PIN_SCHEMA)
async def serial_pin_to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_parented(var, config[CONF_SERIAL_IO])

    cg.add(var.set_pin(config[CONF_NUMBER]))
    cg.add(var.set_inverted(config[CONF_INVERTED]))
    return var
