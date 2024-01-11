#include "lib/data.h"
#include "lib/view.h"
#include "lib/control.h"
#include "lib/model.h"

void FixConsoleWindow() {
	HWND consoleWindow = GetConsoleWindow();
	LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
	style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
	SetWindowLong(consoleWindow, GWL_STYLE, style);
}

void TextColor(int x) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), x);
}

int GetCurrentColor() {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	GetConsoleScreenBufferInfo(console, &consoleInfo);
	WORD textAttributes = consoleInfo.wAttributes;
	return textAttributes;
}

void SetConsoleBlank() {
	int h = HEIGHT, w = WIDTH;
	TextColor(WHITE);
	for (int i = 0; i < h; i++) {
		GotoXY(0, i);
		for (int j = 0; j < w; j++)
			cout << SPACE;
	}
	GotoXY(0, 0);
}

void HideCursor(bool ok) {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = !ok;
	SetConsoleCursorInfo(consoleHandle, &info);
}

void SetConsoleSize(int w, int h) {
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r); //stores the console's current dimensions
	MoveWindow(console, r.left, r.top, w, h, TRUE); // 800 width, 100 height
}

void FontSize(int x) {
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;
	cfi.dwFontSize.Y = x;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_BOLD;
	wcscpy_s(cfi.FaceName, L"Consolas");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

void DrawBoard(int c, int r, int x, int y, int color) {
	int tmp = GetCurrentColor();
	GotoXY(x, y);
	TextColor(color);

	// top row
	cout << TOP_LEFT << H_LINE << H_LINE << H_LINE;
	for (int i = 1; i < c; i++) {
		cout << TOP_CROSS << H_LINE << H_LINE << H_LINE;
	}
	cout << TOP_RIGHT;
	GotoXY(x, y + 1);
	for (int i = 0; i <= c; i++) {
		cout << V_LINE << SPACE << SPACE << SPACE;
	}

	// all the middle row
	for (int i = 1; i < r; i++) {
		GotoXY(x, y + i * 2);
		cout << LEFT_CROSS << H_LINE << H_LINE << H_LINE;
		for (int j = 1; j < c; j++)
			cout << CROSS << H_LINE << H_LINE << H_LINE;
		cout << RIGHT_CROSS;
		GotoXY(x, y + i * 2 + 1);
		for (int j = 0; j <= c; j++) {
			cout << V_LINE << SPACE << SPACE << SPACE;
		}
	}

	// bottom row
	GotoXY(x, y + 2 + 2 * (r - 1));
	cout << BOTTOM_LEFT << H_LINE << H_LINE << H_LINE;
	for (int i = 1; i < c; i++) {
		cout << BOTTOM_CROSS << H_LINE << H_LINE << H_LINE;
	}
	cout << BOTTOM_RIGHT;
	TextColor(tmp);
}

void DrawBox(int w, int h, int x, int y, int color, int Time) {
	int tmp = GetCurrentColor();
	TextColor(color);
	for (int i = 0; i < w / 2; i++) {
		GotoXY(x + w / 2 - i, y);
		cout << BOX_H_LINE;
		GotoXY(x + w / 2 + i, y);
		cout << BOX_H_LINE;
		Sleep(Time);
	}
	GotoXY(x, y);
	cout << BOX_TOP_LEFT;
	GotoXY(x + w - 1, y);
	cout << BOX_TOP_RIGHT;
	for (int i = 1; i < h - 1; i++) {
		GotoXY(x, y + i);
		cout << BOX_V_LINE;
		for (int j = 1; j < w - 1; j++)
			cout << SPACE;
		cout << BOX_V_LINE;
		Sleep(Time);
	}
	GotoXY(x, y + h - 1);
	cout << BOX_BOTTOM_LEFT;
	GotoXY(x + w - 1, y + h - 1);
	cout << BOX_BOTTOM_RIGHT;
	for (int i = w / 2 - 1; i >= 0; i--) {
		GotoXY(x + w / 2 - i, y + h - 1);
		cout << BOX_H_LINE;
		GotoXY(x + w / 2 + i - (w % 2 == 0), y + h - 1);
		cout << BOX_H_LINE;
		Sleep(Time);
	}
	TextColor(tmp);
}

void DrawWall(int w, int h, int x, int y, int color) {
	int tmp = GetCurrentColor();
	TextColor(color);
	for (int i = 0; i < w / 2; i++) {
		GotoXY(x + w / 2 - i, y);
		cout << BOX_TOP;
		GotoXY(x + w / 2 + i, y);
		cout << BOX_TOP;
	}
	GotoXY(x, y);
	cout << BOX_TOP_LEFT;
	GotoXY(x + w - 1, y);
	cout << BOX_TOP_RIGHT;
	for (int i = 1; i < h - 1; i++) {
		GotoXY(x, y + i);
		cout << BOX_LEFT;
		for (int j = 1; j < w - 1; j++)
			cout << BOX_CROSS;
		cout << BOX_RIGHT;
	}
	GotoXY(x, y + h - 1);
	cout << BOX_BOTTOM_LEFT;
	GotoXY(x + w - 1, y + h - 1);
	cout << BOX_BOTTOM_RIGHT;
	for (int i = w / 2 - 1; i >= 0; i--) {
		GotoXY(x + w / 2 - i, y + h - 1);
		cout << BOX_BOTTOM;
		GotoXY(x + w / 2 + i - (w % 2 == 0), y + h - 1);
		cout << BOX_BOTTOM;
	}
	TextColor(tmp);
}

void PrintTank(int x, int y, int color, pair <int, int> direction) {
	int tmp = GetCurrentColor();
	GotoBoard(x, y);
	TextColor(color);
	cout << Tank(direction);
	TextColor(tmp);
}

void PrintDefeatedTank(int x, int y, int color) {
	int tmp = GetCurrentColor();
	GotoBoard(x, y);
	TextColor(color);
	cout << char(177);
	TextColor(tmp);
}

void PrintBullet(int j, int i, gameObject& obj) {
	GotoBoard(j, i);
	int tmp = GetCurrentColor();
	TextColor(obj.color);
	if (obj.damage == 1) {
		if (obj.direction == make_pair(1, 0) || obj.direction == make_pair(-1, 0)) {
			cout << V_LINE;
		}
		else {
			cout << H_LINE;
		}
	}
	TextColor(tmp);
}

void DisplayPlayerHP(gameObject& player) {
	int tmp = GetCurrentColor();
	TextColor(GREEN);
	GotoXY(4, 4); cout << "HP: ";
	for (int i = 0; i < 5; i++) cout << "    ";
	GotoXY(8, 4);
	for (int i = 0; i < player.hp; i++) cout << HP_BAR;
	TextColor(tmp);
}

int GameOver() {
	for (int i = 0; i < 3; i++) {
		GotoBoard(25, 14);
		cout << "GAME OVER.";
		for (int j = 0; j < i; j++) cout << ".";
		Sleep(700);
	}
	while (_kbhit()) _getch();
	_getch();
	return GAME_OVER;
}

void HoverButton(int pos) {
	string button[] = { "     NEW GAME     ", "     CONTINUE     ", "    SCOREBOARD    ", "       HELP       " };
	int tmpColor = GetCurrentColor();
	TextColor(RED & 15 | BACKGROUND_YELLOW);
	GotoXY(52, 14 + pos * 2);
	cout << button[pos];
	TextColor(tmpColor);
}

void UnhoverButton(int pos) {
	string button[] = { "     NEW GAME     ", "     CONTINUE     ", "    SCOREBOARD    ", "       HELP       " };
	int tmpColor = GetCurrentColor();
	TextColor(BLACK);
	GotoXY(52, 14 + pos * 2);
	cout << button[pos];
	TextColor(tmpColor);
}

void MainMenu(board& a) {
	int tmpColor = GetCurrentColor();
	SetConsoleBlank();
	ifstream cin("assets/logo.txt");
	SetConsoleOutputCP(65001);
	TextColor(BLUE);
	int score = 0;
	GotoXY(0, 1);
	while (cin.good()) {
		string s;
		getline(cin, s);
		cout << s << endl;
	}
	SetConsoleOutputCP(437);
	cin.close();

	DrawBox(20, 9, 51, 13, BLACK, 0);
	HoverButton(0);
	for (int i = 1; i < 4; i++) UnhoverButton(i);
	int pos = 0;
	while (true) {
		unsigned char c = toupper(_getch());
		if (c == ENTER) {
			switch (pos)
			{
			case 0:
				StartGame(a, 1, score);
				return;
			case 1:

				break;
			case 2:

				break;
			case 3:

				break;
			}
		}
		else if (c == W || c == S) {
			int oldPos = pos;
			if (c == S) pos = (pos + 1) % 4;
			else pos = (pos - 1 + 4) % 4;
			UnhoverButton(oldPos);
			HoverButton(pos);
		}

	}
	TextColor(tmpColor);
}

void DisplayScore(int score) {
	int tmp = GetCurrentColor();
	TextColor(YELLOW);
	GotoXY(1, 5); cout << "Score: ";
	for (int i = 0; i < 5; i++) cout << "    ";
	GotoXY(8, 5);
	cout << score;
	TextColor(tmp);
}

int Finish() {
	for (int i = 0; i < 3; i++) {
		GotoBoard(25, 14);
		cout << "ROUND CLEAR.";
		for (int j = 0; j < i; j++) cout << ".";
		Sleep(700);
	}
	while (_kbhit()) _getch();
	GotoBoard(20, 15);
	cout << "Press ENTER to continue";
	while (true) {
		char c = toupper(_getch());
		if (c == ENTER) return FINISH;
	}
}