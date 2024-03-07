#include <bits/stdc++.h>

using namespace std;

unordered_set<string> R, I, S, SB, U, UJ;
unordered_map<string, unordered_map<string, string>> instructionData;
unordered_map<string, string> regToStr;
unordered_map<string, int> directivesSizes;
int MAX_IMM_12, MAX_IMM_20;

void initialiseStaticData()
{
    MAX_IMM_12 = pow(2, 11);
    MAX_IMM_20 = pow(2, 19);

    R = {"and", "add", "or", "sll", "slt", "sra", "srl", "sub", "xor", "mul", "div", "rem"};
    I = {"addi", "andi", "ori", "lb", "ld", "lh", "lw", "jalr"};
    S = {"sb", "sw", "sd", "sh"};
    SB = {"beq", "bne", "bge", "blt"};
    U = {"auipc", "lui"};
    UJ = {"jal"};

    directivesSizes[".byte"] = 1;
    directivesSizes[".half"] = 2;
    directivesSizes[".word"] = 4;
    directivesSizes[".dword"] = 8;
    directivesSizes[".asciiz"] = 1;

    instructionData["add"]["opcode"] = "0110011";
    instructionData["add"]["func3"] = "000";
    instructionData["add"]["func7"] = "0000000";

    instructionData["and"]["opcode"] = "0110011";
    instructionData["and"]["func3"] = "111";
    instructionData["and"]["func7"] = "0000000";

    instructionData["or"]["opcode"] = "0110011";
    instructionData["or"]["func3"] = "110";
    instructionData["or"]["func7"] = "0000000";

    instructionData["sll"]["opcode"] = "0110011";
    instructionData["sll"]["func3"] = "001";
    instructionData["sll"]["func7"] = "0000000";

    instructionData["slt"]["opcode"] = "0110011";
    instructionData["slt"]["func3"] = "010";
    instructionData["slt"]["func7"] = "0000000";

    instructionData["sra"]["opcode"] = "0110011";
    instructionData["sra"]["func3"] = "101";
    instructionData["sra"]["func7"] = "0100000";

    instructionData["srl"]["opcode"] = "0110011";
    instructionData["srl"]["func3"] = "101";
    instructionData["srl"]["func7"] = "0000000";

    instructionData["sub"]["opcode"] = "0110011";
    instructionData["sub"]["func3"] = "000";
    instructionData["sub"]["func7"] = "0100000";

    instructionData["xor"]["opcode"] = "0110011";
    instructionData["xor"]["func3"] = "100";
    instructionData["xor"]["func7"] = "0000000";

    instructionData["mul"]["opcode"] = "0110011";
    instructionData["mul"]["func3"] = "000";
    instructionData["mul"]["func7"] = "0000001";

    instructionData["div"]["opcode"] = "0110011";
    instructionData["div"]["func3"] = "100";
    instructionData["div"]["func7"] = "0000001";

    instructionData["rem"]["opcode"] = "0110011";
    instructionData["rem"]["func3"] = "110";
    instructionData["rem"]["func7"] = "0000001";

    instructionData["addi"]["opcode"] = "0010011";
    instructionData["addi"]["func3"] = "000";

    instructionData["andi"]["opcode"] = "0010011";
    instructionData["andi"]["func3"] = "111";

    instructionData["ori"]["opcode"] = "0010011";
    instructionData["ori"]["func3"] = "110";

    instructionData["lb"]["opcode"] = "0000011";
    instructionData["lb"]["func3"] = "000";

    instructionData["lh"]["opcode"] = "0000011";
    instructionData["lh"]["func3"] = "001";

    instructionData["lw"]["opcode"] = "0000011";
    instructionData["lw"]["func3"] = "010";

    instructionData["ld"]["opcode"] = "0000011";
    instructionData["ld"]["func3"] = "011";

    instructionData["jalr"]["opcode"] = "1100111";
    instructionData["jalr"]["func3"] = "000";

    instructionData["sb"]["opcode"] = "0100011";
    instructionData["sb"]["func3"] = "000";

    instructionData["sh"]["opcode"] = "0100011";
    instructionData["sh"]["func3"] = "001";

    instructionData["sw"]["opcode"] = "0100011";
    instructionData["sw"]["func3"] = "010";

    instructionData["sd"]["opcode"] = "0100011";
    instructionData["sd"]["func3"] = "011";

    instructionData["beq"]["opcode"] = "1100011";
    instructionData["beq"]["func3"] = "000";

    instructionData["bne"]["opcode"] = "1100011";
    instructionData["bne"]["func3"] = "001";

    instructionData["blt"]["opcode"] = "1100011";
    instructionData["blt"]["func3"] = "100";

    instructionData["bge"]["opcode"] = "1100011";
    instructionData["bge"]["func3"] = "101";

    instructionData["auipc"]["opcode"] = "0010111";

    instructionData["lui"]["opcode"] = "0110111";

    instructionData["jal"]["opcode"] = "1101111";

    regToStr["x0"] = "00000";
    regToStr["x1"] = "00001";
    regToStr["x2"] = "00010";
    regToStr["x3"] = "00011";
    regToStr["x4"] = "00100";
    regToStr["x5"] = "00101";
    regToStr["x6"] = "00110";
    regToStr["x7"] = "00111";
    regToStr["x8"] = "01000";
    regToStr["x9"] = "01001";
    regToStr["x10"] = "01010";
    regToStr["x11"] = "01011";
    regToStr["x12"] = "01100";
    regToStr["x13"] = "01101";
    regToStr["x14"] = "01110";
    regToStr["x15"] = "01111";
    regToStr["x16"] = "10000";
    regToStr["x17"] = "10001";
    regToStr["x18"] = "10010";
    regToStr["x19"] = "10011";
    regToStr["x20"] = "10100";
    regToStr["x21"] = "10101";
    regToStr["x22"] = "10110";
    regToStr["x23"] = "10111";
    regToStr["x24"] = "11000";
    regToStr["x25"] = "11001";
    regToStr["x26"] = "11010";
    regToStr["x27"] = "11011";
    regToStr["x28"] = "11100";
    regToStr["x29"] = "11101";
    regToStr["x30"] = "11110";
    regToStr["x31"] = "11111";
}