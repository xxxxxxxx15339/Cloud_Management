#include <gtest/gtest.h>
#include "KubernetesCluster.hpp"
using namespace std;

TEST(ClusterTest, InitialStateEmpty) {
    KubernetesCluster cluster("test-cluster");
    EXPECT_TRUE(cluster.getNodes().empty());
    EXPECT_TRUE(cluster.getPods().empty());
    EXPECT_EQ(cluster.getName(), "test-cluster");
}

TEST(ClusterTest, AddServerRegistersNode) {
    KubernetesCluster cluster("test");
    auto srv = make_shared<Server>("node1", 4.0, 8.0);
    cluster.addServer(srv);

    const auto& nodes = cluster.getNodes();
    ASSERT_EQ(nodes.size(), 1u);
    EXPECT_NE(nodes[0]->getMetrics().find("node1"), string::npos);
}

TEST(ClusterTest, SchedulePodSuccessAndFailure) {
    KubernetesCluster cluster("test");
    cluster.addServer(make_shared<Server>("node1", 4.0, 8.0));

    // Pod qui tient
    auto p1 = make_unique<Pod>("pod1");
    p1->addContainer(make_unique<Container>("c1", 2.0, 3.0, "img"));
    EXPECT_NO_THROW({cluster.schedulePod(p1);});
    EXPECT_EQ(cluster.getPods().size(), 1u);
    EXPECT_EQ(p1, nullptr);   // ownership bien deplacer (implementation de std::move)

    auto p2 = make_unique<Pod>("pod2");
    p2->addContainer(make_unique<Container>("c2", 10.0, 10.0, "img"));
    EXPECT_THROW({cluster.schedulePod(p2);}, AllocationException);
    EXPECT_NE(p2, nullptr);
    EXPECT_EQ(cluster.getPods().size(), 1u);
}

TEST(ClusterTest, DeployPods) {
    KubernetesCluster cluster("c");
    cluster.addServer(make_shared<Server>("n1", 3.0, 3.0));

    vector<unique_ptr<Pod>> list;

    list.push_back(make_unique<Pod>("p1"));
    list.back()->addContainer(make_unique<Container>("c1", 1.0, 1.0, "img"));

    list.push_back(make_unique<Pod>("p2"));
    list.back()->addContainer(make_unique<Container>("c2", 5.0, 5.0, "img"));

    cluster.deployPods(list);

    EXPECT_EQ(cluster.getPods().size(), 1u);
    EXPECT_EQ(list[0], nullptr);
    EXPECT_NE(list[1], nullptr);
}

TEST(ClusterTest, GetMetricsAndStream) {
    KubernetesCluster cluster("prod");
    cluster.addServer(std::make_shared<Server>("srv", 2.0, 2.0));

    auto pod = std::make_unique<Pod>("pod");
    pod->addContainer(std::make_unique<Container>("c", 1.0, 1.0, "img"));
    cluster.schedulePod(pod);

    std::string m = cluster.getMetrics();
    EXPECT_NE(m.find("Cluster Metrics:"), std::string::npos);
    EXPECT_NE(m.find("Servers:"),          std::string::npos);
    EXPECT_NE(m.find("Pods:"),             std::string::npos);
    EXPECT_NE(m.find("srv"),               std::string::npos);
    EXPECT_NE(m.find("Container: c"),      std::string::npos);

    std::ostringstream oss;
    oss << cluster;
    EXPECT_EQ(oss.str(), m);
}