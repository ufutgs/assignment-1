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
typedef struct playerList pList;
typedef struct monsterList mList;
typedef struct charList cList;

/*
status boolean = [class][attack type][status]
status_check = [Poison][Stun][Silent][Blind]
attack type : 1 = magical attack, 0 = physical attack
*/

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
	void (*printMonsterInformation) (monster *m);
	monster * (*newMonster)();
	int (*attackPlayer)(player *p, monster *m);
};

struct pList
{
	player** list;
	int size;
	void (*init) (int i);
};

struct mList
{
	monster** list;
	int size;
	void (*init) (int i);
};

struct cList
{
	char** list;
	int size;
	void (*init) (int i);
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

void list_free(struct pList* plist,struct mList* mlist);
void list_init (struct pList* plist,struct mList* mlist);
#endif /* ASSIGNMENT1_GOOJUNXI_H_ */
