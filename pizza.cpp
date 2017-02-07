/*
 * Google HashCode 2017
 * Practice Problem - Pizza
 *
 * Author: AndreFCruz
 */


#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

typedef unsigned int uint;



/* Fins all pairs of numbers whose product is n
 */
vector<pair<uint, uint>> find_products(uint n) {
    vector<pair<uint, uint>> result;
    
    for (uint k = 1; k <= n; ++k) {
        if (k * (n / k) == n) { // integer division resulted in correct pair?
            result.push_back(make_pair(k, n / k));
        }
    }
    
    return result;
}

vector<pair<uint, uint>> find_all_products(uint n) {
    vector<pair<uint,uint>> result;
    
    for (uint i = n; i > 1; --i) {
        auto tmp = find_products(i);
        result.insert(end(result), begin(tmp), end(tmp));
    }
    
    return result;
}

class Slice {
private:
    pair<uint, uint> rows;
    pair<uint, uint> cols;
    
public:
    Slice(uint row_i, uint row_f, uint col_i, uint col_f) : rows(make_pair(row_i, row_f)), cols(make_pair(col_i, col_f)) {
        cerr << "New Slice: " << row_i << " " << row_f << " " << col_i << " " << col_f << endl;
    }
    
    friend ostream & operator<<(ostream & out, const Slice & s) {
        out << s.rows.first << " " << s.rows.second << " " << s.cols.first << " " << s.cols.second;
        return out;
    }
};

class Pizza {
private:
    uint rows;
    uint cols;
    uint min_ing;	// minimum number of ingredients per slice
    uint max_cells;	// maximum number of cells per slice
    vector<string> contents;	// Matrix of ingredients
    vector<Slice> slices;
    vector< vector<bool> > visited;
    
    void markVisited(uint ri, uint rf, uint ci, uint cf) {
        for (uint row = ri; row <= rf; ++row) {
            for (uint col = ci; col <= cf; ++col) {
                visited[row][col] = true;
            }
        }
    }
    
    bool isValid(uint ri, uint rf, uint ci, uint cf) const {
        if (ri > rf || ci > cf || rf >= rows || cf >= cols)
            return false;
        
        bool tomatoes = false, mushrooms = false;
        
        for (uint row = ri; row <= rf; ++row) {
            for (uint col = ci; col <= cf; ++col) {
                if (visited[row][col]) {
                    return false;
                }
                if (contents[row][col] == 'T')
                    tomatoes = true;
                else if (contents[row][col] == 'M')
                    mushrooms = true;
            }
        }
        
        return (tomatoes && mushrooms);
    }
    
public:
    Pizza(ifstream & ifs) {
        ifs >> rows >> cols >> min_ing >> max_cells;
        ifs.ignore(3, '\n');
        
        string line;
        for (uint i = 0; i < rows; ++i) {
            getline(ifs, line);
            contents.push_back(line);
        }
        
        visited = vector<vector<bool>>(rows, vector<bool>(cols, false));
    }
    
    friend ostream & operator<<(ostream & out, const Pizza & p) {
        out << p.slices.size() << endl;
        
        for (const Slice & s : p.slices)
            out << s << endl;
        
        return out;
    }
    
    /* Divides the pizza in slices so as to minimize unsliced area
     * Naive approach -- to be improved.
     *
     * @return the number of cells left unsliced
     */
    void divide() {
        slices.clear();
        auto sizes = find_all_products(max_cells);
        bool finished = false;
        
        uint row = 0, col = 0;
        while (! finished) {
            while (visited[row][col] == true) {
                if (col < cols) {
                    col++;
                } else if (row < rows) {
                    row++;
                    col = 0;
                } else {
                    finished = true;
                    break;
                }
            }
            
            for (auto choice : sizes) {
                if (! isValid(row, row + choice.first - 1, col, col + choice.second - 1))
                    continue;
                
                slices.emplace_back(row, row + choice.first - 1, col, col + choice.second - 1);
                markVisited(row, row + choice.first - 1, col, col + choice.second - 1);
                
                /* for optimization purposes
                 row += choice.first - 1;
                 col += choice.second - 1;
                 */
            }
            
            if (! (row < rows && col < cols))
                finished = true;
        }
        
        uint score = 0;
        for (uint r = 0; r < rows; ++r) {
            for (uint c = 0; c < cols; ++c) {
                if (! visited[r][c])
                    score++;
            }
        }
        cerr << "**SCORE** " << score << endl;
    }
};



int main ( int argc, char * argv[] ) {
    if (argc != 1 + 2) {
        cout << "Incorrect number of arguments. Usage: ./pizza input_file_path output_file" << endl;
        return 0;
    }
    
    string input (argv[1]);
    string output (argv[2]);
    
    ifstream ifs (input);
    if (! ifs.is_open()) {
        cout << "Input file not found! Was \"" << input << "\"" << endl;
        return 0;
    }
    
    Pizza pizza(ifs);
    
    pizza.divide();
    
    // TODO: change to file ostream
    cout << pizza;
    
    
    
    return 0;
}
