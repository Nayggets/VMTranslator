#pragma once
#include <fstream>
#include <vector>
#include <string>
class CodeWriter{
public:
    CodeWriter(std::string fileName);
    ~CodeWriter();
    void writeCode(std::vector<std::string> code);
private:
    std::ofstream fileOut;
};
