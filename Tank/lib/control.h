#pragma once
void GotoXY(int column, int line);
int WhereX();
int WhereY();
bool MoveUp(board& a, gameObject& obj);
bool MoveDown(board& a, gameObject& obj);
bool MoveLeft(board& a, gameObject& obj);
bool MoveRight(board& a, gameObject& obj);
void GotoBoard(int x, int y);