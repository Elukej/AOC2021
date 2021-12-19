#include <fstream>
#include <iostream>
#include <string>

std::string catNums(const std::string &sum_str, const std::string &str1)
{
    return "[" + sum_str + "," + str1 + "]";
}

void explode(std::string &str, int &pos, int &num_brackets, 
             bool left_split = false)
{   
    int left, right, left_pos = 0, right_pos = 0; 
    sscanf(str.substr(pos,7).c_str(), "[%d,%d]", &left, &right); 
     //c_str returns const char pointer for c functions
    int digit_len = std::to_string(left).length() 
                    + std::to_string(right).length();
    str.replace(pos, 3 + digit_len, "0"); // explosion pair always has two single digits numbers
    num_brackets--;

    int brackets_left = num_brackets, brackets_right = num_brackets;
    for (int i = pos - 1; i > 0; i--) {
        if (isdigit(str[i])) {
            if (isdigit(str[i-1])) {
                str.replace(i-1, 2, std::to_string(left + 
                                            std::stoi(str.substr(i-1, 2))));
                left_pos = i - 1;
                break;
            }
            else {
                if (left + str[i] - 48 > 9) pos++;
                str.replace(i, 1, std::to_string(left + str[i] - 48));
                left_pos = i;
                break;
            }   
        }
        if (str[i] == '[') brackets_left--;
        if (str[i] == ']') brackets_left++;
    }

    for (int i = pos + 1; i < str.length(); i++) {
        if (isdigit(str[i])) {
            int len = 1;
            if (isdigit(str[i+1])) len++;
            str.replace(i, len, std::to_string(right + 
                                            std::stoi(str.substr(i,2))));
            right_pos = i;
            i = str.length();
        }
        if (str[i] == '[') brackets_right++;
        if (str[i] == ']') brackets_right--;
    }

    if (left_pos > 0 && left_split)
        if (std::stoi(str.substr(left_pos,2)) > 9 ) {
            pos = left_pos - 1;    
            num_brackets = brackets_left;
            return;
        }
    pos = right_pos - 2; 
    brackets_right = (brackets_right == 0)? num_brackets : brackets_right;
    num_brackets = (str[pos+1] == '[')? brackets_right - 1 : brackets_right;
}

void split (std::string &str, int &pos) {
    int temp = std::stoi(str.substr(pos,2));
    int left, right;
    if (temp & 1) {
        left = temp / 2; right = temp / 2 + 1;
    }
    else left = (right = temp / 2);
    std::string rep = "[" + std::to_string(left) + "," 
                        + std::to_string(right) + "]";
    str.replace(pos, 2, rep);
    pos--;
}

void reduceNum(std::string &str)
{
    int pos = 0, num_brackets = 0;
    while (pos < str.length()) {
        if (str[pos] == '[') num_brackets++;
        if (str[pos] == ']') num_brackets--;
        if (num_brackets == 5) explode(str, pos, num_brackets);
        pos++;
    }
    pos = (num_brackets = 0);
    while (pos < str.length()) {
        if (str[pos] == '[') num_brackets++;
        if (str[pos] == ']') num_brackets--;
        if (num_brackets == 5) explode(str, pos, num_brackets, true);
        if (isdigit(str[pos]))
            if (std::stoi(str.substr(pos,2)) > 9)
                split(str, pos);
        pos++;
    }
}

int magnitudeCalc(std::string &str, int pos) //first pos is 0
{   // function assumes its always called on str[pos] = '['
    int mag = 0, num_brackets = 0;
    if (isdigit(str[pos+1])) {
        mag += 3 * (str[pos+1] - 48);
        pos += 2; //skip digit, arrive to comma 
    }
    else {
        mag += 3 * magnitudeCalc(str, pos+1);
        pos++; // move position so it doesnt calculate bracket we are inside already
    }
    do { //shift till first num on same level, or ',' before '['
        num_brackets = (str[pos] == '[' )? num_brackets + 1 : 
                        (str[pos] == ']')? num_brackets - 1 : num_brackets;
        pos++;
    } while (num_brackets > 0);  
    if (isdigit(str[pos])) 
        mag += 2 * (str[pos] - 48);
    else {
        if (str[pos] == '[') pos--; //return pos to showing ','
        mag += 2 * magnitudeCalc(str, pos+1); //pos+1 skips comma to next [
    }
    return mag;
}

int main(int argc, char** argv)
{
    std::ifstream input(argv[1]);
    std::cout << "SnailFish Homework!" << std::endl;
    if (!input) {
        std::cout << "Error opening the file!" << std::endl;
        return EXIT_FAILURE;
    }

    std::string sum_str, line;
    std::getline(input, sum_str);

    while (std::getline(input, line)) {
        sum_str = catNums(sum_str, line);
        reduceNum(sum_str);
    }
    std::cout << sum_str << std::endl;
    std::cout << magnitudeCalc(sum_str, 0) << std::endl; 
}
 