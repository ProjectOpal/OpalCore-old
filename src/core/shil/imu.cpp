/*
// Simulated IMU for SHIL that connects to FlightGear simulator
//
// Author: Samuel Rosenstein, samr37l@gmail.com
// Date: 4/25/2017
//
*/

#include "core/shil/imu.hpp"

using namespace core;
using namespace core::sensors;

namespace shil {
  #pragma pack(1)
  struct ImuInternal {
    float altitude_agl_m;
    float altitude_m;
    float latitude_deg;
    float longitude_deg;
    float roll_deg;
    float pitch_deg;
    float heading_magnetic_deg;
    float roll_rate_degps;
    float pitch_rate_degps;
    float yaw_rate_degps;
    float airspeed_kt;
  };

  void IMU::Run() {
    Gyro my_gyro;
    AirSpeed my_airspeed;
    Location my_location;
    while(true) {
      string source_address;
      unsigned short source_port;
      try {
        this->sock->recvFrom(this->msg_buffer, 255, source_address,
                             source_port);
        ImuInternal *imu_internal = reinterpret_cast<ImuInternal *>(msg_buffer);
        my_gyro = {static_cast<int32_t>(imu_internal->roll_rate_degps * 1000),
                   static_cast<int32_t>(imu_internal->pitch_rate_degps * 1000),
                   static_cast<int32_t>(imu_internal->yaw_rate_degps * 1000) };
        Hecate::gyro.Write(my_gyro);

        my_airspeed =
            {static_cast<int32_t>(imu_internal->airspeed_kt * 51.4444)};
        Hecate::airspeed.Write(my_airspeed);

        my_location = {imu_internal->latitude_deg,
                       imu_internal->longitude_deg,
                       static_cast<int32_t>(imu_internal->altitude_m) * 100};
        Hecate::location.Write(my_location);

        this->actual_state =
            ComputedState{static_cast<int32_t>(imu_internal->roll_deg * 1000),
                static_cast<int32_t>(imu_internal->pitch_deg * 1000),
                static_cast<int32_t>(imu_internal->heading_magnetic_deg * 1000),
                my_location,
                -1,
                static_cast<int32_t>(imu_internal->airspeed_kt * 51.4444)};
        // Change this in the future:
        Hecate::comp_state.Write(this->actual_state);
       } catch(SocketException ex) {
        // This fails due to context switching. However...it seems to work even
        //    without a critical section.
        Yield();
      }
    }
  }

} // namespace shil
