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

void DrawBoxMini(int w, int h, int x, int y, int color) {
	int tmp = GetCurrentColor();
	TextColor(color);
	GotoXY(x, y);
	cout << TOP_LEFT;
	for (int i = 1; i < w - 1; i++)
		cout << H_LINE;
	cout << TOP_RIGHT;

	for (int i = 1; i < h - 1; i++) {
		GotoXY(x, i + y);
		cout << V_LINE;
		for (int j = 1; j < w - 1; j++)
			cout << SPACE;
		cout << V_LINE;
	}

	GotoXY(x, h + y - 1);
	cout << BOTTOM_LEFT;
	for (int i = 1; i < w - 1; i++)
		cout << H_LINE;
	cout << BOTTOM_RIGHT;
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
	else if(obj.damage == 2) {
		if (obj.direction == make_pair(1, 0) || obj.direction == make_pair(-1, 0)) {
			cout << char(249);
		}
		else {
			cout << char(249);
		}
	}
	else {
		if (obj.direction == make_pair(1, 0) || obj.direction == make_pair(-1, 0)) {
			cout << char(4);
		}
		else {
			cout << char(4);
		}
	}
	// The more damage, the shape of the bullet will change
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
	string button[] = { "     NEW GAME     ", "     CONTINUE     ", "    SCOREBOARD    ", "       HELP       ", "       EXIT       " };
	int tmpColor = GetCurrentColor();
	TextColor(RED & 15 | BACKGROUND_YELLOW);
	GotoXY(52, 14 + pos * 2);
	cout << button[pos];
	TextColor(tmpColor);
}

void UnhoverButton(int pos) {
	string button[] = { "     NEW GAME     ", "     CONTINUE     ", "    SCOREBOARD    ", "       HELP       ", "       EXIT       " };
	int tmpColor = GetCurrentColor();
	TextColor(BLACK);
	GotoXY(52, 14 + pos * 2);
	cout << button[pos];
	TextColor(tmpColor);
}

void ShowRank(gameSound& sound) {
	ifstream cin("data/ranking/rank.txt");
	int score, map;
	string name, date;
	vector <gameState> v;
	while (cin >> score) {
		cin >> map;
		cin.ignore();
		getline(cin, name);
		cin >> date;
		if (name == "") name = "<empty>";
		v.push_back(gameState(score, map, name, date));
	}
	cin.close();
	sort(v.begin(), v.end(), [](const gameState& a, const gameState& b) {
		if (a.score != b.score) return a.score > b.score;
		return a.map < b.map;
	});
	int n = v.size();
	SetConsoleBlank();
	GotoXY(47, 27);
	TextColor(BLACK);
	cout << " " << L_TRIANGLE << " PRESS ESC TO COMEBACK " << R_TRIANGLE << " ";
	DrawBox(70, 25, 26, 2, BLUE, 0);
	TextColor(CYAN);
	GotoXY(28, 3); cout << "RANK";
	GotoXY(41, 3); cout << "PLAYER";
	GotoXY(56, 3); cout << "SCORE";
	GotoXY(66, 3); cout << "MAP";
	GotoXY(81, 3); cout << "DATE";
	for (int i = 0; i < min(n, 10); i++) {
		if (i > 2) TextColor(BLACK);
		else if (i == 0) TextColor(RED);
		else if (i == 1) TextColor(YELLOW);
		else TextColor(GREEN);
		GotoXY(29, 5 + i * 2); cout << i + 1;
		if (v[i].player.size() > 16) 
			v[i].player = v[i].player.substr(0, 13) + "...";
		GotoXY(43 - v[i].player.size() / 2, 5 + i * 2); cout << v[i].player;
		GotoXY(56, 5 + i * 2); cout << v[i].score;
		GotoXY(67, 5 + i * 2); cout << v[i].map;
		GotoXY(74, 5 + i * 2); cout << v[i].date;
	}
	while (true) {
		char c = toupper(_getch());
		if (c == ESC) break;
	}
}

