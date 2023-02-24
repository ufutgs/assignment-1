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
#include<unistd.h>
#include "assignment1_goojunxi.h"


int main(void) {
	struct pList plist;
	struct mList mlist;
	int i;
	int counter;
	counter=0;
	i=1;
	srand(time(NULL));
	list_init(&plist,&mlist);
	print_profile(&plist, &mlist);
	printf("Now Loading... \n\n\n\n");
	printf("Loading completed.\n\n");
	printf("Quest Begin\n");
	while(i)
	{
		int k;
		int target;
		counter+=1;
		printf("\n-|- Round %d -|-\n",counter);
		/*Player status ailment update*/
		for(k=0;k<plist.size;k++)
			plist.list[k]->status_update(plist.list[k]);

		printf("-|- Player Round -|-\n");
		/*Player Round*/
		for(k=0;k<plist.size;k++)
		{
			player* p;
			monster* m;
			p = plist.list[k];
			target = rand()%mlist.size;
			m = mlist.list[target];
			p->attackMonster(p,m);
			if(m->hp<=0)
			{
				fflush( stdout );
				sleep(1);
				mlist.on_m_death(&mlist,target);
				if(mlist.size==0)
				{
					i = 0;
					break;
				}
			}
		}
		printf("\n-|- Monster Round -|-\n");
		/*Monster Round*/
	 for(k=0;k<mlist.size;k++)
		{
			player* p;
			monster* m;
			m = mlist.list[k];
			target = rand()%plist.size;
			p = plist.list[target];
			m->attackPlayer(p,m);
			if(p->hp<=0)
			{
				fflush( stdout );
				sleep(1);
				plist.on_p_death(&plist,target);
				if(plist.size==0)
				{
					i = 0;
					break;
				}
			}
		}

	}
	if(plist.size==0)
		printf("Quest Failed...\n");
	else
		printf("Quest Success !!\n");
	printf("Summarizing...\n");
	printf("\n============Report============\n\n");
	printf("Monster slayed: %d\n",mlist.init_size-mlist.size);
	printf("Player fainted: %d\n",plist.init_size-plist.size);
	printf("\nGame Over\n\n BYE BYE");
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
	int target;
	if(index >7 || index<0 || val < 0 || val >1)
		return;
	 target = 128 >> index;
	*booleans =(val==0) ? (char) *booleans & ~target :(char) *booleans|target;
}

void print_boolean(char *booleans, int index)
{
	int i = get_boolean_value(booleans, index);
	printf("value: %d\n",(i>1 || i<0) ? 1 : i);
}

void printPlayerInformation (player *p)
{
	char *ptr = &(p->status);
	printf("\n==============================\n\n");
	printf("Name: %s\n",p->name);
	printf("HP: %d\nAttack Point: %d\nDefense Point: %d\n",p->hp,p->atk,p->def);
	printf("Poisoned: %s\n",(get_boolean_value(ptr, 4)==0)?"No":"Yes");
	printf("Stunned: %s\n",(get_boolean_value(ptr, 5)==0)?"No":"Yes");
	printf("Silenced: %s\n",(get_boolean_value(ptr, 6)==0)?"No":"Yes");
	printf("Blinded: %s\n",(get_boolean_value(ptr, 7)==0)?"No":"Yes");
	printf("Class: %s\n",p->print_class(p));

}

char * print_class(player* p)
{
	int class = 0;
	int i;
	for(i=0;i<3;i++)
	{
		class += get_boolean_value(&(p->status), i)*pow(2,i);
	}
	switch(class){
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
	blinded = get_boolean_value(&(p->status),7);
	silenced = get_boolean_value(&(p->status),6);
	printf("%s has targeted monster %d\n",p->name,m->id);
	if(stunned)
	{
		printf("%s stunned!\n",p->name);
		return;
	}
	roll = (((silenced&&attack_type)||(blinded&&!attack_type))
			&&rand()%2) ? 0: rand()%10+1-m->def+p->atk;
	roll = (roll<0) ? 0 : roll;
	m->hp-=roll;
	if((silenced&&attack_type))
		printf("%s is silenced !\n",p->name);
	else if((blinded&&!attack_type))
		printf("%s is blinded !\n",p->name);
	printf("%s deal %d damage to monster %d. Monster HP: %d\n",p->name,roll,m->id,m->hp);
}

player * newPlayer(char *name)
{
	int i;
	int atk_type;
	int random;
	player *p;
	atk_type=0;
	p = (player*)malloc(sizeof(player));
	p->name = name;
	p->hp = rand()%101 + 100;
	p->atk = rand()%21 + 5;
	p->def = rand()%21;
	p->status_track = (char) 0;
	p->status = (char) 0;
	for(i=0;i<3;i++)
	{
		random = rand()%2;
		update_boolean_value(&(p->status), i, random);
		atk_type += random*pow(2,i);
	}
	update_boolean_value(&(p->status),3,atk_type%2);
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
		if(remaining<0)
			continue;
		if(remaining%2==0)
			update_boolean_value(&(p->status_track),i*2, 0);
		else
			update_boolean_value(&(p->status_track),i*2, 1);
		if(remaining>=2)
			update_boolean_value(&(p->status_track),i*2+1, 1);
		else
			update_boolean_value(&(p->status_track),i*2+1, 0);
		if(remaining==0)
		{
			update_boolean_value(&(p->status),i+4, 0);
			switch(i)
			{
			case 0:
				printf("Player %s has recovered from posion!\n",p->name);
				break;
			case 1:
				printf("Player %s has recovered from stun!\n",p->name);
				break;
			case 2:
				printf("Player %s has recovered from silenced!\n",p->name);
				break;
			case 3:
				printf("Player %s has recovered from Blinded!\n",p->name);
				break;
			}


		}
	}
}

