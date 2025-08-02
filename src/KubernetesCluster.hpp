#ifndef KUBERNETESCLUSTER_HPP
#define KUBERNETESCLUSTER_HPP

#include "Pod.hpp"
#include "Server.hpp"
#include <list>
using namespace std; 

class KubernetesCluster {
    private:
        string name_;
        vector<shared_ptr<Server>> nodes_;
        vector<unique_ptr<Pod>> pods_;
    public:

        KubernetesCluster(string name);
        ~KubernetesCluster();

        void deployPods(vector<unique_ptr<Pod>>& pods);
        void addServer(const shared_ptr<Server>& server);
        void schedulePod(unique_ptr<Pod>& pod);

        string getMetrics() const;
        friend ostream& operator<<(ostream& os, const KubernetesCluster& k);

        vector<shared_ptr<Server>>& getNodes() noexcept;
        vector<unique_ptr<Pod>>& getPods() noexcept;
        string getName() const noexcept;

};

#endif