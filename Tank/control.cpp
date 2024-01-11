#include "lib/data.h"
#include "lib/view.h"
#include "lib/control.h"
#include "lib/model.h"

void GotoXY(int column, int line) {
	COORD coord;
	coord.X = column;
	coord.Y = line;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int WhereX() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
		return csbi.dwCursorPosition.X;
	return -1;
}

int WhereY() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
		return csbi.dwCursorPosition.Y;
	return -1;
}

void GotoBoard(int x, int y) {
	GotoXY(--x + 30, --y + 2);
}

bool MoveUp(board& a, gameObject& obj) {
	bool stay = 0;
	if (obj.direction != make_pair(-1, 0)) stay = 1;
	int& i = obj.y, & j = obj.x;
	if (!stay) {
		gameObject& nxt = a[i - 1][j];
		if (nxt.type == WALL || nxt.type == PLAYER || nxt.type == ENEMY) return 0;
	}
	obj.direction = { -1, 0 };
	GotoBoard(j, i); cout << SPACE;
	a[i][j] = gameObject(j, i, 0, {0, 0});
	i -= 1 ^ stay;
	PrintTank(j, i, obj.color, obj.direction);
	a[i][j] = obj;
	return 1;
}

bool MoveDown(board& a, gameObject& obj) {
	bool stay = 0;
	if (obj.direction != make_pair(1, 0)) stay = 1;
	int& i = obj.y, & j = obj.x;
	if (!stay) {
		gameObject& nxt = a[i + 1][j];
		if (nxt.type == WALL || nxt.type == PLAYER || nxt.type == ENEMY) return 0;
	}
	obj.direction = { 1, 0 };
	GotoBoard(j, i); cout << SPACE;
	a[i][j] = gameObject(j, i, 0, {0, 0});
	i += 1 ^ stay;
	PrintTank(j, i, obj.color, obj.direction);
	a[i][j] = obj;
	return 1;
}

bool MoveLeft(board& a, gameObject& obj) {
	bool stay = 0;
	if (obj.direction != make_pair(0, -1)) stay = 1;
	int& i = obj.y, & j = obj.x;
	if (!stay) {
		gameObject& nxt = a[i][j - 1];
		if (nxt.type == WALL || nxt.type == PLAYER || nxt.type == ENEMY) return 0;
	}
	obj.direction = { 0, -1 };
	GotoBoard(j, i); cout << SPACE;
	a[i][j] = gameObject(j, i, 0, { 0, 0 });
	j -= 1 ^ stay;;
	PrintTank(j, i, obj.color, obj.direction);
	a[i][j] = obj;
	return 1;
}

bool MoveRight(board& a, gameObject& obj) {
	bool stay = 0;
	if (obj.direction != make_pair(0, 1)) stay = 1;
	int& i = obj.y, & j = obj.x;
	if (!stay) {
		gameObject& nxt = a[i][j + 1];
		if (nxt.type == WALL || nxt.type == PLAYER || nxt.type == ENEMY) return 0;
	}
	obj.direction = { 0, 1 };
	GotoBoard(j, i); cout << SPACE;
	a[i][j] = gameObject(j, i, 0, { 0, 0 });
	j += 1 ^ stay;
	PrintTank(j, i, obj.color, obj.direction);
	a[i][j] = obj;
	return 1;
}
