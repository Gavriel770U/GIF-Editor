#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedList.h"

/*
	Function that creates a Frame and returns a pointer to it. 
	Input: name - the name of the frame, 
		   duration - the duration of the frame (milliseconds),
		   path - the path to the frame.
	Output: pointer to a Frame struct with the given parameters. 

*/
Frame* createFrame(char* name, unsigned int duration, char* path)
{
	Frame* frame = NULL;
	int length = 0;

	frame = (Frame*)malloc(sizeof(Frame));
	if (!frame)
	{
		printf("Memory allocation failed!\n"); 
		exit(MEMORY_ALLOCATION_ERROR_CODE);
	}

	length = strlen(name);
	frame->name = (char*)malloc(sizeof(char) * (length + INC));
	if (!frame->name)
	{
		printf("Memory allocation failed!\n");
		exit(MEMORY_ALLOCATION_ERROR_CODE);
	}
	strcpy(frame->name, name);
	frame->name[length] = NULL_CHAR;

	frame->duration = duration;

	length = strlen(path);
	frame->path = (char*)malloc(sizeof(char) * (length + INC));
	if (!frame->path)
	{
		printf("Memory allocation failed!\n");
		exit(MEMORY_ALLOCATION_ERROR_CODE);
	}
	strcpy(frame->path, path);
	frame->path[length] = NULL_CHAR;

	return frame; 
}

/*
	Function that frees the allocated memory for a Frame struct.
	Input: frame - pointer to a frame to free.
	Output: None.
*/
void freeFrame(Frame* frame)
{
	if (!frame)
	{
		return; 
	}
	free(frame->name); 
	free(frame->path);
	free(frame); 
	frame = NULL;
}

/*
	Function that creates FrameNode struct and returns pointer to it.
	Input: frame - pointer to Frame struct that will be the frame property of the FrameNode.
	Output: pointer to a FrameNode strcut with the given frame property and NULL as next FrameNode*.
*/
FrameNode* createFrameNode(Frame* frame)
{
	FrameNode* frameNode = NULL; 
	if (!frame)
	{
		return NULL;
	}

	frameNode = (FrameNode*)malloc(sizeof(FrameNode));
	if (!frameNode)
	{
		printf("Allocation memory failed!\n");
		exit(MEMORY_ALLOCATION_ERROR_CODE);
	}

	frameNode->frame = frame;
	frameNode->next = NULL; 
	return frameNode;
}


/*
	Function that frees allocated memory for single FrameNode*.
	Input: node - pointer to FrameNode* to free.
	Output: None.
*/
void freeFrameNode(FrameNode** node)
{
	if (!*node)
	{
		return;
	}
	freeFrame((*node)->frame);
	(*node)->next = NULL;
	free(*node);
	*node = NULL;
}

/*
	Function that frees allocated memory for FrameNode* list.
	Input: head - pointer to a head node of FrameNode* list.
	Output: None.
*/
void freeFrameNodeList(FrameNode** head)
{
	FrameNode* current = *head;
	FrameNode* next = NULL;

	while (current)
	{
		next = current->next;
		freeFrameNode(&current);
		current = next;
	}

	*head = NULL;
}

/**
* Function that returns the length of FrameNode* list. 
* Input: head - head node of FrameNode* list. 
* Output: length of the list. 
*/
int frameNodeListLength(FrameNode* head)
{
	FrameNode* current = head;
	int counter = 0; 
	while (current)
	{
		counter++;
		current = current->next;
	}
	return counter;
}

/*
	Function that inserts created FrameNode* created with given Frame* into FrameNode* list.
	Input: head - pointer to FrameNode* list.
	       frame - frame to set as property for the new FrameNode* that will be added to the list. 
	Output: None.
*/
void insertFrameToList(FrameNode** head, Frame* frame)
{
	FrameNode* frameToAdd = createFrameNode(frame); 
	FrameNode* current = *head; 

	if (!*head)
	{
		*head = frameToAdd;
		return; 
	}
	while (current->next)
	{
		current = current->next; 
	}
	current->next = frameToAdd;
}

/*
	Function that checks if there is FrameNode* with a given name in a FrameNode* list.
	Input: head - FrameNode* list head.
		   name - the name of the frame to find.
		   notFoundMessage - the message that will be printed if the frame does not exist in the list. 
	Output: None.
*/
int isFrameNameAlreadyExistsInList(FrameNode* head, char* name, char* notFoundMessage)
{
	FrameNode* current = head;
	int count = 1;

	while (current)
	{
		if (EQUAL_STRINGS_VALUE == strcmp(current->frame->name, name))
		{
			return count;
		}
		current = current->next;
		count++;
	}
	if (notFoundMessage)
	{
		printf(NOT_FOUND_MESSAGE);
	}
	return NOT_FOUND;
}

/*
	Function that searches for FrameNode* in FrameNode* list with frame property with given name and returns pointer to it. 
	Input: head - head node inside FrameNode* list.
		   frameName - the name of the frame to search for. 
	Output: FrameNode* of the frame found, or NULL if there is no FrameNode* with frame property with frameName.
*/
FrameNode* findFrameNodeByFrameNameInList(FrameNode* head, char* frameName)
{
	FrameNode* current = head; 
	while (current)
	{
		if (EQUAL_STRINGS_VALUE == strcmp(current->frame->name, frameName))
		{
			return current; 
		}
		current = current->next; 
	}

	return NULL;
}

