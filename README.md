# 2D-Histogram-Filter (C++)
This 2D-Histogram-Filter was developed/coded during the "Into to Self-Driving Cars"-Nanodegree program and was my first approach towards localization.

The task was to translate given Python code into C++, but I first tried to implement my own solutions into the given basic framework. They worked, but could be optimized. 
I kept my original code in comments.
Following functions should be implemented: 
  * localizer.cpp: `sense`, `move`, `initialize_beliefs`
  * helpers.cpp:   `normalize`, `blur`

## How to run the code?

  1. Download the files;
  2. Run terminal command: `g++ -std=c++11 simulate.cpp` in the folder
  3. Run terminal command: `./a.out
  4. See the results of sensing, moving 1 down and sensing again! Feel free to change the simulation.
  
## How to change the simulation?

There are 3 functions: `sense`, `move` and `show_grid`

  * `sense`: 
    * Function: Update grid of beliefs (possibilites) after sensing
    * Inputs:
      * (char) sensed color (r / g),
      * (vector < vector <char> >) map (initialized map aka grid of colors),
      * (vector < vector <char> >) beliefs (location possibilities for each field of the map),
      * (float) possibility to sense correct,
      * (float) possibility to sense wrong
    * Output:
      * (vector < vector <char> >) updated beliefs
  * `move`:
    * Function: Update grid of beliefs (possibilites) after moving
    * Input:
      * (int) ammount of movement in y-direction,
      * (int) ammount of movement in x-direction,
      * (vector < vector <char> >) beliefs,
      * (float) blurring parameter (spreading posibility towards 3x3 adjacent grids)
    * Output:
      * (vector < vector <char> >) updated beliefs
  * `show_grid`:
    * Function: Print out a vector < vector <char> >
    * Input:
      * (vector < vector <char> >) grid that should be printed,
    * Output:
      * TO CONSOLE
    
  
