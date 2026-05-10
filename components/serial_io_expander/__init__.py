import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import uart
from esphome.const import CONF_ID, CONF_INPUT, CONF_INVERTED, CONF_NUMBER

CONF_SERIAL_IO = "serial_io_expander"


serial_io_expander_ns = cg.esphome_ns.namespace("serial_io_expander")
Serial_io_expanderComponent = serial_io_expander_ns.class_(
    "SerialIOExpander", cg.Component, uart.UARTDevice
)
SerialIOGPIOPin = serial_io_expander_ns.class_(
    "SerialIOGPIOPin", cg.GPIOPin, cg.Parented.template(Serial_io_expanderComponent)
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(Serial_io_expanderComponent),
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
        cv.Required(CONF_SERIAL_IO): cv.use_id(Serial_io_expanderComponent),
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
