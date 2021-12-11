#include <iostream>
#include <vector>
#include <string>

#ifndef LUKA_STRING_H
#define LUKA_STRING_H
namespace luka_string {
const std::string WHITESPACE = " \n\r\t\f\v";
 
std::string& ltrim(std::string &s);
 
std::string& rtrim(std::string &s);

std::string& trim(std::string &s);

std::vector<std::string> split(std::string str, std::string delim);

std::vector<int> num_parser(std::string &str);
}
#endif /* LUKA_STRING_H */
