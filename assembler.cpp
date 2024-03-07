// Main file containing the assembler logic

#include <bits/stdc++.h>
#include <fstream>       //For reading and writing in external files
#include "static.cpp"    //For using static/constant data
#include "utilities.cpp" //For utility functions for strings and numerical formats

using namespace std;

vector<string> dataLines, textLines, outputLines; // Store data, text and output in form of a vector
unordered_map<string, long long> labels;          // Stores labels along with their corresponding PC

// -------------------------------------------------------------------------------------------------------------
// Function for pre-processing assembly code
// and filling textLines, dataLines and labels vectors
void preProcessInput(string fileName)
{
    fstream file;
    file.open(fileName, ios::in); // Opening file

    string inp, label;
    long long pc = 0; // Initialising Program Counter

    bool isData = false;
    bool isText = true;

    if (file.is_open())
    {
        while (getline(file, inp)) // Taking one line at a time as input
        {
            trim(inp);

            if (inp == "" || inp[0] == '#') // If line is empty or is a comment
            {
                continue;
            }

            if (inp == ".data")
            {
                isData = true;
                isText = false;
                continue;
            }
            if (inp == ".text")
            {
                isData = false;
                isText = true;
                continue;
            }

            rectifyCode(inp); // Remove commas and extra spaces from command

            if (isData) // Handle data segment
            {
                dataLines.push_back(inp);
            }

            if (isText) // Handle text segment
            {
                // Check for labels and store them along with their corresponding PC
                size_t colonPos = inp.find(':');
                if (colonPos != string::npos) // If label detected
                {
                    label = inp.substr(0, colonPos);
                    trim(label);

                    inp = inp.substr(colonPos + 1);
                    trim(inp);

                    labels[label] = pc; // Adding label
                }

                textLines.push_back(inp);
                pc += 4;
            }
        }
    }
    else
    {
        // Error Handling
        cout << "Error in reading " << fileName << endl;
        cout << "Exiting program ." << endl;
        exit(-1);
    }

    file.close(); // Closing file
}

// -------------------------------------------------------------------------------------------------------------

// Function to handle R Format commands
void assemble_R(string code, long long pc)
{
    vector<string> words = split(code); // Split command into individual tokens
    string rs1, rs2, rd, opcode, func3, func7;

    // Fetching static Data
    opcode = instructionData[words[0]]["opcode"];
    func3 = instructionData[words[0]]["func3"];
    func7 = instructionData[words[0]]["func7"];

    rd = regToStr[words[1]];
    rs1 = regToStr[words[2]];
    rs2 = regToStr[words[3]];

    string bin = func7 + rs2 + rs1 + func3 + rd + opcode; // Assemble machine code in binary

    // Error Handling
    if (bin.length() != 32)
    {
        cout << "Error at pc : " << pc << endl
             << "Code : " << code << endl;
        exit(-1);
    }

    string output = decToHex(pc) + " " + binToHex(bin); // Building final output

    outputLines.push_back(output); // Adding output
}

// -------------------------------------------------------------------------------------------------------------

// Function to assemble I Format commands
void assemble_I(string code, long long pc)
{
    vector<string> words = split(code); // Split command into individual tokens
    string opcode, rs1, rd, func3, imm;
    int i;

    // Fetching static Data
    opcode = instructionData[words[0]]["opcode"];
    func3 = instructionData[words[0]]["func3"];
    rd = regToStr[words[1]];

    unordered_set<string> loadCommands = {"lb", "ld", "lh", "lw"};

    if (loadCommands.find(words[0]) != loadCommands.end()) // For load commands
    {
        // Finding position of parenthesis
        int startPos = words[2].find('(');
        int endPos = words[2].find(')');

        i = stoi(words[2].substr(0, startPos)); // extracting immediate value

        rs1 = regToStr[words[2].substr(startPos + 1, endPos - startPos - 1)]; // extracting rs1
    }
    else // For other I format commads
    {
        rs1 = regToStr[words[2]]; // extracting rs1
        i = stoi(words[3]);       // extracting immediate value
    }
    // Error Handling
    if (i < -MAX_IMM_12 || i > MAX_IMM_12 - 1)
    {
        cout << "Error at pc : " << pc << endl
             << "Immediate value out of bounds" << endl;
        exit(-1);
    }

    imm = decToImm(i, 12); // converting immediate to binary

    string bin = imm + rs1 + func3 + rd + opcode; // assembling command

    // Error Handling
    if (bin.length() != 32)
    {
        cout << "Error at pc : " << pc << endl
             << "Code : " << code << endl;
        exit(-1);
    }

    string output = decToHex(pc) + " " + binToHex(bin); // building final output

    outputLines.push_back(output); // adding output
}

// -------------------------------------------------------------------------------------------------------------

