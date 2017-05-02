/*
// Simulated IMU for SHIL that connects to FlightGear simulator
//
// Author: Samuel Rosenstein, samr37l@gmail.com
// Date: 4/25/2017
//
*/

#include "imu.hpp"
#include "task.h"

void SHIL::IMU::Run() {
  string source_address;
  unsigned short source_port;
  try {
    this->sock->recvFrom(this->msg_buffer, 255, source_address,
                         source_port);
  } catch(SocketException ex) {
    // This fails due to context switching. However...it seems to work even without a critical section.
    return;
  }

}