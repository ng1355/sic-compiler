/* sic compiler for CS-UY 3943 by Nikita Georgiou and Jack Martinez */ 

#include "labler.hpp"

labler::labler(const std::string& prefix): prefix(prefix) {}

std::string labler::getnew(){
    ++count; 
    return prefix + std::to_string(count);
}

std::string labler::getlast() const {
    return prefix + std::to_string(count);
}

unsigned labler::getlasti() const {
    return count;
}

std::string labler::operator [] (const unsigned num) const {
    return prefix + std::to_string(num); 
}

void labler::operator ++ () {
    ++count;
}

void labler::operator -- () {
    --count;
}
