#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <regex>
#include <algorithm>

std::vector<std::string> tokenizing(const std::string str);
std::vector<std::string> tokenizing_2(const std::string str);
bool pred(const std::pair< std::string, int > &a, const std::pair< std::string, int > &b);

int main() {
    std::vector<std::string> v2, commands;
    std::ifstream file_in;
    std::string line;
    std::string buffer;
    std::vector<std::pair<std::string, int>> word_new;
    std::map<std::string, int> word_count;
    std::string file_name  = "/Users/xloride/CLionProjects/test/book-war-and-peace.txt";

    file_in.open(file_name, std::ifstream::in);

    if (file_in.is_open()){
        while(getline(file_in, line))
        {
            v2.push_back(line);
//            std::cout << line << std::endl;
            std::transform(line.begin(), line.end(), line.begin(), tolower);
            commands = tokenizing_2(line);
            for(int i = 0; i < commands.size(); i++)
            {
//                if (commands[i].empty()) continue;
//                std::transform(commands[i].begin(), commands[i].end(), commands[i].begin(), tolower);
                if ( word_count.find(commands[i]) != word_count.end() )
                {
                    word_count[commands[i]]++;
                } else{
                        word_count[commands[i]] = 1;
                    }

            }
        }
    }

    std::vector< std::pair< std::string, int > > vec{word_count.begin(), word_count.end()};
    std::sort(vec.begin(), vec.end(), pred);
    int counter = 0;
//    std::cout << vec << std::endl;
    for (auto it = vec.end()-1; it != vec.begin()-1; --it){
        std::cout << it->first << " : " << it->second << std::endl;
        counter += it->second;
    }
    std::cout << counter << std::endl;
//    std::cout << word_count["friend"] << std::endl;
//    std::cout << word_count.find("hygf")->second << std::endl;
//    for (auto it = word_count.begin(); it != word_count.end(); ++it){
//        std::cout << it->first << " : " << it->second << std::endl;
//    }
    return 0;
}

std::vector<std::string> tokenizing(const std::string str) {
    std::vector<std::string> words;
    int prev = str.find_first_not_of(" ", 0);
    int now = str.find_first_of(" ", prev);
    while (std::string::npos != now || std::string::npos != prev) {
        words.push_back(str.substr(prev, now - prev));
        prev = str.find_first_not_of(" ", now);
        now = str.find_first_of(" ", prev);

    }
    return(words);
}

std::vector<std::string> tokenizing_2(const std::string str) {
//    std::regex regex{R"([\s,\.!?\'\"\\;:\-\/\(\)]+)"}; // split on space and comma
    std::regex regex{R"([a-z]+)"};
//    std::regex regex{R"([\s]+)"};
    std::sregex_token_iterator it{str.begin(), str.end(), regex, 0};
    std::vector<std::string> words{it, {}};
    return(words);

}

bool pred(const std::pair< std::string, int > &a, const std::pair< std::string, int > &b)
{
    return a.second < b.second;
}