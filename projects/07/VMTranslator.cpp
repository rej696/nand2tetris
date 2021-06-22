#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>

using namespace std;

enum Command_T {
    C_NULL,
    C_ARITHMETIC,
    C_PUSH,
    C_POP,
    C_LABEL,
    C_GOTO,
    C_IF,
    C_FUNCTION,
    C_RETURN,
    C_CALL
};

class Parser {
    public:
    Parser(string filename) : line_count(0), filename(filename){
        current_command.clear();
        file.open(filename + ".vm");
        cout << filename << endl;
        cout << "created parser\n";
    }

    bool hasMoreCommands() {
        if (getline(file, current_line)) {
            return true;
        }
        else {
            file.close();
            cout << "file empty" << endl;
            return false;
        }
    }

    bool advance() {
        // read next command and makes it current command
        current_command.clear();
        string tmp = "";
        stringstream str_strm(current_line);
        while (str_strm >> tmp) {
            if (tmp == "//") {
                break;
            }
            current_command.push_back(tmp);
        }
        if (!current_command.empty()) {
            line_count++;
            cout << current_line << endl; // DEBUG
            cout << "advance parser to line " << line_count << endl; // DEBUG
            return true;
        } else {
            return false;
        }
    }

    Command_T commandType() {
        // return type that represents the type of the current command
        if (current_command[0] == "add") {return C_ARITHMETIC;}
        else if (current_command[0] == "sub") {return C_ARITHMETIC;}
        else if (current_command[0] == "neg") {return C_ARITHMETIC;}
        else if (current_command[0] == "eq") {return C_ARITHMETIC;}
        else if (current_command[0] == "gt") {return C_ARITHMETIC;}
        else if (current_command[0] == "lt") {return C_ARITHMETIC;}
        else if (current_command[0] == "and") {return C_ARITHMETIC;}
        else if (current_command[0] == "or") {return C_ARITHMETIC;}
        else if (current_command[0] == "not") {return C_ARITHMETIC;}
        else if (current_command[0] == "pop") {return C_POP;}
        else if (current_command[0] == "push") {return C_PUSH;}
        else if (current_command[0] == "label") {return C_LABEL;}
        else if (current_command[0] == "goto") {return C_GOTO;}
        else if (current_command[0] == "if-goto") {return C_IF;}
        else if (current_command[0] == "function") {return C_FUNCTION;}
        else if (current_command[0] == "call") {return C_CALL;}
        else if (current_command[0] == "return") {return C_RETURN;}
        else {return C_NULL;}
    }

    string arg1() {
        switch (commandType())
        {
        case C_ARITHMETIC:
            return current_command[0];
        case C_RETURN:
            // TODO error handling
            return "";
        case C_NULL:
            // TODO error handling
            return "";
        default:
            return current_command[1];
        }
    }

    int arg2() {
        switch (commandType())
        {
        case C_PUSH:
            return stoi(current_command[2]);
        case C_POP:
            return stoi(current_command[2]);
        case C_FUNCTION:
            return stoi(current_command[2]);
        case C_CALL:
            return stoi(current_command[2]);
        default:
            // TODO error handling
            return -1;
        }
    }

    private:
    ifstream file;
    string filename;
    string current_line;
    int line_count;
    vector<string> current_command;

};

class CodeWriter {
    public:
    CodeWriter(string filename) : filename(filename), line_count(0), eq_count(0), gt_count(0), lt_count(0) {
        file.open(filename + ".asm");
        cout << "created codewriter\n";
    }

