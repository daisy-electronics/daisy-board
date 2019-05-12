#pragma once

#include "packet.h"

namespace Protocol {
  void setup_packet_handling();
  void handle_packet(const Protocol::Packet &pck);
};
