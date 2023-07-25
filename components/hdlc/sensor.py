import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, uart

from esphome.const import (
    CONF_ACTIVE_POWER,
    CONF_ID,
    CONF_CURRENT,
    CONF_POWER,
    CONF_REACTIVE_POWER,
    CONF_VOLTAGE,
    DEVICE_CLASS_REACTIVE_POWER,
    DEVICE_CLASS_VOLTAGE,
    DEVICE_CLASS_CURRENT,
    DEVICE_CLASS_POWER,
    STATE_CLASS_MEASUREMENT,
    STATE_CLASS_TOTAL_INCREASING,
    UNIT_AMPERE,
    UNIT_KILOWATT_HOURS,
    UNIT_VOLT,
    UNIT_VOLT_AMPS_REACTIVE,
    UNIT_WATT,
)

DEPENDENCIES = ["uart"]

CONF_PHASE_1 = "phase_1"
CONF_PHASE_2 = "phase_2"
CONF_PHASE_3 = "phase_3"

CONF_MOMENTARY = "momentary"
CONF_CUMULATIVE = "cumulative"

CONF_IMPORT = "import"
CONF_EXPORT = "export"

hdlc_ns = cg.esphome_ns.namespace("hdlc")
HDLC = hdlc_ns.class_("HDLC", cg.Component, uart.UARTDevice)

ACTIVE_REACTIVE_SCHEMA = cv.Schema(
    {
        cv.Optional(CONF_ACTIVE_POWER): sensor.sensor_schema(
            unit_of_measurement=UNIT_WATT,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_POWER,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_REACTIVE_POWER): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT_AMPS_REACTIVE,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_REACTIVE_POWER,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
    }
)

ACTIVE_REACTIVE_CUMULATIVE_SCHEMA = cv.Schema(
    {
        cv.Optional(CONF_ACTIVE_POWER): sensor.sensor_schema(
            unit_of_measurement=UNIT_WATT,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_POWER,
            state_class=STATE_CLASS_TOTAL_INCREASING,
        ),
        cv.Optional(CONF_REACTIVE_POWER): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT_AMPS_REACTIVE,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_REACTIVE_POWER,
            state_class=STATE_CLASS_TOTAL_INCREASING,
        ),
    }
)

IMPORT_EXPORT_SCHEMA = {
    cv.Optional(CONF_IMPORT): ACTIVE_REACTIVE_SCHEMA,
    cv.Optional(CONF_EXPORT): ACTIVE_REACTIVE_SCHEMA,
}

HDLC_PHASE_SCHEMA = cv.Schema(
    {
        cv.Optional(CONF_VOLTAGE): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_VOLTAGE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_CURRENT): sensor.sensor_schema(
            unit_of_measurement=UNIT_AMPERE,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_CURRENT,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        **IMPORT_EXPORT_SCHEMA,
    }
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(HDLC),
        cv.Optional(CONF_PHASE_1): HDLC_PHASE_SCHEMA,
        cv.Optional(CONF_PHASE_2): HDLC_PHASE_SCHEMA,
        cv.Optional(CONF_PHASE_3): HDLC_PHASE_SCHEMA,
        cv.Optional(CONF_MOMENTARY): IMPORT_EXPORT_SCHEMA,
        cv.Optional(CONF_CUMULATIVE): cv.Schema(
            {
                cv.Optional(CONF_IMPORT): ACTIVE_REACTIVE_CUMULATIVE_SCHEMA,
                cv.Optional(CONF_EXPORT): ACTIVE_REACTIVE_CUMULATIVE_SCHEMA,
            }
        ),
    }
).extend(uart.UART_DEVICE_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    if CONF_MOMENTARY in config:
        impexp = config[CONF_MOMENTARY]
        if CONF_IMPORT in impexp:
            powertype = impexp[CONF_IMPORT]
            if CONF_ACTIVE_POWER in powertype:
                sens = await sensor.new_sensor(powertype[CONF_ACTIVE_POWER])
                cg.add(var.set_momentary_active_import_sensor(sens))
            if CONF_REACTIVE_POWER in powertype:
                sens = await sensor.new_sensor(powertype[CONF_REACTIVE_POWER])
                cg.add(var.set_momentary_reactive_import_sensor(sens))
        if CONF_EXPORT in impexp:
            powertype = impexp[CONF_EXPORT]
            if CONF_ACTIVE_POWER in powertype:
                sens = await sensor.new_sensor(powertype[CONF_ACTIVE_POWER])
                cg.add(var.set_momentary_active_export_sensor(sens))
            if CONF_REACTIVE_POWER in powertype:
                sens = await sensor.new_sensor(powertype[CONF_REACTIVE_POWER])
                cg.add(var.set_momentary_reactive_export_sensor(sens))

    if CONF_CUMULATIVE in config:
        impexp = config[CONF_CUMULATIVE]
        if CONF_IMPORT in impexp:
            powertype = impexp[CONF_IMPORT]
            if CONF_ACTIVE_POWER in powertype:
                sens = await sensor.new_sensor(powertype[CONF_ACTIVE_POWER])
                cg.add(var.set_cumulative_active_import_sensor(sens))
            if CONF_REACTIVE_POWER in powertype:
                sens = await sensor.new_sensor(powertype[CONF_REACTIVE_POWER])
                cg.add(var.set_cumulative_reactive_import_sensor(sens))
        if CONF_EXPORT in impexp:
            powertype = impexp[CONF_EXPORT]
            if CONF_ACTIVE_POWER in powertype:
                sens = await sensor.new_sensor(powertype[CONF_ACTIVE_POWER])
                cg.add(var.set_cumulative_active_export_sensor(sens))
            if CONF_REACTIVE_POWER in powertype:
                sens = await sensor.new_sensor(powertype[CONF_REACTIVE_POWER])
                cg.add(var.set_cumulative_reactive_export_sensor(sens))

    for i, phase in enumerate([CONF_PHASE_1, CONF_PHASE_2, CONF_PHASE_3]):
        if phase not in config:
            continue
        conf = config[phase]
        if CONF_VOLTAGE in conf:
            sens = await sensor.new_sensor(conf[CONF_VOLTAGE])
            cg.add(var.set_voltage_sensor(i, sens))
        if CONF_CURRENT in conf:
            sens = await sensor.new_sensor(conf[CONF_CURRENT])
            cg.add(var.set_current_sensor(i, sens))
        if CONF_POWER in conf:
            sens = await sensor.new_sensor(conf[CONF_POWER])
            cg.add(var.set_power_sensor(i, sens))
        if CONF_IMPORT in conf:
            powertype = conf[CONF_IMPORT]
            if CONF_ACTIVE_POWER in powertype:
                sens = await sensor.new_sensor(powertype[CONF_ACTIVE_POWER])
                cg.add(var.set_active_import_sensor(i, sens))
            if CONF_REACTIVE_POWER in powertype:
                sens = await sensor.new_sensor(powertype[CONF_REACTIVE_POWER])
                cg.add(var.set_reactive_import_sensor(i, sens))
        if CONF_EXPORT in conf:
            powertype = conf[CONF_EXPORT]
            if CONF_ACTIVE_POWER in powertype:
                sens = await sensor.new_sensor(powertype[CONF_ACTIVE_POWER])
                cg.add(var.set_active_export_sensor(i, sens))
            if CONF_REACTIVE_POWER in powertype:
                sens = await sensor.new_sensor(powertype[CONF_REACTIVE_POWER])
                cg.add(var.set_reactive_export_sensor(i, sens))
