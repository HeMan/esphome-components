import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID

CODEOWNERS = ["@HeMan"]

DEPENDENCIES = ["uart"]
AUTO_LOAD = ["sensor"]

hdlc_ns = cg.esphome_ns.namespace("hdlc")
Hdlc = hdlc_ns.class_("Hdlc", cg.Component, uart.UARTDevice)

CONF_HDLC_ID = "hdlc_id"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(Hdlc),
    }
).extend(uart.UART_DEVICE_SCHEMA)


async def to_config(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
