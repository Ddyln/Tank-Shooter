#pragma once
void HideCursor(bool ok);
void FixConsoleWindow();
void TextColor(int x);
int GetCurrentColor();
void SetConsoleBlank();
void HideCursor(bool ok);
void SetConsoleSize(int w, int h);
void FontSize(int x);
void DrawBoard(int r, int c, int x, int y, int color);
void DrawBox(int w, int h, int x, int y, int color, int Time);
void DrawWall(int w, int h, int x, int y, int color);
void PrintTank(int x, int y, int color, pair <int, int> direction);
void PrintDefeatedTank(int x, int y, int color = RED);
void PrintBullet(int j, int i, gameObject& obj);
void DisplayPlayerHP(gameObject& player);
int GameOver();
void MainMenu(board& a, gameSound& sound);
void DisplayScore(int score);
int Finish();
void AskSave(int map, int score, string player);
void Continue(board& a, gameSound& sound);
void ShowRank(gameSound& sound);
bool AskPlayerName(string& player);
void Congratulation();
void BackGround(int x, int y, int color);
void Loading();