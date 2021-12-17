#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <limits>

struct Packet {
    int version;
    int type_ID;
    int length_type_ID;
    int num_packets;
    int bit_length;
    long literal;
    int pack_len;
    std::vector<Packet> sub_packs;
    Packet(){}
    Packet(int v, int t, char l, int n=-1, int b = -1, 
           long lit = -1, int pl = 0):
            version(v), type_ID(t), length_type_ID(l),
            bit_length(b), num_packets(n), literal(lit), pack_len(pl) {}
    Packet& set(int v, int t, char l, int n=-1, int b = -1, 
                long lit = -1, int pl = 0)
    {
        version = v; type_ID = t; length_type_ID = l;
        bit_length = b; num_packets = n; literal = lit; pack_len = pl; 
        return *this;
    }
};

const char* hex_char_to_bin(char c)
{
    // TODO handle default / error
    switch(toupper(c))
    {
        case '0': return "0000";
        case '1': return "0001";
        case '2': return "0010";
        case '3': return "0011";
        case '4': return "0100";
        case '5': return "0101";
        case '6': return "0110";
        case '7': return "0111";
        case '8': return "1000";
        case '9': return "1001";
        case 'A': return "1010";
        case 'B': return "1011";
        case 'C': return "1100";
        case 'D': return "1101";
        case 'E': return "1110";
        case 'F': return "1111";
        default: return "Error!";
    }
}

std::string hex_string_to_bin (std::string line)
{
    std::string temp("");
    for (auto ch : line)
        temp += hex_char_to_bin(ch);
    return temp;
}

Packet packetParser(std::string &bits, int &pos, int &ver_sum)
{   
    Packet packet;
    int pack_len = 6; // min length
    int version = std::stoi(bits.substr(pos, 3), nullptr, 2);
    pos += 3; ver_sum += version;
    int type_ID = std::stoi(bits.substr(pos, 3), nullptr, 2);
    pos += 3;
    int length_type_ID = (type_ID == 4)? -1 : bits[pos++] - 48;
    int num_packets, bit_length;
    
    long literal = -1;
    if (length_type_ID == -1) { //is a literal
        bit_length = (num_packets = -1);
        std::string temp;
        bool cond = false;
        while (!cond) {
            if (bits[pos] == '0') cond = true;
            temp += bits.substr(pos +1, 4);
            pos += 5; pack_len += 5;
        }
        literal = std::stol(temp, nullptr, 2);
    }
    
    if (length_type_ID == 1) {
        bit_length = -1;
        num_packets = std::stoi(bits.substr(pos, 11), nullptr, 2);   
        pos += 11; pack_len += 12; //because of length_type id, which wasnt accounted in pack_len
        int temp = num_packets;
        while (temp--) 
            packet.sub_packs.push_back(packetParser(bits, pos, ver_sum));
    }   

    if (length_type_ID == 0) {
        num_packets = -1;
        bit_length = std::stoi(bits.substr(pos, 15), nullptr, 2);
        pos += 15; pack_len += 16;//because of length_type id, which wasnt accounted in pack_len
        int temp = bit_length, old_pos;
        do {
            old_pos = pos;
            packet.sub_packs.push_back(packetParser(bits, pos, ver_sum));
        } while (temp -= (pos - old_pos)); // length of subpack is reflected by old and new positions in string
    }
    packet.set(version, type_ID, length_type_ID, num_packets, bit_length,
               literal, pack_len);
    return packet;
}

long packetValue(Packet packet)
{   
    long val;
    switch(packet.type_ID) {
        case 0: {
            val = 0;
            for (auto &mem : packet.sub_packs)
                val += packetValue(mem);
            return val;
        }
        case 1: {
            val = 1;
            for (auto &mem : packet.sub_packs)
                val *= packetValue(mem);
            return val;
        }
        case 2: {
            val = std::numeric_limits<long>::max();
            for (auto &mem : packet.sub_packs)
                val = (val > packetValue(mem))? packetValue(mem) : val;
            return val; 
        }
        case 3: {
            val = 0;
            for (auto &mem : packet.sub_packs)
                val = (val < packetValue(mem))? packetValue(mem) : val;
            return val;
        } 
        case 4: return packet.literal;
        case 5: return val = (packetValue(packet.sub_packs.front()) > 
                              packetValue(packet.sub_packs.back()))? 1 : 0;
        case 6: return val = (packetValue(packet.sub_packs.front()) < 
                              packetValue(packet.sub_packs.back()))? 1 : 0;
        case 7: return val = (packetValue(packet.sub_packs.front()) == 
                              packetValue(packet.sub_packs.back()))? 1 : 0;
        default: return 0;
    }
}

int main(int argc, char** argv)
{
    std::ifstream input(argv[1]);
    std::cout << "Decoder!" << std::endl;
    if (!input) {
        std::cout << "Error opening the file!" << std::endl;
        return EXIT_FAILURE;
    }

    std::string line;
    std::getline(input, line);
    std::string bits_string = hex_string_to_bin(line);
    bits_string.erase(bits_string.find_last_not_of("0") + 1); // remove trailing zeros
    int ver_sum = 0, pos = 0;
    Packet bit_packet = packetParser(bits_string, pos, ver_sum);
    std::cout << "Main packet value: " << packetValue(bit_packet) << std::endl;

} //Nije 1264857430921