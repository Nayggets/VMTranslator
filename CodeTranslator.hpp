#pragma once
#include <iterator>
#include <unordered_map>
#include <iostream>
#include <string>
#include <vector>
class CodeTranslator
{
private:
    std::vector<std::string> codeToTranslate;
    std::vector<std::string> codeTranslate;
    std::unordered_map<std::string, int> lineOfCommand;
    std::unordered_map<std::string,int> memorySegment;
    std::string filename;
    int eq;
    int gt;
    int lt;

private:
    void ChooseCommandArithmetic(int numberCommand);
    void AssemblyAdd();
    void AssemblySub();
    void AssemblyNeg();
    void AssemblyEq();
    void AssemblyGt();
    void AssemblyLt();
    void AssemblyAnd();
    void AssemblyOr();
    void AssemblyNot();
    void Assemblypush(std::string line);
    void Assemblypop(std::string line);
    void translateToCode(std::string line);


public:
    CodeTranslator(std::vector<std::string> codeToTranslate, std::string filename);
    ~CodeTranslator();
    std::vector<std::string> allTranslate();
};

bool correctLine(std::string& line)
{
    if(line[0] == '/'){
        return false;
    }
    auto iterator = {std::begin(line)};
    int i = 0;
    while (line[i] != '/' && line[i] != '\0')
    {
        i++;
    }

    std::string correctLine = {std::begin(line), std::begin(line) + i};
    line = std::move(correctLine);
    return true;
}

CodeTranslator::CodeTranslator(std::vector<std::string> codeToTranslateNotClean,std::string filename)
{
    int i = 0;
    while(filename[i] != '.'){
        i++;
    }
    for(; i < filename.size() ;){
        filename.pop_back();
    }
    
    this->filename = filename;

    lineOfCommand = {
        {"add",1},
        {"sub",2},
        {"neg",3},
        {"eq",4},
        {"gt",5},
        {"lt",6},
        {"and",7},
        {"or",8},
        {"not",9},
        {"push",10},
        {"pop",11},
    };
    memorySegment = {
        {"constant",1},
        {"static",2},
        {"local",3},
        {"argument",4},
        {"this",5},
        {"that",6},
        {"temp",7},
        {"pointer",8}
    };
    for (auto e : codeToTranslateNotClean)
    {
        if(correctLine(e)){
            codeToTranslate.push_back(e);
        }
    }
}

CodeTranslator::~CodeTranslator()
{
}

void CodeTranslator::translateToCode(std::string line)
{
    bool tocheck = false;
    bool pushOrPop = false;
    std::string checkCommand = {};
    auto begin = std::begin(line);
    auto endCommand = std::begin(line);
    int commandNumber = 0;
    for (int i = 0; i < line.size(); i++)
    {
        endCommand = std::begin(line) + i;
        checkCommand = {begin, endCommand};
        commandNumber = lineOfCommand[checkCommand];
        if(commandNumber != 0){
            i = line.size();
            if(commandNumber < 10){
                ChooseCommandArithmetic(commandNumber);
            }
            else if(commandNumber == 10){
                Assemblypush(line);
            }
            else if(commandNumber == 11){
                Assemblypop(line);
            }
        }
    }
}

void CodeTranslator::ChooseCommandArithmetic(int numberCommand)
{
    switch (numberCommand)
    {
    case 1:
        AssemblyAdd();
        break;
    case 2:
        AssemblySub();
        break;
    case 3:
        AssemblyNeg();
        break;
    case 4:
        AssemblyEq();
        break;
    case 5:
        AssemblyGt();
        break;
    case 6:
        AssemblyLt();
        break;
    case 7:
        AssemblyAnd();
        break;
    case 8:
        AssemblyOr();
        break;
    case 9:
        AssemblyNot();
        break;
    }
}
void CodeTranslator::AssemblyAdd()
{
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("AM=M-1");
    codeTranslate.push_back("D=M");
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("AM=M-1");
    codeTranslate.push_back("M=D+M");
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("M=M+1");
}


void CodeTranslator::AssemblySub()
{
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("AM=M-1");
    codeTranslate.push_back("D=M");
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("AM=M-1");
    codeTranslate.push_back("M=M-D");
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("M=M+1");
}

