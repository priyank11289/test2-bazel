#include <gtest/gtest.h>
#include "adas/speed_monitor.h"

TEST(SpeedMonitorTest, InitialAverageIsZero) {
    adas::SpeedMonitor monitor(100);
    EXPECT_DOUBLE_EQ(monitor.getAverage(), 0.0);
    EXPECT_FALSE(monitor.isSpeeding());
}

TEST(SpeedMonitorTest, DetectsSpeeding) {
    adas::SpeedMonitor monitor(100);
    monitor.addReading(80);
    EXPECT_FALSE(monitor.isSpeeding());
    
    monitor.addReading(110);
    EXPECT_TRUE(monitor.isSpeeding());
    EXPECT_DOUBLE_EQ(monitor.getAverage(), 95.0);
}
