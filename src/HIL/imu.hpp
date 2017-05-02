/*
// Simulated IMU for SHIL that connects to FlightGear simulator
//
// Author: Samuel Rosenstein, samr37l@gmail.com
// Date: 4/25/2017
//
*/

#include <stdio.h>
#include <iostream>

#include "PracticalSocket.h"

#include "FreeRTOS.h"
#include "task.h"
#include "thread.hpp"
#include "ticks.hpp"
#include "timer.hpp"

namespace SHIL {

  class IMU : public cpp_freertos::Timer {
   public:
    IMU(int run_every, int server_port) :
                         cpp_freertos::Timer(run_every),
                         sock(new UDPSocket(server_port))
                         {
    };

   protected:
    virtual void Run();

   private:
    // The socket that will receive data from FlightGear.
    UDPSocket *sock;
    // The buffer in which to store the message.
    char msg_buffer[255];
  };

} // namespace SHIL