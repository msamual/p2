#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

void printMap(int leftRocket, int rightRocket, int ballY, int ballX)
{
	printf("\033[2J\033[1;1H");
	printf("----------------------------------------------------------------------------------\n");
	for(int y = 1; y < 26; ++y)
	{
		for(int x = 0; x < 82; ++x)
		{
			if (x == 0 || x == 81)
				printf("#");
			else if (x == 1 && (y == leftRocket || y == leftRocket + 1 || y == leftRocket + 2))
				printf("|");
			else if (x == 80 && (y == rightRocket || y == rightRocket + 1 || y == rightRocket + 2))
				printf("|");
			else if (y == ballY && x == ballX)
				printf("O");
			else
				printf(" ");
		}
		printf("\n");
	}
	printf("----------------------------------------------------------------------------------\n");
}

void setTerminal(struct termios *term, struct termios *termSave)
{
	tcgetattr(0, term);
	*termSave = *term;
	(*term).c_lflag &= (~ICANON & ~ECHO);
	(*term).c_cc[VTIME] = 0;
	(*term).c_cc[VMIN] = 1;
	tcsetattr(0, TCSANOW, term);
}

int isRocketHere(int y, int x, int lRocket, int rRocket)
{
	return	(x == 1 && (y == lRocket || y == lRocket + 1 || y == lRocket + 2))
			|| 
			(x == 80 && (y == rRocket || y == rRocket + 1 || y == rRocket + 2));
}

void moveBall(int *ballY, int *ballX, int *dirY, int *dirX, int lRocket, int rRocket)
{
	int y = *ballY + *dirY;
	int x = *ballX + *dirX;

	if (y == 0 || y == 26)
		*dirY *= -1;
	else
		*ballY = y;
	if(isRocketHere(y, x, lRocket, rRocket))
		*dirX *= -1;
	else
		*ballX = x;
}

void moveRockets(char *input, int *lRocket, int *rRocket)
{
	char c;
	while (*input)
	{
		c = *input++;
		if (c == 'a' && *lRocket != 1)
			--*lRocket;
		if (c == 'z' && *lRocket != 23)
			++*lRocket;
		if (c == 'k' && *rRocket != 1)
			--*rRocket;
		if (c == 'm' && *rRocket != 23)
			++*rRocket;
	}
}

void readInput(char *str)
{
	int i = 0;
	i = read(0, str, 100);
	str[i] = 0;
}

int main()
{
	char *input = malloc(100);
	int leftRocket = 12, rightRocket = 12;
	int ballX = 40, ballY = 13, dirY = 1 , dirX = 1;
	struct termios term;
	struct termios termSave;

	setTerminal(&term, &termSave);
	while(1)
	{
		printMap(leftRocket, rightRocket, ballY, ballX);
		readInput(input);
		moveRockets(input, &leftRocket, &rightRocket);
		moveBall(&ballY, &ballX, &dirY, &dirX, leftRocket, rightRocket);
	}
	tcsetattr(0, TCSANOW, &termSave);
}
