#include "CodeTranslator.hpp"

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

CodeTranslator::CodeTranslator(std::vector<std::string>& codeToTranslateNotClean,const std::string& filename)
{
    returnNumber = 1;
    this->filename = filename;
    int i = 0;
    while(this->filename[i] != '.'){
        i++;
    }
    for(; i < filename.size() ;){
        this->filename.pop_back();
    }

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
        {"call",12},
        {"goto",13},
        {"if-goto",14},
        {"label",15},
        {"fonction",16}
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
    for (auto& e : codeToTranslateNotClean)
    {
        if(correctLine(e)){
            codeToTranslate.push_back(e);
        }
    }
}

CodeTranslator::CodeTranslator(std::unordered_map<std::vector<std::string>,std::string> folderCodePerFile)
{

}

CodeTranslator::~CodeTranslator()
{

}

void CodeTranslator::translate_code(const std::string& line)
{
    std::string checkCommand = {};
    auto begin = std::begin(line);
    auto endCommand = std::begin(line);
    int commandNumber = 0;
    for (int i = 0; i < line.size() + 1; i++)
    {
        endCommand = std::begin(line) + i;
        checkCommand = {begin, endCommand};
        commandNumber = lineOfCommand[checkCommand];
        if(commandNumber != 0){
            i = line.size();
            if(commandNumber < 10){
                choose_arithmetic_operation(commandNumber);
            }
            else if(commandNumber == 10){
                assembly_push(line);
            }
            else if(commandNumber == 11){
                assembly_pop(line);
            }
            else if(commandNumber == 12){
                assembly_call(line);
            }
            else if(commandNumber == 13){
                assembly_goto(line);
            }
            else if(commandNumber == 14){
                assembly_if_goto(line);
            }
            else if(commandNumber == 15){
                assembly_label(line);
            }
            else if(commandNumber == 16){
                assembly_fonction(line);
            }
        }
    }
}

void CodeTranslator::choose_arithmetic_operation(int numberCommand)
{
    switch (numberCommand)
    {
    case 1:
        assembly_add();
        break;
    case 2:
        assembly_sub();
        break;
    case 3:
        assembly_neg();
        break;
    case 4:
        assembly_eq();
        break;

    case 5:
        assembly_gt();
        break;
    case 6:
        assembly_lt();
        break;
    case 7:
        assembly_and();
        break;
    case 8:
        assembly_or();
        break;
    case 9:
        assembly_not();
        break;
    }
}

void CodeTranslator::assembly_label(const std::string& line)
{

    std::string labelName = {std::begin(line) + 5,std::end(line)};
    labelName = {"(" + labelName + ")"};
    codeTranslate.push_back(labelName);
    
}

void CodeTranslator::assembly_return()
{
    codeTranslate.push_back("@LCL");
    codeTranslate.push_back("D=M");
    codeTranslate.push_back("@13");
    codeTranslate.push_back("M=D");
    codeTranslate.push_back("@5");
    codeTranslate.push_back("D=A");
    codeTranslate.push_back("@LCL");
    codeTranslate.push_back("A=D-A");
    codeTranslate.push_back("D=M");
    codeTranslate.push_back("@14");
    codeTranslate.push_back("M=D");

    codeTranslate.push_back("@SP");
    codeTranslate.push_back("A=M");
    codeTranslate.push_back("D=M");
    codeTranslate.push_back("@ARG");
    codeTranslate.push_back("A=M");
    codeTranslate.push_back("M=D");
    codeTranslate.push_back("@ARG");
    codeTranslate.push_back("D=M");
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("M=D+1");
    
    codeTranslate.push_back("@13");
    codeTranslate.push_back("AM=M-1");
    codeTranslate.push_back("D=M");
    codeTranslate.push_back("@THAT");
    codeTranslate.push_back("M=D");

    codeTranslate.push_back("@13");
    codeTranslate.push_back("AM=M-1");
    codeTranslate.push_back("D=M");
    codeTranslate.push_back("@THIS");
    codeTranslate.push_back("M=D");

    codeTranslate.push_back("@13");
    codeTranslate.push_back("AM=M-1");
    codeTranslate.push_back("D=M");
    codeTranslate.push_back("@ARG");
    codeTranslate.push_back("M=D");

    codeTranslate.push_back("@13");
    codeTranslate.push_back("AM=M-1");
    codeTranslate.push_back("D=M");
    codeTranslate.push_back("@LCL");
    codeTranslate.push_back("M=D");


    codeTranslate.push_back("@14");
    codeTranslate.push_back("A=M");
    codeTranslate.push_back("0;JMP");




}

