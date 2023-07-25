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
#include "hdlc.h"
#include "esphome/core/log.h"

namespace esphome {
namespace hdlc {
static const char *const TAG = "hdlc";
void HDLC::setup() {
    ESP_LOGD(TAG, "In setup");
}

void HDLC::loop() {
    readHDLCMessage();
}
void HDLC::parseRow(ParsedMessage* parsed, char* obisCode, double value) {
    ESP_LOGV("parseRow", "OBIS %s value %f", obisCode, value);
    if (strncmp(obisCode, "1.8.0", 5) == 0) {
        parsed->cumulativeActiveImport = value;
    }
    else if (strncmp(obisCode, "2.8.0", 5) == 0) {
        parsed->cumulativeActiveExport = value;
    }
    else if (strncmp(obisCode, "3.8.0", 5) == 0) {
        parsed->cumulativeReactiveImport = value;
    }
    else if (strncmp(obisCode, "4.8.0", 5) == 0) {
        parsed->cumulativeReactiveExport = value;
    }
    else if (strncmp(obisCode, "1.7.0", 5) == 0) {
        parsed->momentaryActiveImport = value;
    }
    else if (strncmp(obisCode, "2.7.0", 5) == 0) {
        parsed->momentaryActiveExport = value;
    }
    else if (strncmp(obisCode, "3.7.0", 5) == 0) {
        parsed->momentaryReactiveImport = value;
    }
    else if (strncmp(obisCode, "4.7.0", 5) == 0) {
        parsed->momentaryReactiveExport = value;
    }
    else if (strncmp(obisCode, "21.7.0", 6) == 0) {
        parsed->phases[0].active_import = value;
    }
    else if (strncmp(obisCode, "22.7.0", 6) == 0) {
        parsed->phases[0].active_export = value;
    }
    else if (strncmp(obisCode, "41.7.0", 6) == 0) {
        parsed->phases[1].active_import = value;
    }
    else if (strncmp(obisCode, "42.7.0", 6) == 0) {
        parsed->phases[1].active_export = value;
    }
    else if (strncmp(obisCode, "61.7.0", 6) == 0) {
        parsed->phases[2].active_import = value;
    }
    else if (strncmp(obisCode, "62.7.0", 6) == 0) {
        parsed->phases[2].active_export = value;
    }
    else if (strncmp(obisCode, "23.7.0", 6) == 0) {
        parsed->phases[0].reactive_import = value;
    }
    else if (strncmp(obisCode, "24.7.0", 6) == 0) {
        parsed->phases[0].reactive_export = value;
    }
    else if (strncmp(obisCode, "43.7.0", 6) == 0) {
        parsed->phases[1].reactive_import = value;
    }
    else if (strncmp(obisCode, "44.7.0", 6) == 0) {
        parsed->phases[1].reactive_export = value;
    }
    else if (strncmp(obisCode, "63.7.0", 6) == 0) {
        parsed->phases[2].reactive_import = value;
    }
    else if (strncmp(obisCode, "64.7.0", 6) == 0) {
        parsed->phases[2].reactive_export = value;
    }
    else if (strncmp(obisCode, "32.7.0", 6) == 0) {
        parsed->phases[0].voltage = value;
    }
    else if (strncmp(obisCode, "52.7.0", 6) == 0) {
        parsed->phases[1].voltage = value;
    }
    else if (strncmp(obisCode, "72.7.0", 6) == 0) {
        parsed->phases[2].voltage = value;
    }
    else if (strncmp(obisCode, "31.7.0", 6) == 0) {
        parsed->phases[0].current = value;
    }
    else if (strncmp(obisCode, "51.7.0", 6) == 0) {
        parsed->phases[1].current = value;
    }
    else if (strncmp(obisCode, "71.7.0", 6) == 0) {
        parsed->phases[2].current = value;
    }
}


void HDLC::publishSensors(ParsedMessage* parsed) {
    cumulative_active_import_sensor_->publish_state(parsed->cumulativeActiveImport);
    cumulative_active_export_sensor_->publish_state(parsed->cumulativeActiveExport);
    cumulative_reactive_import_sensor_->publish_state(parsed->cumulativeReactiveImport);
    cumulative_reactive_export_sensor_->publish_state(parsed->cumulativeReactiveExport);
    momentary_active_import_sensor_->publish_state(parsed->momentaryActiveImport);
    momentary_active_export_sensor_->publish_state(parsed->momentaryActiveExport);
    momentary_reactive_import_sensor_->publish_state(parsed->momentaryReactiveImport);
    momentary_reactive_export_sensor_->publish_state(parsed->momentaryReactiveExport);
    for (uint8_t i=0; i < 3; i++) {
        phase_sensors[i].voltage_sensor_->publish_state(parsed->phases[i].voltage); 
        phase_sensors[i].current_sensor_->publish_state(parsed->phases[i].current); 
        phase_sensors[i].active_import_sensor_->publish_state(parsed->phases[i].active_import); 
        phase_sensors[i].reactive_import_sensor_->publish_state(parsed->phases[i].reactive_import); 
        phase_sensors[i].active_export_sensor_->publish_state(parsed->phases[i].active_export); 
        phase_sensors[i].reactive_export_sensor_->publish_state(parsed->phases[i].reactive_export); 
    }
}


bool HDLC::readHDLCStruct(ParsedMessage* parsed) {
    if (!read_array(buffer, 3))
        return false;

    if (buffer[0] != 0x02) {
        return false;
    }

    char obis[12];

    uint8_t struct_len = buffer[1];
    ESP_LOGV("hdlc", "Struct length is %d", struct_len);

    uint8_t tag = buffer[2];

    if (tag != 0x09) {
        ESP_LOGE("hdcl", "Unexpected tag %X in struct, bailing out", tag);
        return false;
    }

    uint8_t str_length = read();
    if (!read_array(buffer, str_length)) {
        ESP_LOGE("hdlc", "Unable to read %d bytes of OBIS code", str_length);
        return false;
    }
    buffer[str_length] = 0; // Null-terminate
    sprintf(obis, "%d.%d.%d", buffer[2], buffer[3], buffer[4]);

    tag = read();

    bool is_signed = false;
    uint32_t uvalue = 0;
    int32_t value = 0;
    if (tag == 0x09) {
        str_length = read();
        if (!read_array(buffer, str_length)) {
            ESP_LOGE("hdlc", "Unable to read %d bytes of string", str_length);
            return false;
        }

        buffer[str_length] = 0;
        //ESP_LOGD("hdlc", "Read string length %d", str_length);
    }
    else if(tag == 0x06) {
        read_array(buffer, 4);
        uvalue = buffer[3] | buffer[2] << 8 | buffer[1] << 16 | buffer[0] << 24;
        //ESP_LOGD("hdlc", "Value of uvalue is %u", uvalue);
    }
    else if (tag == 0x10) {
        read_array(buffer, 2);
        is_signed = true;
        value = buffer[1] | buffer[0] << 8;
        //ESP_LOGD("hdlc", "(Signed) Value of value is %d", value);
    }
    else if (tag == 0x12) {
        read_array(buffer, 2);
        uvalue = buffer[1] | buffer[0] << 8;
        //ESP_LOGD("hdlc", "(Unsigned) Value of uvalue is %u", uvalue);
    }
    else {
        ESP_LOGE("hdlc", "unknown tag %X", tag);
    }

    int8_t scaler;
    uint8_t unit;
    if (struct_len == 3) {
        read_array(buffer, 6);
        scaler = buffer[3];
        unit = buffer[5];
        //ESP_LOGD("hdlc", "Scaler %u", scaler);
        //ESP_LOGD("hdlc", "Unit %d", buffer[5]);

        if (!is_signed) {
            value = uvalue;
        }
        double scaled_value = pow(10, scaler) * value;

        // Report Wh and varh as kWh and kvarh.
        if (unit == 30 || unit == 32) {
            scaled_value = scaled_value / 1000;
        }
        parseRow(parsed, obis, scaled_value);
    }
    return true;
}


/* Reads messages formatted according to "Branschrekommendation v1.2", which
   at the time of writing (20210207) is used by Tekniska Verken's Aidon 6442SE
   meters. This is a binary format, with a HDLC Frame.

   This code is in no way a generic HDLC Frame parser, but it does the job
   of decoding this particular data stream.
   */
void HDLC::readHDLCMessage() {
    if (available()) {
        uint8_t data = 0;
        uint16_t crc = 0x0000;
        ParsedMessage parsed = ParsedMessage();

        while (parseHDLCState == OUTSIDE_FRAME) {
            data = read();
            if (data == 0x7e) {
                // ESP_LOGD("hdlc", "Found start of frame");
                parseHDLCState = FOUND_FRAME;
                break;

                int8_t next = peek();

                // ESP_LOGD("hdlc", "Next is %d", next);

                if (next == 0x7e) {
                    read(); // We were actually at the end flag, consume the start flag of the next frame.
                }
                else if (next == -1) {
                    ESP_LOGE("hdlc", "No char available after flag, out of sync. Returning");
                    parseHDLCState = OUTSIDE_FRAME;
                    return;
                }
            }
        }

        if (parseHDLCState == FOUND_FRAME) {
            // Read various static HDLC Frame information we don't care about
            if (!read_array(buffer, 12)) {
                ESP_LOGE("hdlc", "Expected 12 bytes - out of sync. Returning");
                parseHDLCState = OUTSIDE_FRAME;
                return;
            }
            // ESP_LOGD("hdlc", "Got %d HDLC bytes, now reading 4 Invoke ID And Priority bytes", len);
            if (!read_array(buffer, 4)) {
                ESP_LOGE("hdlc", "Expected 4 bytes");
            }
            if ((buffer[0] != 0x40) || (buffer[1] != 0x00) || (buffer[2] != 0x00) || (buffer[3] != 0x00)) {
                ESP_LOGE("hdlc", "Expected 0x40 0x00 0x00 0x00, got %X %X %X %X - out of sync, returning.", buffer[0], buffer[1], buffer[2], buffer[3]);
                parseHDLCState = OUTSIDE_FRAME;
                return;
            }
        }

        data = read(); // Expect length of time field, usually 0
                       //ESP_LOGD("hdlc", "Length of datetime field is %d", data);
        read_array(buffer, data);

        data = read();
        //ESP_LOGD("hdlc", "Expect 0x01 (array tag), got %02X", data);
        if (data != 0x01) {
            parseHDLCState = OUTSIDE_FRAME;
            return;
        }

        uint8_t array_length = read();
        //ESP_LOGD("hdlc", "Array length is %d", array_length);

        for (int i=0;i<array_length;i++) {
            if (!readHDLCStruct(&parsed)) {
                parseHDLCState = OUTSIDE_FRAME;
                return;
            }
        }

        publishSensors(&parsed);


        while (true) {
            data = read();
            //ESP_LOGD("hdlc", "Read char %02X", data);
            if (data == 0x7e) {
                //ESP_LOGD("hdlc", "Found end of frame");
                parseHDLCState = OUTSIDE_FRAME;
                return;
            }
        }
    }
}
void HDLC::dump_config() {  // NOLINT(readability-function-cognitive-complexity)
    ESP_LOGCONFIG(TAG, "HDLC:");
    ESP_LOGCONFIG(TAG, "Phase 1");
    LOG_SENSOR(TAG, "Voltage ", phase_sensors[0].voltage_sensor_);
    LOG_SENSOR(TAG, "Current ", phase_sensors[0].current_sensor_);
    LOG_SENSOR(TAG, "Power ", phase_sensors[0].active_import_sensor_);
    ESP_LOGCONFIG(TAG, "Phase 2");
    LOG_SENSOR(TAG, "Voltage ", phase_sensors[1].voltage_sensor_);
    LOG_SENSOR(TAG, "Current ", phase_sensors[1].current_sensor_);
    LOG_SENSOR(TAG, "Power ", phase_sensors[1].active_import_sensor_);
    ESP_LOGCONFIG(TAG, "Phase 3");
    LOG_SENSOR(TAG, "Voltage ", phase_sensors[2].voltage_sensor_);
    LOG_SENSOR(TAG, "Current ", phase_sensors[2].current_sensor_);
    LOG_SENSOR(TAG, "Power ", phase_sensors[2].active_import_sensor_);
    ESP_LOGCONFIG(TAG, "Cumulative");
    LOG_SENSOR(TAG, "Active import", cumulative_active_import_sensor_);
    LOG_SENSOR(TAG, "Active export", cumulative_active_export_sensor_);
    LOG_SENSOR(TAG, "Reactive import", cumulative_reactive_import_sensor_);
    LOG_SENSOR(TAG, "Reactive export", cumulative_reactive_export_sensor_);
    ESP_LOGCONFIG(TAG, "Momentary");
    LOG_SENSOR(TAG, "Active import", momentary_active_import_sensor_);
    LOG_SENSOR(TAG, "Active export", momentary_active_export_sensor_);
    LOG_SENSOR(TAG, "Reactive import", momentary_reactive_import_sensor_);
    LOG_SENSOR(TAG, "Reactive export", momentary_reactive_export_sensor_);
}

} // namespace hdlc
} // namespace esphome
