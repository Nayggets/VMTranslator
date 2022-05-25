#include <string>
#include <iterator>
#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#include "CodeReader.hpp"
#include "CodeWriter.hpp"
#include "CodeTranslator.hpp"

int main(int argc, char* argv[])
{
    std::string nameBefore = argv[1];
    int check = 0;
    int j = 0; 
    for(int i = 0 ; i < nameBefore.size()-1 ;i++){
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
    if(notDirectory == true){
        CodeReader c(argv[1]);
        std::cout << "reader" << std::endl;
        std::vector<std::string> buffer = c.fillBuffer();
        std::cout << "buffer filed" << std::endl;
        CodeTranslator c1 = CodeTranslator(false);
        c1.translate_file(buffer,realname);
        std::cout << "Translator" << std::endl;
        buffer = c1.return_translate_code();
        std::cout << "AllTranslate" << std::endl;
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
        std::cout << "Writer create" << std::endl;
        c2.writeCode(buffer);
        std::cout << "code writed" << std::endl;
    }
    else{
        DIR *rep;
 
        rep = opendir(argv[1]);
        struct dirent *lecture;
 
        bool point = false;
        bool translatorExist = false;
        CodeTranslator c2(true);
        std::vector<std::string> buffer; 
        while ((lecture = readdir(rep))) {
                buffer.clear();
                point = false;
                int i = 0;
                for(auto& c : lecture->d_name){
                    if(point == false){
                        i++;
                    }
                    if(c == '.'){
                        point = true;
                    }            
                }
                if(point == true && lecture->d_name[i] == 'v' && lecture->d_name[i+1] == 'm'){
                    std::string name2 = lecture->d_name;
                    std::string name = argv[1];
                    for(auto c : name2){
                        name.push_back(c);
                    }
                    CodeReader c(name);
                    buffer = std::move(c.fillBuffer());
                    for(auto& c : buffer){
                        std::cout << c << std::endl;
                    }
                    c2.translate_file(buffer,name);

                }
                // ton traitement
        }
        buffer = c2.return_translate_code();
        for(auto c : buffer){
            std::cout << c << std::endl;
        }
        int i = 0;
        std::string name = argv[1];
        std::string copyArgv = realname;
        if(name[name.size()-1] != '/'){
            name.push_back('/');
        }
        for(auto& c : copyArgv){
            name.push_back(c);
        }
        if(name[name.size()-1] == '/'){
            name.pop_back();
        }
        name.push_back('.');
        name.push_back('a');
        name.push_back('s');
        name.push_back('m');
        std::cout << name << std::endl;
        CodeWriter c3(name);
        c3.writeCode(buffer);
        
    }

    
    return 0;
    //
}