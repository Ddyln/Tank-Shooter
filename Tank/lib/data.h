#pragma once
#define _CRT_SECURE_NO_WARNINGS
// Included libraries
#include <iostream>
#include <cctype>
#include <Windows.h>
#include <conio.h>
#include <iomanip>
#include <fstream>
#include <vector>
#include <math.h>
#include <string>
#include <mmsystem.h>
#include <windows.h>
#include <queue>
#include <sstream>
#include <list>
#include "SFML/Audio.hpp"
// Namespace
using namespace std;

// Console size
#define WIDTH 120
#define HEIGHT 29
#define BOARD_WIDTH 60
#define BOARD_HEIGHT 26
#define BOARD_X 5
#define BOARD_Y 2

// Color code
#define BLACK (15 << 4)
#define BLUE ((15 << 4) | 1)
#define GREEN ((15 << 4) | 2)
#define CYAN ((15 << 4) | 3)
#define RED ((15 << 4) | 4)
#define MAGENTA ((15 << 4) | 5)
#define YELLOW ((15 << 4) | 6)
#define WHITE ((15 << 4) | 7)
#define GRAY ((15 << 4) | 8)
#define LIGHT_BLUE ((15 << 4) | 9)
#define LIGHT_GREEN ((15 << 4) | 10)
#define LIGHT_CYAN ((15 << 4) | 11)
#define LIGHT_RED ((15 << 4) | 12)
#define LIGHT_MAGENTA ((15 << 4) | 13)
#define LIGHT_YELLOW ((15 << 4) | 14)
#define BRIGHT_WHITE ((15 << 4) | 15)
#define BACKGROUND_YELLOW (14 << 4)
#define BACKGROUND_RED (12 << 4)

// ASCII code
// special key
#define ENTER 13
#define ESC 27
#define BACK_SPACE 8

#define H_LINE (char)196
#define V_LINE (char)179
#define CROSS (char)197
#define TOP_LEFT (char)218
#define TOP_RIGHT (char)191
#define BOTTOM_LEFT (char)192
#define BOTTOM_RIGHT (char)217
#define TOP_CROSS (char)194
#define BOTTOM_CROSS (char)193
#define	LEFT_CROSS (char)195
#define RIGHT_CROSS (char)180
#define L_TRIANGLE ((char)16)
#define R_TRIANGLE ((char)17)
#define BOX_TOP_LEFT (char)201
#define BOX_TOP_RIGHT (char)187
#define BOX_BOTTOM_LEFT (char)200
#define BOX_BOTTOM_RIGHT (char)188
#define BOX_CROSS (char)206
#define BOX_TOP (char)203
#define BOX_BOTTOM (char)202
#define BOX_RIGHT (char)185
#define BOX_LEFT (char)204
#define BOX_V_LINE (char)186
#define BOX_H_LINE (char)205
#define BOX_X  WIDTH / 2 - 39
#define BOX_Y HEIGHT / 2 - 13
#define BOX_W 40
#define BOX_H 20
#define TANK_UP (char)207
#define TANK_DOWN (char)209
#define TANK_LEFT (char)182
#define TANK_RIGHT (char)199
#define HIT 1
#define BULLET_COLLISION '*'
// moving keys
#define myKeyW 87
#define myKeyA 65
#define myKeyS 83
#define myKeyD 68
#define TAB 9
#define GAME_OVER 1
#define FINISH 2
const char HP_BAR[] = { (char)219, (char)219, (char)219, (char)219};

// others
#define fi first
#define se second
#define SPACE (char)32


// Data types
enum typeOfObject {
	GROUND,
	WALL,
	PLAYER,
	BULLET,
	ENEMY
};

struct gameObject {
	int x, y, hp, T, damage, color, flag;
	typeOfObject type;
	pair <int, int> direction;
	struct moving {
		int T, remain;
		moving() {
			T = remain = 0;
		}
		moving(int _T, int _remain) {
			T = _T;
			remain = _remain;
		}
	};
	struct shooting {
		int T, remain;
		shooting() {
			T = remain = 0;
		}
		shooting(int _T, int _remain) {
			T = _T;
			remain = _remain;
		}
	};
	shooting shot;
	moving move;
	struct hitEffect {
		int T, remain;
		hitEffect() {
			T = remain = 0;
		}
		hitEffect(int _T, int _remain) {
			T = _T;
			remain = _remain;
		}
	};
	hitEffect he;
	gameObject() {
		x = y = hp = T = damage = flag = 0;
		type = GROUND;
		color = BLACK;
		direction = { 0, 0 };
	}
	gameObject(int _x, int _y, int _hp, pair <int, int> _direction, typeOfObject _type = GROUND) {
		flag = 0;
		x = _x;
		y = _y;
		hp = _hp;
		type = _type;
		color = BLACK;
		direction = _direction;
		T = -1;
		damage = 0;
		if (type == BULLET)
			T = 25;
		else if (type == ENEMY)
			T = 80;
	}
	gameObject(int _x, int _y, int _hp, pair <int, int> _direction, int _damage, int _color, typeOfObject _type = GROUND) {
		flag = 0;
		x = _x ;
		y = _y;
		hp = _hp;
		type = _type;
		color = _color;
		direction = _direction;
		T = -1;
		damage = _damage;
		if (type == BULLET)
			T = 25;
		else if (type == ENEMY) 
			T = 80;
	}
};

struct board {
	gameObject a[28][62];
	board() {
		for (int i = 0; i < 28; i++)
			for (int j = 0; j < 62; j++) {
				a[i][j] = gameObject(j, i, 0, { 0, 0 }, 0, GROUND);
			}
	}
	gameObject*  operator[] (int i) {
		return a[i];
	}
};

struct effectQueueElement {
	int T, x, y, remain, color;
	char c;
	effectQueueElement() {
		T = x = y = remain = color = 0;
		c = '@';
	}
	effectQueueElement(int _T, int _x, int _y, int _remain, int _color, char _c) {
		T = _T;
		x = _x;
		y = _y;
		remain = _remain;
		color = _color;
		c = _c;
	}
};

struct enemyDestInfo {
	int dx, dy, length, T, remain;
	enemyDestInfo() {
		dx = dy = T = length = remain = 0;
	}
	enemyDestInfo(int _x, int _y, int _length, int _T, int _remain) {
		dx = _x;
		dy = _y;
		length = _length;
		T = _T;
		remain = _remain;
	}
};

struct gameState {
	int score, map;
	string player, date;
	gameState() {
		score = map = 0;
		player = date = "";
	}
	gameState(int _score, int _map, string _player, string _date) {
		score = _score;
		map = _map;
		player = _player;
		date = _date;
	}
};

struct gameSound {
	int cnt;
	sf::SoundBuffer buffer[256];
	sf::Sound snd[256];
	void add(int& cnt, int val) {
		(cnt += val) %= 256;
	}
	gameSound() {
		cnt = 0;
	}
	void play(const string& fn) {
		buffer[cnt].loadFromFile("assets/sound/" + fn + ".wav");
		snd[cnt].setVolume(100.f);
		snd[cnt].setBuffer(buffer[cnt]);
		snd[cnt].play();
		add(cnt, 1);
	}
};