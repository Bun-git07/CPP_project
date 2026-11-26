#ifndef BEHAVE_HPP
#define BEHAVE_HPP

#include "class.hpp"
#include <iostream>
#include <string>
#include <queue>
#include <stack>

using namespace std;

struct CPU
{
    int A = 0, B = 0, C = 0, D = 0, E = 0, H = 0, L = 0;
    int S = 0, Z = 0, AC = 0, P = 0, CY = 0;
    string last_cmd = "START";
};

class Simulator
{
private:
    CPU cpu;
    queue<string> q;    // Queue: holds instructions in FIFO order
    stack<CPU> history; // Stack: saves previous CPU states for Undo

    int *get_reg(const string &r)
    {
        if (r == "A")
            return &cpu.A;
        if (r == "B")
            return &cpu.B;
        if (r == "C")
            return &cpu.C;
        if (r == "D")
            return &cpu.D;
        if (r == "E")
            return &cpu.E;
        if (r == "H")
            return &cpu.H;
        if (r == "L")
            return &cpu.L;
        return nullptr;
    }

    int parse_hex(string val)
    {
        val = to_upper(trim(val));
        if (!val.empty() && val.back() == 'H')
            val.pop_back();
        return stoi(val, nullptr, 16);
    }

    void update_flags(int result)
    {
        cpu.Z = (result == 0) ? 1 : 0;
        cpu.S = (result >= 128) ? 1 : 0;
        int ones = 0;
        for (int i = 0; i < 8; i++)
        {
            if ((result >> i) & 1)
                ones++;
        }
        cpu.P = (ones % 2 == 0) ? 1 : 0;
    }

public:
    // Getters for encapsulated Queue/Stack sizes
    int get_queue_size() const { return static_cast<int>(q.size()); }
    int get_history_size() const { return static_cast<int>(history.size()); }

    void add_instruction(string line)
    {
        line = to_upper(trim(line));
        if (!line.empty())
        {
            q.push(line);
        }
    }

    void step_next()
    {
        if (q.empty())
        {
            cout << "Queue is empty! Nothing left to run." << endl;
            return;
        }

        string cmd = q.front();
        q.pop(); // FIFO pop

        history.push(cpu); // LIFO push snapshot before execution
        cpu.last_cmd = cmd;

        string op, arg1, arg2;
        size_t space = cmd.find(' ');
        if (space == string::npos)
        {
            op = cmd;
        }
        else
        {
            op = cmd.substr(0, space);
            string rest = cmd.substr(space + 1);
            size_t comma = rest.find(',');
            if (comma == string::npos)
            {
                arg1 = trim(rest);
            }
            else
            {
                arg1 = trim(rest.substr(0, comma));
                arg2 = trim(rest.substr(comma + 1));
            }
        }

        if (op == "MVI")
        {
            int *reg = get_reg(arg1);
            if (reg)
                *reg = parse_hex(arg2) & 0xFF;
        }
        else if (op == "MOV")
        {
            int *dst = get_reg(arg1);
            int *src = get_reg(arg2);
            if (dst && src)
                *dst = *src;
        }
        else if (op == "ADD")
        {
            int *src = get_reg(arg1);
            if (src)
            {
                int sum = cpu.A + *src;
                cpu.CY = (sum > 0xFF) ? 1 : 0;
                cpu.A = sum & 0xFF;
                update_flags(cpu.A);
            }
        }
        else if (op == "ADI")
        {
            int sum = cpu.A + parse_hex(arg1);
            cpu.CY = (sum > 0xFF) ? 1 : 0;
            cpu.A = sum & 0xFF;
            update_flags(cpu.A);
        }
        else if (op == "SUB")
        {
            int *src = get_reg(arg1);
            if (src)
            {
                int diff = cpu.A - *src;
                cpu.CY = (diff < 0) ? 1 : 0;
                cpu.A = diff & 0xFF;
                update_flags(cpu.A);
            }
        }
        else if (op == "INR")
        {
            int *reg = get_reg(arg1);
            if (reg)
            {
                *reg = (*reg + 1) & 0xFF;
                update_flags(*reg);
            }
        }
        else if (op == "DCR")
        {
            int *reg = get_reg(arg1);
            if (reg)
            {
                *reg = (*reg - 1) & 0xFF;
                update_flags(*reg);
            }
        }
        else if (op == "HLT")
        {
            while (!q.empty())
                q.pop();
            cout << "Processor Halted (HLT)." << endl;
        }
        else
        {
            cout << "Unsupported instruction: " << cmd << endl;
            history.pop();
            return;
        }

        cout << "\nExecuted: " << cmd << endl;
        show_cpu();
    }

    void undo()
    {
        if (history.empty())
        {
            cout << "Nothing to undo!" << endl;
            return;
        }
        string undone = cpu.last_cmd;
        cpu = history.top();
        history.pop(); // LIFO pop

        cout << "\nUndid: " << undone << endl;
        show_cpu();
    }

    void show_cpu() const
    {
        cout << hex << uppercase;
        cout << "  Registers -> A:" << cpu.A << "  B:" << cpu.B << "  C:" << cpu.C
             << "  D:" << cpu.D << "  E:" << cpu.E << "  H:" << cpu.H << "  L:" << cpu.L << endl;
        cout << dec;
        cout << "  Flags     -> S:" << cpu.S << "  Z:" << cpu.Z << "  AC:" << cpu.AC
             << "  P:" << cpu.P << "  CY:" << cpu.CY << endl;
    }

    void reset()
    {
        cpu = CPU();
        while (!q.empty())
            q.pop();
        while (!history.empty())
            history.pop();
    }
};

#endif