#include "protocol.h"
#include "protocol_internal.h"
#include "protocol_constants.h"
#include "relay.h"
#include "soil_moisture.h"
#include "dht_sensor.h"
#include "ds18b20.h"

namespace Protocol {
  State _state = State::Idle;

  namespace {
    uint8_t packet_type;
    uint8_t subject;
    static void (*read_data)(uint8_t bit) = nullptr;
    void choose_read_data_function();
  };

  void put(char ch) {
    static uint8_t i = 0;

    for (uint8_t bit_i = 0; bit_i < 8; bit_i++) {
      uint8_t bit = bitRead(ch, 7 - bit_i);

      switch (_state) {
        case State::Idle:
          packet_type = 0;
          bitWrite(packet_type, 1, bit);
          _state = State::PacketTypeReading;
          break;

        case State::PacketTypeReading:
          bitWrite(packet_type, 0, bit);
          subject = 0;
          i = 0;
          _state = State::SubjectReading;
          break;

        case State::SubjectReading:
          bitWrite(subject, 3 - i, bit);
          i++;
          if (i == 4) {
            choose_read_data_function();
            _state = State::DataReading;
          }
          break;

        case State::DataReading:
          read_data(bit);
          break;

        case State::RedundantBitsSkipping:
          _state = State::Idle;
          return;
      }
    }
  }

  namespace {
    void choose_read_data_function() {
      if (packet_type == EVENT) {
        // none yet
      } else if (packet_type == REQUEST) {
        switch (subject) {
          case INBOUND_REQUEST_SET_RELAY:
            read_data = Relay::read_set_relay_data;
            break;
          case INBOUND_REQUEST_GET_RELAY:
            read_data = Relay::read_get_relay_data;
            break;
          case INBOUND_REQUEST_TOGGLE_RELAY:
            read_data = Relay::read_toggle_relay_data;
            break;
          case INBOUND_REQUEST_GET_SOIL_MOISTURE:
            read_data = SoilMoisture::read_get_soil_moisture_data;
            break;
          case INBOUND_REQUEST_GET_DHT:
            read_data = DHT::read_get_dht_data;
            break;
          case INBOUND_REQUEST_GET_DS18B20:
            read_data = DS18B20::read_get_ds18b20_data;
            break;
        }
      }
    }
  }
};
