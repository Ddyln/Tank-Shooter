#include "lib/data.h"
#include "lib/view.h"
#include "lib/control.h"
#include "lib/model.h"

string CurrentDateTime() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
	return buf;
}

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

bool EnterText(string& s, int len) {
	unsigned char c;
	HideCursor(0);
	while (true) {
		c = _getch();
		if (c == ENTER) {
			break;
		}
		else if (c == ESC) {
			int n = min(s.size(), len);
			GotoXY(WhereX() - n, WhereY());
			for (int i = 0; i < n; i++)
				cout << ' ';
			s = "";
			HideCursor(1);
			return 0;
		}
		else if (c == BACK_SPACE) {
			if (s.size()) {
				s.pop_back();
				if (s.size() + 1 < len) {
					GotoXY(WhereX() - 1, WhereY());
					cout << ' ';
					GotoXY(WhereX() - 1, WhereY());
				}
				else {
					GotoXY(WhereX() - len + 1, WhereY());
					int n = s.size();
					cout << s.substr(n - len + 1);
				}
			}
		}
		else if (c == 224) {
			c = _getch();
		}
		else if (c != TAB) {
			s += c;
			if (s.size() < len)
				cout << c;
			else {
				GotoXY(WhereX() - len + 1, WhereY());
				int n = s.size();
				cout << s.substr(n - len + 1);
			}
		}
	}
	HideCursor(1);
	return 1;
}

string ToString(int n) {
	if (n == 0) return "0";
	string res = "";
	while (n) {
		res += n % 10 + '0';
		n /= 10;
	}
	reverse(res.begin(), res.end());
	return res;
}

gameObject LoadMap(board& a, list <gameObject>& enemy, list <enemyDestInfo>& enemyDestination, int map, int score, vector<int> bonus_stats) {
	TextColor(BLACK);
	GotoXY(9, 2); cout << "Level " << map;
	ifstream cin("assets/map0" + ToString(map) + ".txt");
	int m = BOARD_HEIGHT, n = BOARD_WIDTH;

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
	hp += bonus_stats[0];
	damage += bonus_stats[1];
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
			GotoBoard(j, i);
			cout << Wall(a, i, j);
		}
	}
	PrintTank(player.x, player.y, GREEN, player.direction);
	player.move = gameObject::moving(0, 100 - bonus_stats[2]);
	player.shot = gameObject::shooting(0, 200 - bonus_stats[3]);
	a[player.y][player.x] = player;
	for (auto& tank : enemy) {
		PrintTank(tank.x, tank.y, tank.color, tank.direction);
		tank.shot = gameObject::shooting(0, 1000);
		a[tank.y][tank.x] = tank;
		GotoXY(0, 0);
	}
	DisplayPlayerHP(player);
	DisplayPlayerStat(bonus_stats);
	DisplayScore(score);
	return player;
}

