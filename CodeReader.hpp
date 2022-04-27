#pragma once
#include <string>
#include <fstream>
#include <vector>

class CodeReader
{
    
    
private:
    std::ifstream reader;
    std::vector<std::string> dataRead;
public:
    CodeReader(std::string fileName);
    ~CodeReader();
    std::vector<std::string> fillBuffer();
};

CodeReader::CodeReader(std::string fileName)
{
    reader = std::ifstream(fileName);
}

CodeReader::~CodeReader()
{
}

std::vector<std::string> CodeReader::fillBuffer()
{
    std::string buffer;
    while(std::getline(reader,buffer)){
        dataRead.push_back(buffer);       
    }
    
    return dataRead;
}