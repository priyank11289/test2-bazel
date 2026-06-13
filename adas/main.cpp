#include <iostream>
#include "adas/speed_monitor.h"

int main() {
  adas::SpeedMonitor monitor(100);
  monitor.addReading(80);
  monitor.addReading(110);

  std::cout << "Average Speed: " << monitor.getAverage() << "\n";
  std::cout << "Is Speeding: " << (monitor.isSpeeding() ? "Yes" : "No") << "\n";

  return 0;
}
