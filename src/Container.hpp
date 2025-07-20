#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include "Resource.hpp"

class Container : public Resource {

private:
    string image_;

public:
    Container(string id, double cpu, double mem, string image);
    ~Container() override;

    void start() override;
    void stop() override;

    // Getter for image : 
    string& getImage() const;

    string getMetrics() const override;
    friend ostream& operator<<(ostream& os, const Container& c);
};

#endif