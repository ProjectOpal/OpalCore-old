#include "SHIL/FreeRTOSmacros.hpp"

#include "core/hectate/hecate.hpp"
// #include "imu.hpp"



using namespace cpp_freertos;
using namespace std;
using namespace core;
using namespace core::sensors;

int main (void) {
  Gyro my_gyro = {static_cast<int32_t>(imu_internal->roll_rate_degps * 1000),
                  static_cast<int32_t>(imu_internal->pitch_rate_degps * 1000),
                  static_cast<int32_t>(imu_internal->yaw_rate_degps * 1000) };
  Hecate::gyro.Write(my_gyro);
  // shil::IMU imu(5500);

  // Thread::StartScheduler();
  return 0;
}
