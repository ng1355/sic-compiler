// #sic compiler for CS-UY 3943 by Nikita Georgiou and Jack Martinez

#pragma once

#include "assembler.hpp" 

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

extern char current_factor; 
extern char ret_type;  /* either i or f */ 
extern assembler mass; 

void operation_check(const char *left, const char *right); 
void operation_check(const char *left, const int right); 
void operation_check(const char *left, const float right);
void function_check(const char *func, const char *param);
void return_check();
void boolean_check(std::vector<char*> vlist, std::vector<int> ilist, 
        std::vector<float> flist);
