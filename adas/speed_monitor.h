#pragma once
#include <vector>

namespace adas {
  class SpeedMonitor {
  public:
    SpeedMonitor(int limit);
    void addReading(int speed);
    double getAverage() const;
    bool isSpeeding() const;
  private:
    int speed_limit;
    std::vector<int> readings;
  };
}
