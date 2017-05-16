#include "FreeRTOSmacros.hpp"

#include "hecate/hecate.hpp"
#include "shil/imu.hpp"

#include <stdio.h>
#include <iostream>

using namespace cpp_freertos;
using namespace std;
using namespace core;
using namespace core::sensors;

int main (void) {
  shil::IMU imu(5500);
  Thread::StartScheduler();
  return 0;
}
