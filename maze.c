/***********************
 * Elijah Johnson
 * Lab Secton 001
 * 9/20/2018
 *
 * Solves any solveable maze using depth first
 * search and adjacency matrices
 *********************/


#include <stdio.h>

#define TOTAL_DIR 4
#define MAX_GRID 40
#define MAX_MAZE_WIDTH MAX_GRID * 4 + 1
#define MAX_MAZE_HEIGHT MAX_GRID * 2 + 1
#define getRow(i) (i * 2 + 1)
#define getCol(j) (j * 4 + 2)

enum DIRECTIONS 
{
  SOUTH = 0xC,
  EAST = 0xC0,
  NORTH = 0xC00,
  WEST = 0xC000
};

/*********************
 * Validates whether the char at the 
 *  row and col is valid
 * @param char c the char to check can be any ascii value
 * @param row the current row to check, can be 1-40
 * @param col the current col to check can be 1-40
 * @param totalCols the total number of columns in the grid
 * @return 0 if it is an error or 1 if it is valid
 * *******************/
int validateChar(char c, int row, int col, int totalCols)
{
  /*Check if it is a valid character*/
  if (c != ' ' && c != '-' && c != '|' && c != '+')
  {
    printf("Error\n\n");
    return 0;
  }

  /*Check if it is a corner*/
  if (row % 2 == 0 && col % 4 == 0)
  {
    if (c != '+')
    {
      printf("Error\n\n");
      return 0;
    }
  }
  /*Check if it is a first or last col*/
  else if (col == 0 || col == getCol(totalCols) - 1)
  {
    if (row % 2)
    {
      if (c != '|')
      {
        printf("Error\n\n");
        return 0;
      }
    }
  }
  /*Check if it is a first or last row*/
  else if (row == 0 || row == getRow(totalCols) - 1)
  {
    if (col % 4)
    {
      if (c != '-')
      {
        printf("Error\n\n");
        return 0;
      }
    }
  }

  return 1;
}

/***********************
 * This function reads in a maze from stdin
 * if maze is invalid returns -1 and prints an error message
 * @param maze[][] out paramater, the 2d char array
 *  to be filled with the maze
 * @param char *c out paramater, the current character
 *  used as an out to see if EOF is reached
 * @return int size, the size of the maze in gridSpaces
 *  or -1 if an error occurs
 * **********************/
int readMaze(char maze[][MAX_MAZE_WIDTH], char *c)
{
  int rows = 0;
  int gridRows = 0;
  int cols = 0;
  int numGrid = 0;
  int count = 0;
  int prevChar = 0;
  /*Read in the first line of the maze and get number of cols*/
  while ((*c = getchar()) != '\n' && *c != EOF)
  {
    if (*c != '-' && *c != '+')
    {
      printf("Error\n\n");
      return -1;
    }
    maze[rows][count] = *c;
    count++;
    if (*c == '+')
    {
      numGrid++;
    }
  }

  /*if first loop didnt execute return an error*/
  if (count == 0)
  {
    return -1;
  }
  prevChar = *c;
  rows++;
  /*Number of girdCols in maze*/
  cols = numGrid - 1;
  /*Number of rows there should be*/
  numGrid *= 2;

  /*Read in the rest of the maze */
  for (count = 0; numGrid > 0; numGrid--)
  {
    while ((*c = getchar()) != '\n' && *c != EOF)
    {
      if (prevChar == '\n' && *c == '|')
      {
        gridRows++;
      }
      prevChar = *c;
      /*Validate ever char*/
      if (!validateChar(*c, rows, count, cols))
      {
        return -1;
      }

      maze[rows][count] = *c;
      count++;
    }
    /* Break if we read two new lines in a row */
    if (prevChar == '\n' && *c == '\n')
    {
      break;
    }
    prevChar = *c;
    count = 0;
    rows++;
  }

  /*If the maze is not square return an error */
  if (gridRows != cols)
  {
    printf("Error\n\n");
    return -1;
  }

  return gridRows;
}

