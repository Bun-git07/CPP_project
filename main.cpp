#include "class.hpp"
#include "behave.hpp"
#include <iostream>
#include <string>

using namespace std;

const string STORAGE_FILE = "custom_instructions.txt";
const string CATEGORY_NAMES[5] = {"Data Transfer", "Arithmetic", "Logical", "Branch", "Control"};

int choose_category()
{
    cout << "\nSelect Category:" << endl;
    for (int i = 0; i < 5; i++)
    {
        cout << "  " << (i + 1) << ". " << CATEGORY_NAMES[i] << endl;
    }
    cout << "Category (1-5): ";
    int cat;
    cin >> cat;
    cin.ignore(1000, '\n');
    if (cat < 1 || cat > 5)
        return -1;
    return cat - 1;
}

// CRUD Management Menu (Create, Update, Delete + Auto-Save File I/O)
void manage_instructions_menu(ins_list lists[5])
{
    int opt = -1;
    while (opt != 0)
    {
        cout << "\n=== Manage Instruction Library (CRUD) ===" << endl;
        cout << "  1. Add New Instruction (Create)" << endl;
        cout << "  2. Update Existing Instruction (Update)" << endl;
        cout << "  3. Delete Instruction (Delete)" << endl;
        cout << "  0. Back to Main Menu" << endl;
        cout << "Choice: ";
        cin >> opt;
        cin.ignore(1000, '\n');

        if (opt == 1)
        {
            int cat = choose_category();
            if (cat == -1)
            {
                cout << "Invalid category." << endl;
                continue;
            }

            string mnem, syn, bytes, desc, act, flg, ex;
            cout << "Mnemonic (e.g., ANA): ";
            getline(cin, mnem);
            cout << "Syntax (e.g., ANA r): ";
            getline(cin, syn);
            cout << "Bytes (e.g., 1): ";
            getline(cin, bytes);
            cout << "Meaning/Description: ";
            getline(cin, desc);
            cout << "Action (e.g., (A) <- (A) & (r)): ";
            getline(cin, act);
            cout << "Flags Affected (e.g., S, Z, AC, P, CY): ";
            getline(cin, flg);
            cout << "Example (e.g., ANA B): ";
            getline(cin, ex);

            lists[cat].add_tail(new ins(mnem, syn, bytes, desc, act, flg, ex));
            save_instructions_to_file(STORAGE_FILE, lists);
            cout << "Instruction added and saved to " << STORAGE_FILE << "!" << endl;
        }
        else if (opt == 2)
        {
            cout << "Enter mnemonic or syntax to update (e.g., ADD or ADD r): ";
            string target;
            getline(cin, target);

            bool updated = false;
            for (int i = 0; i < 5; i++)
            {
                if (lists[i].search_ins(target) != nullptr)
                {
                    string new_desc, new_flags, new_ex;
                    cout << "New Description (leave blank to keep current): ";
                    getline(cin, new_desc);
                    cout << "New Flags (leave blank to keep current): ";
                    getline(cin, new_flags);
                    cout << "New Example (leave blank to keep current): ";
                    getline(cin, new_ex);

                    lists[i].update_ins(target, new_desc, new_flags, new_ex);
                    updated = true;
                    break;
                }
            }
            if (updated)
            {
                save_instructions_to_file(STORAGE_FILE, lists);
                cout << "Instruction updated and saved!" << endl;
            }
            else
            {
                cout << "Instruction not found." << endl;
            }
        }
        else if (opt == 3)
        {
            cout << "Enter mnemonic or syntax to delete (e.g., MOV r, M): ";
            string target;
            getline(cin, target);

            bool deleted = false;
            for (int i = 0; i < 5; i++)
            {
                if (lists[i].delete_ins(target))
                {
                    deleted = true;
                    break;
                }
            }
            if (deleted)
            {
                save_instructions_to_file(STORAGE_FILE, lists);
                cout << "Instruction deleted from Doubly Linked List and saved!" << endl;
            }
            else
            {
                cout << "Instruction not found." << endl;
            }
        }
    }
}

