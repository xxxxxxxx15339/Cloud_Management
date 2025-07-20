#ifndef RESOURCE_H
#define RESOURCE_H

#include <iostream>
#include <string>
#include <memory>
using namespace std;

class Resource {
    
public:
    // start should flip active_ to true
    virtual void start() = 0;

    // stop should flip active_ to false
    virtual void stop() = 0;

    // Get all the information (including active)
    virtual string getMetrics() const = 0;
    
protected:
    string id_;
    double cpu_;
    double mem_;
    bool active_;

    Resource(string id, double cpu, double mem);
    virtual ~Resource();

};

#endif