    void writeArithmetic(string command) {
        if (command == "add") {
            file << "// ADD\n" 
            << "@SP\nA=M\nD=M\n" // Store top stack item in D reg
            << "@SP\nM=M-1\n" // Decrement stack pointer
            << "@SP\nA=M\nM=D+M\n"; // add D to top of stack 
        }
        else if (command == "sub") {
            file << "// SUB\n"
            << "@SP\nA=M\nD=M\n" // store top stack item in D reg
            << "@SP\nM=M-1\n" // decrement stack pointer
            << "@SP\nA=M\nM=M-D\n"; // sub D from top of stack
        }
        else if (command == "neg") {
            file << "// NEG\n"
            << "@SP\nA=M\nM=-M\n"; // negate item on top of stack
        }
        else if (command == "eq") {
            file << "// EQ\n"
            << "@SP\nA=M\nD=M\n" // store top stack item in D reg
            << "@SP\nM=M-1\n" // decrement stack pointer
            << "@SP\nA=M\nMD=M-D\n" // sub D from top of stack, store in top of stack and D register
            << "@EQ" << eq_count << "\n" // set A register to label
            << "D;JEQ\n" // Jump to EQ label if D is equal to 0
            << "@SP\nA=M\nM=0\n" // Store 0 in stack because values are not equal
            << "@EQEND" << eq_count << "\n0;JMP" // jump to end of calcuation
            << "(EQ" << eq_count << ")\n" // EQ label
            << "@SP\nA=M\nM=1\n" // Store 1 in stack because values are equal
            << "(EQEND" << eq_count << ")\n"; // EQEND label
            eq_count++;
        }
        else if (command == "gt") {
            file << "// GT\n"
            << "@SP\nA=M\nD=M\n" // store top stack item in D reg
            << "@SP\nM=M-1\n" // decrement stack pointer
            << "@SP\nA=M\nMD=M-D\n" // sub D from top of stack, store in top of stack and D register
            << "@GT" << gt_count << "\n" // set A register to label
            << "D;JGT\n" // Jump to GT label if D > 0
            << "@SP\nA=M\nM=0\n" // Store 0 in stack because D <= M
            << "@GTEND" << gt_count << "\n0;JMP" // jump to end of calcuation
            << "(GT" << gt_count << ")\n" // GT label
            << "@SP\nA=M\nM=1\n" // Store 1 in stack because D > M
            << "(GTEND" << gt_count << ")\n"; // GTEND label
            gt_count++;
        }
        else if (command == "lt") {
            file << "// LT\n"
            << "@SP\nA=M\nD=M\n" // store top stack item in D reg
            << "@SP\nM=M-1\n" // decrement stack pointer
            << "@SP\nA=M\nMD=M-D\n" // sub D from top of stack, store in top of stack and D register
            << "@LT" << lt_count << "\n" // set A register to label
            << "D;JLT\n" // Jump to LT label if D < 0
            << "@SP\nA=M\nM=0\n" // Store 0 in stack because D >= M
            << "@LTEND" << lt_count << "\n0;JMP" // jump to end of calcuation
            << "(LT" << lt_count << ")\n" // LT label
            << "@SP\nA=M\nM=1\n" // Store 1 in stack because D < M
            << "(LTEND" << lt_count << ")\n"; // LTEND label
            lt_count++;
        }
        else if (command == "and") {
            file << "// AND\n"
            << "@SP\nA=M\nD=M\n" // store top stack item in D reg
            << "@SP\nM=M-1\n" // decrement stack pointer
            << "@SP\nA=M\nM=D&M\n"; // and D and Top of stack
        }
        else if (command == "or") {
            file << "// OR\n"
            << "@SP\nA=M\nD=M\n" // store top stack item in D reg
            << "@SP\nM=M-1\n" // decrement stack pointer
            << "@SP\nA=M\nM=D|M\n"; // or D and top of stack
        }
        else if (command == "not") {
            file << "// NOT\n"
            << "@SP\nA=M\nM=!M\n"; // not top of stack
        }

    }

    void writePushPop(Command_T command, string segment, int index) {
        string seg_addr;
        if (segment == "constant" && command == C_PUSH) {
            file << "// PUSH CONSTANT\n"
            << "@SP\nM=M+1\n" // increment stack pointer
            << "@" << index << endl // get index of segment
            << "D=A\n" // store index in D reg
            << "@SP\nA=M\nM=D\n"; // store D reg at top of stack
        }
        else {
            if (segment == "local") {
                seg_addr = "@LCL\n";
            }
            else if (segment == "argument") {
                seg_addr = "@ARG\n";
            }
            else if (segment == "this") {
                seg_addr = "@THIS\n";
            }
            else if (segment == "that") {
                seg_addr = "@THAT\n";
            }
            else if (segment == "static") {
                stringstream ss;
                ss << '@' << filename << '.' << index << '\n';
                seg_addr = ss.str();
            }
            else if (segment == "temp") {
                stringstream ss;
                ss << '@' << index << '\n';
                seg_addr = ss.str();
            }
            else if (segment == "pointer" && index == 0) {
                seg_addr = "@THIS\n";
            }
            else if (segment == "pointer" && index == 1) {
                seg_addr = "@THAT\n";
            }
            else {
                // TODO ERROR HANDLING
                cout << "invalid push/pop command: " << command << segment << index << endl;
                return;
            }
        
            if (command == C_PUSH) {
                file << "// PUSH\n"
                << "@SP\nM=M+1\n" // increment stack pointer
                << "@" << index << endl // get index of segment
                << "D=A\n" // store index in D reg
                << seg_addr // select segment pointer // TODO MAKE VARIABLE
                << "D=D+M\n" // add index to segement pointer
                << "A=D\nD=M\n" // store contents of segment index in D reg
                << "@SP\nA=M\nM=D\n"; // store D reg at top of stack
            }
            else if (command == C_POP) {
                file << "// POP\n"
                << "@" << index << endl // get index of segment
                << "D=A\n" // store index in D reg
                << seg_addr // select segment pointer // TODO MAKE VARIABLE
                << "D=D+M\n" // add index to segement pointer
                << "@addr\nM=D\n" // store in addr variable
                << "@SP\nA=M\nD=M\n" // store top of stack in D reg
                << "@addr\nM=D\n" // store D reg at addr
                << "@SP\nM=M-1\n"; // decrement stack pointer
            }
            else {
                // TODO ERROR HANDLING
                cout << "invalid command: " << command << segment << index << endl;
            }
        }
    }

    void close() {
        file.close();
    }

    private:
    ofstream file;
    string filename;
    int line_count;
    int eq_count;
    int gt_count;
    int lt_count;

};


int main(int argc, char const *argv[])
{
    // figure out how to get file arguments
    string filename { argv[1] };
    Parser parser = Parser(filename);
    CodeWriter codewriter = CodeWriter(filename);

    while (parser.hasMoreCommands()) {
        if (parser.advance()) {
            Command_T command_type = parser.commandType();
            if (command_type == C_ARITHMETIC) {
                codewriter.writeArithmetic(parser.arg1());
            }
            else if ((command_type == C_PUSH) || (command_type == C_POP)) {
                codewriter.writePushPop(command_type, parser.arg1(), parser.arg2());
            }
        }
    }

    codewriter.close();
    return 0;
}