/********************
 * Prints out the maze to stdout based on the size
 * @param maze[][] the maze to be printed out with a size 
 *  of size, can be any range of chars
 * @param size, the size of the maze in grid spaces, used to calc
 *  how many times the loops should loop
 * @param c the current character, only print out new line at end 
 *  if it is not EOF
 *  Loops through the double array printing out every character seen
 * ********************/
void printMaze(char maze[][MAX_MAZE_WIDTH], int size, char c)
{
  int i;
  int j;

  for (i = 0; i < getRow(size); i++)
  {
    for (j = 0; j < getCol(size) - 1; j++)
    {
      printf("%c", maze[i][j]);
    }
    printf("\n");
  }
  if (c != EOF)
  {
    printf("\n");
  }
}

/************************
 * Uses depth first search to find a solution to the maze
 * @param int adj[][] the array that holds adjacency values
 *   for the graph of the maze
 * @param char visited[] the array of visited nodes
 * @param int size the size of the maze, range 1-40
 * @param node the current node we are checking, range 0-size * size - 1
 * @param char maze[][] the array that is the maze, gets added
 *  to and then backtracked if needed 
 * ********************/
int depthFirstSearch(int adj[][TOTAL_DIR], char visited[40 * 40], 
    int size, int node, char maze[][MAX_MAZE_WIDTH])
{
  int j;
  int next;
  int row[2] = {0};
  int col[2] = {0};
  /*Put the starting star in the mze */
  if (node == 0)
  {
    maze[getRow(node / size)][getCol(node % size)] = '*';
  }
  
  /*If we are at end maze is solved return 1*/
  if (node == size * size - 1)
  {
    return 1;
  }
  /*Mark this node as visited */
  visited[node] = 1;
  for (j = 0; j < TOTAL_DIR; j++)
  {
    /*If the adj nodes are not visited put stars in the 
     * right place and check the next square using dfs */
    if (!visited[adj[node][j]])
    {
      next = adj[node][j];
      row[0] = getRow(next / size);
      col[0] = getCol(next % size);

      maze[row[0]][col[0]] = '*';
      
      if (node % size == next % size)
      {
        if (next > node)
        {
          row[1] = row[0] - 1;
        }
        else
        {
          row[1] = row[0] + 1;
        }
        col[1] = col[0];
      }
      else
      {
        if (next > node)
        {
          col[1] = col[0] - 2;
        }
        else
        {
          col[1] = col[0] + 2;
        }
        row[1] = row[0];
      }

      maze[row[1]][col[1]] = '*';
      
      if (depthFirstSearch(adj, visited, size, next, maze))
      {  
        return 1;
      }
      /* Backtrack by getting rid of stars if no solution is found */
      maze[row[0]][col[0]] = ' ';
      maze[row[1]][col[1]] = ' ';
    }
  }

  return 0;
}

/************************
 * Checks what direction walls are in
 * @param maze[][] the maze to check walls in
 * @param int row the row to check in the maze
 *  0-40 * 4 + 1
 * @param int col the col to check in the maze
 *  0-40 * 2 + 1
 * @return An or'ed value with all of the directions
 *  walls appear in
 * *********************/
int checkDirections(char maze[][MAX_MAZE_WIDTH], int row, int col)
{
  int i;
  int j;
  int returnVal = 0;
  /*Check four adjacent squares and see if a wall is there */
  for (i = row - 1; i <= row + 1; i++)
  {
    for (j = col - 2; j <= col + 2; j += 2)
    {
      if (maze[i][j] == '-')
      {
        if (i < row)
        {
          returnVal |= NORTH;
        }
        else if (i > row)
        {
          returnVal |= SOUTH;
        }
      }
      else if (maze[i][j] == '|')
      {
        if (j < col)
        {
          returnVal |= WEST;
        }
        else if (j > col)
        {
          returnVal |= EAST;
        }
      }
    }
  }

  return returnVal;
}

/********************
 * Creates an adjacency matrix based on the maze
 * @param char maze[][] the maze to create adjacency based off of
 * @param size the size of the maze, 1-40
 * @param int adj[][] the array to be filled with adjacency values
 * Checks the walls for every cell and then puts a 1 in adjacency
 * matrix if no wall appears in that direction
 * *****************/
