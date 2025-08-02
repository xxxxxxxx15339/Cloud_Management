#include "Pod.hpp"

Pod::Pod(string name)
    : name_(name) {};

Pod::~Pod() = default;

void Pod::addContainer(unique_ptr<Container> c) {
    containers_.push_back(move(c));
}

void Pod::setLabel(const string& key, const string& value) {
    labels_[key] = value;
}

void Pod::setName(const string& s) {
    name_ = s;
}

void Pod::startAll() {
    for (auto& container: containers_) {
        container->start();
    }
}

void Pod::stopAll() {
    for (auto& container: containers_) {
        container->stop();
    }
}

string Pod::getMetrics() const {
    string P = "Pod=[\n      labels={";
    for (auto it = labels_.cbegin(); it != labels_.cend(); ++it){
        P += it->first + ":" + it->second;

        if (next(it) != labels_.cend()) {
            P += ",";
        }
    }
    P += "}\n";
    P += "      Containers={\n";
    for (auto it = containers_.cbegin(); it != containers_.cend(); ++it){  // iterator of pointers ----> (*it) = pointer ----> (*it)->function()
        P += (*it)->getMetrics();
        if (next(it) != containers_.cend()) {
            P += "\n";
        }
    }  
    P += "}\n]";

    return P;
}

ostream& operator<<(ostream& os, const Pod& p) {
    os << p.getMetrics();
    return os;
}

vector<unique_ptr<Container>>& Pod::getContainers() noexcept {
    return containers_;
};
unordered_map<string, string>& Pod::getLabels() noexcept {
    return labels_;
};
string Pod::getName() {
    return name_;
}
