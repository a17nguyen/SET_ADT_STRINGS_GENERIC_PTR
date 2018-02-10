/*
 * File:	table.c
 * Name:	Andrew Nguyen
 * Lab Section:	Fiday
 *
 * Description:	This program implements a set ADT for generic pointers. The implementation
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

struct set
{
	int count;
	int length;
	char** data;
	char *flags;
	int(*compare)();
	unsigned(*hash)();
};

typedef struct set SET;

//	createSet
//	O(1)
//	returns a pointer to a new set with length maxElts
SET *createSet(int maxElts, int (*compare)(), unsigned (*hash)()) 
{
	SET *sp;
	sp = malloc(sizeof(SET));
	assert(sp != NULL);
	
	//initializes pointers
	sp -> count = 0;
	sp -> length = maxElts;
	sp -> data = malloc(sizeof(void*) *maxElts);
	sp -> flags = malloc(sizeof(char)*maxElts);
	sp -> compare = compare;
	sp -> hash = hash;
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

//	search
//	O(n)
//	uses sequential search method to locate an existing element
static int search(SET *sp, void *elt, bool *found)
{
	int i, hash, x = -1;
	
	//implements linear hashing
	for(i = 0; i < sp -> length; i++)
	{
		hash = ((sp-> hash)(elt) + i) % sp -> length;
		
		//if idx is empty
		if(sp -> flags[hash] == empty)
		{
			*found = false;
			
			if(x != -1)
			{
				return x;
			}
			return hash;
		}

		//if idx is filled
		if(sp -> flags[hash] == filled)
		{
			if((*sp -> compare)(elt, sp -> data[hash]) == 0)
			{
				*found = true;
				return hash;
			}
		}

		//if idx is deleted
		if(sp -> flags[hash] == deleted && x == -1)
		{
			x = hash;
		}
	}
	return -1;
}

//	addElement
//	Best case O(1), Worst case O(n)
//	searches then adds non-existing element to the set
void addElement(SET *sp, void *elt)
{
	assert(sp != NULL && elt != NULL);
	bool found = true;
	int idx = search(sp, elt, &found);

	if(sp -> count == sp -> length)
	return;

	//if not found, add element and mark idx as filled
	if(!found)
	{
		sp -> data[idx] = elt;
		sp -> flags[idx] = filled;
		sp -> count++;
	}
}	


//	removeElement
//	Best Case O(1), Worst Case O(n)
//	removes element in the set by freeing the element and taking it out of the hash table
void removeElement(SET *sp, void *elt)
{
	assert(sp != NULL && elt != NULL);
	bool found = true;
	int idx = search(sp, elt, &found);

	//if found, remove and mark idx as deleted
	if(found)
	{
		sp -> flags[idx] = deleted;
		sp -> count--;
	}
}

//	findElement
//	O(n)
//	finds element by searching for the element by the index in the data
//	returns the address of the element
void *findElement(SET *sp, void *elt)
{
	assert(sp != NULL && elt != NULL);
	bool found = true;
	int idx = search(sp, elt, &found);
	if(found)
	{	
		return sp -> data[idx];
	}
	return NULL;
		
}

//	getElements
//	O(n)
/*	allocates a new spot for memory and copies the 
 *	array into new area and returns the copies array of elts
 */
void *getElements(SET *sp)
{	
	assert(sp != NULL);
	void **newloc;
	int count = 0;
	newloc = malloc(sizeof(void*)*sp -> count);
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