void CodeTranslator::AssemblyEq()
{
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("AM=M-1");
    codeTranslate.push_back("D=M");
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("AM=M-1");
    codeTranslate.push_back("D=D-M");
    codeTranslate.push_back("@TRUE_EQ" + std::to_string(eq));
    codeTranslate.push_back("D;JEQ");
    codeTranslate.push_back("@FALSE_EQ" + std::to_string(eq));
    codeTranslate.push_back("0;JMP");
    codeTranslate.push_back("(TRUE_EQ" + std::to_string(eq) + ')');
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("A=M");
    codeTranslate.push_back("M=-1");
    codeTranslate.push_back("@END_EQ" + std::to_string(eq));
    codeTranslate.push_back("0;JMP");
    codeTranslate.push_back("(FALSE_EQ" + std::to_string(eq) + ')');
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("A=M");
    codeTranslate.push_back("M=0");
    codeTranslate.push_back("@END_EQ" + std::to_string(eq));
    codeTranslate.push_back("0;JMP");
    codeTranslate.push_back("(END_EQ" + std::to_string(eq) + ')');
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("M=M+1");

    eq++;
}

void CodeTranslator::AssemblyGt()
{
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("AM=M-1");
    codeTranslate.push_back("D=M");
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("AM=M-1");
    codeTranslate.push_back("D=D-M");
    codeTranslate.push_back("@TRUE_GT" + std::to_string(gt));
    codeTranslate.push_back("D;JLT");
    codeTranslate.push_back("@FALSE_GT" + std::to_string(gt));
    codeTranslate.push_back("0;JMP");
    codeTranslate.push_back("(TRUE_GT" + std::to_string(gt) + ')');
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("A=M");
    codeTranslate.push_back("M=-1");
    codeTranslate.push_back("@END_GT" + std::to_string(gt));
    codeTranslate.push_back("0;JMP");
    codeTranslate.push_back("(FALSE_GT" + std::to_string(gt) + ')');
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("A=M");
    codeTranslate.push_back("M=0");
    codeTranslate.push_back("@END_GT" + std::to_string(gt));
    codeTranslate.push_back("0;JMP");
    codeTranslate.push_back("(END_GT" + std::to_string(gt) + ')');
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("M=M+1");
    gt++;
}

void CodeTranslator::AssemblyAnd()
{
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("AM=M-1");
    codeTranslate.push_back("D=M");
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("AM=M-1");
    codeTranslate.push_back("M=D&M");
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("M=M+1");
}

void CodeTranslator::AssemblyLt()
{
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("AM=M-1");
    codeTranslate.push_back("D=M");
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("AM=M-1");
    codeTranslate.push_back("D=D-M");
    codeTranslate.push_back("@TRUE_LT" + std::to_string(lt));
    codeTranslate.push_back("D;JGT");
    codeTranslate.push_back("@FALSE_LT" + std::to_string(lt));
    codeTranslate.push_back("0;JMP");
    codeTranslate.push_back("(TRUE_LT" + std::to_string(lt) + ')');
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("A=M");
    codeTranslate.push_back("M=-1");
    codeTranslate.push_back("@END_LT" + std::to_string(lt));
    codeTranslate.push_back("0;JMP");
    codeTranslate.push_back("(FALSE_LT" + std::to_string(lt) + ')');
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("A=M");
    codeTranslate.push_back("M=0");
    codeTranslate.push_back("@END_LT" + std::to_string(lt));
    codeTranslate.push_back("0;JMP");
    codeTranslate.push_back("(END_LT" + std::to_string(lt) + ')');
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("M=M+1");
    lt++;
}

void CodeTranslator::AssemblyNeg()
{
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("AM=M-1");
    codeTranslate.push_back("M=-M");
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("M=M+1");

}

void CodeTranslator::AssemblyNot()
{
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("AM=M-1");
    codeTranslate.push_back("M=!M");
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("M=M+1");
}

void CodeTranslator::AssemblyOr()
{
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("AM=M-1");
    codeTranslate.push_back("D=M");
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("AM=M-1");
    codeTranslate.push_back("M=D|M");
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("M=M+1");
}

