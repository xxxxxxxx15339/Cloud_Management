#include <gtest/gtest.h>
#include "Container.hpp"


TEST(ContainerTest, InitialStateIsInactive)
{
    Container c("c1", 1.5, 2.5, "img1");
    string m = c.getMetrics();
    EXPECT_NE(m.find("active:false"), string::npos) // string::npos == acrive:false ca veut dire que active:false n'existe pas dans le getMetrics()
        << "A la construction, active_ doit valoir false";
}

TEST(ContainerTest, StartSetsActiveTrue)
{
    Container c("c2", 3.0, 4.0, "img2");
    c.start();
    string m = c.getMetrics();
    EXPECT_NE(m.find("active:true"), string::npos)
        << "Apres appel de start(), active doit valoir true";
}

TEST(ContainerTest, StopSetsActiveFalse)
{
    Container c("c3", 4.0, 1.0, "img3");
    c.start();
    c.stop();
    string m = c.getMetrics();
    EXPECT_NE(m.find("active:false"), string::npos)
        << "Apres appel de stop(), active doit valoir false";
}

TEST(ContainerTest, MetricsContainCpuAndMen)
{
    Container c("foo", 7.1, 8.2, "img4");
    string m = c.getMetrics();

    EXPECT_NE(m.find("[Container: foo: "), string::npos)
        << "La chaine doit contenir l'identifiant";
    EXPECT_NE(m.find("7.100000 CPU"), string::npos)
        << "La chaine doit contenir cpu data";
    EXPECT_NE(m.find("8.200000 Memory"), string::npos)
        << "La chaine doit contenir memory data";
    EXPECT_NE(m.find("img4"), string::npos)
        << "La chaine doit contenir image data";
}


TEST(ContainerTest, StremInsertionOutputsMetrics) {
    Container c("bar", 5.5, 6.6, "img");
    c.start();
    std::ostringstream oss;
    oss << c;
    EXPECT_EQ(oss.str(), c.getMetrics())
        << "operator << doit produire exactement la chaine retournee par getMetrics()";
}