void CodeTranslator::assembly_goto(const std::string& line)
{
    
    std::string jumpName = {std::begin(line) + 4,std::end(line)};
    jumpName = {"@" + jumpName};
    codeTranslate.push_back(jumpName);
    codeTranslate.push_back("0;JMP");
}

void CodeTranslator::assembly_if_goto(const std::string& line)
{
    std::string jumpName = {std::begin(line) + 5,std::end(line)};
    jumpName = {"@" + jumpName};
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("AM=M-1");
    codeTranslate.push_back("D=M");
    codeTranslate.push_back(jumpName);
    codeTranslate.push_back("D;JLT");
}

void CodeTranslator::assembly_fonction(const std::string& line)
{
    int i = 0 ; 
    while(line[i] > '9' || line[i] < '0'){
        i++;
    }
    i--;
    std::string jumpName = {std::begin(line) + 4,std::begin(line) + i};
    jumpName = {"(" + filename + '.' + jumpName + ")"};
    std::string number {""};
    i = 0;
    while(line[i] > '9' || line[i] < '0'){
        i++;
    }
    while(line[i] <= '9' && line[i] >= '0'){
        number.push_back(line[i]);
        i++;
    }
    int localNumber = std::stoi(number);
    for(int i = 0 ; i < localNumber ; i++){
        codeTranslate.push_back("@SP");
        codeTranslate.push_back("A=M");
        codeTranslate.push_back("M=0");
        codeTranslate.push_back("@SP");
        codeTranslate.push_back("M=M+1");
    }
}

void CodeTranslator::assembly_call(const std::string& line)
{
    std::string number {""};
    int i = 0;
    while(line[i] > '9' || line[i] < '0'){
        i++;
    }
    while(line[i] <= '9' && line[i] >= '0'){
        number.push_back(line[i]);
        i++;
    }
    number = "@" + number;
    codeTranslate.push_back("@" +  filename + "$.ret" + std::to_string(returnNumber));
    codeTranslate.push_back("D=A");
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("A=M");
    codeTranslate.push_back("M=D");
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("M=M+1");

    codeTranslate.push_back("@LCL");
    codeTranslate.push_back("D=M");
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("A=M");
    codeTranslate.push_back("M=D");
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("M=M+1");
    codeTranslate.push_back("@ARG");
    codeTranslate.push_back("D=M");
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("A=M");
    codeTranslate.push_back("M=D");
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("M=M+1");
    codeTranslate.push_back("@THIS");
    codeTranslate.push_back("D=M");
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("A=M");
    codeTranslate.push_back("M=D");
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("M=M+1");
    codeTranslate.push_back("@THAT");
    codeTranslate.push_back("D=M");
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("A=M");
    codeTranslate.push_back("M=D");
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("M=M+1");
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("D=M");
    codeTranslate.push_back("@5");
    codeTranslate.push_back("D=D-A");
    codeTranslate.push_back(number);
    codeTranslate.push_back("D=D-A");
    codeTranslate.push_back("@ARG");
    codeTranslate.push_back("M=D");
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("D=M");
    codeTranslate.push_back("LCL");
    codeTranslate.push_back("M=D");

    i = 0 ; 
    while(line[i] > '9' || line[i] < '0'){
        i++;
    }
    i--;

    std::string jumpName = {std::begin(line) + 4,std::begin(line) + i};
    jumpName = {"@" + filename + '.' + jumpName};
    codeTranslate.push_back(jumpName);
    codeTranslate.push_back("0;JMP");
    codeTranslate.push_back("(" + filename + "$.ret" + std::to_string(returnNumber) + ")");

}

void CodeTranslator::assembly_bootstrap()
{
    codeTranslate.push_back("@256");
    codeTranslate.push_back("D=M");
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("M=D");
}

void CodeTranslator::assembly_add()
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


void CodeTranslator::assembly_sub()
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

void CodeTranslator::assembly_eq()
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

void CodeTranslator::assembly_gt()
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

void CodeTranslator::assembly_and()
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

void CodeTranslator::assembly_lt()
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

void CodeTranslator::assembly_neg()
{
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("AM=M-1");
    codeTranslate.push_back("M=-M");
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("M=M+1");

}

void CodeTranslator::assembly_not()
{
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("AM=M-1");
    codeTranslate.push_back("M=!M");
    codeTranslate.push_back("@SP");
    codeTranslate.push_back("M=M+1");
}

void CodeTranslator::assembly_or()
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

void CodeTranslator::assembly_push(const std::string& line)
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

void CodeTranslator::assembly_pop(const std::string& line)
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
        translate_code(e);
    }
    return codeTranslate;
}
