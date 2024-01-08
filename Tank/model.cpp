#include "lib/data.h"
#include "lib/view.h"
#include "lib/control.h"
#include "lib/model.h"

char Tank(pair <int, int> direction) {
	if (direction == make_pair(1, 0)) return TANK_DOWN;
	if (direction == make_pair(-1, 0)) return TANK_UP;
	if (direction == make_pair(0, 1)) return TANK_RIGHT;
	if (direction == make_pair(0, -1)) return TANK_LEFT;
	return '@';
}

char Wall(board &a, int i, int j) {
	int dir = 0;
	if (a[i - 1][j].type == 1) dir ^= (1 << 0);
	if (a[i][j + 1].type == 1) dir ^= (1 << 1);
	if (a[i + 1][j].type == 1) dir ^= (1 << 2);
	if (a[i][j - 1].type == 1) dir ^= (1 << 3);
	
	if (!dir) return SPACE;
	else if (dir == 0b0011) return BOX_BOTTOM_LEFT;
	else if (dir == 0b0101) return BOX_V_LINE;
	else if (dir == 0b1001) return BOX_BOTTOM_RIGHT;
	else if (dir == 0b1010) return BOX_H_LINE;
	else if (dir == 0b0110) return BOX_TOP_LEFT;
	else if (dir == 0b1100) return BOX_TOP_RIGHT;
	else if (dir == 0b0111) return BOX_LEFT;
	else if (dir == 0b1011) return BOX_BOTTOM;
	else if (dir == 0b1101) return BOX_RIGHT;
	else if (dir == 0b1110) return BOX_TOP;
	else return BOX_CROSS;
}

gameObject LoadMap(board& a, list <gameObject>& enemy, list <enemyDestInfo>& enemyDestination) {
	ifstream cin("assets/map01.txt");
	int m = 26, n = 60;
	int x0 = 30, y0 = 2;

	for (int i = 1; i <= m; i++)
		for (int j = 1; j <= n; j++) {
			int x; cin >> x;
			a[i][j].type = (x ? WALL : GROUND);
		}
	enemy.clear();
	int x, y, hp, damage; 
	pair <int, int> dir;
	gameObject player;
	cin >> x >> y >> hp >> dir.first >> dir.second >> damage;
	player = gameObject(x, y, hp, dir, damage, GREEN, PLAYER);
	int numOfEnemies = 0;
	cin >> numOfEnemies;
	for (int i = 0; i < numOfEnemies; i++) {
		cin >> x >> y >> hp >> dir.first >> dir.second >> damage;
		enemy.push_back(gameObject(x, y, hp, dir, damage, RED, ENEMY));
		enemyDestination.push_back(enemyDestInfo(-1, -1, -1, -1, -1));
	}
	cin.close();
	TextColor(BLACK);
	for (int i = 1; i <= m; i++) {
		for (int j = 1; j <= n; j++) {
			if (!a[i][j].type) continue;
			GotoXY(x0 + j - 1, y0 + i - 1);
			cout << Wall(a, i, j);
		}
	}
	PrintTank(player.x, player.y, GREEN, player.direction);
	for (const auto& tank : enemy) {
		PrintTank(tank.x, tank.y, tank.color, tank.direction);
		a[tank.y][tank.x] = tank;
		GotoXY(0, 0);
	}
	return player;
}