// Function to assemble S Format commands
void assemble_S(string code, long long pc)
{
    vector<string> words = split(code); // Split command into individual tokens
    string opcode, rs1, rs2, func3, imm;

    int startPos = words[2].find('(');
    int endPos = words[2].find(')');
    int i = stoi(words[2].substr(0, startPos)); // extracting immediate value

    // Error Handling
    if (i < -MAX_IMM_12 || i > MAX_IMM_12 - 1)
    {
        cout << "Error at pc : " << pc << endl
             << "Immediate value out of bounds" << endl;
        exit(-1);
    }

    // Fetching static Data
    opcode = instructionData[words[0]]["opcode"];
    func3 = instructionData[words[0]]["func3"];
    rs2 = regToStr[words[1]];
    rs1 = regToStr[words[2].substr(startPos + 1, endPos - startPos - 1)]; // extracting rs1
    imm = decToImm(i, 12);                                                // converting immediate to binary

    string bin = imm.substr(0, 7) + rs2 + rs1 + func3 + imm.substr(7) + opcode; // assembling instruction

    // Error Handling
    if (bin.length() != 32)
    {
        cout << "Error at pc : " << pc << endl
             << "Code : " << code << endl;
        exit(-1);
    }

    string output = decToHex(pc) + " " + binToHex(bin); // building final output

    outputLines.push_back(output); // adding output
}

// -------------------------------------------------------------------------------------------------------------

// Function to assemble SB Format commands
void assemble_SB(string code, long long pc)
{
    vector<string> words = split(code); // Split command into individual tokens
    string opcode, rs1, rs2, func3, imm;
    int i;

    // Fetching static Data
    rs1 = regToStr[words[1]];
    rs2 = regToStr[words[2]];
    func3 = instructionData[words[0]]["func3"];
    opcode = instructionData[words[0]]["opcode"];

    // extracting immediate value
    if ((words[3][0] >= 'A' && words[3][0] <= 'Z') || (words[3][0] >= 'a' && words[3][0] <= 'z')) // for labels
    {
        i = labels[words[3]] - pc;
    }
    else // for direct offsets
    {
        i = stoi(words[3]);
    }

    // Error Handling
    if (i < -(MAX_IMM_12 * 2) || i > (MAX_IMM_12 * 2) - 1)
    {
        cout << "Error at pc : " << pc << endl
             << "Immediate value out of bounds" << endl;
        exit(-1);
    }

    imm = decToImm(i, 13); // converting immediate to binary

    string bin = imm[0] + imm.substr(2, 6) + rs2 + rs1 + func3 + imm.substr(8, 4) + imm[1] + opcode; // assembling instruction

    // Error Handling
    if (bin.length() != 32)
    {
        cout << "Error atextLinest pc : " << pc << endl
             << "Code : " << code << endl;
        exit(-1);
    }

    string output = decToHex(pc) + " " + binToHex(bin); // building final output

    outputLines.push_back(output); // adding output
}

// -------------------------------------------------------------------------------------------------------------

// Function to assemble U Format commands
void assemble_U(string code, long long pc)
{
    vector<string> words = split(code); // Split command into individual tokens
    string opcode, rd, imm;
    int i;

    // Fetching static Data
    rd = regToStr[words[1]];
    opcode = instructionData[words[0]]["opcode"];
    i = stoi(words[2]); // extracting immediate value

    // Error Handling
    if (i < -MAX_IMM_20 || i > MAX_IMM_20 - 1)
    {
        cout << "Error at pc : " << pc << endl
             << "Immediate value out of bounds" << endl;
        exit(-1);
    }

    imm = decToImm(i, 20); // converting immediate to binary

    string bin = imm + rd + opcode; // assembling instruction

    // Error Handling
    if (bin.length() != 32)
    {
        cout << "Error atextLinest pc : " << pc << endl
             << "Code : " << code << endl;
        exit(-1);
    }

    string output = decToHex(pc) + " " + binToHex(bin); // building final output

    outputLines.push_back(output); // adding output
}

// -------------------------------------------------------------------------------------------------------------

// Function to assemble UJ Format commands
void assemble_UJ(string code, long long pc)
{
    vector<string> words = split(code); // Split command into individual tokens
    string opcode, rd, imm;
    int i;

    // Fetching static Data
    rd = regToStr[words[1]];
    opcode = instructionData[words[0]]["opcode"];

    // extracting immediate value
    if ((words[2][0] >= 'A' && words[2][0] <= 'Z') || (words[2][0] >= 'a' && words[2][0] <= 'z')) // for labels
    {
        i = labels[words[3]] - pc;
    }
    else // for direct offsets
    {
        i = stoi(words[2]);
    }

    // Error Handling
    if (i < -(MAX_IMM_20 * 2) || i > (MAX_IMM_20 * 2) - 1)
    {
        cout << "Error at pc : " << pc << endl
             << "Immediate value out of bounds" << endl;
        exit(-1);
    }

    imm = decToImm(i, 21); // converting immediate to binary

    string bin = imm[0] + imm.substr(10, 10) + imm[9] + imm.substr(1, 8) + rd + opcode; // assembling instruction

    // Error Handling
    if (bin.length() != 32)
    {
        cout << "Error atextLinest pc : " << pc << endl
             << "Code : " << code << endl;
        exit(-1);
    }

    string output = decToHex(pc) + " " + binToHex(bin); // building final output

    outputLines.push_back(output); // adding output
}

