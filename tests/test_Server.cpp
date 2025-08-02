#include <gtest/gtest.h>
#include "Server.hpp"

TEST(ServerTest, InitialValues) {
    Server s("node1", 4.0, 8.0);
    EXPECT_DOUBLE_EQ(s.getInitialCpu(), 4.0);
    EXPECT_DOUBLE_EQ(s.getInitialMem(), 8.0);
    EXPECT_DOUBLE_EQ(s.getAvailableCpu(), 4.0);
    EXPECT_DOUBLE_EQ(s.getAvailableMem(), 8.0);
}

TEST(ServerTest, AllocateReducesAvailable) {
    Server s("node2", 10.0, 20.0);
    EXPECT_NO_THROW({s.allocate(3.5, 5.5);});
    EXPECT_DOUBLE_EQ(s.getAvailableCpu(), 10.0 - 3.5);
    EXPECT_DOUBLE_EQ(s.getAvailableMem(), 20.0 - 5.5);
}

TEST(ServerTest, AllocateReturnsFalseWhenInsufficient) {
    Server s("nodeC", 1.0, 1.0);
    EXPECT_THROW({s.allocate(2.0, 0.5);}, AllocationException);
    EXPECT_THROW({s.allocate(0.5, 2.0);}, AllocationException);
    EXPECT_DOUBLE_EQ(s.getAvailableCpu(), 1.0);
    EXPECT_DOUBLE_EQ(s.getAvailableMem(), 1.0);
}

TEST(ServerTest, GetMetricsContainsAllFields) {
    Server s("srv", 3.0, 6.0);
    std::string m = s.getMetrics();
    EXPECT_NE(m.find("[Server: srv: "), std::string::npos);
    EXPECT_NE(m.find("3.000000 Initial Cpu"),    std::string::npos);
    EXPECT_NE(m.find("6.000000 Initial Memory"), std::string::npos);
    EXPECT_NE(m.find("3.000000Available Cpu"),   std::string::npos);
    EXPECT_NE(m.find("6.000000Available Mem"),   std::string::npos);
}


TEST(ServerTest, StreamInsertionMatchesGetMetrics) {
    Server s("srv2", 2.0, 5.0);
    std::ostringstream oss;
    oss << s;
    EXPECT_EQ(oss.str(), s.getMetrics());
}

TEST(ServerTest, AllocateJustAboveLimits) {
    Server s("srv3", 5.0, 10.0);

    EXPECT_THROW ({
        s.allocate(5.00000000001, 10.0000000001);
    }, AllocationException);
}

TEST(ServerTest, AllocateJustBelowLimits) {
    Server s("srv4", 5.0, 10.0);

    EXPECT_NO_THROW ({
        s.allocate(4.999999, 9.9999999);
    });
}