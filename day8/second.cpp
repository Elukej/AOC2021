#include <fstream>
#include "../include/luka_string.h"

const std::string PANEL_SYMBOLS = "abcdefg";

struct LED {
    char top;
    char top_left;
    char top_right;
    char middle;
    char bottom_right;
    char bottom_left;
    char bottom;
    LED(){}
};

typedef std::string Digit; 

template <typename T>
bool elem(typename T::value_type num, T l)
{
    for (auto it = l.begin(); it != l.end(); it++) 
        if (*it == num) return true;
    return false;
}


std::string diff(const std::string &s1, const std::string &s2) // works for small english alphabet letters
{
    std::string temp;
    int arr[25] = {0};
    for (auto ch : s2) arr[ch - 97]++; //fill the map
    for (auto ch : s1) if (arr[ch - 97] == 0) temp.push_back(ch);
    return temp;
}

bool isPerm(const std::string &str1, const std::string &str2 )
{
    if (str1.length() != str2.length()) return false;
    int s1 = 0, s2 = 0, p1 = 1, p2 =1;
    for (int i = 0; i < str1.length(); i++) {
        s1 += str1[i]; p1 *= str1[i];
        s2 += str2[i]; p2 *= str2[i];
    }
    return s1 == s2 && p1 == p2;
}

std::vector<Digit> decipher_digits(std::vector<std::string> &vec)
{
    LED led;
    std::vector<Digit> digits(10);
    for (auto it = vec.begin(); it != vec.end(); it++) 
        if (elem<std::initializer_list<int>>((*it).length(), {2,3,4,7})) {
            if ((*it).length() == 2) digits[1] = *it;
            if ((*it).length() == 3) digits[7] = *it;
            if ((*it).length() == 4) digits[4] = *it;
            if ((*it).length() == 7) digits[8] = *it;
            vec.erase(it); it--;
        }

    led.top = diff(digits[7], digits[1]).front(); // top signal determined from diff of 1 and 7
    std::string diff4from1 = diff(digits[4], digits[1]); // signals in 4 but not in 1
    std::string diff8from069;
    for (auto str : vec) 
        if (str.length() == 6) diff8from069 += diff(digits[8], str); // diff of 0,6 and 9 from 8.
    led.top_left = diff(diff4from1, diff8from069).front(); // top left signal
    led.middle = diff(diff4from1, std::string(1,led.top_left)).front(); // middle comes as leftover of diff4from1
    std::string almost9 = digits[4] + led.top;
   
    for (auto it = vec.begin(); it != vec.end(); it++) 
        if ((*it).length() == 6) {
            if (diff(digits[8], (*it)).front() == led.middle) digits[0] = (*it); 
            if (diff((*it), almost9).length() == 1) {
                 digits[9] = (*it); 
                 led.bottom = diff((*it), almost9).front();
            }
            if (!diff(digits[1], (*it)).empty()) {
                digits[6] = (*it);
                led.top_right = diff(digits[1], (*it)).front();
            }
            vec.erase(it); it--;
        }
        
    led.bottom_right = diff(digits[1], std::string(1, led.top_right)).front();
    led.bottom_left = diff(digits[8], digits[9]).front();
    digits[2] = std::string("") + led.top + led.top_right
    + led.middle + led.bottom_left + led.bottom;
    digits[3] = digits[7] + led.middle + led.bottom;
    digits[5] = std::string("") + led.top + led.top_left
    + led.middle + led.bottom_right + led.bottom; 
    
    return digits;
}


int which_digit(const std::string str,const std::vector<Digit> &digits) 
{    
    for (int i = 0; i < digits.size(); i++)
        if (isPerm(str, digits[i])) return i;
    return -1;
}

int main(int argc, char** argv)
{
    std::cout << "LED digit recognition:" << std::endl;
    std::ifstream input(argv[1]);
    if(!input) {
        std::cout << "Error opening file!" << std::endl;
        return EXIT_FAILURE;
    }

    std::string line;
    int total_sum = 0;
    while (std::getline(input, line)) {
        std::vector<std::string> 
        vec_left = luka_string::split(luka_string::split(line, " | ").front(), " "),
        vec_right = luka_string::split(luka_string::split(line, " | ").back(), " ");
        std::vector<Digit> digits = decipher_digits(vec_left);
        int temp = 0;
        for (auto str : vec_right) 
            temp = temp * 10 + which_digit(str, digits);
        total_sum += temp;
    }
    std::cout << "Sum of digits is: " << total_sum << std::endl;
}
