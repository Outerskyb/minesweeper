#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<conio.h>
#include<Windows.h>

#define WIDTH 20
#define HEIGHT 20
#define UP 72
#define LEFT 75
#define RIGHT 77
#define DOWN 80

int map[WIDTH][HEIGHT];//보여주는것
int real[WIDTH][HEIGHT];//실제맵
int open[WIDTH][HEIGHT];//open여부
int move;
int number_of_mine;

typedef struct _op {
	int x;
	int y;
}op;

op position;

void remove_cursor(void) {
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

void init_map(void) {
	memset(map, 0, sizeof(map));
	memset(real, 0, sizeof(real));
	memset(open, 0, sizeof(open));
}

void gotoxy(int x, int y)//gotoxy
{
	COORD pos = { x - 1, y - 1 };//커서가 X좌표에서 -1 한값. Y좌표에서 -1한 값으로 이동
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void make_mine(void) {
	static int count = 0;
	if (count == number_of_mine) {
		return;
	}
	int x = rand() % WIDTH;
	int y = rand() % HEIGHT;
	if (real[y][x] == 0) {
		real[y][x] = 9;
		count++;
	}
	else {
		make_mine();
	}
}

void make_number(void) {
	int i, j;
	int number;
	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			if (real[i][j] != 0) {
				continue;
			}
			number = 0;
			if (i != HEIGHT - 1) {
				if (real[i + 1][j] == 9) {
					number++;
				}
				if (j != WIDTH - 1) {
					if (real[i + 1][j + 1] == 9) {
						number++;
					}
				}
				if (j != 0) {
					if (real[i + 1][j - 1] == 9) {
						number++;
					}
				}
			}
			if (i != 0) {
				if (real[i - 1][j] == 9) {
					number++;
				}
				if (j != WIDTH - 1) {
					if (real[i - 1][j + 1] == 9) {
						number++;
					}
				}
				if (j != 0) {
					if (real[i - 1][j - 1] == 9) {
						number++;
					}
				}

			}
			if (real[i][j] == 9) {
				number++;
			}if (j != WIDTH - 1) {
				if (real[i][j + 1] == 9) {
					number++;
				}
			}if (j != 0) {
				if (real[i][j - 1] == 9) {
					number++;
				}
			}
			real[i][j] = number;
		}
	}
}

void update_map(void) {
	int i, j;
	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			if (open[i][j] == 1) {
				map[i][j] = real[i][j];
			}
		}
	}
}
void show_map(void) { //맵표현 gotoxy
					  /*
					  매직넘버
					  0:빈칸
					  1:숫자１
					  2:숫자２
					  3:숫자３
					  4:숫자４
					  5:숫자５
					  6:숫자６
					  7:숫자７
					  8:숫자８
					  9:지뢰
					  10:깃발
					  */
	int i, j;

	gotoxy(1, 1);
	printf("\n");


	printf("┌");
	for (j = 0; j < WIDTH; j++) { //메인창 바닥
		printf("─");
	}
	printf("┐");
	printf("\n");

	for (i = 0; i <= HEIGHT; i++) { //메인창의 높이
		if (i != HEIGHT) {
			printf("┃"); //메인창 테두리
			for (j = 0; j < WIDTH; j++) { //메인창 너비
				if (map[i][j] == 0) {
					printf("　");//벽
				}
				else if (map[i][j] == 1) {
					printf("１");//전각문자
				}
				else if (map[i][j] == 2) {
					printf("２");//hole
				}
				else if (map[i][j] == 3) {
					printf("３");//stone
				}
				else if (map[i][j] == 4) {
					printf("４");//전각문자
				}
				else if (map[i][j] == 5) {
					printf("５");//hole
				}
				else if (map[i][j] == 6) {
					printf("６");//stone
				}
				else if (map[i][j] == 7) {
					printf("７");//전각문자
				}
				else if (map[i][j] == 8) {
					printf("８");//hole
				}
				else if (map[i][j] == 9) {
					printf("※");//stone
				}
				else if (map[i][j] == 10) {
					printf("▶");//전각문자
				}
			}
			printf("┃");// 테두리
			if (i == 4) {
				printf("moves:%d", move);
			}
			printf("\n");
		}
		else {
			printf("└");
			for (j = 0; j < WIDTH; j++) { //메인창 바닥
				printf("─");
			}
			printf("┘");
			printf("\n");
		}

	}
}

int select_box(int input) {
	if (input == ' ')
	{
		if (map[position.x][position.y] == 9)
			return 0;
		else return 1;
	}
	else if (input == 'f')
	{
		if (map[position.y][position.x] == 10)
		{
			map[position.y][position.x] = real[position.x][position.y];
		}
		else
		{
			map[position.y][position.x] = 10;
		}
	}
}

void move_current_position(int way) {
	if (way == UP) {
		position.y--;
		if (position.y == 2)
			position.y = 3;
	}
	else if (way == DOWN) {
		position.y++;
		if (position.y == 23)
			position.y = 22;
	}
	else if (way == LEFT) {
		position.x--;
		if (position.x == 1)
			position.x = 2;
	}
	else if (way == RIGHT) {
		position.x++;
		if (position.x == 22)
			position.x = 21;
	}
}

void game_over(void) {
	init_map();
	show_map();
	gotoxy((WIDTH + 7) / 2, (HEIGHT + 1) / 2);
	printf("Game Over");
	gotoxy((WIDTH + 7) / 2, ((HEIGHT + 1) / 2) + 1);
	printf("move :%d", move);
}

int main(void) {
	position.x = 2;
	position.y = 3;
}