void CodeTranslator::Assemblypush(std::string line)
{
    auto begin = std::begin(line) + 5;
    auto endCommand = begin;
    std::string checkCommand = {""};
    int memorySegmentNumber = 0;
    for (int i = 0; i < line.size(); i++)
    {
        endCommand = begin + i;
        checkCommand = {begin, endCommand};
        memorySegmentNumber = memorySegment[checkCommand];
        if(memorySegmentNumber != 0){
            i = line.size();
        }
    }
    std::string number {""};
    int i = 0;
        while(line[i] > '9' || line[i] < '0'){
            i++;
        }
        while(line[i] <= '9' && line[i] >= '0'){
            number.push_back(line[i]);
            i++;
        }

    if(memorySegmentNumber == 1){
        
        codeTranslate.push_back("@" + number);
        codeTranslate.push_back("D=A");
        codeTranslate.push_back("@SP");
        codeTranslate.push_back("A=M");
        codeTranslate.push_back("M=D");
        codeTranslate.push_back("@SP");
        codeTranslate.push_back("M=M+1");
        
    }
    else if(memorySegmentNumber == 2){
        codeTranslate.push_back("@" + filename + '.' + number);
        codeTranslate.push_back("D=M");
        codeTranslate.push_back("@SP");
        codeTranslate.push_back("A=M");
        codeTranslate.push_back("M=D");
        codeTranslate.push_back("@SP");
        codeTranslate.push_back("M=M+1");

    }
    else if(memorySegmentNumber == 3){
        codeTranslate.push_back("@" + number);
        codeTranslate.push_back("D=A");
        codeTranslate.push_back("@LCL");
        codeTranslate.push_back("A=M+D");
        codeTranslate.push_back("D=M");
        codeTranslate.push_back("@SP");
        codeTranslate.push_back("A=M");
        codeTranslate.push_back("M=D");
        codeTranslate.push_back("@SP");
        codeTranslate.push_back("M=M+1");
    }
    else if(memorySegmentNumber == 4){
        codeTranslate.push_back("@" + number);
        codeTranslate.push_back("D=A");
        codeTranslate.push_back("@ARG");
        codeTranslate.push_back("A=M+D");
        codeTranslate.push_back("D=M");
        codeTranslate.push_back("@SP");
        codeTranslate.push_back("A=M");
        codeTranslate.push_back("M=D");
        codeTranslate.push_back("@SP");
        codeTranslate.push_back("M=M+1");
    }
    else if(memorySegmentNumber == 5){
        codeTranslate.push_back("@" + number);
        codeTranslate.push_back("D=A");
        codeTranslate.push_back("@THIS");
        codeTranslate.push_back("A=M+D");
        codeTranslate.push_back("D=M");
        codeTranslate.push_back("@SP");
        codeTranslate.push_back("A=M");
        codeTranslate.push_back("M=D");
        codeTranslate.push_back("@SP");
        codeTranslate.push_back("M=M+1");
    }
    else if(memorySegmentNumber == 6){
        codeTranslate.push_back("@" + number);
        codeTranslate.push_back("D=A");
        codeTranslate.push_back("@THAT");
        codeTranslate.push_back("A=M+D");
        codeTranslate.push_back("D=M");
        codeTranslate.push_back("@SP");
        codeTranslate.push_back("A=M");
        codeTranslate.push_back("M=D");
        codeTranslate.push_back("@SP");
        codeTranslate.push_back("M=M+1");
    }
    else if(memorySegmentNumber == 7){
        codeTranslate.push_back("@" + number);
        codeTranslate.push_back("D=A");
        codeTranslate.push_back("@5");
        codeTranslate.push_back("A=A+D");
        codeTranslate.push_back("D=M");
        codeTranslate.push_back("@SP");
        codeTranslate.push_back("A=M");
        codeTranslate.push_back("M=D");
        codeTranslate.push_back("@SP");
        codeTranslate.push_back("M=M+1");
    }
    else if(memorySegmentNumber == 8){
        if(number == "1"){
            codeTranslate.push_back("@THAT");
            codeTranslate.push_back("D=M");
            codeTranslate.push_back("@SP");
            codeTranslate.push_back("A=M");
            codeTranslate.push_back("M=D");
            codeTranslate.push_back("@SP");
            codeTranslate.push_back("M=M+1");
        }
        else{
            codeTranslate.push_back("@THIS");
            codeTranslate.push_back("D=M");
            codeTranslate.push_back("@SP");
            codeTranslate.push_back("A=M");
            codeTranslate.push_back("M=D");
            codeTranslate.push_back("@SP");
            codeTranslate.push_back("M=M+1");            
        }
    }

}

