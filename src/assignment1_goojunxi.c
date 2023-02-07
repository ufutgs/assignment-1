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
#include "assignment1_goojunxi.h"

player** player_list;
monster** monster_list;

int main(void) {
	char c = 'c';
	player * p1 = newPlayer("come");
	update_boolean_value(&c, 0, 0);
	printf("updated c is %c\n", c);
	print_boolean(&c, 1);
	free(p1);
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

char * print_class(player *p)
{
	return &p->status;
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

player * newPlayer(char *name)
{
	player *p = malloc(sizeof(player));
	p->name = name;
	p->hp = rand()%200 + 100;
	p->atk = rand()%20 + 5;
	p->def = rand()%30;
	p->status_track = (char) 0;

	return p;
}
