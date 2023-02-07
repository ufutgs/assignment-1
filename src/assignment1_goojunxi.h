/*
 * assignment1_goojunxi.h
 *
 *  Created on: Feb 7, 2023
 *      Author: user
 */

#ifndef ASSIGNMENT1_GOOJUNXI_H_
#define ASSIGNMENT1_GOOJUNXI_H_
int get_boolean_value (char *booleans, int index);
void update_boolean_value(char *booleans, int index, int val);
void print_boolean(char *booleans, int index);

typedef struct player player;
typedef struct monster monster;
struct player
{
	char * name;
	int hp,atk,def;
	char status_track;
	char status;
	void (*printPlayerInformation) (player *p);
	void (*attackMonster) (player *p, monster *m);
	player* (*newPlayer) (char *name);
	char *(*print_class) (player *p);

};

 struct monster
{
	int hp,atk,def,id;
	int status_rate;
	void (*printPlayerInformation) (monster *m);
	monster * (*newMonster)();
	int (*attackPlayer)(player *p, monster *m);
};


void printPlayerInformation (player *p);
void attackMonster (player *p, monster *m);
player * newPlayer(char *name);
char * print_class(player *p);

void playerStatusLog();
void playerAttackLog(monster* m);

void printMonsterInformation (monster *m);
monster * newMonster();
int attackPlayer(player *p, monster *m);
#endif /* ASSIGNMENT1_GOOJUNXI_H_ */
