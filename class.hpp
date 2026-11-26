#ifndef CLASS_HPP
#define CLASS_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>

using namespace std;

inline string trim(const string &str)
{
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos)
        return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

inline string to_upper(string s)
{
    for (char &c : s)
        c = toupper(static_cast<unsigned char>(c));
    return s;
}

class ins
{
private:
    string mnemonic;
    string syntax;
    string bytes;
    string description;
    string action;
    string flags;
    string example;
    ins *next;
    ins *prev;

public:
    // Overloaded Constructor 1: Default
    ins()
    {
        mnemonic = "";
        syntax = "";
        bytes = "1";
        description = "";
        action = "";
        flags = "None";
        example = "";
        next = nullptr;
        prev = nullptr;
    }

    // Overloaded Constructor 2: 3 Parameters (Quick creation)
    ins(string m, string desc, string ex)
    {
        mnemonic = m;
        syntax = m;
        bytes = "1";
        description = desc;
        action = "";
        flags = "None";
        example = ex;
        next = nullptr;
        prev = nullptr;
    }

    // Overloaded Constructor 3: Full 7 Parameters
    ins(string m, string syn, string b, string desc, string act, string flg, string ex)
    {
        mnemonic = m;
        syntax = syn;
        bytes = b;
        description = desc;
        action = act;
        flags = flg;
        example = ex;
        next = nullptr;
        prev = nullptr;
    }

    // Getters
    string get_mnemonic() const { return mnemonic; }
    string get_syntax() const { return syntax; }
    string get_bytes() const { return bytes; }
    string get_description() const { return description; }
    string get_action() const { return action; }
    string get_flags() const { return flags; }
    string get_example() const { return example; }
    ins *get_next() const { return next; }
    ins *get_prev() const { return prev; }

    // Setters
    void set_mnemonic(const string &m) { mnemonic = m; }
    void set_syntax(const string &s) { syntax = s; }
    void set_bytes(const string &b) { bytes = b; }
    void set_description(const string &d) { description = d; }
    void set_action(const string &a) { action = a; }
    void set_flags(const string &f) { flags = f; }
    void set_example(const string &e) { example = e; }
    void set_next(ins *n) { next = n; }
    void set_prev(ins *p) { prev = p; }

    void print() const
    {
        cout << "  [" << syntax << "]  (Bytes: " << bytes << " | Flags: " << flags << ")" << endl;
        cout << "     Meaning : " << description << endl;
        if (!action.empty())
            cout << "     Action  : " << action << endl;
        if (!example.empty())
            cout << "     Example : " << example << endl;
    }
};

class ins_list
{
private:
    ins *head;
    ins *tail;
    int size;

public:
    ins_list()
    {
        head = nullptr;
        tail = nullptr;
        size = 0;
    }

    ~ins_list()
    {
        ins *t = head;
        while (t != nullptr)
        {
            ins *next_node = t->get_next();
            delete t;
            t = next_node;
        }
    }

    int get_size() const { return size; }
    ins *get_head() const { return head; }
    ins *get_tail() const { return tail; }

    // CRUD - CREATE (Overloaded Method 1: Insert existing node pointer)
    void add_tail(ins *new_ins)
    {
        if (size == 0)
        {
            head = new_ins;
            tail = new_ins;
        }
        else
        {
            tail->set_next(new_ins);
            new_ins->set_prev(tail);
            tail = new_ins;
        }
        size++;
    }

    // CRUD - CREATE (Overloaded Method 2: Construct and insert from strings)
    void add_tail(string mnemonic, string desc, string example)
    {
        ins *node = new ins(mnemonic, desc, example);
        add_tail(node);
    }

    // CRUD - READ (Overloaded Method 1: Search by mnemonic or syntax)
    ins *search_ins(const string &target) const
    {
        string q = to_upper(trim(target));
        ins *t = head;
        while (t != nullptr)
        {
            if (to_upper(t->get_mnemonic()) == q || to_upper(t->get_syntax()) == q)
            {
                return t;
            }
            t = t->get_next();
        }
        return nullptr;
    }

    // CRUD - READ (Overloaded Method 2: Search by 0-based index)
    ins *search_ins(int index) const
    {
        if (index < 0 || index >= size)
            return nullptr;
        ins *t = head;
        for (int i = 0; i < index; i++)
        {
            t = t->get_next();
        }
        return t;
    }

    // CRUD - UPDATE: Modify an existing instruction's details
    bool update_ins(const string &target, const string &new_desc, const string &new_flags, const string &new_ex)
    {
        ins *found = search_ins(target);
        if (found == nullptr)
            return false;

        if (!new_desc.empty())
            found->set_description(new_desc);
        if (!new_flags.empty())
            found->set_flags(new_flags);
        if (!new_ex.empty())
            found->set_example(new_ex);
        return true;
    }

    // CRUD - DELETE: Remove node from Doubly Linked List and free memory
    bool delete_ins(const string &target)
    {
        ins *found = search_ins(target);
        if (found == nullptr)
            return false;

        if (found == head && found == tail)
        {
            head = nullptr;
            tail = nullptr;
        }
        else if (found == head)
        {
            head = found->get_next();
            head->set_prev(nullptr);
        }
        else if (found == tail)
        {
            tail = found->get_prev();
            tail->set_next(nullptr);
        }
        else
        {
            found->get_prev()->set_next(found->get_next());
            found->get_next()->set_prev(found->get_prev());
        }

        delete found;
        size--;
        return true;
    }

