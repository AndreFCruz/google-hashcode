/*
 * Google HashCode 2017
 * Practice Problem - Pizza
 *
 * Author: AndreFCruz
 */


#include <iostream>
#include <vector>
#include <set>
#include <fstream>

using namespace std;

typedef unsigned int uint;

// Globals
bool visited[1000][1000];

/* Fins all pairs of numbers whose product is n
 */
set<pair<uint, uint>> find_products(uint n) {
    set<pair<uint, uint>> result;
    
    for (uint k = 1; k * k <= n; ++k) {
        if (k * (n / k) == n)   // integer division resulted in correct pair?
            result.insert(make_pair(k, n / k));
    }
    
    return result;
}

class Slice {
private:
    pair<uint, uint> rows;
    pair<uint, uint> cols;
    
public:
    Slice(uint row_i, uint row_f, uint col_i, uint col_f) : rows(make_pair(row_i, row_f)), cols(make_pair(col_i, col_f)) {
        cerr << "New Slice: " << row_i << row_f << col_i << col_f << endl;
    }
    
    void markVisited() const {
        for (uint r = rows.first; r <= rows.second; ++r) {
            for (uint c = cols.first; c <= cols.second; ++c) {
                visited[r][c] = true;
            }
        }
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
	vector<string> content;	// Matrix of ingredients
    vector<Slice> slices;
    
public:
	Pizza(ifstream & ifs) {
        ifs >> rows >> cols >> min_ing >> max_cells;
        ifs.ignore(3, '\n');
        
        string line;
        for (uint i = 0; i < rows; ++i) {
            getline(ifs, line);
            content.push_back(line);
        }
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
        auto sizes = find_products(max_cells);
        bool finished = false;
        
        // example choice -- to be tuned
        auto choice = *(sizes.begin());
        
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
                }
            }
            
            slices.emplace_back(row, row + choice.first, col, col + choice.second);
            
            slices.back().markVisited();
            
        }
        
        cerr << "Score (more is worse): ";
        uint score = 0;
        for (uint r = 0; r < rows; ++r) {
            for (uint c = 0; c < cols; ++c) {
                if (! visited[r][c])
                    score++;
            }
        }
        cerr << score << endl;
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
