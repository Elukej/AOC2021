#include "luka_string.h"

 
std::string& luka_string::ltrim(std::string &s)
{
    size_t start = s.find_first_not_of(WHITESPACE);
    return s.erase(0, start);
}
 
std::string& luka_string::rtrim(std::string &s)
{
    size_t end = s.find_last_not_of(WHITESPACE);
    return s.erase(end + 1);
}
 
std::string& luka_string::trim(std::string &s) {
   return rtrim(ltrim(s));
}

std::vector<std::string> luka_string::split(std::string str, std::string delim)
{
    std::vector<std::string> vec;  size_t pos; 
    while (( pos = str.find(delim)) != std::string::npos) {
        if (pos) vec.push_back(str.substr(0,pos));
        str.erase(0, pos + delim.length());
        ltrim(str);
    }
    vec.push_back(str);

    return vec;
}

std::vector<int> luka_string::num_parser(std::string &str)
{
    std::vector<int> vec;
    auto pos = str.begin(); bool num_ind = false;
    for (auto it1 = str.begin(), it2 = str.begin(); it1 != str.end(); it1++) {
        if (isdigit(*it1)) {
            if (!num_ind) { 
                it2 = it1; 
                num_ind = true;
            }
        }
        else 
            if (num_ind) {
                vec.push_back(std::stoi(str.substr(std::distance(str.begin(),it2),
                    std::distance(it2, it1) )));
                /*
                str.erase(str.begin(), it1 + 1); //erase till next char that might be num
                it1 = str.begin() - 1; //nakon brisanja, iterator zadrzava istu referentnu poziciju u odnosu na pocetak\
                te ga treba resetovati na pocetak */
                num_ind = false;
            }
            else it2++;
        if (it1 + 1 == str.end() && num_ind)
                vec.push_back(std::stoi(str.substr(std::distance(str.begin(),it2),
                    std::distance(it2, it1 + 1) )));
    }
    return vec;
}
