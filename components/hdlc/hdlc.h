//-------------------------------------------------------------------------------------
// ESPHome P1 Electricity Meter custom sensor
// Copyright 2020 PÃ¤r SvanstrÃ¶m
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
class ParsedMessage {
  public:
    double cumulativeActiveImport;
    double cumulativeActiveExport;

    double cumulativeReactiveImport;
    double cumulativeReactiveExport;

    double momentaryActiveImport;
    double momentaryActiveExport;

    double momentaryReactiveImport;
    double momentaryReactiveExport;

    double momentaryActiveImportL1;
    double momentaryActiveExportL1;

    double momentaryActiveImportL2;
    double momentaryActiveExportL2;

    double momentaryActiveImportL3;
    double momentaryActiveExportL3;

    double momentaryReactiveImportL1;
    double momentaryReactiveExportL1;

    double momentaryReactiveImportL2;
    double momentaryReactiveExportL2;

    double momentaryReactiveImportL3;
    double momentaryReactiveExportL3;

    double voltageL1;
    double voltageL2;
    double voltageL3;

    double currentL1;
    double currentL2;
    double currentL3;
};

class P1Reader : public Component, public uart::UARTDevice {
  char buffer[BUF_SIZE];

  const int8_t OUTSIDE_FRAME = 0;
  const int8_t FOUND_FRAME = 1;


  int8_t parseHDLCState = OUTSIDE_FRAME;

  public:
    sensor::Sensor *cumulativeActiveImport = new sensor::Sensor();
    sensor::Sensor *cumulativeActiveExport = new sensor::Sensor();

    sensor::Sensor *cumulativeReactiveImport = new sensor::Sensor();
    sensor::Sensor *cumulativeReactiveExport = new sensor::Sensor();

    sensor::Sensor *momentaryActiveImport = new sensor::Sensor();
    sensor::Sensor *momentaryActiveExport = new sensor::Sensor();

    sensor::Sensor *momentaryReactiveImport = new sensor::Sensor();
    sensor::Sensor *momentaryReactiveExport = new sensor::Sensor();

    sensor::Sensor *momentaryActiveImportL1 = new sensor::Sensor();
    sensor::Sensor *momentaryActiveExportL1 = new sensor::Sensor();

    sensor::Sensor *momentaryActiveImportL2 = new sensor::Sensor();
    sensor::Sensor *momentaryActiveExportL2 = new sensor::Sensor();

    sensor::Sensor *momentaryActiveImportL3 = new sensor::Sensor();
    sensor::Sensor *momentaryActiveExportL3 = new sensor::Sensor();

    sensor::Sensor *momentaryReactiveImportL1 = new sensor::Sensor();
    sensor::Sensor *momentaryReactiveExportL1 = new sensor::Sensor();

    sensor::Sensor *momentaryReactiveImportL2 = new sensor::Sensor();
    sensor::Sensor *momentaryReactiveExportL2 = new sensor::Sensor();

    sensor::Sensor *momentaryReactiveImportL3 = new sensor::Sensor();
    sensor::Sensor *momentaryReactiveExportL3 = new sensor::Sensor();

    sensor::Sensor *voltageL1 = new sensor::Sensor();
    sensor::Sensor *voltageL2 = new sensor::Sensor();
    sensor::Sensor *voltageL3 = new sensor::Sensor();

    sensor::Sensor *currentL1 = new sensor::Sensor();
    sensor::Sensor *currentL2 = new sensor::Sensor();
    sensor::Sensor *currentL3 = new sensor::Sensor();

    P1Reader(uart::UARTComponent *parent) : uart::UARTDevice(parent) {}


    void setup() override {
    }

    void loop() override {
      readHDLCMessage();
    }


