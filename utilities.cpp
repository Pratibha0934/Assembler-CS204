#include <bits/stdc++.h>

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

// Convert decimal to hexadecimal
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

// Convert decimal to immediate value
string decToImm(int dec, int len)
{
    string bin = "";
    int n = dec;

    if (dec < 0)
    {
        n = n + pow(2, len);
    }

    int rem;

    while (n > 0)
    {
        bin = string(1, (char)(n % 2 + 48)) + bin;
        n = n / 2;
    }

    int bl = bin.length();

    for (int i = 0; i < len - bl; i++)
    {
        if (dec < 0)
        {
            bin = "1" + bin;
        }
        else
        {
            bin = "0" + bin;
        }
    }

    return bin;
}
/////////////////////////////////////////////////////////////////////////////////////////////