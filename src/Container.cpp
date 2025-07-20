#include "Container.hpp"

Container::Container(string id, double cpu, double mem, string image)
: Resource(move(id), cpu, mem) , image_(move(image))
{}

Container::~Container() = default;

void Container::start() {
    active_ = true;
}

void Container::stop() {
    active_ = false;
}

string Container::getMetrics() const {
    return "[Container: " + id_ + ": " + to_string(cpu_) + " CPU, " 
    + to_string(mem_) + " Memory, " + image_ + "]";
}

ostream& operator<<(ostream& os, const Container& c) {
    os << c.getMetrics();
    return os;
}




