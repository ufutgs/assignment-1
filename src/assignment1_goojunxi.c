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
	list_init(&plist,&mlist);
	list_free(&plist,&mlist);
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
		class += get_boolean_value(&(p->status), i)*pow(2,i);
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
	int roll;
	int stunned;
	int blinded;
	int silenced;
	int attack_type;
	attack_type = get_boolean_value(&(p->status),3);
	stunned = get_boolean_value(&(p->status),5);
	blinded = get_boolean_value(&(p->status),6);
	silenced = get_boolean_value(&(p->status),7);
	if(stunned)
		return;
	roll = (((silenced&&attack_type)||(blinded&&!attack_type))
			&&rand()%2) ? 0: rand()%10+1-m->def+p->atk;
	m->hp-=roll;
}

player * newPlayer(char *name)
{
	char stat = (char) 0;
	int i;
	int atk_type;
	int random;
	player *p;
	atk_type=0;
	p = (player*)malloc(sizeof(player));
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
	p->status_update = &status_update;
	return p;
}

void status_update(player* p)
{
	int i;
	int remaining;
	for(i=0;i<4;i++)
	{
		remaining = get_boolean_value(&(p->status_track),i*2) + 2*get_boolean_value(&(p->status_track),i*2 + 1) - 1;
		if(remaining%2==0)
			update_boolean_value(&(p->status_track),i*2, 0);
		else
			update_boolean_value(&(p->status_track),i*2, 1);
		if(remaining>=2)
			update_boolean_value(&(p->status_track),i*2+1, 1);
		else
			update_boolean_value(&(p->status_track),i*2+1, 0);
		if(remaining==0)
			update_boolean_value(&(p->status),i+4, 0);
	}
}

void printMonsterInformation (monster *m)
{
	printf("========Monster Profile========\n");
	printf("ID- %d\n",m->id);
	printf("HP- %d\nAttack Point- %d\nDefense Point- %d\n",m->hp,m->atk,m->def);
}

monster * newMonster()
{
	monster* m = (monster*)malloc(sizeof(monster));
	m->hp = rand()%1001 + 1000;
	m->atk = rand()%21 + 5;
	m->def = rand()%10;
	m->id = (int) time(NULL);
	m->success_rate = 4;
	m->printMonsterInformation = &printMonsterInformation;
	m->newMonster = &newMonster;
	m->attackPlayer = & attackPlayer;
	return m;
}

int attackPlayer(player *p, monster *m)
{
	int roll;
	int stat;
	if(rand()%4==0)
	{
		roll = rand()%5 + 1 + m->atk - p->def;
		if(get_boolean_value(&(p->status),4)==1)
			roll+= roll/4;
		if(rand()%m->success_rate==0)
		{
			stat = rand()%4;
			update_boolean_value(&(p->status), stat + 4 , 1);
			update_boolean_value(&(p->status_track), stat*2 , 1);
			update_boolean_value(&(p->status_track), stat*2 + 1 , 1);
		}
		p->hp -= roll;
		return 1;
	}
	return 0;
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
	printf("Now Loading...\n");
	p.newPlayer = &newPlayer;
	m.newMonster = &newMonster;
	plist->list = (player**)malloc(4*sizeof(player*));
	mlist->list = (monster**)malloc(6*sizeof(monster*));
	plist->size=0;
	mlist->size=0;
	plist->on_p_death = &on_p_death;
	mlist->on_m_death = &on_m_death;
	for(i =0; i<4; i++)
	{
		plist->list[i] = p.newPlayer("test");
		plist->size+=1;
	}
	for(i =0; i<(rand()%6+1); i++)
	{
		mlist->list[i] = m.newMonster();
		mlist->size+=1;
	}
	printf("Loading Completed.\n");
}

void on_p_death(struct pList* plist, int i)
{
	int k;
	free(plist->list[i]);
	for(k=i;k<plist->size-1;k++)
	{
		plist->list[i] = plist->list[i+1];
	}
	plist->size-=1;
}

void on_m_death(struct mList* mlist, int i)
{
	int k;
	free(mlist->list[i]);
	for(k=i;k<mlist->size-1;k++)
	{
		mlist->list[i] = mlist->list[i+1];
	}
	mlist->size-=1;
}

