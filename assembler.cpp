#include <iostream>
#include <bitset>
#include <fstream>
#include "utilities.cpp"

using namespace std;

vector<string> input, output;
vector<pair<string, long long>> labels;

void preProcessInput(string fileName)
{
    fstream file;
    file.open(fileName, ios::in);

    string inp, label;
    long long pc = 0;

    if (file.is_open())
    {
        while (getline(file, inp))
        {

            size_t colonPos = inp.find(':');
            if (colonPos != string::npos)
            {
                label = inp.substr(0, colonPos);
                trim(label);

                inp = inp.substr(colonPos + 1);
                trim(inp);

                pair<string, long long> newLabel(label, pc);
                labels.push_back(newLabel);
            }

            input.push_back(inp);
            pc += 4;
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
    preProcessInput("input.asm");
    return 0;
}