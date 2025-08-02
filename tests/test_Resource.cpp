#include <gtest/gtest.h>
#include <sstream>
#include "Resource.hpp"

class TestResource : public Resource
{
public:
    TestResource(const string &id, double cpu, double mem)
        : Resource(id, cpu, mem) {}

    void start() override
    {
        active_ = true;
    }

    void stop() override
    {
        active_ = false;
    }

    string getMetrics() const override
    {
        std::ostringstream oss;
        oss << "Resource[" << id_ << "]"
            << "cpu:" << cpu_
            << " mem:" << mem_
            << " active:" << std::boolalpha << active_;
        return oss.str();
    }
};

TEST(ResourceTest, InitialStateIsInactive)
{
    TestResource r("r1", 1.5, 2.5);
    string m = r.getMetrics();
    EXPECT_NE(m.find("active:false"), string::npos) // string::npos == acrive:false ca veut dire que active:false n'existe pas dans le getMetrics()
        << "A la construction, active_ doit valoir false";
}

TEST(ResourceTest, StartSetsActiveTrue)
{
    TestResource r("r2", 3.0, 4.0);
    r.start();
    string m = r.getMetrics();
    EXPECT_NE(m.find("active:true"), string::npos)
        << "Apres appel de start(), active doit valoir true";
}

TEST(ResourceTest, StopSetsActiveFalse)
{
    TestResource r("r3", 4.0, 1.0);
    r.start();
    r.stop();
    string m = r.getMetrics();
    EXPECT_NE(m.find("active:false"), string::npos)
        << "Apres appel de stop(), active doit valoir false";
}

TEST(ResourceTest, MetricsContainCpuAndMen)
{
    TestResource r("foo", 7.1, 8.2);
    string m = r.getMetrics();

    EXPECT_NE(m.find("Resource[foo]"), string::npos)
        << "La chaine doit contenir l'identifiant";
    EXPECT_NE(m.find("cpu:7.1"), string::npos)
        << "La chaine doit contenir cpu data";
    EXPECT_NE(m.find("mem:8.2"), string::npos)
        << "La chaine doit contenir memory data";
}
