#include "adas/speed_monitor.h"
#include "common/math_utils.h"

namespace adas {
  SpeedMonitor::SpeedMonitor(int limit) : speed_limit(limit) {}

  void SpeedMonitor::addReading(int speed) {
    readings.push_back(speed);
  }

  double SpeedMonitor::getAverage() const {
    return common::calculateAverage(readings.data(), readings.size());
  }

  bool SpeedMonitor::isSpeeding() const {
    if (readings.empty()) return false;
    return readings.back() > speed_limit;
  }
}
