#include "CodeReader.hpp"
CodeReader::CodeReader(std::string fileName)
{
    
    reader = std::ifstream(fileName);
}

CodeReader::~CodeReader()
{
    reader.close();
}

std::vector<std::string> CodeReader::fillBuffer()
{
    std::string buffer;
    while(std::getline(reader,buffer)){
        dataRead.push_back(buffer);       
    }
    
    return dataRead;
}