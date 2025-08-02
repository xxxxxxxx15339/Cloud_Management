#include "Container.hpp"

#include <sstream>
#include <iomanip>

Container::Container(string id, double cpu, double mem, string image)
: Resource(move(id), cpu, mem) , image_(move(image))
{
    active_ = false;
}

Container::~Container() = default;

void Container::start() {
    active_ = true;
}

void Container::stop() {
    active_ = false;
}

string Container::getMetrics() const {
    std::ostringstream oss;
    oss << "[Container: " << id_ << ": "
        << std::fixed << std::setprecision(6) << cpu_ << " CPU, "
        << std::fixed << std::setprecision(6) << mem_ << " Memory, "
        << image_ << ", active:" << (active_ ? "true" : "false") << "]";
    return oss.str();
}

ostream& operator<<(ostream& os, const Container& c) {
    os << c.getMetrics();
    return os;
}