/*
	Function that removes a FrameNode* from a FrameNode* list by frame's property name.
	Input: head - pointer to FrameNode* list. 
		   frameName - the FrameNode* that has this frame name and has to be deleted.
	Output: None.
*/
void removeFrameNodeFromList(FrameNode** head, char* frameName)
{
	FrameNode* current = *head; 
	FrameNode* temp = NULL;
	if (!*head)
	{
		return;
	}
	else if (EQUAL_STRINGS_VALUE == strcmp((*head)->frame->name, frameName))
	{
		*head = (*head)->next;
		free(current);
		return;
	}
	while (current->next && EQUAL_STRINGS_VALUE != strcmp(current->next->frame->name, frameName))
	{
		current = current->next;
	}
	if (current->next)
	{
		temp = current->next;
		current->next = temp->next;
		free(temp);
		return;
	}
	printf("No such frame with name %s\n", frameName);
}

/*
	Function that deletes the last node of a FrameNode* list. 
	Input: head - pointer to FrameNode* list head node.
	Output: None.
*/
void deleteLastNode(FrameNode** head)
{
	FrameNode* current = *head;
	FrameNode* previous = NULL;

	if (!*head)
	{
		printf("List is empty. Unable to delete last node.\n");
		return;
	}

	if (!(*head)->next)
	{
		free((*head)->frame);
		free(*head);
		*head = NULL;
		return;
	}

	while (current->next != NULL)
	{
		previous = current;
		current = current->next;
	}

	free(current->frame);
	free(current);
	previous->next = NULL;
}

/*
	Function that changes a FrameNode* duration in a FrameNode* list by frame's property name and given new duration value.
	Input: head - pointer to FrameNode* list.
		   frameName - the FrameNode* that has this frame name to change its duration.
		   newDuration - the new duration to set to the FrameNode* with given frame name. 
	Output: None.
*/
void changeFrameNodeDurationInList(FrameNode* head, char* frameName, unsigned int newDuration)
{
	FrameNode* node = findFrameNodeByFrameNameInList(head, frameName);
	if (node)
	{
		node->frame->duration = newDuration;
	}
}

/*
	Function that changes the duration of all FrameNode* in the list.
	Input: head - head node of FrameNode* list. 
		   newDuration - the duration to set to all FrameNode* in the list. 
	Output: None.
*/
void changeAllFrameNodesDurationsInList(FrameNode* head, unsigned int newDuration)
{
	FrameNode* current = head; 
	while (current)
	{
		current->frame->duration = newDuration;
		current = current->next; 
	}
}

/*
	Function that prints FrameNode* list.
	Input: head - FrameNode* list head.
	Output: None.
*/
void printFrameNodeList(FrameNode* head)
{
	FrameNode* current = head;
	printf("                Name            Duration        Path\n");
	while (current)
	{
		printf("                %s               %u ms        %s\n", current->frame->name, current->frame->duration, current->frame->path);
		current = current->next;
	}
	printf("\n");
}

/**
	Function that inserts a given frame to FrameNode* list in a given k position.
	Input: head - pointer to FrameNode* list head node.
		   frame - frame to insert.
		   k - index where to insert.
	Output: None.
*/
void insertFrameAtPositionK(FrameNode** head, Frame* frame, int k)
{
	FrameNode* current = *head; 
	FrameNode* toAdd = createFrameNode(frame); 
	int i = 1;

	if (FIRST_NODE_INDEX == k)
	{
		toAdd->next = *head; 
		*head = toAdd; 
		return;
	}
	for (i = 1; current && i < k; i++)
	{
		current = current->next;
	}
	if (current)
	{
		current->next = toAdd;
	}
}

/**
	Function that deletes FrameNode* in FrameNode* list in a given k position.
	Input: head - pointer to FrameNode* list head node.
		   k - index where to delete.
	Output: None.
*/
void deleteFrameAtPositionK(FrameNode** head, int k)
{
	FrameNode* current = *head;
	FrameNode* temp = NULL;
	int i = 1; 

	if (FIRST_NODE_INDEX == k)
	{
		*head = (*head)->next;
		freeFrameNode(&current);
		return;
	}

	for (i = 1; current && i < k; i++)
	{
		temp = current; 
		current = current->next; 
	}

	if (current->next)
	{
		temp->next = current->next;
	}
	else
	{
		temp->next = NULL;
	}
	if (current)
	{
		freeFrameNode(&current);
	}
}

/*
	Function that changes a FrameNode* position in a FrameNode* list by frame's property name and given position.
	Input: head - pointer to FrameNode* list.
		   frameName - the FrameNode* that has this frame name and has to be changed in position.
		   newPosition - the new position of the FrameNode* in the list.
		   NOTE: the indexing starts from 1 to n, where n is the length of the list.
	Output: None.
*/
void changeFrameNodePosition(FrameNode** head, char* frameName, int newPosition)
{
	Frame* saveFrame = NULL;
	int listLength = frameNodeListLength(*head); 
	int currentIndex = isFrameNameAlreadyExistsInList(*head, frameName, NOT_FOUND_MESSAGE);
	FrameNode* toCopyData = findFrameNodeByFrameNameInList(*head, frameName);
	
	saveFrame = (Frame*)malloc(sizeof(Frame));
	if (!saveFrame)
	{
		printf("Memory allocation error!\n");
		exit(MEMORY_ALLOCATION_ERROR_CODE);
	}

	saveFrame = createFrame(toCopyData->frame->name, toCopyData->frame->duration, toCopyData->frame->path);

	if (!currentIndex)
	{
		return;
	}
	if (currentIndex == newPosition)
	{
		return;
	}
	if (currentIndex < newPosition)
	{
		newPosition--;
	}
	deleteFrameAtPositionK(head, currentIndex);
	insertFrameAtPositionK(head, saveFrame, newPosition);
}