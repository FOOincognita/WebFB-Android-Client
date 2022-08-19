#pragma once

//*	Definitions
#define DEFAULT_IP	  "172.31.1.1"
#define DEFAULT_PORT  "10001"
#define MAXPKT		  16384
#define MAX_WPS    	  8192
#define TIMEOUT		  5

// Typedefs
typedef short   error_t;
typedef double  latitude_t;

// Global Variables
std::uint16_t buf[MAXPKT];    // Buffer for socket data transfer

// Lookup Tables for numerical systems conversions
const std::unordered_map<char, std::string> hexMap = // Bruteforce: hexadecimal to binary
{ 
    {'0',"0000"},
    {'1',"0001"},
    {'2',"0010"},
    {'3',"0011"},
    {'4',"0100"},
    {'5',"0101"},
    {'6',"0110"},
    {'7',"0111"},
    {'8',"1000"},
    {'9',"1001"},
    {'a',"1010"},
    {'A',"1010"},
    {'b',"1011"},
    {'B',"1011"},
    {'c',"1100"},
    {'C',"1100"},
    {'d',"1101"},
    {'D',"1101"},
    {'e',"1110"},
    {'E',"1110"},
    {'f',"1111"},
    {'F',"1111"}
};

// Hex to Bin 
const std::unordered_map<std::string, std::string> binMap = 
{
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

// Oct to Bin
const std::unordered_map<std::string, std::string> octMap = 
{
    {"0","000"},
    {"1","001"},
    {"2","010"},
    {"3","011"},
    {"4","100"},
    {"5","101"},
    {"6","110"},
    {"7","111"}
};
