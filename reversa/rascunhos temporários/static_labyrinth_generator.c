#include <stdio.h>  
#include <stdlib.h> // for random numbers
#include <time.h> // seed for random

#define ROWS 20 // define the number of rows
#define COLS 20 // define the number of columns

int main(void){
	int labyrinth[20][20]; // declare the matrix

/*

	// generate the labyrinth
	for(int i=0; i < 20; i++){
		for (int j=0; j < 20; j++){
			labyrinth[i][j] = i+j; // tmp
		}

	}
*/



	srand(time(NULL));


  	// Generate a random number between 1 and 6 
  	int random_number = rand() % 2 + 1;  
	printf("The random number is: %d\n", random_number); 

	// generate the labyrinth
	for(int i=0; i < 20; i++){
		for (int j=0; j < 20; j++){
            labyrinth[i][j] = rand() % 2; // 0 represents wall, 1 represents a wallkable path
		}
	}

    // Set entrance and exit
    labyrinth[0][0] = 0; // Entrance
    labyrinth[ROWS - 1][COLS - 1] = 0; // Exit

	// print the labyrinth
	for(int i=0; i < 20; i++){
		for(int j=0; j < 20; j++){
			if (labyrinth[i][j] == 0){
				printf("#	"); // Wall
			}
			else if (i == 0 && j == 0){
				printf("E	"); // Entrance 
			}
			else if (i == (ROWS-1) && j == (COLS-1)){
				printf("X	"); // Exit
			}else{
				printf("W	"); // Wallkable
			}
						
		}
		printf("\n");
	}




	return 0;
}
