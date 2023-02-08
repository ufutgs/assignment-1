/*
 ============================================================================
 Name        : assignment.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "assignment1_goojunxi.h"


int main(void) {
	struct pList plist;
	struct mList mlist;
	int i;
	char c = 'c';
	list_init(&plist,&mlist);
	update_boolean_value(&c, 0, 0);
	printf("updated c is %c\n", c);
	print_boolean(&c, 1);
	list_free(plist, mlist);
	return 0;
}

int get_boolean_value (char *booleans, int index)
{
	if (index>7 || index<0)
		return (int) *booleans;
	return (int) ( (*booleans & (128 >> index)) >> (7-index) );
}

void update_boolean_value(char *booleans, int index , int val)
{
	int target,b;
	if(index >7 || index<0 || val < 0 || val >1)
		return;
	 target = *booleans&( 128 >> index);
	 b = (val==0)? 0 : (int) ( 128 >> index);
	*booleans =(char) *booleans ^ (b^target);
}

void print_boolean(char *booleans, int index)
{
	int i = get_boolean_value(booleans, index);
	printf("value: %d\n",(i>1 || i<0) ? 1 : i);
}

void printPlayerInformation (player *p)
{
	char *ptr = &(p->status);
	printf("========Player Profile========\n");
	printf("Name- %s\n",p->name);
	printf("HP- %d\nAttack Point- %d\nDefense Point- %d\n",p->hp,p->atk,p->def);
	printf("====Status Profile====\n");
	printf("Poisoned- %s\n",(get_boolean_value(ptr, 3)==0)?"No":"Yes");
	printf("Stunned- %s\n",(get_boolean_value(ptr, 4)==0)?"No":"Yes");
	printf("Silenced- %s\n",(get_boolean_value(ptr, 5)==0)?"No":"Yes");
	printf("Blinded- %s\n",(get_boolean_value(ptr, 6)==0)?"No":"Yes");
	printf("Class- %s\n",p->print_class(p));

}

char * print_class(player* p)
{
	int class = 0;
	int i;
	for(i=0;i<3;i++)
	{
		class += get_boolean_value(p->status, i)*pow(2,i);
	}
	switch(i){
	case 0:
		return "Warrior";
	case 1:
		return "Mage";
	case 2:
		return "Rogue";
	case 3:
		return "Priest";
	case 4:
		return "Monk";
	case 5:
		return "Druid";
	case 6:
		return "Beserker";
	case 7:
		return "Paladin";
	default:
		return "Invalid class";
	}

}

void attackMonster (player *p, monster *m)
{

}

player * newPlayer(char *name)
{
	char stat = (char) 0;
	int i;
	int atk_type;
	int random;
	atk_type=0;
	player *p = (player*)malloc(sizeof(player));
	p->name = name;
	p->hp = rand()%201 + 100;
	p->atk = rand()%21 + 5;
	p->def = rand()%31;
	p->status_track = (char) 0;
	for(i=0;i<3;i++)
	{
		random = rand()%2;
		update_boolean_value(&stat, i, random);
		atk_type += random*pow(2,i);
	}
	update_boolean_value(&stat,3,atk_type%2);
	p->status = stat;
	p->printPlayerInformation = &printPlayerInformation;
	p->attackMonster = &attackMonster;
	p->print_class = &print_class;
	p->newPlayer = &newPlayer;
	return p;
}

void printMonsterInformation (monster *m)
{

}

monster * newMonster()
{
	monster* m = (monster*)malloc(sizeof(monster));
	m->hp = rand()%1001 + 1000;
	m->atk = rand()%21+5;
	m->def = rand()%11;
	m->id = (int) time(NULL);
	m->status_rate = 4;
	m->printMonsterInformation = &printMonsterInformation;
	m->newMonster = &newMonster;
	m->attackPlayer = & attackPlayer;
	return m;
}

int attackPlayer(player *p, monster *m)
{

}

void list_free(struct pList* plist,struct mList* mlist)
{
	int i;
	for(i=0;i<plist->size;i++)
	{
		free(plist->list[i]);
	}
	free(plist->list);
	for(i=0;i<mlist->size;i++)
	{
		free(mlist->list[i]);
	}
	free(mlist->list);
}

void list_init (struct pList* plist,struct mList* mlist)
{
	int i;
	struct player p;
	struct monster m;
	p.newPlayer = &newPlayer;
	plist->list = (player**)malloc(4*sizeof(player*));
	mlist->list = (monster**)malloc(6*sizeof(monster*));
	plist->size=0;
	mlist->size=0;
	for(i =0; i<4; i++)
	{
		plist->list[i] = p.newPlayer("test");
		plist->size+=1;
	}
	for(i =0; i<(rand()%6+1); i++)
	{
		mlist->list[i] = (monster*)malloc(sizeof(monster));
		mlist->size+=1;
	}
}

