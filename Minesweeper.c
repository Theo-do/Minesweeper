#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

//defining min and max for Rows*Colls according to Smallest possible minesweeper(min) and computer RAM(ciel)
#define ciel 720 // For memory reasons
#define min 10  // because there wouldnt be any mines (anything "<1" turns to 0 concerning mines in this program
		// DEN exoume kanei to anadromiko anoigma koutiwn, den douleve me tipota

int** InstMines(int **ptr, int **ptr2, int **ptr3, int Rows, int Colls, int mines); 
void PrintBoard(int x, int y, int **board, int **closem, int Rows, int Colls, int **marked);
void PrintEmpty(int Rows, int Colls);
void Cords(int *pt1, int *pt2, int a, int b, char *pt3);
void Open(int x, int y, int *pt1, int *pt2,  int **board, int N, int M);
int  Checkmine(int x, int y, int **board);
void Mark(int x, int y, int *pt1, int *pt2, int **board, int **marked, int N, int M);
void cheat(int x, int y, int *pt1, int *pt2, int **board, int **closem, int N, int M);
int Gamestate(int **board, int N, int M);
int Adjchecker(int x, int y, int **board, int **closem, int N, int M);
int main(void)
{
	int x, y, N, M, diff, mines, **board, **marked, **closem, i, j, *pt1, *pt2, flag1 = 0, flag2 = 0, wins = 0;
	char ans, *pt3, order[20];
	
	srand(time(NULL));
	
	pt3 = order;
	pt1 = &x;
	pt2 = &y;	
	
	printf("Welcome to Minesweeper!\n would you like to play? y/n\n");
	scanf("%c", &ans);
	
	if(ans != 'y')
	{
		printf("Ok! Goodbye.\n");
		exit(1);
	}
	
	//Size of the board 
	do
    	{
		printf("Input the size of the board...(N , M)\n");
		scanf("%d %d", &N, &M);
        
		if((N * M < min) || (N * M > ciel))
		{
		    printf("Board is too small or too big...\n");
		}
        
    	}
    	while((N * M < min) || (N * M > ciel));
   
	//difficulty setting
	do
	{
		printf("Select difficulty :\n1)Noob\n2)Normal\n3)Hard\n4)Insane\n");
		scanf("%d", &diff);
		if(diff < 1 || diff > 4)
		{
			printf("Wrong input try again...\n");
		}
	}
	while(diff < 1 || diff > 4);
	
	PrintEmpty(N, M); //printing empty board
	
	//starting game loop
	while((N * M) <= ciel)
	{
	
		//memory allocation
		printf("Allocating mem...\n");
		board = (int**)malloc(N * sizeof(int*));
		marked = (int**)malloc(N * sizeof(int*));
		closem = (int**)malloc(N * sizeof(int*));
			
		if((board == NULL) || (marked == NULL) || (closem == NULL))
		{
			printf("Oops something went wrong, memory not allocated!\n");
			exit(1);
		}
		else
			printf("Memory allocated.\n");
			
			
		for(i = 0 ; i < M ; i++)
		{
			board[i] = (int*)malloc(M * sizeof(int));
			marked[i] = (int*)malloc(M * sizeof(int));
			closem[i] = (int*)malloc(M * sizeof(int));
				
			if((board == NULL) || (marked == NULL) || (closem == NULL))
			{
				printf("Oops something went wrong, memory not allocated!\n");
				exit(1);
			}
				
		}

		//calculating mines depending on difficulty
		switch(diff)
		{
			case 1:
			
				mines = (N*M)*0.10;
				break;
			case 2:
				mines = (N*M)*0.15;
				break;
			case 3:
				mines = (N*M)*0.20;
				break;
			case 4:
				mines = (N*M)*0.25;
				break;
		}
		
		//installing mines in the board and filling boolean board
		InstMines(board, marked, closem, N, M, mines);
		
		//filling a count array with close mines to each square
		for(i = 0 ; i < N ; i++)
		{
			for(j = 0 ; j < M ; j++)
			{
				if(closem[i][j] != 1)
					closem[i][j] = Adjchecker(i, j, board, closem, N, M);
				else
					closem[i][j] = 0;
			}
		}
			
		do
		{
			//getting cordinates out of players order
			Cords(pt1,pt2, N, M, pt3);
			x--;
			y--;
	
			printf("%d %d\n", x, y);
			printf("%s\n", order);
	
	
			//calling the specified order's function
			if(strcmp(order, "open") == 0)
			{
				Open(x, y, pt1, pt2, board, N, M);
			}
			else if(strcmp(order, "mark") == 0)
			{
				Mark(x, y, pt1, pt2, board, marked, N, M);
			}
			else if(strcmp(order, "cheat") == 0)
			{
				cheat(x, y, pt1, pt2, board, closem, N, M);
			}
	
			//printing the board
			PrintBoard(x, y, board, closem, N, M, marked);
		
			flag1 = Gamestate(board, N, M);
		}
		while(flag1 == 0);
		
		
	
		if(flag1 == 1)
		{
			flag1 = 0;
	
			wins++;
			//changing the borders every level
			N+= wins * 5;
			M+= wins * 5;
			printf("You won this level good job!\n");
		}
		
	}
	flag2 = 1;
	
	if(flag2 == 1)
	{
		printf("Congratulations, you won ALL the rounds! :)\n");
	}
	
	//freeing allocated memory
	for(i = 0 ; i < N ; i++)
	{
		free(board[i]);
		free(marked[i]);
		free(closem[i]);
	}
				
	free(board);
	free(marked);
	free(closem);
	
	
	
	
	return 0;
}
int** InstMines(int **ptr, int **ptr2, int **ptr3, int Rows, int Colls, int mines)
{
	int i, j, k, l;	
	
	for(i = 0 ; i < Rows ; i++)
	{
		for(j = 0 ; j < Colls ; j++)
		{
			ptr[i][j] = 0;
		}
	}
	
	for(i = 0 ; i < mines ; i++)
	{
		do
		{	
			k = rand() % Rows;
			l = rand() % Colls;
		}
		while(ptr[k][l] == 1);
		
		ptr[k][l] = 1;	
	
	}
	
	for(i = 0 ; i < Rows ; i++)
	{
		for(j = 0 ; j < Colls ; j++)
		{
			ptr2[i][j] = 0;
		}
	}
	
	for(i = 0 ; i < Rows ; i++)
	{
		for(j = 0 ; j < Colls ; j++)
		{
			ptr3[i][j] = 0;
		}
	}
		
}		
void PrintBoard(int x, int y, int **board, int **closem, int Rows, int Colls, int **marked)
{
	int i, j, count1 = 0, count2 = 0;
	
	printf("   ");
	for(j = 0 ; j < Colls ; j++)
	{
		printf(" %d", j+1);
	}
	printf("\n");
	for(j = 0 ; j < (2*Colls)+3 ; j++)
	{
		printf("-");
	}	
	printf("\n");
	for(i = 0 ; i < Rows ; i++)
	{
		if(i >= 0 && i < 9)
			printf("%d | ", i+1);
		else
			printf("%d| ", i+1);
		for(j = 0 ; j < Colls ; j++)
		{
			
			if((marked[i][j] == 9) && (board[i][j] != -1))
			{
				printf("@ ");
			}
			else if((board[i][j] == 0) || (board[i][j] == 1))
			{
				printf("# ");
			}
			else if(board[i][j] == -1)
			{
				if(closem[i][j] > 0)
				{
					printf("%d ", closem[i][j]);
				}
				else if(closem[i][j] == 0)
				{
					printf(". ");
				}
				count1++; //counting open blocks
			}
			
		}
		printf("\n");
	}
	printf("\n");
	printf("%d blocks are open.\nMake your move(s):\n", count1);
}
void Cords(int *pt1, int *pt2, int N, int M, char *pt3)
{
	char result[20];
	
	scanf("%s", result);
	if(strncmp(result, "open", 4) == 0)
	{
		sscanf(result, "open(%d,%d)", pt1, pt2);
		strcpy(pt3, "open");
	}
	else if(strncmp(result, "cheat", 5) == 0)
	{
		sscanf(result, "cheat(%d,%d)", pt1, pt2);
		strcpy(pt3, "cheat");
	}
	else if(strncmp(result, "mark", 4) == 0)
	{
		sscanf(result, "mark(%d,%d)", pt1, pt2);
		strcpy(pt3, "mark");
	}
	else if(strncmp(result, "exit", 4) == 0)
	{
		printf("\nGoodbye player!\n");
		exit(0);
	}
	else
	{
		printf("Wrong order name! try again...\n");
		Cords(pt1,pt2, N, M, result);
	}
	if(((*pt1) > N || *pt1 <= 0) || ((*pt2) > M || *pt2 <= 0))
	{
		printf("Wrong cordinates! try again...\n");
		Cords(pt1,pt2, N, M, result);
	}
	
}				
void Open(int x, int y, int *pt1, int *pt2, int **board, int N, int M)
{
	char result[20];
	
	if(board[x][y] == -1)
	{
		printf("\nThis square is already opened, try again\n");
		
		return;
		
	}
	else
	{
		if(Checkmine(x, y, board) == 1)
		{
			printf("You lost!\n");
			exit(1);
		}
		else
		{
			board[x][y] = -1;
		}			
	}
}
int  Checkmine(int x, int y, int **board)
{
	if(board[x][y] == 1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
void Mark(int x, int y, int *pt1, int *pt2, int **board, int **marked, int N, int M)
{
	char result[20];
	
	if(board[x][y] == -1)
	{
		printf("\nThis square is already opened, try again\n");
		
		return;
	}
	else
	{
		marked[x][y] = 9;
	}
}
void cheat(int x, int y, int *pt1, int *pt2, int **board, int **closem, int N, int M)
{
	char result[20], count = 0, i, j;
	
	
	count = Adjchecker(x, y, board, closem, N, M);
	
	if(board[x][y] == -1)
	{
		do
		{
			printf("\nThis square is already opened, no need to cheat :)\n");
			scanf("%s", result);
			if(strncmp(result, "cheat", 4) == 0)
			{
				sscanf(result, "cheat(%d,%d)", pt1, pt2);
			}
			else
			{
				printf("This is not the cheat command\n");
			}	
		}
		while((board[x][y] == -1) || (result != "cheat"));
		if(((*pt1) > N || *pt1 <= 0) || ((*pt2) > M || *pt2 <= 0))
		{
			printf("Wrong cordinates! try again...\n");
			Cords(pt1,pt2, N, M, result);
		}
	}
	else if(board[x][y] == 1)
	{
		printf("Careful! This square contains a mine.\n");
		if(count > 0)
			printf("There are also %d mines adjacent to it.\n", count);
	}
	else if((board[x][y] == 0) && (count == 0))
	{
		printf("This square is empty and there are no mines adjacent to it\n");
	}
}
int Gamestate(int **board, int N, int M)
{
	int i, j, flag = 1;
	
	for(i = 0 ; i < N ; i++)
	{
		for(j = 0 ; j < M ; j++)
		{
			if(board[i][j] == 0)
			{
				flag = 0;
			}
		}
	}
	
	return flag;
}
int Adjchecker(int x, int y, int **board, int **closem, int N, int M)
{
	int count = 0;
	
	if((y > 0) && (board[x][y-1] == 1))
	{
		count++;
	}
	if((y < M - 1) && (board[x][y+1] == 1))
	{
		count++;
	}
	if((x > 0) && (board[x-1][y] == 1))
	{
		count++;
	}
	if((x < N - 1) && (board[x+1][y] == 1))
	{
		count++;
	}
	if(((x > 0) && (y > 0)) && (board[x-1][y-1] == 1))
	{
		count++;
	}
	if((x > 0) && (y < M - 1) && (board[x-1][y+1] == 1))
	{
		count++;
	}
	if(((x < N - 1) && (y > 0)) && (board[x+1][y-1] == 1))	
	{
		count++;
	}
	if(((x < N - 1) && (y < y - 1)) && (board[x+1][y+1] == 1))
	{
		count++;
	}
	
	return count;
}
void PrintEmpty(int Rows, int Colls)
{
	int i, j;
	
	printf("   ");
	for(j = 0 ; j < Colls ; j++)
	{
		printf(" %d", j+1);
	}
	printf("\n");
	for(j = 0 ; j < (2*Colls)+3 ; j++)
	{
		printf("-");
	}	
	printf("\n");
	for(i = 0 ; i < Rows ; i++)
	{
		if(i >= 0 && i < 9)
			printf("%d | ", i+1);
		else
			printf("%d| ", i+1);
		for(j = 0 ; j < Colls ; j++)
		{
			printf("# ");
		}
		printf("\n");
	}
	printf("\n");
	printf("0 blocks are open.\nMake your move(s):\n");
}
