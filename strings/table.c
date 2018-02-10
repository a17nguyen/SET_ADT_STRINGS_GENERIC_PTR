/*
 * File:	table.c
 * Name:	Andrew Nguyen
 * Lab Section:	Fiday
 *
 * Description:	This program implements a set ADT for strings. The implementation
 * 		utilizes a hash table and linear probing to store elements.
 */		

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "set.h"
#define empty 0
#define filled 1
#define deleted 2

unsigned strhash(char *s)
{
	unsigned hash = 0;

	while (*s != '\0')
	{
		hash = 31 * hash + *s ++;
	}
	return hash;
}
struct set
{
	int count;
	int length;
	char** data;
	char *flags;
};

//	createSET
//	O(1)
//	returns a pointer to a new set with length maxElts
SET *createSet(int maxElts) 
{
	//initialize and allocate memory
	SET *sp;
	sp = malloc(sizeof(SET));
	assert(sp != NULL);
	
	//initialize pointer
	sp -> count = 0;
	sp -> length = maxElts;
	sp -> data = malloc(sizeof(char*) *maxElts);
	sp -> flags = malloc(sizeof(char)*maxElts);
	assert(sp -> data != NULL);

	int i;
	for(i = 0; i < maxElts; i++) 
	{
		sp -> flags[i] = empty;
	}
	return sp;
}

//	destroySet
//	O(n)
//	frees the array and strings in memory pointed by sp
void destroySet(SET *sp)
{
	assert(sp != NULL);
	int i;
	
	//free data array and pointer
	for(i = 0; i < sp -> length; i++) 
	{
		if(sp -> flags[i] == filled)
		{
		free(sp -> data[i]);
		}
	}
	free(sp -> flags);
	free(sp -> data);
	free(sp);
}

//	numElements
//	O(1)
//	returns the number of elements in the set
int numElements(SET *sp)
{
	assert(sp != NULL);
	return sp -> count;
}

//	private search
//	O(n)
//	uses sequential search method to locate an existing element in hash table
static int search(SET *sp, char *elt, bool *found)
{
	int i, hash, x = -1;

	//implement linear hashing of [h(k) + i] % m
	for(i = 0; i < sp -> length; i++)
	{
		hash = (strhash(elt) + i) % sp -> length;
		
		//if slot is empty
		if(sp -> flags[hash] == empty)
		{
			*found = false;
			
			if(x != -1)
			{
				return x;
			}
			return hash;
		}

		//if slot is filled
		if(sp -> flags[hash] == filled)
		{
			if(strcmp(elt, sp -> data[hash]) == 0)
			{
				*found = true;
				return hash;
			}
		}

		//if slot is deleted
		if(sp -> flags[hash] == deleted && x == -1)
		{
			x = hash;
		}
	}
	return -1;
}

//	addElement
//	Best case O(1), Worst Case O(n)
//	searches then adds non-existing element to the set
void addElement(SET *sp, char *elt)
{
	assert(sp != NULL && elt != NULL);
	bool found = true;
	int idx = search(sp, elt, &found);

	if(sp -> count == sp -> length)
	return;

	//if not found, add element and mark idx as filled
	if(!found)
	{
		sp -> data[idx] = strdup(elt);
		sp -> flags[idx] = filled;
		sp -> count++;
	}
}	


//	removeElement
//	Best case O(1), Worst case O(n)
//	removes element in the set by freeing the element and taking it out of the hash table
void removeElement(SET *sp, char *elt)
{
	assert(sp != NULL && elt != NULL);
	bool found = true;
	int idx = search(sp, elt, &found);
	
	//if found, remove element and mark idx as deleted
	if(found)
	{
		sp -> flags[idx] = deleted;
		free(sp -> data[idx]);
		sp -> count--;
	}
}

//	findElement
//	O(n)
//	finds element by searching for the element by the index in the data
//	returns the address of the element
char *findElement(SET *sp, char *elt)
{
	assert(sp != NULL && elt != NULL);
	bool found = true;
	int idx = search(sp, elt, &found);

	if(found)
	{	
		return sp->data[idx];
	}
	return NULL;	
}
//	getElements
//	O(n)
/*	allocates a new spot for memory and copies the 
 *	array into new area and returns the copies array of elts
 */
char **getElements(SET *sp)
{	
	assert(sp != NULL);
	char **newloc;
	int count = 0;
	newloc = malloc(sizeof(char*)*sp -> count);
	assert(newloc != NULL);

	int i;
	for(i = 0; i < sp -> length; i++)
	{
		if(sp -> flags[i] == filled)
		{
			newloc[count++] = sp -> data[i];
		}
	}
	return newloc;
}

