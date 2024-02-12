#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROWS 20
#define COLS 20

struct Player
{
    int life;
    int posX;
    int posY;
};

void printLabyrinth(char labyrinth[ROWS][COLS])
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            printf("%c ", labyrinth[i][j]);
        }
        printf("\n");
    }
}

int main(void)
{
    char labyrinth[ROWS][COLS];
    char language = '0';
    char movement;
    int win = 0;
    FILE *ptr;

    srand(time(NULL));

    // Gerar o labirinto...

    struct Player player;
    player.life = 10;
    player.posX = ROWS - 1;
    player.posY = 0;

    // Iniciar o jogo
    printf("Bem-vindo ao Labirinto!\n\n");

    while (player.life > 0 && !win)
    {
        // Imprimir o labirinto e o estado atual do jogador
        printf("Labirinto:\n");
        printLabyrinth(labyrinth);
        printf("\nVida restante: %d\n", player.life);
        printf("Posição atual: (%d, %d)\n", player.posX, player.posY);
        printf("\n");

        // Solicitar movimento do jogador
        printf("Escolha seu próximo movimento:\n");
        printf("[W] - Cima\n[A] - Esquerda\n[S] - Baixo\n[D] - Direita\n");
        printf("Movimento: ");
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
    return 0;
}