void Help() {
	SetConsoleBlank();
	DrawBox(98, 25, 11, 2, CYAN, 0);
	DrawBoxMini(94, 23, 13, 3, LIGHT_CYAN);
	TextColor(RED);
	GotoXY(20, 8);
	cout << L_TRIANGLE << " RULE " << R_TRIANGLE;
	GotoXY(40, 5);
	cout << "The game is played on a board with 26 rows and 60 columns." << endl;
	GotoXY(40, 7);
	cout << "To win the round, player needs to defeat all the enemies" << endl;
	GotoXY(40, 8);
	cout << "The score will be added by 1000 for each defeated ememies." << endl;
	GotoXY(40, 9);
	cout << "Getting hit by an enemy will decrease the HP by 1 and the" << endl;
	GotoXY(40, 10);
	cout << "score by 500. ";
	GotoXY(40, 12);
	cout << "Defeat all the enemies to advance to the next round.";

	TextColor(GREEN);
	GotoXY(20, 15);
	cout << L_TRIANGLE << " MOVE " << R_TRIANGLE;
	GotoXY(40, 15);
	cout << "Up: W";
	GotoXY(40, 16);
	cout << "Down: S";
	GotoXY(55, 15);
	cout << "Left: A";
	GotoXY(55, 16);
	cout << "Right: D";
	GotoXY(70, 15);
	cout << "Shoot: Enter";
	//GotoXY(70, 16);
	//cout << "Pause game: Esc";
	TextColor(MAGENTA);
	GotoXY(18, 21);
	cout << L_TRIANGLE << " ABOUT US " << R_TRIANGLE;
	GotoXY(78, 21);
	cout << "DSA's lab project";
	GotoXY(73, 22);
	cout << "22CTT4A - VNUHCM US - 1/2024";
	GotoXY(40, 20);
	cout << "Nguyen Duy Lam    -  22120181";
	GotoXY(40, 21);
	cout << "Dang Duy Lan      -  22120182";
	GotoXY(40, 22);
	cout << "Nguyen Nhat Long  -  22120194";
	GotoXY(40, 23);
	cout << "Tang Senh Manh    -  22120202";
	TextColor(LIGHT_RED);
	GotoXY(47, 26);
	cout << " " << L_TRIANGLE << " PRESS ESC TO COMEBACK " << R_TRIANGLE << " ";
	while (true) {
		char c = _getch();
		if (c == ESC)
			return;
	}
}

void HoverSave(const vector <gameState>& v, int pos, int page) {
	int tmp = GetCurrentColor();
	TextColor(RED & 15 | BACKGROUND_YELLOW);
	GotoXY(16, 5 + 2 * pos);
	string s = v[8 * page + pos].date;
	if (s == "") s = "<empty>";
	int n = 38 - s.size();
	n /= 2;
	for (int i = 0; i < n; i++) s = " " + s + " ";
	if (s.size() < 38) s = " " + s;
	cout << s;
	// --------------
	s = v[8 * page + pos].player;
	if (s.size() > 20) {
		while (s.size() > 17) s.pop_back();
		s += "...";
	}
	DrawBoxMini(30, 5, 75, 7, BLACK);
	if (!s.empty()) {
		TextColor(GREEN);
		GotoXY(76, 8);
		cout << "Player: " << s;
		GotoXY(76, 9);
		TextColor(RED);
		cout << "Score: " << v[8 * page + pos].score;
		TextColor(YELLOW);
		GotoXY(76, 10);
		cout << "Map: " << v[8 * page + pos].map;
	}
	TextColor(tmp);
}

void UnhoverSave(const vector <gameState>& v, int pos, int page) {
	int tmp = GetCurrentColor();
	TextColor(BLACK);
	GotoXY(16, 5 + 2 * pos);
	string s = v[8 * page + pos].date;
	if (s == "") s = "<empty>";
	int n = 38 - s.size();
	n /= 2;
	for (int i = 0; i < n; i++) s = " " + s + " ";
	if (s.size() < 38) s = " " + s;
	cout << s;
	TextColor(tmp);
}

void Continue(board& a, gameSound& sound, vector<int> &bonus_stats) {
	ifstream cin("data/save.txt");
	string fn;
	vector <gameState> v;
	while (cin >> fn) {
		ifstream cin("data/save/" + fn + ".txt");
		int map, score;
		string name;
		cin >> map >> score;
		cin.ignore();
		getline(cin, name);
		cin.close();
		string nfn = fn;
		for (char& c : nfn) if (c == '_') c = ':';
		v.push_back(gameState(score, map, name, nfn));
	}
	int total = 8, pos = 0, page = 0;
	while (v.size() % total != 0) v.push_back(gameState(0, 0, "", ""));
	int n = v.size();
	SetConsoleBlank();
	GotoXY(45, 26);
	TextColor(MAGENTA);
	cout << " " << L_TRIANGLE << " PRESS ESC TO COMEBACK " << R_TRIANGLE << " ";
	DrawBox(40, 17, 15, 4, BLUE, 0);
	for (int i = 1; i < total; i++) UnhoverSave(v, i, 0);
	HoverSave(v, 0, 0);
	string nPage = ToString(n / total);
	if (nPage.size() < 2) nPage = "0" + nPage;
	TextColor(CYAN);
	GotoXY(31, 21); cout << "<01 / " + nPage << ">";
	while (true) {
		char c = toupper(_getch());
		if (c == ENTER) {
			if (v[8 * page + pos].player == "") continue;
			StartGame(a, v[8 * page + pos].map, v[8 * page + pos].score, v[8 * page + pos].player, sound, bonus_stats);
			break;
		}
		if (c == myKeyS || c == myKeyW) {
			int nPos = pos;
			if (c == myKeyS) nPos = (nPos + 1) % total;
			else nPos = (nPos - 1 + total) % total;
			UnhoverSave(v, pos, page);
			HoverSave(v, nPos, page);
			pos = nPos;
		}
		else if (c == myKeyA || c == myKeyD) {
			if (c == myKeyA) page = (page - 1 + n / total) % (n / total);
			else page = (page + 1) % (n / total);
			GotoXY(32, 21);
			if (ToString(page + 1).size() < 2) cout << "0";
			cout << ToString(page + 1);
			pos = 0;
			for (int i = 1; i < total; i++) UnhoverSave(v, i, page);
			HoverSave(v, 0, page);
		}
		else if (c == ESC) break;
	}
	cin.close();
}