  private:
    void parseRow(ParsedMessage* parsed, char* obisCode, double value) {
      //ESP_LOGD("parseRow", "OBIS %s value %f", obisCode, value);
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
        parsed->momentaryActiveImportL1 = value;
      }
      else if (strncmp(obisCode, "22.7.0", 6) == 0) {
        parsed->momentaryActiveExportL1 = value;
      }
      else if (strncmp(obisCode, "41.7.0", 6) == 0) {
        parsed->momentaryActiveImportL2 = value;
      }
      else if (strncmp(obisCode, "42.7.0", 6) == 0) {
        parsed->momentaryActiveExportL2 = value;
      }
      else if (strncmp(obisCode, "61.7.0", 6) == 0) {
        parsed->momentaryActiveImportL3 = value;
      }
      else if (strncmp(obisCode, "62.7.0", 6) == 0) {
        parsed->momentaryActiveExportL3 = value;
      }
      else if (strncmp(obisCode, "23.7.0", 6) == 0) {
        parsed->momentaryReactiveImportL1 = value;
      }
      else if (strncmp(obisCode, "24.7.0", 6) == 0) {
        parsed->momentaryReactiveExportL1 = value;
      }
      else if (strncmp(obisCode, "43.7.0", 6) == 0) {
        parsed->momentaryReactiveImportL2 = value;
      }
      else if (strncmp(obisCode, "44.7.0", 6) == 0) {
        parsed->momentaryReactiveExportL2 = value;
      }
      else if (strncmp(obisCode, "63.7.0", 6) == 0) {
        parsed->momentaryReactiveImportL3 = value;
      }
      else if (strncmp(obisCode, "64.7.0", 6) == 0) {
        parsed->momentaryReactiveExportL3 = value;
      }
      else if (strncmp(obisCode, "32.7.0", 6) == 0) {
        parsed->voltageL1 = value;
      }
      else if (strncmp(obisCode, "52.7.0", 6) == 0) {
        parsed->voltageL2 = value;
      }
      else if (strncmp(obisCode, "72.7.0", 6) == 0) {
        parsed->voltageL3 = value;
      }
      else if (strncmp(obisCode, "31.7.0", 6) == 0) {
        parsed->currentL1 = value;
      }
      else if (strncmp(obisCode, "51.7.0", 6) == 0) {
        parsed->currentL2 = value;
      }
      else if (strncmp(obisCode, "71.7.0", 6) == 0) {
        parsed->currentL3 = value;
      }
    }


    void publishSensors(ParsedMessage* parsed) {
      cumulativeActiveImport->publish_state(parsed->cumulativeActiveImport);
      cumulativeActiveExport->publish_state(parsed->cumulativeActiveExport);

      cumulativeReactiveImport->publish_state(parsed->cumulativeReactiveImport);
      cumulativeReactiveExport->publish_state(parsed->cumulativeReactiveExport);

      momentaryActiveImport->publish_state(parsed->momentaryActiveImport);
      momentaryActiveExport->publish_state(parsed->momentaryActiveExport);

      momentaryReactiveImport->publish_state(parsed->momentaryReactiveImport);
      momentaryReactiveExport->publish_state(parsed->momentaryReactiveExport);

      momentaryActiveImportL1->publish_state(parsed->momentaryActiveImportL1);
      momentaryActiveExportL1->publish_state(parsed->momentaryActiveExportL1);

      momentaryActiveImportL2->publish_state(parsed->momentaryActiveImportL2);
      momentaryActiveExportL2->publish_state(parsed->momentaryActiveExportL2);

      momentaryActiveImportL3->publish_state(parsed->momentaryActiveImportL3);
      momentaryActiveExportL3->publish_state(parsed->momentaryActiveExportL3);

      momentaryReactiveImportL1->publish_state(parsed->momentaryReactiveImportL1);
      momentaryReactiveExportL1->publish_state(parsed->momentaryReactiveExportL1);

      momentaryReactiveImportL2->publish_state(parsed->momentaryReactiveImportL2);
      momentaryReactiveExportL2->publish_state(parsed->momentaryReactiveExportL2);

      momentaryReactiveImportL3->publish_state(parsed->momentaryReactiveImportL3);
      momentaryReactiveExportL3->publish_state(parsed->momentaryReactiveExportL3);

      voltageL1->publish_state(parsed->voltageL1);
      voltageL2->publish_state(parsed->voltageL2);
      voltageL3->publish_state(parsed->voltageL3);

      currentL1->publish_state(parsed->currentL1);
      currentL2->publish_state(parsed->currentL2);
      currentL3->publish_state(parsed->currentL3);
    }


    bool readHDLCStruct(ParsedMessage* parsed) {
      if (Serial.readBytes(buffer, 3) != 3)
        return false;

      if (buffer[0] != 0x02) {
        return false;
      }

      char obis[7];

      uint8_t struct_len = buffer[1];
      //ESP_LOGD("hdlc", "Struct length is %d", struct_len);

      uint8_t tag = buffer[2];

      if (tag != 0x09) {
        ESP_LOGE("hdcl", "Unexpected tag %X in struct, bailing out", tag);
        return false;
      }

      uint8_t str_length = read();
      if (Serial.readBytes(buffer, str_length) != str_length) {
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
        if (Serial.readBytes(buffer, str_length) != str_length) {
          ESP_LOGE("hdlc", "Unable to read %d bytes of string", str_length);
          return false;
        }

        buffer[str_length] = 0;
        //ESP_LOGD("hdlc", "Read string length %d", str_length);
      }
      else if(tag == 0x06) {
        Serial.readBytes(buffer, 4);
        uvalue = buffer[3] | buffer[2] << 8 | buffer[1] << 16 | buffer[0] << 24;
        //ESP_LOGD("hdlc", "Value of uvalue is %u", uvalue);
      }
      else if (tag == 0x10) {
        Serial.readBytes(buffer, 2);
        is_signed = true;
        value = buffer[1] | buffer[0] << 8;
        //ESP_LOGD("hdlc", "(Signed) Value of value is %d", value);
      }
      else if (tag == 0x12) {
        Serial.readBytes(buffer, 2);
        uvalue = buffer[1] | buffer[0] << 8;
        //ESP_LOGD("hdlc", "(Unsigned) Value of uvalue is %u", uvalue);
      }
      else {
        ESP_LOGE("hdlc", "unknown tag %X", tag);
      }

      int8_t scaler;
      uint8_t unit;
      if (struct_len == 3) {
        Serial.readBytes(buffer, 6);
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
    void readHDLCMessage() {
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
          int len = Serial.readBytes(buffer, 12);
          if (len != 12) {
            ESP_LOGE("hdlc", "Expected 12 bytes, got %d bytes - out of sync. Returning", len);
            parseHDLCState = OUTSIDE_FRAME;
            return;
          }
          // ESP_LOGD("hdlc", "Got %d HDLC bytes, now reading 4 Invoke ID And Priority bytes", len);
          len = Serial.readBytes(buffer, 4);
          if ((len != 4) || (buffer[0] != 0x40) || (buffer[1] != 0x00) || (buffer[2] != 0x00) || (buffer[3] != 0x00)) {
            ESP_LOGE("hdlc", "Expected 0x40 0x00 0x00 0x00, got %X %X %X %X - out of sync, returning.", buffer[0], buffer[1], buffer[2], buffer[3]);
            parseHDLCState = OUTSIDE_FRAME;
            return;
          }
        }

        data = read(); // Expect length of time field, usually 0
        //ESP_LOGD("hdlc", "Length of datetime field is %d", data);
        Serial.readBytes(buffer, data);

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
};

} // namespace hdlc
} // namespace esphome
