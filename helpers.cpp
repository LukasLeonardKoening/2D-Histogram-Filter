/**
	helpers.cpp

	Purpose: helper functions which are useful when
	implementing a 2-dimensional histogram filter.
 
*/

#include <vector>
#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include "helpers.h"

using namespace std;

typedef vector< vector <float> > float_matrix;
typedef vector< vector <char> > char_matrix;

/**
    Normalizes a grid of numbers. 

    @param grid - a two dimensional grid (vector of vectors of floats)
		   where each entry represents the unnormalized probability 
		   associated with that grid cell.

    @return - a new normalized two dimensional grid where the sum of 
    	   all probabilities is equal to one.
*/
float_matrix normalize(float_matrix &grid) {
	float_matrix newGrid = zeros(grid.size(), grid[0].size());
    
    // Calculate normalizer
    float normalizer = 0;
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[0].size(); j++) {
            normalizer += grid[i][j];
        }
    }
    
    // Apply normalizer
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[0].size(); j++) {
            newGrid[i][j] = grid[i][j]/normalizer;
        }
    }
	return newGrid;
}

/**
    Blurs (and normalizes) a grid of probabilities by spreading 
    probability from each cell over a 3x3 "window" of cells. This 
    function assumes a cyclic world where probability "spills 
    over" from the right edge to the left and bottom to top. 

    EXAMPLE - After blurring (with blurring=0.12) a localized 
    distribution like this:

    0.00  0.00  0.00 
    0.00  1.00  0.00
    0.00  0.00  0.00 

    would look like this:
	
	0.01  0.02	0.01
	0.02  0.88	0.02
	0.01  0.02  0.01

    @param grid - a two dimensional grid (vector of vectors of floats)
		   where each entry represents the unnormalized probability 
		   associated with that grid cell.

	@param blurring - a floating point number between 0.0 and 1.0 
		   which represents how much probability from one cell 
		   "spills over" to it's neighbors. If it's 0.0, then no
		   blurring occurs. 

    @return - a new normalized two dimensional grid where probability 
    	   has been blurred.
*/
float_matrix blur(vector < vector < float> > &grid, float blurring) {
    int rows = grid.size();
    int cols = grid[0].size();

    float_matrix newGrid = zeros(rows, cols);
    
    float prob = 1.0-blurring;
    float adjacent_prob = blurring / 6.0;
    float corner_prob = blurring / 12.0;
    
    float_matrix window = {
        {corner_prob, adjacent_prob, corner_prob},
        {adjacent_prob, prob, adjacent_prob},
        {corner_prob, adjacent_prob, corner_prob}
    };
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            
            // optimized solution:
            float grid_val = grid[i][j];
            for (int dx = -1; dx < 2; dx++) {
                for (int dy = -1; dy < 2; dy++) {
                    float multiplier = window[dx+1][dy+1];
                    int new_i = ((i + dy) % cols + cols) % cols;
                    int new_j = ((j + dx) % rows + rows) % rows;
                    newGrid[new_i][new_j] += grid_val * multiplier;
                }
            }
            
            
            /* My own code - can be optimized!
            newGrid[i][j] += grid[i][j] * prob;
            
            // Left and right
            if (j - 1 < 0) {
                newGrid[i][j-1+cols] += grid[i][j] * adjacent_prob;
            } else {
                newGrid[i][j-1] += grid[i][j] * adjacent_prob;
            }
            if (j + 1 >= cols) {
                newGrid[i][j+1-cols] += grid[i][j] * adjacent_prob;
            } else {
                newGrid[i][j+1] += grid[i][j] * adjacent_prob;
            }

            // row above
            if (i-1 < 0) {
                newGrid[i-1+rows][j] += grid[i][j] * adjacent_prob;

                if (j - 1 < 0) {
                    newGrid[i-1+rows][j-1+cols] += grid[i][j] * corner_prob;
                } else {
                    newGrid[i-1+rows][j-1] += grid[i][j] * corner_prob;
                }
                if (j + 1 >= cols) {
                    newGrid[i-1+rows][j+1-cols] += grid[i][j] * corner_prob;
                } else {
                    newGrid[i-1+rows][j+1] += grid[i][j] * corner_prob;
                }
            } else {
                newGrid[i-1][j] += grid[i][j] * adjacent_prob;

                if (j - 1 < 0) {
                    newGrid[i-1][j-1+cols] += grid[i][j] * corner_prob;
                } else {
                    newGrid[i-1][j-1] += grid[i][j] * corner_prob;
                }
                if (j + 1 >= cols) {
                    newGrid[i-1][j+1-cols] += grid[i][j] * corner_prob;
                } else {
                    newGrid[i-1][j+1] += grid[i][j] * corner_prob;
                }
            }

            // row below
            if (i+1 >= rows) {
                newGrid[i+1-rows][j] += grid[i][j] * adjacent_prob;

                if (j - 1 < 0) {
                    newGrid[i+1-rows][j-1+cols] += grid[i][j] * corner_prob;
                } else {
                    newGrid[i+1-rows][j-1] += grid[i][j] * corner_prob;
                }
                if (j + 1 >= cols) {
                    newGrid[i+1-rows][j+1-cols] += grid[i][j] * corner_prob;
                } else {
                    newGrid[i+1-rows][j+1] += grid[i][j] * corner_prob;
                }
            } else {
                newGrid[i+1][j] += grid[i][j] * adjacent_prob;

                if (j - 1 < 0) {
                    newGrid[i+1][j-1+cols] += grid[i][j] * corner_prob;
                } else {
                    newGrid[i+1][j-1] += grid[i][j] * corner_prob;
                }
                if (j + 1 >= cols) {
                    newGrid[i+1][j+1-cols] += grid[i][j] * corner_prob;
                } else {
                    newGrid[i+1][j+1] += grid[i][j] * corner_prob;
                }
            }
            */
        }
    }
    
	return normalize(newGrid);
}

/**
    Helper function for reading in map data

    @param s - a string representing one line of map data.

    @return - A row of chars, each of which represents the
    color of a cell in a grid world.
*/
vector <char> read_line(string s) {
	vector <char> row;

	size_t pos = 0;
	string token;
	string delimiter = " ";
	char cell;

	while ((pos = s.find(delimiter)) != std::string::npos) {
		token = s.substr(0, pos);
		s.erase(0, pos + delimiter.length());

		cell = token.at(0);
		row.push_back(cell);
	}

	return row;
}

/**
    Helper function for reading in map data

    @param file_name - The filename where the map is stored.

    @return - A grid of chars representing a map.
*/
vector < vector <char> > read_map(string file_name) {
	ifstream infile(file_name);
	vector < vector <char> > map;
	if (infile.is_open()) {

		char color;
		vector <char> row;
		
		string line;

		while (std::getline(infile, line)) {
			row = read_line(line);
			map.push_back(row);
		}
	}
	return map;
}

/**
    Creates a grid of zeros

    For example:

    zeros(2, 3) would return

    0.0  0.0  0.0
    0.0  0.0  0.0

    @param height - the height of the desired grid

    @param width - the width of the desired grid.

    @return a grid of zeros (floats)
*/
vector < vector <float> > zeros(int height, int width) {
	int i, j;
	vector < vector <float> > newGrid;
	vector <float> newRow;

	for (i=0; i<height; i++) {
		newRow.clear();
		for (j=0; j<width; j++) {
			newRow.push_back(0.0);
		}
		newGrid.push_back(newRow);
	}
	return newGrid;
}
