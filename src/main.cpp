#include "FreeRTOSmacros.hpp"

#include <stdio.h>
#include <iostream>

#include "core/hecate/hecate.hpp"
#include "core/shil/imu.hpp"
#include "fixed_wing/stabalization/stabalization.hpp"
#include "fixed_wing/stabalization/protos/parameters.pb.hpp"
#include "core/shil/broadcaster.hpp"


using namespace cpp_freertos;
using namespace std;
using namespace core;
using namespace core::sensors;

int main (void) {
  shil::IMU imu(5500);
  fixed_wing::stabalization::StabalizationThread stab();
  shil::Broadcaster broadcaster(5501);

  Thread::StartScheduler();
  return 0;
}
