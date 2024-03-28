#include <stdio.h>
#include <stdlib.h> // for random numbers
#include <time.h>	// seed for random

#define ROWS 20 // define the number of rows
#define COLS 20 // define the number of columns

// Define a struct for player
struct Player
{
	int life;
	int posX;
	int posY;
};

int main(void)
{
	char labyrinth[20][20]; // declare the matrix
	char usefully_labyrinth[20][20];
	char language = '0';
	char movement;
	int win = 0;
	FILE *ptr;

	srand(time(NULL));

	// Generate a random number between 1 and 6
	int random_number = rand() % 2 + 1;
	printf("The random number is: %d\n", random_number);

	// generate the labyrinth
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			labyrinth[i][j] = '@'; // initialize with '@' character
		}
	}

	// make wallkable path
	// make G
	for (int i = 0; i < 20; i++)
	{
		labyrinth[i][0] = 'g'; // 0 represents wall, 1 represents a wallkable path
	}
	// make G
	for (int j = 0; j < 3; j++)
	{
		labyrinth[0][j] = 'g';	// 0 represents wall, 1 represents a wallkable path
		labyrinth[8][j] = 'g';	// 0 represents wall, 1 represents a wallkable path
		labyrinth[19][j] = 'g'; // 0 represents wall, 1 represents a wallkable path
	}

	// make G
	for (int i = 8; i < 20; i++)
	{
		labyrinth[i][2] = 'g'; // 0 represents wall, 1 represents a wallkable path
	}

	// wallkabble
	labyrinth[19][3] = 'w';

	// make R
	for (int i = 0; i < 20; i++)
	{
		labyrinth[i][4] = 'r'; // 0 represents wall, 1 represents a wallkable path
	}

	// wallkabble
	labyrinth[19][5] = 'w';

	// make R
	for (int j = 4; j < 6; j++)
	{
		labyrinth[0][j] = 'r'; // 0 represents wall, 1 represents a wallkable path
	}

	// make R

	for (int i = 1; i < 7; i++)
	{
		labyrinth[i][6] = 'r'; // 0 represents wall, 1 represents a wallkable path
	}

	// make R
	labyrinth[7][5] = 'r';
	labyrinth[8][6] = 'r';

	// make R

	for (int i = 8; i < 20; i++)
	{
		labyrinth[i][6] = 'r'; // 0 represents wall, 1 represents a wallkable path
	}

	// make wallkable
	labyrinth[19][7] = 'w';

	// make I
	for (int i = 0; i < 4; i++)
	{
		labyrinth[i][8] = 'i'; // 0 represents wall, 1 represents a wallkable path
		labyrinth[i][9] = 'i'; // 0 represents wall, 1 represents a wallkable path
	}

	// make I
	for (int i = 7; i < 20; i++)
	{
		labyrinth[i][8] = 'i'; // 0 represents wall, 1 represents a wallkable path
		labyrinth[i][9] = 'i'; // 0 represents wall, 1 represents a wallkable path
	}

	// make wallkable
	labyrinth[19][10] = 'w';

	// make S
	for (int j = 11; j < 14; j++)
	{
		labyrinth[0][j] = 's';	// 0 represents wall, 1 represents a wallkable path
		labyrinth[8][j] = 's';	// 0 represents wall, 1 represents a wallkable path
		labyrinth[19][j] = 's'; // 0 represents wall, 1 represents a wallkable path
	}
	// make S
	for (int i = 9; i < 20; i++)
	{
		labyrinth[i][13] = 's'; // 0 represents wall, 1 represents a wallkable path
	}

	// make S
	for (int i = 0; i < 9; i++)
	{
		labyrinth[i][11] = 's'; // 0 represents wall, 1 represents a wallkable path
	}

	// make wallkable
	for (int j = 14; j < 20; j++)
	{
		labyrinth[0][j] = 'w'; // 0 represents wall, 1 represents a wallkable path
	}

	// print the labyrinth beautifully
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			printf("%c	", labyrinth[i][j]);
		}
		printf("\n");
	}

	printf("\n");
	printf("\n");
	printf("\n");

	// print the labyrinth usefully

	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			usefully_labyrinth[i][j] = labyrinth[i][j];
			printf("%c", usefully_labyrinth[i][j]);
		}
	}

	printf("\n");
	printf("\n");
	printf("\n");

	// select the text language
	printf("Select the Language\n[0] - English\n[1] - Portuguese (the good one)\n");
	scanf("%c", &language);

	// validate the language chose
	while (language != '0' && language != '1')
	{
		printf("Select the Language\n[0] - English\n[1] - Portuguese (the good one)\n");
		scanf("%c", &language);
	}

	printf("\n");
	printf("\n");

	if (language == 0)
	{
		printf("In an enchanted land called Eldoria, the kind wizard Es0j found himself consumed by the darkness spreading throughout the kingdom. His apprentice, js0n, witnessed the transformation of his master into a malevolent creature and fled to save his life.\n\nEs0j, now possessed by an unrelenting thirst for power, turned the castle where they lived into a deadly maze, where illusions and dangers lurked at every turn. Determined to escape and restore peace to Eldoria, js0n faces daunting challenges as he is pursued by his former mentor, now unrecognizable in his malevolence.\n\nUse your skills to help js0n escape:\n");
	}
	else
	{
		printf("Em uma terra encantada chamada Eldoria, o bondoso mago Es0j viu-se consumido pela escuridão que se espalhava pelo reino. Seu aprendiz, js0n, testemunhou a transformação de seu mestre em uma criatura malévola e fugiu para salvar sua vida.\n\nEs0j, agora possuído por uma sede implacável de poder, transformou o castelo em que viviam em um labirinto mortal, onde ilusões e perigos os cercavam a cada passo. js0n, determinado a escapar e restaurar a paz em Eldoria, enfrenta desafios assustadores enquanto é perseguido por seu antigo mentor, agora irreconhecível em sua maldade.\n\nUse suas habilidades para ajudar js0n a escapar:\n");
	}

	printf("\n");
	printf("\n");

	// Initialize player
	struct Player player;
	player.life = 10;
	player.posX = 19; // inicial row
	player.posY = 0;  // inicial column
	while (player.life != 0 || win == 1) {
		// english
		if (language == '0')
		{
			printf("[W] - Up\n[A] - Left\n[S] - Down\n[D] - Right\n");
			// portuguese (the good one)
		}
		else
		{	
			printf("Remaining Life: %d\n", player.life);
			printf("[W] - Cima\n[A] - Esquerda\n[S] - Baixo\n[D] - Baixo\n");
		}

		scanf(" %c", &movement);

		// process user's movement
		switch (movement)
		{
		case 'w':
			// verify the player's movement
			if (player.posX - 1 < 0)
			{
				printf("js0n fell off the magic cliff\n");
				player.life = 0;
			}
			else if (labyrinth[player.posX - 1][player.posY] == '@')
			{
				printf("js0n was hitten by es0j\n");
				player.life--;
				player.posX--;
			}
			else
			{
				player.posX--;
			}
			break;

		case 's':
			// verify the player's movement
			if (player.posX + 1 > 19)
			{
				printf("js0n fell off the magic cliff\n");
				player.life = 0;
			}
			else if (labyrinth[player.posX + 1][player.posY] == '@')
			{
				printf("js0n was hitten by es0j\n");
				player.life--;
				player.posX++;
			}
			else
			{
				player.posX++;
			}
			break;

		case 'a':
			// verify the player's movement
			if (player.posY - 1 < 0)
			{
				printf("js0n fell off the magic cliff\n");
				player.life = 0;
			}
			else if (labyrinth[player.posX][player.posY - 1] == '@')
			{
				printf("js0n was hitten by es0j\n");
				player.life--;
				player.posY--;
			}
			else
			{
				player.posY--;
			}
			break;

		case 'd':
			// verify the player's movement
			if (player.posY + 1 > 19)
			{
				printf("js0n fell off the magic cliff\n");
				player.life = 0;
			}
			else if (labyrinth[player.posX][player.posY + 1] == '@')
			{
				printf("js0n was hitten by es0j\n");
				player.life--;
				player.posY++;
			}
			else
			{
				player.posY++;
			}
			break;

		default:
			printf("Invalid movement! Please enter w, s, a, or d.\n");
		}
		
		if (player.life == 0){
			printf("js0n was killed by es0j\n");
		}
		
		// player win
		if (player.posX == 19 && player.posY == 19){
			win = 1;
			ptr = fopen("flag.txt", "r"); // Open file in read mode

			// Check if file opened successfully
			if (ptr != NULL)
			{
				// File opened successfully, read and print content
		
				char ch;
				// print the file content
				while ((ch = fgetc(ptr)) != EOF)
				{
					putchar(ch);
				}

				// Close the file after use
				fclose(ptr);
			}
			else{
				// File opening failed
				printf("Error: Failed to open file!\n");
			}
		}
	}
	return 0;
}




