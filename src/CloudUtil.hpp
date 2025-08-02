#ifndef CLOUDUTIL_HPP
#define CLOUDUTIL_HPP

#include "KubernetesCluster.hpp"

class CloudUtil {
    public:
        void display(const KubernetesCluster& cluster);
        void deployPods(KubernetesCluster& cluster, std::vector<std::unique_ptr<Pod>>& pods);
        void saveClusterMetrics(const KubernetesCluster& cluster, const std::string& filename);

};

#endif