#pragma warning(disable:4996)

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
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

int number_of_mine = 51;

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
	int i, j;
	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			map[i][j] = 0;
			open[i][j] = 0;
			real[i][j] = 0;
		}
	}
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
	if (real[y][x] == 0 && (position.x != x && position.y != y)) {
		real[y][x] = 9;
		count++;
	}
	make_mine();
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
			if (open[i][j] == 1 && real[i][j] != 9) {
				map[i][j] = real[i][j];
			}
			else {
				map[i][j] = 11;
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
					  11:오픈안된거
					  12:유저
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
					printf("□");//벽
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
				else if (map[i][j] == 11) {
					printf("◆");//전각문자
				}
				else if (map[i][j] == 12) {
					printf("●");//전각문자
				}
			}
			printf("┃");// 테두리
			if (i == 4) {

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

int game_over(void) {
	init_map();
	show_map();
	gotoxy((WIDTH + 7) / 2, (HEIGHT + 1) / 2);
	printf("Game Over");
	return 1;
}

int clear_check(void) {
	int i, j, cnt = 0;
	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			if (open[i][j] == 1) {
				cnt++;
			}
		}
	}
	if (cnt == number_of_mine) {
		init_map();
		show_map();
		gotoxy((WIDTH + 9) / 2, (HEIGHT + 1) / 2);
		printf("Clear!");
		return 1;
	}

}

void open_box(int i, int j, int k) {
	open[i][j] = 1;
	if (real[i][j + 1] == 0 && j != WIDTH - 1 && k != 3 && open[i][j + 1] != 1) {
		open_box(i, j + 1, 1);
	}
	if (real[i + 1][j] == 0 && i != HEIGHT - 1 && k != 4 && open[i + 1][j] != 1) {
		open_box(i + 1, j, 2);
	}
	if (real[i - 1][j] == 0 && i != 0 && k != 2 && open[i - 1][j] != 1) {
		open_box(i - 1, j, 4);
	}
	if (real[i][j - 1] == 0 && j != 0 && k != 1 && open[i][j - 1] != 1) {
		open_box(i, j - 1, 3);
	}
	if (real[i][j] == 0) {
		if (j != WIDTH - 1 && real[i][j + 1] != 9) {
			open[i][j + 1] = 1;
		}if (j != 0 && real[i][j - 1] != 9) {
			open[i][j - 1] = 1;
		}
		if (i != HEIGHT - 1) {
			if (real[i + 1][j] != 9) {
				open[i + 1][j] = 1;
			}
			if (j != WIDTH - 1 && real[i + 1][j + 1] != 9) {
				open[i + 1][j + 1] = 1;
			}
			if (j != 0 && real[i + 1][j - 1] != 9) {
				open[i + 1][j - 1] = 1;
			}
		}
		if (i != 0) {
			if (real[i - 1][j] != 9) {
				open[i - 1][j] = 1;
			}
			if (j != WIDTH - 1 && real[i - 1][j + 1] != 9) {
				open[i - 1][j + 1] = 1;
			}
			if (j != 0 && real[i - 1][j - 1] != 9) {
				open[i - 1][j - 1] = 1;
			}
		}
	}

}

int select_box(int input) {
	if (input == ' ')
	{
		if (real[position.y][position.x] == 9) {
			game_over();
			return 1;
		}
		else open_box(position.y, position.x, 6);

	}
	else if (input == 'f')
	{
		if (map[position.y][position.x] == 10)
		{
			map[position.y][position.x] = 11;
		}
		else
		{
			map[position.y][position.x] = 10;
		}
	}
	return 0;
}

void move_current_position(int way) {
	if (way == UP) {
		if (position.y != 0)
			position.y--;
	}
	else if (way == DOWN) {
		if (position.y != HEIGHT - 1)
			position.y++;
	}
	else if (way == LEFT) {
		if (position.x != 0)
			position.x--;
	}
	else if (way == RIGHT) {
		if (position.x != WIDTH - 1)
			position.x++;
	}

}

int main(void) {
	int key, check = 0;
	srand(time(NULL));
	position.x = position.y = 0;
	remove_cursor();
	init_map();
	update_map();
	map[position.y][position.x] = 12;
	show_map();
	while (1) {
		key = getch();
		if (key == ' ' || key == 'f') {
			if (check == 0) {
				make_mine();
				make_number();
				check++;
			}
			if (select_box(key) == 1) {
				goto end;
			}
			update_map();
			map[position.y][position.x] = 12;
			show_map();
			if (clear_check() == 1) {
				goto end;
			}
			continue;
		}
		update_map();
		key = getch();
		if (key == UP || key == DOWN || key == RIGHT || key == LEFT) {
			move_current_position(key);
			map[position.y][position.x] = 12;
			show_map();
		}
		show_map();
	}
end:
	return 0;
}
