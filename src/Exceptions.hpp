#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <exception>
#include <string>

class CloudException : public std::exception {
    private:
        std::string message_;
    public:
        CloudException(const std::string& message);
        virtual ~CloudException();
        virtual const char* what() const throw();

};

class AllocationException : public CloudException {
    public:
        AllocationException(const std::string& message);

};

class FileException : public CloudException {
    public:
        FileException(const std::string& message);
};

#endif