void run_simulator_menu(Simulator &sim)
{
    sim.reset();
    cout << "\n=== 8085 Simulator (Queue + Stack Undo) ===" << endl;
    cout << "Supported: MVI, MOV, ADD, ADI, SUB, INR, DCR, HLT" << endl;
    cout << "Enter instructions one per line (e.g., MVI A, 25H). Type 'RUN' when done:" << endl;

    string line;
    while (true)
    {
        cout << "> ";
        getline(cin, line);
        if (to_upper(trim(line)) == "RUN")
            break;
        sim.add_instruction(line);
    }

    int opt = -1;
    while (opt != 0)
    {
        cout << "\n[Queued: " << sim.get_queue_size() << " | Undo steps: " << sim.get_history_size() << "]" << endl;
        cout << "  1. Step Next (Queue Pop)" << endl;
        cout << "  2. Undo Last Step (Stack Pop)" << endl;
        cout << "  0. Back to Main Menu" << endl;
        cout << "Choice: ";
        cin >> opt;
        cin.ignore(1000, '\n');

        if (opt == 1)
            sim.step_next();
        else if (opt == 2)
            sim.undo();
    }
}

void run_quiz_menu()
{
    struct Question
    {
        string prompt;
        string options[4];
        char answer;
        string explanation;
    };

    Question quiz[5] = {
        {"1. Which flags are affected by data transfer instructions like MOV and MVI?",
         {"A. All 5 flags", "B. Only Zero and Carry", "C. None", "D. Only Sign and Parity"},
         'C',
         "Data transfer instructions only copy data and never modify CPU flags."},
        {"2. If Register A = 05H and B = 0AH, what is A after 'ADD B'?",
         {"A. 0FH", "B. 15H", "C. 05H", "D. 00H"},
         'A',
         "05H + 0AH = 15 in decimal, which is 0FH in hexadecimal."},
        {"3. Which flag is NOT affected by the INR (Increment) instruction?",
         {"A. Zero (Z)", "B. Sign (S)", "C. Parity (P)", "D. Carry (CY)"},
         'D',
         "INR and DCR affect S, Z, AC, and P, but leave the Carry (CY) flag unchanged."},
        {"4. How many bytes does 'MVI A, 25H' occupy in memory?",
         {"A. 1 byte", "B. 2 bytes", "C. 3 bytes", "D. 4 bytes"},
         'B',
         "MVI r, d8 takes 1 byte for the opcode and 1 byte for the 8-bit immediate data (2 bytes total)."},
        {"5. What does 'XCHG' do in the 8085 microprocessor?",
         {"A. Clears Register A", "B. Swaps BC with DE", "C. Swaps HL with DE", "D. Halts the CPU"},
         'C',
         "XCHG exchanges the contents of H with D and L with E."}};

    int score = 0;
    cout << "\n=== 8085 Assembly Knowledge Quiz ===" << endl;
    for (int i = 0; i < 5; i++)
    {
        cout << "\n"
             << quiz[i].prompt << endl;
        for (int j = 0; j < 4; j++)
            cout << "  " << quiz[i].options[j] << endl;
        cout << "Your Answer (A/B/C/D): ";
        char ans;
        cin >> ans;
        cin.ignore(1000, '\n');

        if (toupper(ans) == quiz[i].answer)
        {
            cout << "Correct! " << quiz[i].explanation << endl;
            score++;
        }
        else
        {
            cout << "Incorrect (Answer: " << quiz[i].answer << "). " << quiz[i].explanation << endl;
        }
    }
    cout << "\nFinal Score: " << score << " / 5" << endl;
}

