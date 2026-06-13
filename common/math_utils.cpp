#include "common/math_utils.h"

namespace common {
  double calculateAverage(const int* data, int size) {
    if (size == 0) return 0.0;
    double sum = 0;
    for (int i = 0; i < size; ++i) {
      sum += data[i];
    }
    return sum / size;
  }
}
