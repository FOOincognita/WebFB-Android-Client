#pragma once

//*	Definitions
//? WebFB Default Values
#define DEFAULT_IP	  "172.31.1.1"
#define DEFAULT_PORT  "10001"
#define MAXPKT		  16384
#define MAX_WPS    	  8192
#define TIMEOUT		  5

// Typedefs
typedef double latitude_t;
typedef unsigned long ulong32_t;
typedef struct manage_t {
    std::uint16_t  buf[MAXPKT];    // Data Buffer
} manage_t;

// Hex to Bin Lookup
const std::unordered_map<std::string, std::string> hexMap = {
    {"0","0000"},
    {"1","0001"},
    {"2","0010"},
    {"3","0011"},
    {"4","0100"},
    {"5","0101"},
    {"6","0110"},
    {"7","0111"},
    {"8","1000"},
    {"9","1001"},
    {"a","1010"},
    {"A","1010"},
    {"b","1011"},
    {"B","1011"},
    {"c","1100"},
    {"C","1100"},
    {"d","1101"},
    {"D","1101"},
    {"e","1110"},
    {"E","1110"},
    {"f","1111"},
    {"F","1111"}
};

// Hex to Bin Lookup
const std::unordered_map<std::string, std::string> binMap = {
    {"0000", "0"},
    {"0001", "1"},
    {"0010", "2"},
    {"0011", "3"},
    {"0100", "4"},
    {"0101", "5"},
    {"0110", "6"},
    {"0111", "7"},
    {"1000", "8"},
    {"1001", "9"},
    {"1010", "a"},
    {"1011", "b"},
    {"1100", "c"},
    {"1101", "d"},
    {"1110", "e"},
    {"1111", "f"},
    {"1010", "A"},
    {"1011", "B"},
    {"1100", "C"},
    {"1101", "D"},
    {"1110", "E"},
    {"1111", "F"}
};

// Oct to Bin Lookup
const std::unordered_map<std::string, std::string> octMap = {
    {"0","000"},
    {"1","001"},
    {"2","010"},
    {"3","011"},
    {"4","100"},
    {"5","101"},
    {"6","110"},
    {"7","111"}
};
/*
//! Conversion functions
/// Converts hex to decimal
int htod(std::string h) {
    int x(0);
    std::stringstream ss;
    ss << std::hex << h;
    ss >> x;
    return x;
}

// Converts binary to decimal
ulong32_t btod(std::string str) {
    std::bitset<32> bits(str);
    return bits.to_ulong();

}

// Converts hex to binary
std::string htob(std::string hexStr) {
    std::string bin;

    for (auto& i : hexStr) {
        bin += hexMap.at(std::to_string(i));
    }
    return bin;
}

// Converts oct to binary
std::string otob(std::string octStr) {
    std::string bin;
    for (auto& i : octStr) {
        bin += octMap.at(std::to_string(i));
    }
    return bin;
}

// Converts Bin to Hex
std::string btoh(std::string binStr) {
    std::string hex;

    while (binStr.size() % 4 != 0) {
        binStr = "0" + binStr;
    }

    for (int i(0); i != binStr.size(); i += 4) {
        hex += binMap.at(binStr.substr(i, 4));
    }
    return hex;
}
*/