int main()
{
    ins_list lists[5];

    // Load saved CRUD file if it exists; otherwise seed from instruction.txt
    if (load_saved_instructions(STORAGE_FILE, lists))
    {
        cout << "[File I/O] Loaded instruction library from " << STORAGE_FILE << endl;
    }
    else if (load_from_reference_txt("instruction.txt", lists))
    {
        // Seed initial Logical, Branch, Control entries using overloaded add_tail()
        lists[2].add_tail("ANA", "Logical AND register with Accumulator.", "ANA B");
        lists[2].add_tail("ORA", "Logical OR register with Accumulator.", "ORA B");
        lists[3].add_tail("JMP", "Unconditional jump to 16-bit memory address.", "JMP 2000H");
        lists[4].add_tail("HLT", "Stops processor execution.", "HLT");
        save_instructions_to_file(STORAGE_FILE, lists);
        cout << "[File I/O] Seeded from instruction.txt and created " << STORAGE_FILE << endl;
    }
    else
    {
        cout << "[Warning] Reference files missing. Loading minimal defaults..." << endl;
        lists[0].add_tail("MOV", "Copies one register into another.", "MOV B, A");
        lists[1].add_tail("ADD", "Adds a register to A.", "ADD B");
        lists[4].add_tail("HLT", "Stops the processor.", "HLT");
    }

    Simulator sim;
    int choice = -1;

    while (choice != 0)
    {
        cout << "\n==========================================" << endl;
        cout << "   8085 INSTRUCTION LEARNING SIMULATOR    " << endl;
        cout << "==========================================" << endl;
        cout << "  1. Instruction Menu (Read / Browse)" << endl;
        cout << "  2. Search Instruction (Read / Lookup)" << endl;
        cout << "  3. Manage Instructions (Create / Update / Delete)" << endl;
        cout << "  4. Simulate Instruction (Queue & Stack)" << endl;
        cout << "  5. Take Quiz" << endl;
        cout << "  0. Exit" << endl;
        cout << "Choose Option: ";

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        cin.ignore(1000, '\n');

        if (choice == 1)
        {
            cout << "\nSelect Category:" << endl;
            for (int i = 0; i < 5; i++)
            {
                cout << "  " << (i + 1) << ". " << CATEGORY_NAMES[i]
                     << " (" << lists[i].get_size() << ")" << endl;
            }
            cout << "  6. Show All Categories" << endl;
            cout << "Category: ";
            int cat;
            cin >> cat;
            cin.ignore(1000, '\n');

            if (cat >= 1 && cat <= 5)
            {
                cout << "\n=== " << CATEGORY_NAMES[cat - 1] << " ===" << endl;
                lists[cat - 1].print_all();
            }
            else if (cat == 6)
            {
                for (int i = 0; i < 5; i++)
                {
                    cout << "\n=== " << CATEGORY_NAMES[i] << " ===" << endl;
                    lists[i].print_all();
                }
            }
        }
        else if (choice == 2)
        {
            cout << "\nSearch by (1) Mnemonic or (2) Index in Category? ";
            int mode;
            cin >> mode;
            cin.ignore(1000, '\n');

            if (mode == 1)
            {
                cout << "Enter mnemonic (e.g., MOV, ADD): ";
                string target;
                getline(cin, target);
                int found_count = 0;
                for (int i = 0; i < 5; i++)
                {
                    if (lists[i].search_ins(target) != nullptr)
                    {
                        cout << "\n--- " << CATEGORY_NAMES[i] << " ---" << endl;
                        found_count += lists[i].print_matches(target);
                    }
                }
                if (found_count == 0)
                    cout << "No matching instruction found." << endl;
            }
            else if (mode == 2)
            {
                int cat = choose_category();
                if (cat != -1)
                {
                    cout << "Enter index (0 to " << lists[cat].get_size() - 1 << "): ";
                    int idx;
                    cin >> idx;
                    cin.ignore(1000, '\n');
                    ins *res = lists[cat].search_ins(idx); // Calls overloaded search_ins(int)
                    if (res)
                        res->print();
                    else
                        cout << "Index out of range." << endl;
                }
            }
        }
        else if (choice == 3)
        {
            manage_instructions_menu(lists);
        }
        else if (choice == 4)
        {
            run_simulator_menu(sim);
        }
        else if (choice == 5)
        {
            run_quiz_menu();
        }
    }

    cout << "Exiting simulator. Goodbye!" << endl;
    return 0;
}