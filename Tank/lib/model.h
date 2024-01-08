#pragma once
char Tank(pair <int, int> direction);
char Wall(board& a, int i, int j);
gameObject LoadMap(board& a, list <gameObject>& enemy, list <enemyDestInfo>& enemyDestination);
void StartGame(board& a);
void PlayerBulletCollision(board& a, list <gameObject>& enemy, list <pair <gameObject, int>>& playerBullet, gameObject& player, queue <effectQueueElement> effectQueue[2]);
void MoveBullet(board& a, list <pair <gameObject, int>>& playerBullet);
void CheckEffect(queue <effectQueueElement>& effectQueue);