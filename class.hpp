#ifndef CLASS_HPP
#define CLASS_HPP

#include<iostream>
#include<string>
using namespace std;

class ins{
    public:
        string name;
        string description;
        string example;
        ins* next;
        ins* prev;

        ins(string n = "", string d = "", string e = ""){
            name = n;
            description = d;
            example = e;
            next = nullptr;
            prev = nullptr;
        }

        void print(){
            cout << "  " << name << " : " << description << "  (" << example << ")" << endl;
        }
};

class ins_list{
    public:
        ins* head;
        ins* tail;
        int size;

        ins_list(){
            head = nullptr;
            tail = nullptr;
            size = 0;
        }

        ~ins_list(){
            ins* t = head;
            while(t != nullptr){
                ins* next_node = t -> next;
                delete t;
                t = next_node;
            }
        }

        void add_tail(ins* new_ins){
            if(size == 0){
                head = new_ins;
                tail = new_ins;
            } else{
                tail -> next = new_ins;
                new_ins -> prev = tail;
                tail = new_ins;
            }
            size++;
        }

        ins* search_ins(string target_name){
            ins* t = head;
            while(t != nullptr){
                if(t -> name == target_name){
                    return t;
                }
                t = t -> next;
            }
            return nullptr;
        }

        void print_all(){
            ins* t = head;
            while(t != nullptr){
                t -> print();
                t = t -> next;
            }
        }
};

#endif