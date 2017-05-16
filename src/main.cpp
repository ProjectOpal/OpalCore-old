#include "FreeRTOSmacros.hpp"

#include <stdio.h>
#include <iostream>

#include "core/hecate/hecate.hpp"
#include "core/shil/imu.hpp"


using namespace cpp_freertos;
using namespace std;
using namespace core;
using namespace core::sensors;

int main (void) {
  shil::IMU imu(5500);
  Thread::StartScheduler();
  return 0;
}