// -------------------------------------------------------------------------------------------------------------

// Function to assmeble .text segment of code
void assembleText()
{
    string code, op;
    long long pc;

    for (size_t i = 0; i < textLines.size(); i++)
    {
        code = textLines[i]; // extracting complete command
        pc = i * 4;          // getting pc

        int spacePos = code.find(' ');
        if (spacePos != string::npos)
        {
            op = code.substr(0, spacePos); // extracting the operator

            // Determinig the format of command and executing respective function
            if (R.find(op) != R.end()) // R Format
            {
                assemble_R(code, pc);
            }
            else if (I.find(op) != I.end()) // I Format
            {
                assemble_I(code, pc);
            }
            else if (S.find(op) != S.end()) // S Format
            {
                assemble_S(code, pc);
            }
            else if (SB.find(op) != SB.end()) // SB Format
            {
                assemble_SB(code, pc);
            }
            else if (U.find(op) != U.end()) // U Format
            {
                assemble_U(code, pc);
            }
            else if (UJ.find(op) != UJ.end()) // UJ Format
            {
                assemble_UJ(code, pc);
            }
            else // Error Hadling
            {

                cout << "Error at line " << i << " : " << code << endl;
                cout << "Exiting program ." << endl;
                exit(-1);
            }
        }
        else // Error Handling
        {
            cout << "Error at line " << i << " : " << code << endl;
            cout << "Exiting program ." << endl;
            exit(-1);
        }
    }

    // End of Program Output
    string endLine = decToHex(pc + 4) + " $";
    outputLines.push_back(endLine);
}

// -------------------------------------------------------------------------------------------------------------

// Function to assmeble .data segment of code
void assebleData()
{
    vector<string> words;          // stores tokens of a given command
    int size;                      // stores size of data element ot be added
    long long address = 268435456; // Starting address of data
    long long val;                 // Temporarily stores value form a command

    for (auto code : dataLines)
    {
        words = split(code); // split command into individual tokens

        if (directivesSizes.find(words[0]) != directivesSizes.end())
        {
            size = directivesSizes[words[0]]; // extract sixe of data to be stored
        }
        else // Error handling
        {
            cout << "Error at .data segment" << endl;
            cout << "Line : " << code << endl;
            exit(-1);
        }

        if (words[0] == ".asciiz") // for .ascii
        {
            string s = words[1].substr(1, words[1].length() - 2); // extract string

            for (int i = 0; i < s.length(); i++)
            {
                val = s[i]; // get ASCII value

                // Error handling
                if (val < 0 || val > 255)
                {
                    cout << "Error at .data segment" << endl;
                    cout << "Line : " << code << endl;
                    cout << "Value out of bounds" << endl;
                    exit(-1);
                }

                string output = decToHex(address) + " " + decToHex(val); // building final output
                outputLines.push_back(output);                           // adding output
                address += size;                                         // updating data Address
            }
        }
        else // for other directives
        {
            for (int i = 1; i < words.size(); i++)
            {
                val = stol(words[i]); // Extracting values

                // Error handling
                if (val < -(pow(2, size * 8 - 1)) || val > pow(2, size * 8 - 1) - 1)
                {
                    cout << "Error at .data segment" << endl;
                    cout << "Line : " << code << endl;
                    cout << "Value out of bounds" << endl;
                    exit(-1);
                }

                string output = decToHex(address) + " " + decToHex(val); // building final output
                outputLines.push_back(output);                           // adding output
                address += size;                                         // updating data Address
            }
        }
    }
}

// -------------------------------------------------------------------------------------------------------------

// Function to write the output in requird file
void writeOutput(string fileName)
{
    fstream file;
    file.open(fileName, ios::out); // Opening file

    if (file.is_open())
    {
        for (auto s : outputLines)
        {
            file << s << endl;
        }
    }
    else // Error handling
    {
        cout << "Error in reading " << fileName << endl;
        cout << "Exiting program ." << endl;
        exit(-1);
    }

    file.close(); // Closing file
}

// -------------------------------------------------------------------------------------------------------------

// MAIN Function
int main()
{
    initialiseStaticData();        // initialise all constants required for assembling
    preProcessInput("sample.asm"); // Pre-process the program
    assembleText();                // Assemble .text segment
    assebleData();                 // Assemble .data segment
    writeOutput("output.mc");      // Write output

    return 0;
}

// -------------------------------------------------------------------------------------------------------------