#include "class.hpp"
#include<iostream>
#include<string>
#include<fstream>

int main(){
    string category_names[5] = {"Data Transfer", "Arithmetic", "Logical", "Branch", "Control"};
    ins_list lists[5];

    lists[0].add_tail(new ins("MOV", "Copies one register into another.", "MOV B,A"));
    lists[0].add_tail(new ins("MVI", "Loads a value into a register.", "MVI A,05H"));
    lists[1].add_tail(new ins("ADD", "Adds a register to A.", "ADD B"));
    lists[1].add_tail(new ins("SUB", "Subtracts a register from A.", "SUB B"));
    lists[2].add_tail(new ins("ANA", "ANDs a register with A.", "ANA B"));
    lists[3].add_tail(new ins("JMP", "Jumps to an address.", "JMP 2000H"));
    lists[4].add_tail(new ins("HLT", "Stops the processor.", "HLT"));

    for(int i = 0; i < 5; i++){
        cout << endl << "=== " << category_names[i] << " ===" << endl;
        lists[i].print_all();
    }

    string target = "ADD";
    ins* found = nullptr;
    int found_in = -1;
    for(int i = 0; i < 5; i++){
        found = lists[i].search_ins(target);
        if(found != nullptr){
            found_in = i;
            break;
        }
    }

    if(found == nullptr){                                   
        cout << "No instruction found." << endl;
    } else{
        cout << target << " found in " << category_names[found_in] << endl;
        found -> print();
    }
    return 0;
}