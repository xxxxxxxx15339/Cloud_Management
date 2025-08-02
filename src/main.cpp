#include "CloudUtil.hpp"
#include "KubernetesCluster.hpp"
#include "Pod.hpp"
#include "Container.hpp"
#include "Server.hpp"
#include "Exceptions.hpp"
#include <nlohmann/json.hpp>
#include <iostream>
#include <memory>
#include <vector>
#include <fstream>

using json = nlohmann::json;


std::vector<std::unique_ptr<Pod>> ParseJsonFile(const std::string& filename) {

    std::vector<std::unique_ptr<Pod>> pods;

    std::ifstream file(filename);
    if (!file.is_open()) {
        throw FileException("Cannot open this file :" + filename);
    }

    json data;
    file >> data;
    file.close();

    for (const auto& podJson: data) {
        std::unique_ptr<Pod> pod = std::make_unique<Pod>(podJson["name"]);
        if (podJson.contains("labels")) {
            for (auto& [key, value]: podJson["labels"].items()) {
                pod->setLabel(key, value);
            }
        }

        for (const auto& conatinerJson: podJson["containers"]) {
            std::unique_ptr<Container> container = std::make_unique<Container>(
                conatinerJson["id"],
                conatinerJson["cpu"],
                conatinerJson["mem"],
                conatinerJson["image"]
            );
        pod->addContainer(std::move(container));
        }
    pods.push_back(std::move(pod));
    }

    return pods;
}


int main(int argc, char* argv[]) {
    try {
        std::vector<std::unique_ptr<Pod>> pods;

        std::cout << "=== Parsing pods from JSON file ===" << std::endl;
        pods = ParseJsonFile("../data/pods.JSON");

        KubernetesCluster cluster("My cluster");
        cluster.addServer(std::make_shared("Server1", 4.0, 8.0));
        cluster.addServer(std::make_shared("Server2", 4.0, 8.0));
        cluster.addServer(std::make_shared("Server3", 2.0, 4.0));

        CloudUtil util;

        std::cout << "=== Deploying pods to cluster ===" << std::endl;
        util.deployPods(cluster, pods);

        std::cout << "=== Cluster Metrics ===" << std::endl;
        util.display(cluster);

        std::cout << "=== Deployement completed successfully! ===" << std::endl;

    } catch (const FileException& e) {
        std::cerr << "File error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error" << e.what() << std::endl;
    }

    return 0;
}