int BulletCollision(board& a, list <gameObject>& enemy, list <enemyDestInfo>& enemyDestination,list <pair <gameObject, int>>& playerBullet, list <pair <gameObject, int>>& enemyBullet, gameObject& player, queue <effectQueueElement>& effectQueue, int& score, gameSound& sound) {
	int cnt[BOARD_HEIGHT + 2][BOARD_WIDTH + 2];
	memset(cnt, 0, sizeof cnt);
	auto it = playerBullet.begin();
	while (it != playerBullet.end()) {
		int& i = it->fi.y, & j = it->fi.x;
		if (a[i][j].type == WALL) {
			effectQueue.push({ clock(), j, i, 150, BLACK, Wall(a, i, j)});
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
			hp -= it->fi.damage;
			list <gameObject>::iterator tmp;
			int pos = 0;
			for (auto it = enemy.begin(); it != enemy.end(); it++, pos++)
				if (it->x == j && it->y == i)
					tmp = it;
			if (hp < 1) {
				DisplayScore(score += 1000);
				PrintDefeatedTank(j, i, RED & 15 | BACKGROUND_YELLOW);
				a[i][j] = gameObject(j, i, 0, { 0, 0 });
				effectQueue.push({clock(), j, i, 150, BLACK, SPACE});
				list <enemyDestInfo>::iterator it;
				int i = 0;
				for (auto it = enemyDestination.begin(); it != enemyDestination.end(); it++, i++)
					if (i == pos) {
						enemyDestination.erase(it);
						break;
					}
				enemy.erase(tmp);
				if (enemy.empty()) return Finish();
			}
			else {
				tmp->flag |= HIT;
				tmp->color = RED & 15 | BACKGROUND_YELLOW;
				tmp->he = gameObject::hitEffect(clock(), 150);
				tmp->hp = hp;
				PrintTank(j, i, tmp->color, tmp->direction);
			}
			it = playerBullet.erase(it);
			continue;
		}
		else if (a[i][j].type == GROUND) {
			PrintBullet(j, i, it->fi);
			cnt[i][j]++;
		}
		it++;
	}

	it = enemyBullet.begin();
	while (it != enemyBullet.end()) {
		int& i = it->fi.y, & j = it->fi.x;
		if (a[i][j].type == WALL) {
			effectQueue.push({ clock(), j, i, 150, BLACK, Wall(a, i, j) });
			int tmp = GetCurrentColor();
			TextColor(RED & 15 | BACKGROUND_YELLOW);
			GotoBoard(j, i);
			cout << Wall(a, i, j);
			TextColor(tmp);
			it = enemyBullet.erase(it);
			continue;
		}
		else if (a[i][j].type == PLAYER) {
			// play sound
			// ...
			int& hp = a[i][j].hp;
			hp -= it->fi.damage;
			DisplayScore(score -= 500);
			sound.play("hit");
			player.hp = hp;
			DisplayPlayerHP(player);
			list <gameObject>::iterator tmp;
			int pos = 0;
			for (auto it = enemy.begin(); it != enemy.end(); it++, pos++)
				if (it->x == j && it->y == i)
					tmp = it;
			if (hp < 1) {
				PrintDefeatedTank(j, i, GREEN & 15 | BACKGROUND_YELLOW);
				// Game Over
				return GameOver();
			}
			else {
				player.flag |= HIT;
				player.color = GREEN & 15 | BACKGROUND_YELLOW;
				player.he = gameObject::hitEffect(clock(), 150);
				PrintTank(j, i, player.color, player.direction);
			}
			it = enemyBullet.erase(it);
			continue;
		}
		else if (a[i][j].type == ENEMY) {
			// play sound
			// ...
			list <gameObject>::iterator tmp;
			int pos = 0;
			for (auto it = enemy.begin(); it != enemy.end(); it++, pos++)
				if (it->x == j && it->y == i)
					tmp = it;
			tmp->flag |= HIT;
			tmp->color = RED & 15 | BACKGROUND_YELLOW;
			tmp->he = gameObject::hitEffect(clock(), 150);
			PrintTank(j, i, tmp->color, tmp->direction);
			it = enemyBullet.erase(it);
			continue;
		}
		else if (a[i][j].type == GROUND) {
			PrintBullet(j, i, it->fi);
			cnt[i][j]++;
		}
		it++;
	}

	
	// bullets meet bullets
	for (int i = 2; i < BOARD_HEIGHT; i++)
		for (int j = 2; j < BOARD_WIDTH; j++)
			if (cnt[i][j] > 1) {
				it = playerBullet.begin();
				while (it != playerBullet.end()) {
					if (it->first.x == j && it->first.y == i) it = playerBullet.erase(it);
					else it++;
				}
				it = enemyBullet.begin();
				while (it != enemyBullet.end()) {
					if (it->first.x == j && it->first.y == i) it = enemyBullet.erase(it);
					else it++;
				}
				sound.play("bonk");
				int tmp = GetCurrentColor();
				TextColor(RED & 15 | BACKGROUND_YELLOW);
				GotoBoard(j, i); cout << BULLET_COLLISION;
				TextColor(tmp);
				effectQueue.push({ clock(), j, i, 150, BLACK, SPACE });
			}
	return 0;
}

