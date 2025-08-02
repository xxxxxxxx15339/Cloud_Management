#include "Exceptions.hpp"
#include <iostream>

CloudException::CloudException(const std::string& message)
    : message_(message) {}

// message is being passed by reference, which means that i don't actually own the message so it's using move isn't saving me a copy, if i'm passing by value i can use std::move to save me a copy


const char* CloudException::what() const throw() {
    return message_.c_str();
}

CloudException::~CloudException() {}

AllocationException::AllocationException(const std::string& message)
    : CloudException(message) {}

FileException::FileException(const std::string& message) 
    : CloudException(message) {}