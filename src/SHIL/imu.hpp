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


  // TODO(@smr277): Create the gyro noise model...or determine if needed

  // Gyroscope noise model. Models gyroscope noise through
  //  w(t) = w^(t) + b(t) + n(t)
  //  where b(t) is the gyroscope bias and n(t) is white noise.
  //  Bias is computed by slowly incrementing the bias at a user defined rate
  class GyroNoiseModel {
  public:
    // @arg std_dev: The standard deviation of the white noise in deg/sec.
    // @arg bias_rate: The rate of the biad increse in millideg/sec.
    // @arg pos_or_neg: Whether the rate should be positive or negetive.
    //    true for positive, false for negetive.
    GyroNoiseModel(double std_dev, int bias_rate, bool pos_or_neg) :
        generator(std::chrono::system_clock::now().time_since_epoch().count()),
        distribution(0.0, std_dev),
        {
          this->bias_rate = bias_rate;
        };

    // Creates gyroscope noise following the model presented above.
    void CreateGyroNoise(core::sensors::Gyro *gyro);
  private:
    std::default_random_engine generator;
    std::normal_distribution<double> distribution;
    // The rate of bias increase in millideg/sec.
    int bias_rate;
  };

} // namespace SHIL
