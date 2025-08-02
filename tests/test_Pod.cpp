#include <gtest/gtest.h>
#include "Pod.hpp"

TEST(PodTest, GetMetricsContainsAllContainers) {
    Pod pod("pod1");

    pod.addContainer(make_unique<Container>("c1", 1.0, 2.0, "img1"));
    pod.addContainer(make_unique<Container>("c2", 3.0, 4.0, "img2"));

    pod.startAll();
    string metrics = pod.getMetrics();

    EXPECT_NE(metrics.find("[Container: c1: 1.000000 CPU, 2.000000 Memory, img1, active:true]"), string::npos)
        << "Les métriques du container c1 doivent apparaître dans Pod::getMetrics()";
    EXPECT_NE(metrics.find("[Container: c2: 3.000000 CPU, 4.000000 Memory, img2, active:true]"),
              std::string::npos)
        << "Les métriques du container c2 doivent apparaître dans Pod::getMetrics()";
}