    int print_matches(const string &target) const
    {
        string q = to_upper(trim(target));
        int count = 0;
        ins *t = head;
        while (t != nullptr)
        {
            if (to_upper(t->get_mnemonic()) == q || to_upper(t->get_syntax()) == q)
            {
                t->print();
                cout << endl;
                count++;
            }
            t = t->get_next();
        }
        return count;
    }

    void print_all() const
    {
        if (size == 0)
        {
            cout << "  (No instructions in this category)" << endl;
            return;
        }
        ins *t = head;
        int idx = 1;
        while (t != nullptr)
        {
            cout << idx++ << ".";
            t->print();
            cout << endl;
            t = t->get_next();
        }
    }
};

// FILE I/O - SAVE: Persist all 5 Doubly Linked Lists to disk (pipe-delimited format)
inline bool save_instructions_to_file(const string &filename, ins_list lists[5])
{
    ofstream out(filename);
    if (!out.is_open())
        return false;

    for (int cat = 0; cat < 5; cat++)
    {
        ins *curr = lists[cat].get_head();
        while (curr != nullptr)
        {
            out << cat << "|"
                << curr->get_mnemonic() << "|"
                << curr->get_syntax() << "|"
                << curr->get_bytes() << "|"
                << curr->get_description() << "|"
                << curr->get_action() << "|"
                << curr->get_flags() << "|"
                << curr->get_example() << "\n";
            curr = curr->get_next();
        }
    }
    out.close();
    return true;
}

// FILE I/O - LOAD SAVED DB: Reads pipe-delimited storage file
inline bool load_saved_instructions(const string &filename, ins_list lists[5])
{
    ifstream in(filename);
    if (!in.is_open())
        return false;

    string line;
    while (getline(in, line))
    {
        if (trim(line).empty())
            continue;
        stringstream ss(line);
        string cat_str, mnem, syn, bytes, desc, act, flg, ex;

        getline(ss, cat_str, '|');
        getline(ss, mnem, '|');
        getline(ss, syn, '|');
        getline(ss, bytes, '|');
        getline(ss, desc, '|');
        getline(ss, act, '|');
        getline(ss, flg, '|');
        getline(ss, ex, '|');

        int cat = stoi(cat_str);
        if (cat >= 0 && cat < 5)
        {
            lists[cat].add_tail(new ins(mnem, syn, bytes, desc, act, flg, ex));
        }
    }
    in.close();
    return true;
}

// FILE I/O - SEED FROM instruction.txt: Initial parse from reference text file
inline bool load_from_reference_txt(const string &filename, ins_list lists[5])
{
    ifstream file(filename);
    if (!file.is_open())
        return false;

    string line;
    int current_cat = -1;
    ins *current_ins = nullptr;

    while (getline(file, line))
    {
        string t = trim(line);
        if (t.empty() || t[0] == '=' || t[0] == '-')
            continue;

        if (t.find("DATA TRANSFER INSTRUCTIONS") != string::npos)
        {
            current_cat = 0;
            current_ins = nullptr;
            continue;
        }
        if (t.find("ARITHMETIC INSTRUCTIONS") != string::npos)
        {
            current_cat = 1;
            current_ins = nullptr;
            continue;
        }
        if (t.find("LOGICAL INSTRUCTIONS") != string::npos)
        {
            current_cat = 2;
            current_ins = nullptr;
            continue;
        }
        if (t.find("BRANCH INSTRUCTIONS") != string::npos)
        {
            current_cat = 3;
            current_ins = nullptr;
            continue;
        }
        if (t.find("CONTROL INSTRUCTIONS") != string::npos)
        {
            current_cat = 4;
            current_ins = nullptr;
            continue;
        }

        if (current_cat == -1)
            continue;

        if (current_ins != nullptr && (line[0] == ' ' || line[0] == '\t'))
        {
            if (t.find("Meaning :") == 0)
                current_ins->set_description(trim(t.substr(9)));
            else if (t.find("Action  :") == 0)
                current_ins->set_action(trim(t.substr(9)));
            else if (t.find("Flags   :") == 0)
                current_ins->set_flags(trim(t.substr(9)));
            else if (t.find("Example :") == 0)
                current_ins->set_example(trim(t.substr(9)));
            else if (!current_ins->get_description().empty() && t.find(":") == string::npos)
            {
                current_ins->set_description(current_ins->get_description() + " " + t);
            }
            continue;
        }

        size_t op_pos = t.find("Opcode:");
        if (op_pos != string::npos)
        {
            string syn = trim(t.substr(0, op_pos));
            string byte_val = "1";
            size_t b_pos = t.find("Bytes:");
            if (b_pos != string::npos)
                byte_val = trim(t.substr(b_pos + 6));

            string mnem = syn;
            size_t sp_pos = syn.find(' ');
            if (sp_pos != string::npos)
                mnem = syn.substr(0, sp_pos);

            string default_flags = (current_cat == 0) ? "None" : "Varies";
            current_ins = new ins(mnem, syn, byte_val, "", "", default_flags, "");
            lists[current_cat].add_tail(current_ins);
        }
    }

    file.close();
    return true;
}

#endif