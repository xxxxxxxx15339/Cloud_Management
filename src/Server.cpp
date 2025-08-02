#include "Server.hpp"

Server::Server(string id, double initial_cpu, double initial_mem) 
    : Resource(id, initial_cpu, initial_mem), 
        available_cpu_(initial_cpu), 
        available_mem_(initial_mem),
        initial_cpu_(initial_cpu),
        initial_mem_(initial_mem) {}

Server::~Server() = default;

void Server::allocate(double cpu, double mem) {
    if ((cpu <= available_cpu_) && (mem <= available_mem_)) {
        available_cpu_ -= cpu;
        available_mem_ -= mem;
    } else {
        throw AllocationException("Server:allocate failed: insufficient resources");
    }
}

void Server::reset() {
    available_cpu_ = initial_cpu_;
    available_mem_ = initial_mem_;
}

void Server::start() {
    active_ = true;
}

void Server::stop() {
    active_ = false;
}

string Server::getMetrics() const {
    return "[Server: " + id_ + ": " + to_string(initial_cpu_) + " Initial Cpu, " 
    + to_string(initial_mem_) + " Initial Memory, " + to_string(available_cpu_) + "Available Cpu," + to_string(available_mem_) + "Available Mem" + " ]";
}

ostream& operator<<(ostream& os, const Server& s) {
    os << s.getMetrics();
    return os;
}




double Server::getInitialCpu() const {
    return initial_cpu_;
}

double Server::getInitialMem() const {
    return initial_mem_;
}

double Server::getAvailableCpu() const {
    return available_cpu_;
}

double Server::getAvailableMem() const {
    return available_mem_;
}