void PlayerBulletCollision(board& a, list <gameObject>& enemy, list <pair <gameObject, int>>& playerBullet, gameObject& player, queue <effectQueueElement>& effectQueue) {
	auto it = playerBullet.begin();
	while (it != playerBullet.end()) {
		int& i = it->fi.y, & j = it->fi.x;
		if (a[i][j].type == WALL) {
			list <gameObject>::iterator ptr;
			effectQueue.push({ clock(), j, i, 150, BLACK, Wall(a, i, j), ptr});
			int tmp = GetCurrentColor();
			TextColor(RED & 15 | BACKGROUND_YELLOW);
			GotoBoard(j, i);
			cout << Wall(a, i, j);
			TextColor(tmp);
			it = playerBullet.erase(it);
			continue;
		}
		else if (a[i][j].type == ENEMY) {
			// play sound
			// ...
			int& hp = a[i][j].hp;
			hp -= player.damage;
			list <gameObject>::iterator tmp;
			for (auto it = enemy.begin(); it != enemy.end(); it++)
				if (it->x == j && it->y == i)
					tmp = it;

			if (hp < 1) {
				PrintDefeatedTank(j, i, RED & 15 | BACKGROUND_YELLOW);
				a[i][j] = gameObject(j, i, 0, { 0, 0 });
				effectQueue.push({clock(), j, i, 150, BLACK, SPACE, tmp});
			}
			else {
				PrintTank(j, i, RED & 15 | BACKGROUND_YELLOW, a[i][j].direction);
				// IMPORTANT
				effectQueue.push({clock(), j, i, 150, RED, Tank(a[i][j].direction), tmp});
			}
			it = playerBullet.erase(it);
			continue;
		}
		else if (a[i][j].type == GROUND) {
			GotoBoard(j, i);
			int tmp = GetCurrentColor();
			TextColor(GREEN);
			if (player.damage == 1) {
				if (it->fi.direction == make_pair(1, 0) || it->fi.direction == make_pair(-1, 0)) {
					cout << V_LINE;
				}
				else {
					cout << H_LINE;
				}
			}
			TextColor(tmp);
		}
		it++;
	}
}

void MoveBullet(board& a, list <pair <gameObject, int>>& playerBullet) {
	auto it = playerBullet.begin();
	while (it != playerBullet.end()) {
		int& i = it->fi.y, & j = it->fi.x;
		if (clock() - it->se > ((it->fi.direction.fi == 0) ? it->fi.T : it->fi.T * 2 + 13)) {
			if (a[i][j].type == GROUND) {
				GotoBoard(j, i);
				cout << SPACE;
			}
			it->se = clock();
			i += it->fi.direction.fi;
			j += it->fi.direction.se;
		}
		it++;
	}
}

void CheckEffect(queue <effectQueueElement>& effectQueue) {
	int currentTime = clock();
	while (effectQueue.size() && currentTime - effectQueue.front().T > effectQueue.front().remain) {
		GotoBoard(effectQueue.front().x, effectQueue.front().y);
		int tmp = GetCurrentColor();
		TextColor(effectQueue.front().color);
		cout << effectQueue.front().c;
		TextColor(tmp);
		effectQueue.pop();
	}
}

void EnemyMove(list <gameObject>& enemy, list <enemyDestInfo>& enemyDestination) {
	auto e = enemy.begin();
	auto ed = enemyDestination.begin();
	while (e != enemy.end()) {
		if (ed->length < 1) {
			ed->length = rand() % 10 + 1;
			if (rand() % 2) {
				// left or right
				ed->dx = (rand() % 2 ? 1 : -1);
				ed->dy = 0;
			}
			else {
				// up or down
				ed->dy = (rand() % 2 ? 1 : -1);
				ed->dx = 0;
			}
		}

		ed->length--;
		e++, ed++;
	}
}

void StartGame(board& a) {
	list <gameObject> enemy;
	list <enemyDestInfo> enemyDestination;
	gameObject player = LoadMap(a, enemy, enemyDestination);
	list <pair <gameObject, int>> playerBullet;
	queue <effectQueueElement> effectQueue;
	int lastShot = 0, lastMove = 0, timeBetweenShot = 200, timeBetweenMove = 130;
	while (true) {
		PlayerBulletCollision(a, enemy, playerBullet, player, effectQueue);
		CheckEffect(effectQueue);
		
		//EnemyMove();
		PlayerBulletCollision(a, enemy, playerBullet, player, effectQueue);
		if (_kbhit()) {
			unsigned char c = toupper(_getch());
			if (c == ENTER) {
				if (clock() - lastShot > timeBetweenShot) {
					playerBullet.push_front({ gameObject(player.x + player.direction.se, player.y + player.direction.fi, -1, player.direction, BULLET), clock() });
					lastShot = clock();
				}
			}
			else if (c == W || c == A || c == S || c == D) {
				if (clock() - lastMove < timeBetweenMove) continue;
				if (c == W) MoveUp(a, player);
				else if (c == A) MoveLeft(a, player);
				else if (c == S) MoveDown(a, player);
				else if (c == D) MoveRight(a, player);
				lastMove = clock();
			}
		}
		else {
			MoveBullet(a, playerBullet);
		}
	}
}