void CodeTranslator::Assemblypop(std::string line)
{

    auto begin = std::begin(line) + 4;
    auto endCommand = begin;
    std::string checkCommand = {""};
    int memorySegmentNumber = 0;
    for (int i = 0; i < line.size(); i++)
    {
        endCommand = begin + i;
        checkCommand = {begin, endCommand};
        memorySegmentNumber = memorySegment[checkCommand];
        if(memorySegmentNumber != 0){
            i = line.size();
        }
    }
    std::string number {""};
    int i = 0;
    while(line[i] > '9' || line[i] < '0'){
        i++;
    }
    while(line[i] <= '9' && line[i] >= '0'){
        number.push_back(line[i]);
        i++;
    }
    if(memorySegmentNumber == 2){
        codeTranslate.push_back("@SP");
        codeTranslate.push_back("AM=M-1");
        codeTranslate.push_back("D=M");
        codeTranslate.push_back("@" + filename + '.' + number);
        codeTranslate.push_back("M=D");
    }
    else if(memorySegmentNumber == 3){
        codeTranslate.push_back("@" + number);
        codeTranslate.push_back("D=A");
        codeTranslate.push_back("@LCL");
        codeTranslate.push_back("D=M+D");
        codeTranslate.push_back("@13");
        codeTranslate.push_back("M=D");
        codeTranslate.push_back("@SP");
        codeTranslate.push_back("AM=M-1");
        codeTranslate.push_back("D=M");
        codeTranslate.push_back("@13");
        codeTranslate.push_back("A=M");
        codeTranslate.push_back("M=D");


    }
    else if(memorySegmentNumber == 4){
        codeTranslate.push_back("@" + number);
        codeTranslate.push_back("D=A");
        codeTranslate.push_back("@ARG");
        codeTranslate.push_back("D=M+D");
        codeTranslate.push_back("@13");
        codeTranslate.push_back("M=D");
        codeTranslate.push_back("@SP");
        codeTranslate.push_back("AM=M-1");
        codeTranslate.push_back("D=M");
        codeTranslate.push_back("@13");
        codeTranslate.push_back("A=M");
        codeTranslate.push_back("M=D");


    }
    else if(memorySegmentNumber == 5){
        codeTranslate.push_back("@" + number);
        codeTranslate.push_back("D=A");
        codeTranslate.push_back("@THIS");
        codeTranslate.push_back("D=M+D");
        codeTranslate.push_back("@13");
        codeTranslate.push_back("M=D");
        codeTranslate.push_back("@SP");
        codeTranslate.push_back("AM=M-1");
        codeTranslate.push_back("D=M");
        codeTranslate.push_back("@13");
        codeTranslate.push_back("A=M");
        codeTranslate.push_back("M=D");


    }
    else if(memorySegmentNumber == 6){
        codeTranslate.push_back("@" + number);
        codeTranslate.push_back("D=A");
        codeTranslate.push_back("@THAT");
        codeTranslate.push_back("D=M+D");
        codeTranslate.push_back("@13");
        codeTranslate.push_back("M=D");
        codeTranslate.push_back("@SP");
        codeTranslate.push_back("AM=M-1");
        codeTranslate.push_back("D=M");
        codeTranslate.push_back("@13");
        codeTranslate.push_back("A=M");
        codeTranslate.push_back("M=D");


    }
    else if(memorySegmentNumber == 7){
        codeTranslate.push_back("@" + number);
        codeTranslate.push_back("D=A");
        codeTranslate.push_back("@5");
        codeTranslate.push_back("D=A+D");
        codeTranslate.push_back("@13");
        codeTranslate.push_back("M=D");
        codeTranslate.push_back("@SP");
        codeTranslate.push_back("AM=M-1");
        codeTranslate.push_back("D=M");
        codeTranslate.push_back("@13");
        codeTranslate.push_back("A=M");
        codeTranslate.push_back("M=D");


    }
    else if(memorySegmentNumber == 8){
        if(number == "1"){
            codeTranslate.push_back("@SP");
            codeTranslate.push_back("AM=M-1");
            codeTranslate.push_back("D=M");
            codeTranslate.push_back("@THAT");
            codeTranslate.push_back("M=D");
        }
        else{
            codeTranslate.push_back("@SP");
            codeTranslate.push_back("AM=M-1");
            codeTranslate.push_back("D=M");
            codeTranslate.push_back("@THIS");
            codeTranslate.push_back("M=D");
        }
    }
}


std::vector<std::string> CodeTranslator::allTranslate()
{
    for(auto& e : codeToTranslate)
    {
        translateToCode(e);
    }
    return codeTranslate;
}