#include "KubernetesCluster.hpp"
#include "Exceptions.hpp"

KubernetesCluster::KubernetesCluster(string name)
    : name_(name) {}
KubernetesCluster::~KubernetesCluster() = default;

void KubernetesCluster::schedulePod(unique_ptr<Pod>& pod) {
    for (auto& node: nodes_) {
        try {
            // Try to allocate all containers to this node
            for (auto& c: pod->getContainers()) {
                node->allocate(c->getCpu(), c->getMem());  
            }

            // If we get here, all containers were allocated successfully
            pod->startAll();
            pods_.push_back(move(pod));
            return;
        } catch (const AllocationException&) {
            // Reset the node's resources to initial state
            node->reset();
            continue;
        }
    }
    throw AllocationException("Aucun serveur disponible pour ce pod");
};

/*
                               |---> Containers allocation is done ! ---> return;
|---> node->allocate()-------->|
|                              |---> Exception : Serveur can't allocate these Containers ---> catch():continue -----|
|                                                                                                                   |
------------------------------------------------------repeat--------------------------------------------------------|

*/

void KubernetesCluster::deployPods(vector<unique_ptr<Pod>>& pods) {
    for (auto& p: pods) {
        schedulePod(p);
    }    
};

void KubernetesCluster::addServer(const shared_ptr<Server>& server) {
    nodes_.push_back(server);
}

vector<shared_ptr<Server>>& KubernetesCluster::getNodes() noexcept {
    return nodes_;
};
vector<unique_ptr<Pod>>& KubernetesCluster::getPods() noexcept {
    return pods_;
};
string KubernetesCluster::getName() const noexcept {
    return name_;
};

string KubernetesCluster::getMetrics() const {
    std::ostringstream oss;
    oss << "Cluster Metrics:\n";
    oss << "Servers:\n";
    for (const auto& node : nodes_) {
        oss << "  " << node->getMetrics() << "\n";
    }
    oss << "Pods:\n";
    for (const auto& p : pods_) {
        oss << "  " << p->getMetrics() << "\n";
    }
    return oss.str(); 
};
ostream& operator<<(ostream& os, const KubernetesCluster& k) {
    os << k.getMetrics();
    return os;
};
