#include "Resource.hpp"

Resource::Resource(string id, double cpu, double mem)
    : id_(id), cpu_(cpu), mem_(mem), active_(false) 
{}

Resource::~Resource() = default;

string Resource::getId() const {
    return id_;
};

double Resource::getCpu() const {
    return cpu_;
};
 
double Resource::getMem() const {
    return mem_;
};