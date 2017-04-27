#include "imu.pb.hpp"
#include <pb_encode.h>
#include <pb_decode.h>

using core::sensors::ComputedState;

int main(void) {
  ComputedState computed_state = core_sensors_ComputedState_init_zero;

  return 0;
}