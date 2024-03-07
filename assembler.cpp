#include <bits/stdc++.h>
#include <fstream>
#include <stdexcept>
#include "static.cpp"
#include "utilities.cpp"

using namespace std;

vector<string> dataLines, textLines, outputLines;
unordered_map<string, long long> labels;

void preProcessInput(string fileName)
{
    fstream file;
    file.open(fileName, ios::in);

    string inp, label;
    long long pc = 0;

    bool isData = false;
    bool isText = true;

    if (file.is_open())
    {
        while (getline(file, inp))
        {
            trim(inp);

            if (inp == "" || inp[0] == '#')
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

            rectifyCode(inp);

            if (isData)
            {
                dataLines.push_back(inp);
            }

            if (isText)
            {
                size_t colonPos = inp.find(':');
                if (colonPos != string::npos)
                {
                    label = inp.substr(0, colonPos);
                    trim(label);

                    inp = inp.substr(colonPos + 1);
                    trim(inp);

                    labels[label] = pc;
                }

                textLines.push_back(inp);
                pc += 4;
            }
        }
    }
    else
    {
        cout << "Error in reading " << fileName << endl;
        cout << "Exiting program ." << endl;
        exit(-1);
    }

    file.close();
}

void assemble_R(string code, long long pc)
{
    vector<string> words = split(code);
    string rs1, rs2, rd, opcode, func3, func7;

    opcode = instructionData[words[0]]["opcode"];
    func3 = instructionData[words[0]]["func3"];
    func7 = instructionData[words[0]]["func7"];

    rd = regToStr[words[1]];
    rs1 = regToStr[words[2]];
    rs2 = regToStr[words[3]];

    string bin = func7 + rs2 + rs1 + func3 + rd + opcode;

    if (bin.length() != 32)
    {
        cout << "Error at pc : " << pc << endl
             << "Code : " << code << endl;
        exit(-1);
    }

    string output = decToHex(pc) + " " + binToHex(bin);

    outputLines.push_back(output);
}

void assemble_I(string code, long long pc)
{
    vector<string> words = split(code);
    string opcode, rs1, rd, func3, imm;
    int i;

    opcode = instructionData[words[0]]["opcode"];
    func3 = instructionData[words[0]]["func3"];
    rd = regToStr[words[1]];

    unordered_set<string> loadCommands = {"lb", "ld", "lh", "lw"};

    if (loadCommands.find(words[0]) != loadCommands.end())
    {
        int startPos = words[2].find('(');
        int endPos = words[2].find(')');

        i = stoi(words[2].substr(0, startPos));

        rs1 = regToStr[words[2].substr(startPos + 1, endPos - startPos - 1)];
    }
    else
    {
        rs1 = regToStr[words[2]];
        i = stoi(words[3]);
    }

    if (i < -MAX_IMM_12 || i > MAX_IMM_12 - 1)
    {
        cout << "Error at pc : " << pc << endl
             << "Immediate value out of bounds" << endl;
        exit(-1);
    }

    imm = decToImm(i, 12);

    string bin = imm + rs1 + func3 + rd + opcode;

    if (bin.length() != 32)
    {
        cout << "Error at pc : " << pc << endl
             << "Code : " << code << endl;
        exit(-1);
    }

    string output = decToHex(pc) + " " + binToHex(bin);

    outputLines.push_back(output);
}

void assemble_S(string code, long long pc)
{
    vector<string> words = split(code);
    string opcode, rs1, rs2, func3, imm;

    int startPos = words[2].find('(');
    int endPos = words[2].find(')');
    int i = stoi(words[2].substr(0, startPos));

    if (i < -MAX_IMM_12 || i > MAX_IMM_12 - 1)
    {
        cout << "Error at pc : " << pc << endl
             << "Immediate value out of bounds" << endl;
        exit(-1);
    }

    opcode = instructionData[words[0]]["opcode"];
    func3 = instructionData[words[0]]["func3"];
    rs2 = regToStr[words[1]];
    rs1 = regToStr[words[2].substr(startPos + 1, endPos - startPos - 1)];
    imm = decToImm(i, 12);

    string bin = imm.substr(0, 7) + rs2 + rs1 + func3 + imm.substr(7) + opcode;

    if (bin.length() != 32)
    {
        cout << "Error at pc : " << pc << endl
             << "Code : " << code << endl;
        exit(-1);
    }

    string output = decToHex(pc) + " " + binToHex(bin);

    outputLines.push_back(output);
}

void assemble_SB(string code, long long pc)
{
    vector<string> words = split(code);
    string opcode, rs1, rs2, func3, imm;
    int i;

    rs1 = regToStr[words[1]];
    rs2 = regToStr[words[2]];
    func3 = instructionData[words[0]]["func3"];
    opcode = instructionData[words[0]]["opcode"];

    if ((words[3][0] >= 'A' && words[3][0] <= 'Z') || (words[3][0] >= 'a' && words[3][0] <= 'z'))
    {
        i = labels[words[3]] - pc;
    }
    else
    {
        i = stoi(words[3]);
    }

    if (i < -(MAX_IMM_12 * 2) || i > (MAX_IMM_12 * 2) - 1)
    {
        cout << "Error at pc : " << pc << endl
             << "Immediate value out of bounds" << endl;
        exit(-1);
    }

    imm = decToImm(i, 13);

    string bin = imm[0] + imm.substr(2, 6) + rs2 + rs1 + func3 + imm.substr(8, 4) + imm[1] + opcode;

    if (bin.length() != 32)
    {
        cout << "Error atextLinest pc : " << pc << endl
             << "Code : " << code << endl;
        exit(-1);
    }

    string output = decToHex(pc) + " " + binToHex(bin);

    outputLines.push_back(output);
}

