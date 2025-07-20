#ifndef POD_HPP
#define POD_HPP

#include "Container.hpp"
#include <vector>
#include <unordered_map>
#include <memory>
#include <sstream>
#include <string>
#include <iterator>
using namespace std;

class Pod {
    private:
        string name_;
        vector<unique_ptr<Container>> containers_;
        unordered_map<string, string> labels_; // cle/valeur que l'on attache a un Pod
    public:
        Pod();
        ~Pod();

        void addContainer(unique_ptr<Container> c);
        void setLabel(const string& key, const string& value);
        void startAll();
        void stopAll();

        string getMetrics() const;
        friend ostream& operator<<(ostream& os, const Pod& p);
};


#endif