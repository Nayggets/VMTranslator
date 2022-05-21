#include <string>
#include <iterator>
#include <iostream>
#include "CodeReader.hpp"
#include "CodeWriter.hpp"
#include "CodeTranslator.hpp"

int main(int argc, char* argv[])
{
    std::string nameBefore = argv[1];
    int check = 0;
    int j = 0;
    for(int i = 0 ; i < nameBefore.size() ;i++){
        if(nameBefore[i] == '/'){
            check++;
        }
    }
    int othercheck = 0;
    while(othercheck != check){
        if(nameBefore[j] == '/'){
            othercheck++;
        }
        j++;
    }

    std::string realname = {""};
    for(int i = j ; i < nameBefore.size(); i++){
        realname.push_back(nameBefore[i]);
    }

    bool notDirectory = false;
    for(auto& c : realname){
        if(c == '.'){
            notDirectory = true;
        }
    }

    CodeReader c(argv[1]);
    std::vector<std::string> buffer = c.fillBuffer();
    CodeTranslator c1(buffer,realname);
    buffer = c1.allTranslate();
    int i = 0;
    std::string name = argv[1];
    while(name[i] != '.'){
        i++;
    }
    for(; i < name.size() ;){
        name.pop_back();
    }
    name.push_back('.');
    name.push_back('a');
    name.push_back('s');
    name.push_back('m');

    CodeWriter c2(name);
    c2.writeCode(buffer);
    return 0;
    //
}