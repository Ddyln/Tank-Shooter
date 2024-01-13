#pragma once
string CurrentDateTime();
char Tank(pair <int, int> direction);
char Wall(board& a, int i, int j);
string ToString(int n);
bool EnterText(string& s, int len);
gameObject LoadMap(board& a, list <gameObject>& enemy, list <enemyDestInfo>& enemyDestination, int map, int score, vector<int> bonus_stats);
void SaveScore(int score, int map, const string& player);
void EnemyPlayerDetection(board& a, gameObject& player, list <gameObject>& enemy, list <enemyDestInfo>& enemyDestination, list <pair <gameObject, int>>& enemyBullet, gameSound& sound);
void StartGame(board& a, int map, int& score, const string& playerName, gameSound& sound, vector<int> &bonus_stats);
void EnemyMove(board& a, list <gameObject>& enemy, list <enemyDestInfo>& enemyDestination);
int BulletCollision(board& a, list <gameObject>& enemy, list <enemyDestInfo>& enemyDestination, list <pair <gameObject, int>>& playerBullet, list <pair <gameObject, int>>& enemyBullet, gameObject& player, queue <effectQueueElement>& effectQueue, int& score, gameSound& sound);
void MovePlayerBullet(board& a, list <pair <gameObject, int>>& playerBullet);
void CheckEffect(queue <effectQueueElement>& effectQueue, list <gameObject>& enemy, gameObject& player);
bool MapExist(int map);