#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <regex>
#include <algorithm>


bool comp(const std::pair< std::string, int > &a, const std::pair< std::string, int > &b);

int main(int argc, char *argv[]) {

    double counter = 0;
    std::ifstream file_in;
    std::ofstream file_out;
    std::map<std::string, int> word_count;
    std::string line;
    std::string file_name(argv[1]);
    std::string file_name_out(argv[2]);

    std::cout << file_name << std::endl;

    file_in.open(file_name, std::ifstream::in);
    file_out.open(file_name_out, std::ofstream::out);

    if (file_in.is_open()){
        while(file_in >> line)
        {
            if ( word_count.find(line) != word_count.end() )
            {
                word_count[line]++;
            } else{
                word_count[line] = 1;
            }
        }
    }

    std::vector< std::pair< std::string, int > > vec{word_count.begin(), word_count.end()};
    std::sort(vec.begin(), vec.end(), comp);

    for (auto it = vec.end()-1; it != vec.begin()-1; --it){
        counter += it->second;
    }

    for (auto it = vec.end()-1; it != vec.begin()-1; --it){
        file_out << it->first << "," << it->second << "," << (it->second)/counter <<"\n";
    }

    file_in.close();
    file_out.close();
    return 0;
}

bool comp(const std::pair< std::string, int > &a, const std::pair< std::string, int > &b)
{
    return a.second < b.second;
}