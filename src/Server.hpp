#ifndef SERVER_HPP 
#define SERVER_HPP 

#include "Resource.hpp"
#include "Exceptions.hpp"

class Server : public Resource {
    private:
        double available_cpu_;
        double available_mem_;
        double initial_cpu_;
        double initial_mem_;
    
    public:
        Server(string id, double initial_cpu, double initial_mem);
        ~Server() override;

        void allocate(double cpu, double mem);
        void reset();  // Reset resources to initial values

        void start() override;
        void stop() override;

        string getMetrics() const override;
        friend ostream& operator<<(ostream& os, const Server& s);

        // Getters : 
        double getInitialCpu() const;
        double getInitialMem() const;
        double getAvailableCpu() const;
        double getAvailableMem() const;

};



#endif 