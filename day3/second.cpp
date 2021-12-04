#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <forward_list>

using Data = std::forward_list<std::string>;

struct Data_struct {
    Data lista;
    int duzina;
    Data_struct(){}
    Data_struct(Data lista, int duzina) : lista(lista), duzina(duzina) {}
};

Data parse(std::ifstream& input, int& data_len)
{
    Data temp;
    std::string line;
    for ( ; std::getline(input, line) ; ) {
        temp.push_front(line);
        data_len++;
    }
    return temp;
} 

// vraca najcescu cifru na poziciji index iz liste
// ako su jednake pojave i 0 i 1, vraca =
char mcom_count(Data_struct& wrapper, const int& index)
{
    int temp = 0;
    for (Data::iterator it = wrapper.lista.begin();\
    it != wrapper.lista.end(); it++) {
        temp += ((*it)[index] == '1');
    }
    if(wrapper.duzina > 2*temp) return '0';
    if(wrapper.duzina < 2*temp) return '1';
    return '=';
}


void rating(char gas_type, const int& index,
 Data_struct& old_container, Data_struct& new_container) 
{
    char c = mcom_count(old_container, index);
    if (gas_type == 'C' && c == '1') c = '0';
    else if (gas_type == 'C' && c == '0') c = '1';
    if (c == '=') {
        if (gas_type == 'O') c = '1'; // gas types are either O or C
        if (gas_type == 'C') c = '0';
    }
    // if list goes to another, then move
    if (old_container.lista != new_container.lista)
        for ( Data::iterator it1 = old_container.lista.begin(),
            it2 = old_container.lista.before_begin();
            it1 != old_container.lista.end(); ) 
            if ( (*it1)[index] == c ) {
                new_container.lista.push_front(std::move(*it1));
                new_container.duzina++;
                it1++;
                old_container.lista.erase_after(it2);
                old_container.duzina--;
            }
            else {
                it1++;
                it2++;
            }
    // if list just loses elements
    else 
        for ( Data::iterator it1 = old_container.lista.begin(),
            it2 = old_container.lista.before_begin();
            it1 != old_container.lista.end(); ) 
            if ( (*it1)[index] != c ) {
                it1++;
                old_container.lista.erase_after(it2);
                old_container.duzina--;
            }
            else {
                it1++;
                it2++;
            }
}

int main()
{
    std::cout << "Program trazi proivod brojeva dobijenih\
 od najcescih cifara i najredjih cifara iz liste" << std::endl;
    std::ifstream input("input.txt");
    int total = 0;
    Data_struct container(parse(input, total), 0), lcom_container;
    container.duzina = total;
    lcom_container.duzina = 0;

    rating('C', 0, container, lcom_container);
    for (int i = 1; i < container.lista.front().length(); i++) {
        if (container.duzina != 1) rating('O', i, container, container);
        if (lcom_container.duzina != 1) rating('C', i, lcom_container, lcom_container);
        if (container.duzina == 1 && lcom_container.duzina == 1) break;
    }
    std::cout << container.lista.front() << "  " << lcom_container.lista.front() << std::endl;
    int oxygen = std::stoi(container.lista.front(), 0 , 2);
    int co2 = std::stoi(lcom_container.lista.front(), 0 , 2);
    std::cout << oxygen * co2 << std::endl;            
}
