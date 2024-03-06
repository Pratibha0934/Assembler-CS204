#include <bits/stdc++.h>
#include <fstream>
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

            if (inp == "")
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
            }
            else if (S.find(op) != S.end())
            {
            }
            else if (SB.find(op) != SB.end())
            {
            }
            else if (U.find(op) != U.end())
            {
            }
            else if (UJ.find(op) != UJ.end())
            {
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
}

int main()
{
    initialiseStaticData();
    preProcessInput("sample.asm");
    assembleText();
    // for (auto x : dataLines)
    // {
    //     cout << x << endl;
    // }

    // cout << "/////" << endl;

    // for (auto x : textLines)
    // {
    //     cout << x << endl;
    // }
    for (auto x : outputLines)
    {
        cout << x << endl;
    }
    return 0;
}