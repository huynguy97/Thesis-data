#include <iostream>
#include <fstream>
#include <cassert>
using namespace std;

enum Cell {Dead=0, Live};                         // a cell is either Dead or Live (we use the fact that dead = 0 and live = 1)

const char DEAD             = '.' ;               // the presentation of a dead cell (both on file and screen)
const char LIVE             = '*' ;               // the presentation of a live cell (both on file and screen)
const int NO_OF_ROWS        = 40 ;                // the number of rows (height) of the universe (both on file and screen)
const int NO_OF_COLUMNS     = 60 ;                // the number of columns (width) of the universe (both on file and screen)
const int ROWS              = NO_OF_ROWS    + 2 ; // the number of rows in a universe array, including the 'frame' of dead cells
const int COLUMNS           = NO_OF_COLUMNS + 2 ; // the number of columns in a universe array, including the 'frame' of dead cells

const int MAX_FILENAME_LENGTH = 80 ;              // the maximum number of characters for a file name (including termination character)

//  Part 1: one-dimensional arrays
bool enter_filename (char filename [MAX_FILENAME_LENGTH])
{
    // pre-conditions, post-conditions, implementation
}

//  Part 2: setting the scene
bool read_universe_file (ifstream& inputfile, Cell universe [ROWS][COLUMNS])
{
    // pre-conditions, post-conditions, implementation
}

void show_universe (Cell universe [ROWS][COLUMNS])
{
    // pre-conditions, post-conditions, implementation
}

//  Part 3: the next generation
void next_generation (Cell now [ROWS][COLUMNS], Cell next [ROWS][COLUMNS])
{
    // pre-conditions, post-conditions, implementation
}

int main ()
{

}
