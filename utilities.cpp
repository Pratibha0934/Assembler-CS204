#include <bits/stdc++.h>
#include <sstream>

using namespace std;

// String Utility Functions //////////////////////////////////////////////////////////////////
// trim from start
inline void ltrim(string &s)
{
    s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char ch)
                               { return !isspace(ch); }));
}

// trim from end
inline void rtrim(string &s)
{
    s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char ch)
                    { return !isspace(ch); })
                .base(),
            s.end());
}

// trim from both ends
inline void trim(string &s)
{
    rtrim(s);
    ltrim(s);
}

// Remove commas and consecutive spaces
inline void rectifyCode(string &s)
{
    s.erase(remove(s.begin(), s.end(), ','), s.end());
    s.erase(std::unique(s.begin(), s.end(), [](char a, char b)
                        { return a == ' ' && b == ' '; }),
            s.end());
}

// Split a string into words
vector<string> split(string s)
{
    vector<string> output;
    string word;

    istringstream iss(s);

    while (iss >> word)
    {
        output.push_back(word);
    }

    return output;
}
/////////////////////////////////////////////////////////////////////////////////////////////

// Number Base Conversion functions /////////////////////////////////////////////////////////

// Create map between binary number and its equivalent hexadecimal
void createMap(unordered_map<string, char> *um)
{
    (*um)["0000"] = '0';
    (*um)["0001"] = '1';
    (*um)["0010"] = '2';
    (*um)["0011"] = '3';
    (*um)["0100"] = '4';
    (*um)["0101"] = '5';
    (*um)["0110"] = '6';
    (*um)["0111"] = '7';
    (*um)["1000"] = '8';
    (*um)["1001"] = '9';
    (*um)["1010"] = 'A';
    (*um)["1011"] = 'B';
    (*um)["1100"] = 'C';
    (*um)["1101"] = 'D';
    (*um)["1110"] = 'E';
    (*um)["1111"] = 'F';
}

// convert binary to hexadecimal
string binToHex(string bin)
{
    string hex = "0x";
    int val;
    char ch;

    for (int i = 0; i <= 28; i += 4)
    {
        val = 0;
        for (int j = 0; j < 4; j++)
        {
            if (bin[i + j] == '1')
            {
                val += pow(2, 3 - j);
            }
        }

        if (val <= 9)
        {
            ch = 48 + val;
        }
        else
        {
            ch = 55 + val;
        }

        hex += ch;
    }

    return hex;
}

// convert decimal to hexadecimal
string decToHex(long long dec)
{
    string hex = "";

    int rem;
    char ch;

    while (dec != 0)
    {
        rem = dec % 16;

        if (rem < 10)
        {
            ch = rem + 48;
        }
        else
        {
            ch = rem + 55;
        }

        hex += ch;
        dec = dec / 16;
    }

    if (hex == "")
    {
        hex += '0';
    }

    reverse(hex.begin(), hex.end());

    return ("0x" + hex);
}
/////////////////////////////////////////////////////////////////////////////////////////////