void MovePlayerBullet(board& a, list <pair <gameObject, int>>& playerBullet) {
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

void MoveEnemyBullet(board& a, list <pair <gameObject, int>>& enemyBullet) {
	auto it = enemyBullet.begin();
	while (it != enemyBullet.end()) {
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

void CheckEffect(queue <effectQueueElement>& effectQueue, list <gameObject>& enemy, gameObject& player) {
	int currentTime = clock();
	while (effectQueue.size() && currentTime - effectQueue.front().T > effectQueue.front().remain) {
		GotoBoard(effectQueue.front().x, effectQueue.front().y);
		int tmp = GetCurrentColor();
		TextColor(effectQueue.front().color);
		cout << effectQueue.front().c;
		TextColor(tmp);
		effectQueue.pop();
	}
	auto it = enemy.begin();
	while (it != enemy.end()) {
		if (it->flag & HIT) {
			if (clock() - it->he.T > it->he.remain) {
				it->he = gameObject::hitEffect();
				it->color = RED;
				it->flag ^= HIT;
				PrintTank(it->x, it->y, it->color, it->direction);
			}
		}
		it++;
	}
	if (player.flag & HIT) {
		if (clock() - player.he.T > player.he.remain) {
			player.he = gameObject::hitEffect();
			player.color = GREEN;
			player.flag ^= HIT;
			PrintTank(player.x, player.y, player.color, player.direction);
		}
	}
}

void EnemyMove(board& a, list <gameObject>& enemy, list <enemyDestInfo>& enemyDestination) {
	auto e = enemy.begin();
	auto ed = enemyDestination.begin();
	while (e != enemy.end()) {
		if (clock() - e->move.T > e->move.remain) {
			bool ok = 1;
			if (ed->length < 1) {
				if (ed->length == -10) {
					e++;
					ed++;
					continue;
				}
				ed->length = rand() % 15 + 1;
				if (rand() % 2) {
					// left or right
					ed->dx = (rand() % 2 ? 1 : -1);
					ed->dy = 0;
					if (ed->dx > 0) ok = MoveRight(a, *e);
					else ok = MoveLeft(a, *e);
				}
				else {
					// up or down
					ed->dy = (rand() % 2 ? 1 : -1);
					ed->dx = 0;
					if (ed->dy > 0) ok = MoveDown(a, *e);
					else ok = MoveUp(a, *e);
				}
			}
			else {
				if (ed->dx > 0) ok = MoveRight(a, *e);
				else if (ed->dx < 0) ok = MoveLeft(a, *e);
				else if (ed->dy > 0) ok = MoveDown(a, *e);
				else ok = MoveUp(a, *e);
			}
			if (!ok) ed->length = 0;
			e->move = gameObject::moving(clock() + rand() % 123, 400);
			ed->length--;
		}
		e++, ed++;
	}
}

void EnemyPlayerDetection(board& a, gameObject& player, list <gameObject>& enemy, list <enemyDestInfo>& enemyDestination, list <pair <gameObject, int>>& enemyBullet, gameSound& sound) {
	auto e = enemy.begin();
	auto ed = enemyDestination.begin();
	while (e != enemy.end()) {
		if (e->x - player.x == 0 || e->y - player.y == 0) {
			int ndx = player.x - e->x, ndy = player.y - e->y;
			ndx = min(1, abs(ndx)) * (ndx < 0 ? -1 : 1);
			ndy = min(1, abs(ndy)) * (ndy < 0 ? -1 : 1);
			ed->length = -10;
			if (ndx != ed->dx || ndy != ed->dy) {
				ed->dx = ndx, ed->dy = ndy;
				if (ed->dx > 0) MoveRight(a, *e);
				else if (ed->dx < 0) MoveLeft(a, *e);
				else if (ed->dy > 0) MoveDown(a, *e);
				else MoveUp(a, *e);
			}
			if (clock() - e->shot.T > e->shot.remain) {
				sound.play("pew");
				enemyBullet.push_front({ gameObject(e->x + ndx, e->y + ndy, -1, e->direction, e->damage, RED, BULLET), clock() });
				e->shot.T = clock();
			}
		}
		else if (ed->length == -10) ed->length = 0;
		e++;
		ed++;
	}
}

void SaveScore(int score, int map, const string& player) {
	ofstream cout("data/ranking/rank.txt", ios::app);
	cout << score << ' ' << map << endl << player << endl << CurrentDateTime() << endl;
	cout.close();
}

bool MapExist(int map) {
	string f = "assets/map0" + ToString(map) + ".txt";
	return (fopen(f.c_str(), "r") ? 1 : 0);
}

void StartGame(board& a, int map, int& score, const string& playerName, gameSound& sound, vector<int> &bonus_stats) {
	if (!MapExist(map)) {
		Congratulation();
		return;
	}
	SetConsoleBlank();
	list <gameObject> enemy;
	list <enemyDestInfo> enemyDestination;
	gameObject player = LoadMap(a, enemy, enemyDestination, map, score, bonus_stats);
	list <pair <gameObject, int>> playerBullet, enemyBullet;
	queue <effectQueueElement> effectQueue;
	EnemyPlayerDetection(a, player, enemy, enemyDestination, enemyBullet, sound);
	int state = 0;
	while (true) {
		state = BulletCollision(a, enemy, enemyDestination, playerBullet, enemyBullet, player, effectQueue, score, sound);
		//CheckEffect(effectQueue, enemy);
		if (state == GAME_OVER) return;
		if (state == FINISH) {
			SaveScore(score, map, playerName);
			AskSave(++map, score, playerName);
			AskUpgrade(score, playerName, bonus_stats);
			StartGame(a, map, score, playerName, sound, bonus_stats);
			return;
		}
		EnemyMove(a, enemy, enemyDestination);
		state = BulletCollision(a, enemy, enemyDestination, playerBullet, enemyBullet, player, effectQueue, score, sound);
		if (state == GAME_OVER) return;
		if (state == FINISH) {
			SaveScore(score, map, playerName);
			AskSave(++map, score, playerName);
			AskUpgrade(score, playerName, bonus_stats);
			StartGame(a, map, score, playerName, sound, bonus_stats);
			return;
		}

		EnemyPlayerDetection(a, player, enemy, enemyDestination, enemyBullet, sound);
		MoveEnemyBullet(a, enemyBullet);
		state = BulletCollision(a, enemy, enemyDestination, playerBullet, enemyBullet, player, effectQueue, score, sound);
		if (state == GAME_OVER) return;
		if (state == FINISH) {
			SaveScore(score, map, playerName);
			AskSave(++map, score, playerName);
			AskUpgrade(score, playerName, bonus_stats);
			StartGame(a, map, score, playerName, sound, bonus_stats);
			return;
		}
		CheckEffect(effectQueue, enemy, player);

		if (_kbhit()) {
			unsigned char c = toupper(_getch());
			if (c == ENTER) {
				if (clock() - player.shot.T > player.shot.remain) {
					sound.play("pew");
					playerBullet.push_front({ gameObject(player.x + player.direction.se, player.y + player.direction.fi, -1, player.direction, player.damage, GREEN, BULLET), clock() });
					player.shot.T = clock();
				}
			}
			else if (c == ESC) {
				sound.play("click");
				return;
			}
			else if (c == myKeyW || c == myKeyA || c == myKeyS || c == myKeyD) {
				if (clock() - player.move.T < player.move.remain) continue;
				if (c == myKeyW) MoveUp(a, player);
				else if (c == myKeyA) MoveLeft(a, player);
				else if (c == myKeyS) MoveDown(a, player);
				else if (c == myKeyD) MoveRight(a, player);
				player.move.T = clock();
			}
		}
		MovePlayerBullet(a, playerBullet);
	}
}