void assemble_U(string code, long long pc)
{
    vector<string> words = split(code);
    string opcode, rd, imm;
    int i;

    rd = regToStr[words[1]];
    opcode = instructionData[words[0]]["opcode"];
    i = stoi(words[2]);

    if (i < -MAX_IMM_20 || i > MAX_IMM_20 - 1)
    {
        cout << "Error at pc : " << pc << endl
             << "Immediate value out of bounds" << endl;
        exit(-1);
    }

    imm = decToImm(i, 20);

    string bin = imm + rd + opcode;

    if (bin.length() != 32)
    {
        cout << "Error atextLinest pc : " << pc << endl
             << "Code : " << code << endl;
        exit(-1);
    }

    string output = decToHex(pc) + " " + binToHex(bin);

    outputLines.push_back(output);
}

void assemble_UJ(string code, long long pc)
{
    vector<string> words = split(code);
    string opcode, rd, imm;
    int i;

    rd = regToStr[words[1]];
    opcode = instructionData[words[0]]["opcode"];

    if ((words[2][0] >= 'A' && words[2][0] <= 'Z') || (words[2][0] >= 'a' && words[2][0] <= 'z'))
    {
        i = labels[words[3]] - pc;
    }
    else
    {
        i = stoi(words[2]);
    }

    if (i < -(MAX_IMM_20 * 2) || i > (MAX_IMM_20 * 2) - 1)
    {
        cout << "Error at pc : " << pc << endl
             << "Immediate value out of bounds" << endl;
        exit(-1);
    }

    imm = decToImm(i, 21);

    string bin = imm[0] + imm.substr(10, 10) + imm[9] + imm.substr(1, 8) + rd + opcode;

    if (bin.length() != 32)
    {
        cout << "Error atextLinest pc : " << pc << endl
             << "Code : " << code << endl;
        exit(-1);
    }

    string output = decToHex(pc) + " " + binToHex(bin);

    outputLines.push_back(output);
}

void assembleText()
{
    string code, op;
    long long pc;

    for (size_t i = 0; i < textLines.size(); i++)
    {
        code = textLines[i];
        pc = i * 4;

        int spacePos = code.find(' ');
        if (spacePos != string::npos)
        {
            op = code.substr(0, spacePos);

            if (R.find(op) != R.end())
            {
                assemble_R(code, pc);
            }
            else if (I.find(op) != I.end())
            {
                assemble_I(code, pc);
            }
            else if (S.find(op) != S.end())
            {
                assemble_S(code, pc);
            }
            else if (SB.find(op) != SB.end())
            {
                assemble_SB(code, pc);
            }
            else if (U.find(op) != U.end())
            {
                assemble_U(code, pc);
            }
            else if (UJ.find(op) != UJ.end())
            {
                assemble_UJ(code, pc);
            }
            else
            {
                cout << "Error at line " << i << " : " << code << endl;
                cout << "Exiting program ." << endl;
                exit(-1);
            }
        }
        else
        {
            cout << "Error at line " << i << " : " << code << endl;
            cout << "Exiting program ." << endl;
            exit(-1);
        }
    }
    string endLine = decToHex(pc + 4) + " $";

    outputLines.push_back(endLine);
}

void assebleData()
{
    vector<string> words;
    int size;
    long long address = 268435456;
    long long val;

    for (auto code : dataLines)
    {
        words = split(code);

        if (directivesSizes.find(words[0]) != directivesSizes.end())
        {
            size = directivesSizes[words[0]];
        }
        else
        {
            cout << "Error at .data segment" << endl;
            cout << "Line : " << code << endl;
            exit(-1);
        }

        if (words[0] == ".asciiz")
        {
            string s = words[1].substr(1, words[1].length() - 2);

            for (int i = 0; i < s.length(); i++)
            {
                val = s[i];

                if (val < 0 || val > 255)
                {
                    cout << "Error at .data segment" << endl;
                    cout << "Line : " << code << endl;
                    cout << "Value out of bounds" << endl;
                    exit(-1);
                }
                string output = decToHex(address) + " " + decToHex(val);

                outputLines.push_back(output);
                address += size;
            }
        }
        else
        {
            for (int i = 1; i < words.size(); i++)
            {
                val = stol(words[i]);
                if (val < -(pow(2, size * 8 - 1)) || val > pow(2, size * 8 - 1) - 1)
                {
                    cout << "Error at .data segment" << endl;
                    cout << "Line : " << code << endl;
                    cout << "Value out of bounds" << endl;
                    exit(-1);
                }
                string output = decToHex(address) + " " + decToHex(val);

                outputLines.push_back(output);
                address += size;
            }
        }
    }
}

void writeOutput(string fileName)
{
    fstream file;

    file.open(fileName, ios::out);

    if (file.is_open())
    {
        for (auto s : outputLines)
        {
            file << s << endl;
        }
    }
    else
    {
        cout << "Error in reading " << fileName << endl;
        cout << "Exiting program ." << endl;
        exit(-1);
    }

    file.close();
}

int main()
{
    initialiseStaticData();
    preProcessInput("sample.asm");
    assembleText();
    assebleData();
    writeOutput("output.mc");

    return 0;
}