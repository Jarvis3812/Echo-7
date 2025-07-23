// PythonEmbed header
#pragma once
#include <string>
#include <map>
class PythonEmbed {
public:
    PythonEmbed();
    ~PythonEmbed();
    void* callFunction(const std::string& funcName, const std::map<std::string, double>& args);
private:
    void* pModule;
};
