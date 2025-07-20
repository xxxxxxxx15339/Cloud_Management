#include "Resource.hpp"

Resource::Resource(string id, double cpu, double mem)
    : id_(id), cpu_(cpu), mem_(mem), active_(false) 
{}

Resource::~Resource() = default;