void createAdjacencyMatrix(char maze[][MAX_MAZE_WIDTH], int size, 
    int adj[][TOTAL_DIR])
{
  int i;
  int j;
  int wallDirection;
  int gridNum;
  int currentGridNum;
  
  for (i = 0; i < size; i++)
  {
    for (j = 0; j < size; j++)
    {
      /* Using the wall direction set what are adj to the current grid space
       * check south east north and then west */
      currentGridNum = i * size + j;
      wallDirection = checkDirections(maze, getRow(i), getCol(j));
      if ((wallDirection & SOUTH) == 0)
      {
        gridNum = (i + 1) * size + j;
        adj[currentGridNum][0] = gridNum;
      }
      if ((wallDirection & EAST) == 0)
      {
        gridNum = i * size + (j + 1);
        adj[currentGridNum][1] = gridNum;
      
      }
      if ((wallDirection & NORTH) == 0)
      {
        gridNum = (i - 1) * size + j;
        adj[currentGridNum][2] = gridNum;
 
      }
      if ((wallDirection & WEST) == 0)
      {
        gridNum = i * size + (j - 1);
        adj[currentGridNum][3] = gridNum;
      }
    }
  }
}

/**************************
 * Clears adjacent matrix
 * @param char adj[][] the adjacency matrix
 *  to be cleared, values of the matrix are either 0 or 
 *  1
 * Loops through all of the elemerts of adj,
 * setting it to 0 
 * ************************/
void clearAdj(int adj[][TOTAL_DIR], int size)
{
  int i;
  int j;
  for (i = 0; i < size * size; i++)
  {
    for (j = 0; j < TOTAL_DIR; j++)
    {
      adj[i][j] = 0;
    }
  }
}

/************************
 * Clears the maze array to all null terminators
 * @param char maze[][] the maze to be cleared to null
 * @param int size the size of the maze 1-40
 * Sets all the values to '\0' 
 * *********************/
void clearMaze(char maze[][MAX_MAZE_WIDTH], int size)
{
  int i;
  int j;

  for (i = 0; i < getRow(size); i++)
  {
    for (j = 0; j < getCol(size); j++)
    {
      maze[i][j] = '\0';
    }
  }
}

/********************
 * CLears the visited array, of size MAX_GRID
 * MAX_GRID
 * @param char visited[] the array to be cleared
 * @param int size the size of the last maze 
 *  1-40
 * ******************/
void clearVisited(char visited[MAX_GRID * MAX_GRID], int size)
{
  int i;
  for (i = 0; i < size * size; i++)
  {
    visited[i] = 0;
  }
}

/*****************************
 * This function reads until an empty line
 * It clears out a maze if it is an error
 * @param char c the current c 
 * @return the last value read from getchar
 * **************************/
int readUntilEmptyLine(char c)
{
  if (c != EOF && c != '\n')
  {
    while ((c = getchar()) != '\n');
    if ((c = getchar()) != '\n' && c != EOF)
    {
      return readUntilEmptyLine(c);
    }
    return c;
  }

  return c;
}


int main()
{
  int size;
  /*Maze variable to hold the maze */
  char maze[MAX_MAZE_HEIGHT][MAX_MAZE_WIDTH];
  /*adj array to hold what squares are possible
   * initalize to 0*/
  int adj[MAX_GRID * MAX_GRID][4] = {{0}};
  /*array to hold what nodes we have visited */
  char visited[MAX_GRID * MAX_GRID] = {0};
  char c = 0;
  do 
  {
    /*Read in maze and get size from it */
    size = readMaze(maze, &c);
    if (size == -1)
    {
      clearMaze(maze, MAX_GRID);
      c = readUntilEmptyLine(c);
    }
    else 
    {
      /*Create the adjacencyMatrix and preform
       * depth first search on the current maze */
      createAdjacencyMatrix(maze, size, adj);
      if (!depthFirstSearch(adj, visited, size, 0, maze))
      {
        printf("No Solution\n");
        printf("\n");
      }
      else 
      {
        printMaze(maze, size, c);
      }
      /*Clear everything out to default values */
      clearAdj(adj, size);
      clearMaze(maze, size);
      clearVisited(visited, size);
    }
  } 
  /*read in mazes one at a time until EOF */
  while (c != EOF);
  return 0;
}
