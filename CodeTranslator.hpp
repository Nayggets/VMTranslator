#pragma once
#include <iterator>
#include <unordered_map>
#include <iostream>
#include <string>
#include <vector>
class CodeTranslator
{
public:
    CodeTranslator(std::vector<std::string>& codeToTranslate,const std::string& line); //for unique file
    CodeTranslator(std::unordered_map<std::vector<std::string>,std::string>); // for folders
    ~CodeTranslator();
    std::vector<std::string> allTranslate();

private:
    std::vector<std::string> codeToTranslate;
    std::vector<std::string> codeTranslate;
    std::unordered_map<std::string, int> lineOfCommand;
    std::unordered_map<std::string,int> memorySegment;
    std::string filename;
    int returnNumber;
    int eq;
    int gt;
    int lt;

private:
    void translate_code(const std::string& line);
    void choose_arithmetic_operation(int numberCommand);
    void assembly_add();
    void assembly_sub();
    void assembly_neg();
    void assembly_eq();
    void assembly_gt();
    void assembly_lt();
    void assembly_and();
    void assembly_or();
    void assembly_not();
    void assembly_push(const std::string& line);
    void assembly_pop(const std::string& line);
    void assembly_fonction(const std::string& line);
    void assembly_goto(const std::string& line);
    void assembly_if_goto(const std::string& line);
    void assembly_label(const std::string& line);
    void assembly_bootstrap();
    void assembly_call(const std::string& line);
    void assembly_return();

};



