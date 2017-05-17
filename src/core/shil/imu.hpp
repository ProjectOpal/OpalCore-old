/*
// Simulated IMU for SHIL that connects to FlightGear simulator
//
// Author: Samuel Rosenstein, samr37l@gmail.com
// Date: 4/25/2017
//
*/

#ifndef IMU_SHIL_H
#define IMU_SHIL_H

#include <stdio.h>
#include <iostream>
#include <memory>
#include <random>
#include <chrono>
#include "FreeRTOS.h"
#include "task.h"
#include "thread.hpp"
#include "ticks.hpp"
#include "semaphore.hpp"

#include "core/shil/PracticalSocket.h"

#include "core/hecate/protos/imu.pb.hpp"
#include "core/hecate/hecate.hpp"

// TODO(@smr277): Change this to a Timer
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

#endif // IMU_SHIL_H
