#ifndef PIPE_H
#define PIPE_H

#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

class PipeManager;

class Pipe {
private:
    string full_name; 
    double length = 0; 
    double diameter = 0;
    bool underRepair;
public:
    void read();
    void display() const;
    void load(const string &data);
    friend class PipeManager;
};

#endif