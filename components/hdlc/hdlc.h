//-------------------------------------------------------------------------------------
// ESPHome P1 Electricity Meter custom sensor
// Copyright 2020 Pär Svanström
//
// History
//  0.1.0 2020-11-05:   Initial release
//
// MIT License
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-------------------------------------------------------------------------------------

#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace hdlc {
#define BUF_SIZE 60
// TODO: struct and array

  struct ParsedMessage {
    struct ParsedPhase {
      double active_import;
      double active_export;

      double reactive_import;
      double reactive_export;

      double voltage;

      double current;
    };
    double cumulativeActiveImport;
    double cumulativeActiveExport;

    double cumulativeReactiveImport;
    double cumulativeReactiveExport;

    double momentaryActiveImport;
    double momentaryActiveExport;

    double momentaryReactiveImport;
    double momentaryReactiveExport;
    std::array<ParsedPhase, 3> phases;

};

struct PhaseSensors {
    sensor::Sensor *voltage_sensor_{nullptr};
    sensor::Sensor *current_sensor_{nullptr};
    sensor::Sensor *active_import_sensor_{nullptr};
    sensor::Sensor *active_export_sensor_{nullptr};
    sensor::Sensor *reactive_import_sensor_{nullptr};
    sensor::Sensor *reactive_export_sensor_{nullptr};
};

class HDLC : public Component, public uart::UARTDevice {
  public:
    void set_voltage_sensor(uint8_t phase, sensor::Sensor *voltage_sensor) { phase_sensors[phase].voltage_sensor_ = voltage_sensor; }
    void set_current_sensor(uint8_t phase, sensor::Sensor *current_sensor) { phase_sensors[phase].current_sensor_ = current_sensor; }
    void set_active_import_sensor(uint8_t phase, sensor::Sensor *active_import_sensor) { phase_sensors[phase].active_import_sensor_ = active_import_sensor; }
    void set_active_export_sensor(uint8_t phase, sensor::Sensor *active_export_sensor) { phase_sensors[phase].active_export_sensor_ = active_export_sensor; }
    void set_reactive_import_sensor(uint8_t phase, sensor::Sensor *reactive_import_sensor) { phase_sensors[phase].reactive_import_sensor_ = reactive_import_sensor; }
    void set_reactive_export_sensor(uint8_t phase, sensor::Sensor *reactive_export_sensor) { phase_sensors[phase].reactive_export_sensor_ = reactive_export_sensor; }

    void set_cumulative_active_import_sensor(sensor::Sensor *cumulative_active_import_sensor) { cumulative_active_import_sensor_ = cumulative_active_import_sensor; }
    void set_cumulative_active_export_sensor(sensor::Sensor *cumulative_active_export_sensor) { cumulative_active_export_sensor_ = cumulative_active_export_sensor; }

    void set_cumulative_reactive_import_sensor(sensor::Sensor *cumulative_reactive_import_sensor) { cumulative_reactive_import_sensor_ = cumulative_reactive_import_sensor; }
    void set_cumulative_reactive_export_sensor(sensor::Sensor *cumulative_reactive_export_sensor) { cumulative_reactive_export_sensor_ = cumulative_reactive_export_sensor; }

    void set_momentary_active_import_sensor(sensor::Sensor *momentary_active_import_sensor) { momentary_active_import_sensor_ = momentary_active_import_sensor; }
    void set_momentary_active_export_sensor(sensor::Sensor *momentary_active_export_sensor) { momentary_active_export_sensor_ = momentary_active_export_sensor; }

    void set_momentary_reactive_import_sensor(sensor::Sensor *momentary_reactive_import_sensor) { momentary_reactive_import_sensor_ = momentary_reactive_import_sensor; }
    void set_momentary_reactive_export_sensor(sensor::Sensor *momentary_reactive_export_sensor) { momentary_reactive_export_sensor_ = momentary_reactive_export_sensor; }

    void setup() override;
    void loop() override;
    void dump_config() override;
  protected:
    std::array<PhaseSensors, 3> phase_sensors;

    /* Cumulative */
    sensor::Sensor *cumulative_active_import_sensor_{nullptr};
    sensor::Sensor *cumulative_active_export_sensor_{nullptr};

    sensor::Sensor *cumulative_reactive_import_sensor_{nullptr};
    sensor::Sensor *cumulative_reactive_export_sensor_{nullptr};

    sensor::Sensor *momentary_active_import_sensor_{nullptr};
    sensor::Sensor *momentary_active_export_sensor_{nullptr};

    sensor::Sensor *momentary_reactive_import_sensor_{nullptr};
    sensor::Sensor *momentary_reactive_export_sensor_{nullptr};


  private:
    void parseRow(ParsedMessage* parsed, char* obisCode, double value);

    void publishSensors(ParsedMessage* parsed);

    bool readHDLCStruct(ParsedMessage* parsed);

    void readHDLCMessage();

    uint8_t buffer[BUF_SIZE];

    const int8_t OUTSIDE_FRAME = 0;
    const int8_t FOUND_FRAME = 1;


    int8_t parseHDLCState = OUTSIDE_FRAME;

};

} // namespace hdlc
} // namespace esphome
