#include "CloudUtil.hpp"

void CloudUtil::display(const KubernetesCluster& cluster) {
    std::cout << cluster.getMetrics();
}

void CloudUtil::deployPods(KubernetesCluster& cluster, std::vector<std::unique_ptr<Pod>>& pods) {
    for (auto& pod: pods) {
        try {
            cluster.schedulePod(pod);
        } catch(AllocationException& e) {
            std::cout << "Error deploying pod: " << e.what() << std::endl;
            continue;
        }
    }
}