void MainMenu(board& a, gameSound& sound) {
	int tmpColor = GetCurrentColor();
	SetConsoleBlank();
	vector<int> bonus_stats{ 0,0,0,0 };
	ifstream cin("assets/logo.txt");
	SetConsoleOutputCP(65001);
	TextColor(BLUE);
	int score = 0;
	string player = "";
	GotoXY(0, 1);
	while (cin.good()) {
		string s;
		getline(cin, s);
		cout << s << endl;
	}
	SetConsoleOutputCP(437);
	cin.close();

	DrawBox(20, 11, 51, 13, BLACK, 0);
	HoverButton(0);
	for (int i = 1; i < 5; i++) UnhoverButton(i);
	int pos = 0;
	while (true) {
		unsigned char c = toupper(_getch());
		sound.play("click");
		if (c == ENTER) {
			switch (pos)
			{
			case 0:
				if (AskPlayerName(player))
					StartGame(a, 1, score, player, sound, bonus_stats);
				return;
			case 1:
				Continue(a, sound, bonus_stats);
				return;
			case 2:
				ShowRank(sound);
				return;
			case 3:
				Help();
				return;
			case 4:
				GotoXY(0, 24);
				exit(0);
			}
		}
		else if (c == myKeyW || c == myKeyS) {
			int oldPos = pos;
			if (c == myKeyS) pos = (pos + 1) % 5;
			else pos = (pos - 1 + 5) % 5;
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

void AskSave(int map, int score, string player) {
	SetConsoleBlank();
	DrawBox(30, 7, 47, 10, CYAN, 0);
	TextColor(CYAN);
	GotoXY(52, 11); cout << "Do you want to save?";
	DrawBox(7, 3, 53, 13, RED, 0);
	TextColor(RED);
	GotoXY(55, 14); cout << "YES";
	DrawBox(7, 3, 63, 13, BLACK, 0);
	TextColor(BLACK);
	GotoXY(65, 14); cout << "NO";
	int cnt = 0;
	while (true) {
		char c = toupper(_getch());
		if (c == myKeyA || c == myKeyD) {
			cnt ^= 1;
			if (cnt) {
				DrawBox(7, 3, 53, 13, BLACK, 0);
				TextColor(BLACK);
				GotoXY(55, 14); cout << "YES";
				DrawBox(7, 3, 63, 13, RED, 0);
				TextColor(RED);
				GotoXY(65, 14); cout << "NO";
			}
			else {
				DrawBox(7, 3, 53, 13, RED, 0);
				TextColor(RED);
				GotoXY(55, 14); cout << "YES";
				DrawBox(7, 3, 63, 13, BLACK, 0);
				TextColor(BLACK);
				GotoXY(65, 14); cout << "NO";
			}
		}
		else if (c == ENTER) {
			if (cnt) return;
			string fn = CurrentDateTime();
			for (char& c : fn) if (c == ':') c = '_';
			ofstream cout("data/save/" + fn + ".txt");
			cout << map << ' ' << score << endl << player << endl << fn;
			cout.close();
			cout.open("data/save.txt", ios::app);
			cout << fn << endl;
			return;
		}
	}
}

bool AskPlayerName(string& player) {
	SetConsoleBlank();
	TextColor(BLACK);
	GotoXY(52, 11); cout << "What is your name?";
	DrawBox(30, 3, 47, 12, CYAN, 0);
	GotoXY(48, 13); cout << " > ";
	return EnterText(player, 25);
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

void Congratulation() {
	SetConsoleBlank();
	int color[] = { RED, BLUE, YELLOW, GREEN, BLACK, CYAN }, n = 6, cnt = 0;
	while (true) {
		ifstream cin("assets/congrat.txt");
		if (_kbhit()) {
			char c = _getch();
			break;
		}
		SetConsoleOutputCP(65001);
		TextColor(color[cnt++]);
		cnt %= n;
		GotoXY(0, 8);
		while (cin.good()) {
			string s;
			getline(cin, s);
			cout << s << endl;
		}
		SetConsoleOutputCP(437);
		cin.close();
		Sleep(100);
	}
}

void BackGround(int x, int y, int color) {
	SetConsoleBlank();
	ifstream cin("assets/background.txt");
	SetConsoleOutputCP(65001);
	TextColor(color);
	GotoXY(x, y);
	while (cin.good()) {
		string s;
		getline(cin, s);
		cout << s << endl;
	}
	SetConsoleOutputCP(437);
	cin.close();
}

void Loading() {
	DrawBox(40, 3, 40, 26, BLACK, 0);
	TextColor(BLUE);
	GotoXY(41, 27);
	for (int i = 0; i < 38; i++) {
		cout << char(219);
		Sleep(15);
	}
}

void AskUpgrade(int& score, string player, vector<int> &bonus_stats) {
	// Can upgrade HP, damage, shooting speed, moving speed
	int tmpColor = GetCurrentColor();
	SetConsoleBlank();
	GotoXY(0, 1);
	TextColor(YELLOW);
	cout << "SCORE: " << score;
	vector<int> cost{ 5000, 5000, 3000, 3000 };
	DrawBox(30, 9, 45, 13, BLACK, 0);
	HoverUpgradeButton(0);
	for (int i = 1; i < 4; i++) UnhoverUpgradeButton(i);
	int pos = 0;
	UpgradeCost(cost);
	TextColor(RED);
	GotoXY(45, 10);
	cout << "CHOOSE AN UPGRADE FOR YOUR TANK";
	while (true) {
		unsigned char c = toupper(_getch());
		if (c == ENTER) {
			switch (pos)
			{
			case 0: // HP
				if (score - cost[0] < 0) {
					GotoXY(43, 23);
					cout << "NOT ENOUGH SCORE TO BUY THE UPGRADE";
					continue;
				}
				score -= cost[0];
				bonus_stats[0] += 1;
				GotoXY(61, 14); cout << "UPGRADED!";
				Sleep(2000);
				return;
			case 1: // DAMAGE
				if (score - cost[1] < 0) {
					GotoXY(43, 23);
					cout << "NOT ENOUGH SCORE TO BUY THE UPGRADE";
					continue;
				}
				bonus_stats[1] += 1;
				score -= cost[1];
				GotoXY(61, 14+2); cout << "UPGRADED!";
				Sleep(2000);
				return;
			case 2: //MOVING SPEED
				if (score - cost[2] < 0) {
					GotoXY(43, 23);
					cout << "NOT ENOUGH SCORE TO BUY THE UPGRADE";
					continue;
				}
				bonus_stats[2] += 20;
				score -= cost[2];
				GotoXY(61, 14+4); cout << "UPGRADED!";
				Sleep(2000);
				return;
			case 3: //ATTACK SPEED
				if (score - cost[3] < 0) {
					GotoXY(43, 23);
					cout << "NOT ENOUGH SCORE TO BUY THE UPGRADE";
					continue;
				}
				bonus_stats[3] += 30;
				score -= cost[3];
				GotoXY(61, 14+6); cout << "UPGRADED!";
				Sleep(2000);
				return;
			}
		}
		else if (c == myKeyW || c == myKeyS) {
			int oldPos = pos;
			if (c == myKeyS) pos = (pos + 1) % 4;
			else pos = (pos - 1 + 4) % 4;
			UnhoverUpgradeButton(oldPos);
			HoverUpgradeButton(pos);
		}

	}
	TextColor(tmpColor);
}

void UpgradeCost(vector<int> cost) {
	for (int i = 0; i < 4; i++) {
		GotoXY(63, 14+i*2);
		cout << cost[i] << " SCORE";
	}
}

void HoverUpgradeButton(int pos) {
	string button[] = { "HP           +1", "DAMAGE       +1", "MOVING SPEED +20", "ATTACK SPEED +30"};
	int tmpColor = GetCurrentColor();
	TextColor(RED & 15 | BACKGROUND_YELLOW);
	GotoXY(46, 14 + pos * 2);
	cout << button[pos];
	TextColor(tmpColor);
}

void UnhoverUpgradeButton(int pos) {
	string button[] = { "HP              ", "DAMAGE          ", "MOVING SPEED    ", "ATTACK SPEED    " };
	int tmpColor = GetCurrentColor();
	TextColor(BLACK);
	GotoXY(46, 14 + pos * 2);
	cout << button[pos];
	TextColor(tmpColor);
}

