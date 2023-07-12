#ifndef LINKEDLISTH
#define LINKEDLISTH

#define FALSE 0
#define TRUE !FALSE

#define INC 1
#define EQUAL_STRINGS_VALUE 0
#define NOT_FOUND 0
#define FIRST_NODE_INDEX 1
#define NULL_CHAR '\0'
#define MEMORY_ALLOCATION_ERROR_CODE 1
#define NOT_FOUND_MESSAGE "The frame does not exist!\n"
#define NO_NOT_FOUND_MESSAGE NULL

// Frame struct
typedef struct Frame
{
	char*		name;
	unsigned int	duration;
	char*		path;  
} Frame;

// Link (node) struct
typedef struct FrameNode
{
	Frame* frame;
	struct FrameNode* next;
} FrameNode;

Frame* createFrame(char* name, unsigned int duration, char* path);

void freeFrame(Frame* frame);

FrameNode* createFrameNode(Frame* frame);

void freeFrameNode(FrameNode** node);

void freeFrameNodeList(FrameNode** head);

int frameNodeListLength(FrameNode* head);

void insertFrameToList(FrameNode** head, Frame* frame);

int isFrameNameAlreadyExistsInList(FrameNode * head, char* name, char* notFoundMessage);

FrameNode* findFrameNodeByFrameNameInList(FrameNode* head, char* frameName);

void removeFrameNodeFromList(FrameNode** head, char* frameName);

void deleteLastNode(FrameNode** head);

void changeFrameNodePosition(FrameNode** head, char* frameName, int newPosition);

void changeFrameNodeDurationInList(FrameNode* head, char* frameName, unsigned int newDuration);

void changeAllFrameNodesDurationsInList(FrameNode* head, unsigned int newDuration);

void printFrameNodeList(FrameNode* head);

#endif
