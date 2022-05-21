#pragma once
#include <string>
#include <fstream>
#include <vector>

class CodeReader
{
public:
    CodeReader(std::string fileName);
    ~CodeReader();
    std::vector<std::string> fillBuffer();
    
private:
    std::ifstream reader;
    std::vector<std::string> dataRead;
};
