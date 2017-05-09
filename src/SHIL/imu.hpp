/*
// Simulated IMU for SHIL that connects to FlightGear simulator
//
// Author: Samuel Rosenstein, samr37l@gmail.com
// Date: 4/25/2017
//
*/

#include <stdio.h>
#include <iostream>
#include <memory>
#include <random>
#include <chrono>

#include "PracticalSocket.h"

#include "FreeRTOS.h"
#include "task.h"
#include "thread.hpp"
#include "ticks.hpp"
#include "semaphore.hpp"
#include "imu.pb.hpp"

namespace shil {
  class IMU : public cpp_freertos::Thread {
   public:
    IMU(int server_port) :
                         cpp_freertos::Thread("IMU_SHIL", 1000, 1),
                         sock(new UDPSocket(server_port))
                         {
      Start();
    };

    core::sensors::ComputedState actual_state;

   protected:
    virtual void Run();

   private:
    // The socket that will receive data from FlightGear.
    std::unique_ptr<UDPSocket> sock;
    // The buffer in which to store the message.
    char msg_buffer[255];
  };

} // namespace SHIL
