#include "imu.pb.hpp"
#include <pb_encode.h>
#include <pb_decode.h>

using core::sensors::ComputedState;

int main(void) {
  ComputedState computed_state;
  computed_state.roll = 1;

  return 0;
}