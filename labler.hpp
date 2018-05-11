/* sic compiler for CS-UY 3943 by Nikita Georgiou and Jack Martinez */ 

#pragma once

#include <string>

class labler {
public:
    labler() = default; 
    labler(const std::string& prefix); 
    std::string getnew();
    std::string getlast() const; 
    unsigned getlasti() const; 
    std::string operator [] (const unsigned num) const; 
    void operator ++ (); 
    void operator -- (); 
private:
    std::string prefix; 
    unsigned count = 0; 
}; 
