#include <iostream>
#include <vector>
#include <map>
#include <fstream>

// Game of life
class GameOfLife{
    // * - alive
    // _ - died
    int rows;
    int columns;
    using field_type = std::vector<std::vector<char>>;
    field_type field;
    field_type new_field;

    public:
        GameOfLife(int rows_tmp, int columns_tmp, const field_type& field_tmp){
            // Constructor
            // field - field configuration at present time
            // new_field - field's new configuration
            rows = rows_tmp;
            columns = columns_tmp;
            field = field_tmp;
            new_field = field_tmp;
        }

        void play(int num_iterations){
            // make num_iterations
            for(int i = 0; i < num_iterations; i++)
                next_generation();
        }

        void next_generation(){
            // make new field for new generation
            for( int row_i = 0; row_i < rows; row_i++)
                for (int columns_i = 0; columns_i < columns; columns_i++){
                    new_field[row_i][columns_i] = bool_to_char(row_i, columns_i);
                }
            field = new_field;
        }

        char bool_to_char(int row_ind, int column_ind){
            bool is_alive = alive_or_not(row_ind, column_ind);
            if(is_alive)
                return '*';
            else
                return '_';
        }

        bool alive_or_not(int row_ind, int column_ind){
            int tmp_i{0};
            int tmp_j{0};
            std::map<char, int> alives_dies = {{'*', 0}, {'_', 0}};
            for(int i = row_ind-1; i <= row_ind+1; i++)
                for(int j = column_ind-1; j <= column_ind+1; j++){
                    tmp_i = i;
                    tmp_j = j;
                    if((i == row_ind) && (j == column_ind))
                        continue;

                    if( tmp_i < 0 )
                        continue;
//                        tmp_i = rows + tmp_i;

                    if( tmp_i >= rows )
                        continue;
//                        tmp_i = tmp_i - rows;

                    if( tmp_j < 0 )
                        continue;
//                        tmp_j = columns + tmp_j;

                    if( tmp_j >= columns )
                        continue;
//                        tmp_j = tmp_j - columns;
                    alives_dies[field[tmp_i][tmp_j]]++;
                }
            if(field[row_ind][column_ind] == '*'){
                return !((alives_dies['*'] < 2) || (alives_dies['*'] > 3));
            }
            else
                return (alives_dies['*'] == 3);
        }

        void Print(){
            for(int i = 0; i < rows; i++) {
                for (int j = 0; j < columns; j++) {
                    std::cout << field[i][j] <<"    ";
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
};

int main(int argc, char *argv[]) {
    int rows = 0;
    int columns = 0;
    int num_iterations = 0;
    int row_c = 0;
    int column_c = 0;
    char c;
    std::string tmp;
    std::ifstream file;
    std::string filename;

    filename = argv[1];
    num_iterations = std::stoi(argv[2]);

    file.open(filename, std::ifstream::in);

    std::getline(file, tmp);
    for(auto p = tmp.begin(); p != tmp.end(); p++){
        if(*p == ' ') {
            std::string first{tmp.begin(), p};
            rows = std::stoi(first);
            std::string second{tmp.begin(), p};
            columns = std::stoi(second);
        }
    }

    std::vector<std::vector<char>> field(rows, std::vector<char>(columns));

    while(file.get(c)) {
        if (c == '\n'){
            row_c++;
            column_c = 0;
            continue;
        }
        field[row_c][column_c++] = c;
    }

    GameOfLife new_game{rows, columns, field};

    if(num_iterations == 0){
        std::cout << "Please, press \'Enter \' botton to start interactive mode" << std::endl;
        while(std::cin.get()  == '\n' ){
            new_game.Print();
            new_game.next_generation();
        }
    }
    else{
        std::cout << "First configuration." << std::endl;
        new_game.Print();
        new_game.play(num_iterations);
        std::cout << "Configuration after " << num_iterations << " iterations." << std::endl;
        new_game.Print();
    }
    return 0;
}