#include <iostream>
#include <vector>
#include <map>

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
            rows = rows_tmp;
            columns = columns_tmp;
            field = field_tmp;
            new_field = field_tmp;
        }

        void play(int num_iterations){
            // make num_iterations of generations
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
                    std::cout << field[i][j];
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
};

int main() {
    int rows = 3;
    int columns = 3;
    int num_iterations = 0;
    std::vector<std::vector<char>> field(rows, std::vector<char>(columns));
    field = {{'*', '_', '*'},
             {'_', '*', '_'},
             {'*', '_', '*'}};
    GameOfLife new_game{rows, columns, field};
    new_game.Print();
//    new_game.next_generation();
    num_iterations = 10;
    new_game.play(num_iterations);
    new_game.Print();
    return 0;
}