#pragma once
void GotoXY(int column, int line);
int WhereX();
int WhereY();
void MoveUp(board& a, gameObject& obj);
void MoveDown(board& a, gameObject& obj);
void MoveLeft(board& a, gameObject& obj);
void MoveRight(board& a, gameObject& obj);
void GotoBoard(int x, int y);