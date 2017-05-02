/*
// Simulated IMU for SHIL that connects to FlightGear simulator
//
// Author: Samuel Rosenstein, samr37l@gmail.com
// Date: 4/25/2017
//
*/

#include "imu.hpp"
#include <critical.hpp>

void SHIL::IMU::Run() {
  string source_address;
  unsigned short source_port;
  cpp_freertos::CriticalSection::Enter();
  this->sock->recvFrom(this->msg_buffer, 255, source_address,
                       source_port);
  cpp_freertos::CriticalSection::Exit();
  cout << "Received packet from " << source_address << ":"
           << source_port << endl;
}