void printMonsterInformation (monster *m)
{
	printf("\n==============================\n\n");
	printf("ID: %d\n",m->id);
	printf("HP: %d\nAttack Point: %d\nDefense Point: %d\n",m->hp,m->atk,m->def);
}

monster * newMonster()
{
	monster* m = (monster*)malloc(sizeof(monster));
	m->hp = rand()%400 + 100;
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
	printf("Monster %d has targeted %s\n",m->id,p->name);
	if(rand()%4!=0)
	{
		roll = rand()%5 + 1 + m->atk - p->def;
		roll = (roll<0) ? 0 : roll;
		if(get_boolean_value(&(p->status),4)==1)
		{
			printf("%s is poisoned ! Extra damage taken !\n",p->name);
			roll+= roll/4;
		}
		if(rand()%m->success_rate==0)
		{
			char* str;
			stat = rand()%4;
			update_boolean_value(&(p->status), stat + 4 , 1);
			update_boolean_value(&(p->status_track), stat*2 , 1);
			update_boolean_value(&(p->status_track), stat*2 + 1 , 1);
			switch(stat)
			{
				case 0:
					str = "Poisoned";
					break;
				case 1:
					str = "Stunned";
					break;
				case 2:
					str = "Silenced";
					break;
				default:
					str = "Blinded";
			}
			printf("%s has been %s!\n",p->name,str);
		}
		p->hp -= roll;
		printf("Monster %d deal %d damage to %s. Player HP: %d\n",m->id,roll,p->name,p->hp);
		return 1;
	}
	printf("Monster %d missed it's attack !\n",m->id);
	return 0;
}

void list_free(struct pList* plist,struct mList* mlist)
{
	int i;
	for(i=0;i<plist->size;i++)
	{
		free(plist->list[i]->name);
		free(plist->list[i]);
	}
	free(plist->list);
	plist->list = NULL;
	for(i=0;i<mlist->size;i++)
	{
		free(mlist->list[i]);
	}
	free(mlist->list);
	plist->list = NULL;
}

void list_init (struct pList* plist,struct mList* mlist)
{
	int i;
	struct player p;
	struct monster m;
	int monster_count;
	monster_count = rand()%6+1;
	printf("Now Loading...\n");
	p.newPlayer = &newPlayer;
	m.newMonster = &newMonster;
	plist->list = (player**)malloc(4*sizeof(player*));
	mlist->list = (monster**)malloc(monster_count*sizeof(monster*));
	plist->size=0;
	mlist->size=0;
	plist->init_size=0;
	mlist->init_size=0;
	plist->on_p_death = &on_p_death;
	mlist->on_m_death = &on_m_death;
	for(i =0; i<4; i++)
	{
		char* name = (char*) malloc(10*sizeof(name));
		sprintf(name,"Player %d",i+1);
		plist->list[i] = p.newPlayer(name);
		plist->size+=1;
		plist->init_size+=1;
	}
	for(i =0; i<(monster_count); i++)
	{
		mlist->list[i] = m.newMonster();
		mlist->size+=1;
		mlist->init_size+=1;
	}
	printf("Loading Completed.\n");
}

void on_p_death(struct pList* plist, int i)
{
	int k;
	printf("%s has fainted!\n",plist->list[i]->name);
	free(plist->list[i]->name);
	free(plist->list[i]);
	for(k=i;k<plist->size-1;k++)
	{
		plist->list[k] = plist->list[k+1];
	}
	plist->size-=1;
	plist->list[plist->size] = NULL;
}

void on_m_death(struct mList* mlist, int i)
{
	int k;
	printf("Monster %d has slayed!\n",mlist->list[i]->id);
	free(mlist->list[i]);
	for(k=i;k<mlist->size-1;k++)
	{
		mlist->list[k] = mlist->list[k+1];
	}
	mlist->size-=1;
	mlist->list[mlist->size] = NULL;
}

void print_profile(struct pList* plist, struct mList* mlist)
{
	int i;
	printf("\n========Player Profile========\n");
	for(i=0;i<plist->size;i++)
		plist->list[i]->printPlayerInformation(plist->list[i]);
	printf("\n========Monster Profile========\n");
	for(i=0;i<mlist->size;i++)
		mlist->list[i]->printMonsterInformation(mlist->list[i]);

}
