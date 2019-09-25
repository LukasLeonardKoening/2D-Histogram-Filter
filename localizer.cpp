/**
	localizer.cpp

	Purpose: implements a 2-dimensional histogram filter
	for a robot living on a colored cyclical grid by 
	correctly implementing the "initialize_beliefs", 
	"sense", and "move" functions.

*/

#include "localizer.h"
#include "helpers.cpp"
#include <stdlib.h>
#include "debugging_helpers.cpp"

using namespace std;

typedef vector< vector <float> > float_matrix;
typedef vector< vector <char> > char_matrix;

/**
    Initializes a grid of beliefs to a uniform distribution. 

    @param grid - a two dimensional grid map (vector of vectors 
    	   of chars) representing the robot's world. For example:
    	   
    	   g g g
    	   g r g
    	   g g g
		   
		   would be a 3x3 world where every cell is green except 
		   for the center, which is red.

    @return - a normalized two dimensional grid of floats. For 
           a 2x2 grid, for example, this would be:

           0.25 0.25
           0.25 0.25
*/
float_matrix initialize_beliefs(char_matrix grid) {
	float_matrix newGrid = zeros(grid.size(), grid[0].size());
    float prob = 1. / (grid.size() * grid[0].size());
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[0].size(); j++) {
            newGrid[i][j] = prob;
        }
    }
	return newGrid;
}

/**
    Implements robot motion by updating beliefs based on the 
    intended dx and dy of the robot. 

    For example, if a localized robot with the following beliefs

    0.00  0.00  0.00
    0.00  1.00  0.00
    0.00  0.00  0.00 

    and dx and dy are both 1 and blurring is 0 (noiseless motion),
    than after calling this function the returned beliefs would be

    0.00  0.00  0.00
    0.00  0.00  0.00
    0.00  0.00  1.00 

    @param dy - the intended change in y position of the robot

    @param dx - the intended change in x position of the robot

    @param beliefs - a two dimensional grid of floats representing
         the robot's beliefs for each cell before sensing. For 
         example, a robot which has almost certainly localized 
         itself in a 2D world might have the following beliefs:

         0.01 0.98
         0.00 0.01

    @param blurring - A number representing how noisy robot motion
           is. If blurring = 0.0 then motion is noiseless.

    @return - a normalized two dimensional grid of floats 
         representing the updated beliefs for the robot. 
*/
float_matrix move(int dy, int dx, vector < vector <float> > beliefs, float blurring) {
    int rows = beliefs.size();
    int cols = beliefs[0].size();
    float_matrix newGrid = zeros(rows, cols);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            
            /* My own code - can be optimized
             
                // Calculate the row and column modifier (overflow/loop)
                int row_modifier = 0;
                int col_modifier = 0;
                if (i + dy >= rows) {
                    row_modifier = -rows;
                } else if (i + dy < 0) {
                    row_modifier = rows;
                }
                if (j + dx >= cols) {
                    col_modifier = -cols;
                } else if (j + dx < 0) {
                    col_modifier = cols;
                }
                
                newGrid[i+dy + row_modifier][j+dx + col_modifier] = beliefs[i][j];
            */
            
            // optimized solution:
            int new_i = ((i + dy) % cols + cols) % cols;
            int new_j = ((j + dx) % rows + rows) % rows;
            newGrid[new_i][new_j] = beliefs[i][j];
        }
    }
    return blur(newGrid, blurring);
}


/**
    Implements robot sensing by updating beliefs based on the 
    color of a sensor measurement 

	@param color - the color the robot has sensed at its location

	@param grid - the current map of the world, stored as a grid
		   (vector of vectors of chars) where each char represents a 
		   color. For example:

		   g g g
    	   g r g
    	   g g g

   	@param beliefs - a two dimensional grid of floats representing
   		   the robot's beliefs for each cell before sensing. For 
   		   example, a robot which has almost certainly localized 
   		   itself in a 2D world might have the following beliefs:

   		   0.01 0.98
   		   0.00 0.01

    @param p_hit - the RELATIVE probability that any "sense" is 
    	   correct. The ratio of p_hit / p_miss indicates how many
    	   times MORE likely it is to have a correct "sense" than
    	   an incorrect one.

   	@param p_miss - the RELATIVE probability that any "sense" is 
    	   incorrect. The ratio of p_hit / p_miss indicates how many
    	   times MORE likely it is to have a correct "sense" than
    	   an incorrect one.

    @return - a normalized two dimensional grid of floats 
    	   representing the updated beliefs for the robot. 
*/
float_matrix sense(char color, char_matrix grid, float_matrix beliefs, float p_hit, float p_miss) {
    int rows = beliefs.size();
    int cols = beliefs[0].size();
    float_matrix newGrid = zeros(rows, cols);
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid[i][j] == color) {
                newGrid[i][j] = beliefs[i][j] * p_hit;
            } else {
                newGrid[i][j] = beliefs[i][j] * p_miss;
            }
        }
    }
	return normalize(newGrid);
}
