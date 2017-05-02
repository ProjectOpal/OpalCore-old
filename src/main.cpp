#include "FreeRTOSmacros.hpp"

#include "hecate.hpp"
#include "imu.hpp"

using namespace cpp_freertos;
using namespace std;
using namespace core;




int main (void) {
  SHIL::IMU imu(Ticks::MsToTicks(1000), 4000);
  imu.Start();
  Thread::StartScheduler();

  cout << "Scheduler ended!" << endl;
  return 0;
}