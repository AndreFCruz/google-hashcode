/*
 * Google HashCode 2017
 * Practice Problem - Pizza
*/


#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

using namespace std;

typedef unsigned int uint;



/* Finds all ordered pairs of numbers whose product is n
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

/* Finds all ordered pairs of numbers whose product is in range [min, max]
 */
vector<pair<uint, uint>> find_all_products(uint min, uint max) {
    vector<pair<uint,uint>> result;
    
    for (uint i = max; i >= min; --i) {
        auto tmp = find_products(i);
        
        // Thin and Stretched slices last ?
        /* This is actually worse
        sort(tmp.begin(), tmp.end(), [](pair<int,int> p1, pair<int,int> p2) {
            return pow(p1.first - p1.second, 2) < pow(p2.first - p2.second, 2);
        } );
        */
        
        //result.insert(result.begin(), tmp.begin(), tmp.end());
        result.insert(end(result), begin(tmp), end(tmp));
    }
    
    return result;
}

class Slice {
public:
    pair<uint, uint> rows;
    pair<uint, uint> cols;
    
    Slice(uint row_i, uint row_f, uint col_i, uint col_f) : rows(make_pair(row_i, row_f)), cols(make_pair(col_i, col_f)) {}
    
    friend ostream & operator<<(ostream & out, const Slice & s) {
        out << s.rows.first << " " << s.cols.first << " " << s.rows.second << " " << s.cols.second;
        return out;
    }
};

class Pizza {
private:
    uint rows;
    uint cols;
    uint min_ings;	// minimum number of ingredients per slice
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
        
        uint tomatoes = 0, mushrooms = 0;
        
        for (uint row = ri; row <= rf; ++row) {
            for (uint col = ci; col <= cf; ++col) {
                if (visited[row][col]) {
                    return false;
                }
                if (contents[row][col] == 'T')
                    tomatoes++;
                else if (contents[row][col] == 'M')
                    mushrooms++;
            }
        }
        
        return (tomatoes >= min_ings && mushrooms >= min_ings);
    }
    
public:
    Pizza(ifstream & ifs) {
        ifs >> rows >> cols >> min_ings >> max_cells;
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
     */
    void divide() {
        slices.clear();
        auto sizes = find_all_products(min_ings * 2, max_cells);
        bool finished = false, inner_finished = false;
        
        uint row = 0, col = 0;
        while (! finished) {
            
            while (! inner_finished) {
                if (col < cols && row < rows && visited[row][col] == false) {
                    inner_finished = true;
                    break;
                }
                if (col < cols)
                    col++;
                else if (row < rows) {
                    row++;
                    col = 0;
                }
                else {
                    finished = inner_finished = true;
                    break;
                }
            }
            
            for (auto choice : sizes) {
                if (! isValid(row, row + choice.first - 1, col, col + choice.second - 1))
                    continue;
                
                slices.emplace_back(row, row + choice.first - 1, col, col + choice.second - 1);
                markVisited(row, row + choice.first - 1, col, col + choice.second - 1);
                break;
            }
            col++;
            
            inner_finished = false;
        }
    }
    
    void printMetrics() const {
        uint score = 0;
        for (uint r = 0; r < rows; ++r) {
            for (uint c = 0; c < cols; ++c) {
                if (visited[r][c])
                    score++;
            }
        }
        uint sliced = 0;
        for (const auto & s : slices) {
            sliced += (s.rows.second - s.rows.first + 1) * (s.cols.second - s.cols.first + 1);
        }
        
        cout << "**SCORE** " << score << endl;
        cout << "**SLICED** " << score << endl;
        cout << "**TOTAL - SLICED** " << rows * cols - sliced << endl;
        cout << "**TOTAL** " << rows * cols << endl << endl;
        cout << "Rows: " << rows << ". Cols: " << cols << ". MinIngs: " << min_ings << ". Max cells: " << max_cells << endl;
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
    
    ofstream ofs (output);
    ofs << pizza;
    
    pizza.printMetrics();
    
    return 0;
}
