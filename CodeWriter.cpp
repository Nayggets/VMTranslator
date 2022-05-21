#include "CodeWriter.hpp"

CodeWriter::CodeWriter(std::string fileName)
{
    fileOut = std::ofstream(fileName,std::ios::trunc);
}

CodeWriter::~CodeWriter()
{
    fileOut.close();
}

void CodeWriter::writeCode(std::vector<std::string> code)
{
    for(auto e : code){
        fileOut << e << std::endl;
    }    
}