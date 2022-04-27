#pragma once
#include <fstream>
#include <vector>
#include <string>
class CodeWriter{
    private:
        std::ofstream fileOut;

    public:
        CodeWriter(std::string fileName);
        ~CodeWriter();
        void writeCode(std::vector<std::string> code);
};

CodeWriter::CodeWriter(std::string fileName)
{
    fileOut = std::ofstream(fileName,std::ios::trunc);
}

CodeWriter::~CodeWriter()
{

}

void CodeWriter::writeCode(std::vector<std::string> code)
{
    for(auto e : code){
        fileOut << e << std::endl;
    }    
}