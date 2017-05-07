#include "FreeRTOSmacros.hpp"

#include "hecate.hpp"
#include "imu.hpp"



using namespace cpp_freertos;
using namespace std;
using namespace core;
using namespace core::sensors;

int main (void) {
  shil::IMU imu(5500);

  Thread::StartScheduler();
  return 0;
}