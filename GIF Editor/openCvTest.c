/*********************************
*       GIF EDITOR PROJECT		 *
*         openCV Template     	 *
**********************************/

/* TODO:
* 1.5. change position of node function, I got stuck with this function at the end :(
*/

#define _CRT_SECURE_NO_WARNINGS
#define CV_IGNORE_DEBUG_BUILD_GUARD

#include <opencv2/imgcodecs/imgcodecs_c.h>
#include <stdio.h>
#include <stdlib.h>
#include <opencv2/core/core_c.h>
#include <opencv2/highgui/highgui_c.h>
#include <stdbool.h>
#include "linkedList.h"
#include "view.h"

#define MAX_STRING_LENGTH 1000
#define INC 1
#define ONE_ELEMENT 1
#define ENTER "\n"
#define NULL_CHAR '\0'
#define FILE_READ_MODE "r"
#define WRITE_BINARY_MODE "wb"
#define READ_BINARY_MODE "rb"
#define BIN_EXTENSION ".bin"

#define PROJECT_OPTIONS_ERROR_MESSAGE "Invalid choice, try again:\n [0] Create a new project\n [1] Load existing project"
#define CHANGE_INDEX_ERROR_MESSAGE "The movie contains less frames!\nEnter the new index in the movie you wish to place the frame\n"

typedef enum ProjectOptions
{
	NEW_PROJECT_OPTION = 0,
	LOAD_PROJECT_OPTION = 1
} ProjectOptions;

typedef enum Options
{
	EXIT_OPTION = 0, 
	ADD_FRAME_OPTION = 1, 
	REMOVE_FRAME_OPTION = 2, 
	CHANGE_FRAME_POSITION_OPTION = 3,
	CHANGE_FRAME_DURATION_OPTION = 4,
	CHANGE_ALL_FRAMES_DURATION_OPTION = 5,
	PRINT_ALL_FRAMES_LIST_OPTION = 6,  
	PLAY_GIF_OPTION = 7,
	SAVE_PROJECT_OPTION = 8
} Options;

void improvedFgets(char* buffer, int maxCount, FILE* stream);

bool isFileExist(const char* filePath, const char* readMode);

char* createFullPath(char* folderDirectory, char* projectFileName, char* extension);

void saveProject(FrameNode* list, char* directory, char* projectFileName);

FrameNode* loadProject(char* projectFilePath);

void printMenu(void);

int getIntInput(int minValue, int maxValue, char* errorMessage);

void stringInput(char** buffer);

void runGifEditor(void);

int main(void)
{
	runGifEditor();
	getchar();
	return 0;
}

/*
*	Function that serves like better fgets. 
*	Input: buffer - the buffer where the input will be stored.
*		   maxCount - the maximum length of the input.
*		   stream - the stream where the input will be inputed.
*	Output: None.
*/
void improvedFgets(char* buffer, int maxCount, FILE* stream)
{
	fgets(buffer, maxCount, stream);
	buffer[strcspn(buffer, ENTER)] = NULL_CHAR;
}

/*
	Function that checks if file exists. 
	Input: filePath - the path of the file to check if it exists. 
		   readMode - the read mode of the file. 
	Output: true if exists, else false.
*/
bool isFileExist(const char* filePath, const char* readMode)
{
	FILE* file = NULL;
	file = fopen(filePath, readMode);
	if (!file)
	{
		return false; 
	}
	fclose(file);
	file = NULL;
	return true; 
}

/**
*	Function that creates full path to a file using given folder directory, project file name and extenstion.
*   Input: folderDirectory - the folder directory of the file.
*			projectFileName - the file name of the project.
*			extension - the file extesntion of the project.
*	Output: full path that has been created according to given parameters.
*/
char* createFullPath(char* folderDirectory, char* projectFileName, char* extension)
{
	char* fullPath = NULL;
	int totalLength = strlen(folderDirectory) + strlen(projectFileName) + strlen(extension) + INC;

	fullPath = (char*)malloc(sizeof(char) * totalLength);
	if (!fullPath)
	{
		printf("Memory allocation error!\n");
		exit(MEMORY_ALLOCATION_ERROR_CODE);
	}
	fullPath[totalLength] = NULL_CHAR;

	strcpy(fullPath, folderDirectory);
	strcat(fullPath, projectFileName);
	strcat(fullPath, extension);

	return fullPath;
}

/**
*	Function that writes frame data to the file that saves the project in a file,
*	so it will be possible to load this project in the future.
*	Input: frame - the frame to write into the file. 
*		   file - the file in which the frame data will be stored.
*	Output: None.
*/
void writeFrameToFile(const Frame* frame, FILE* file)
{
	size_t nameLength = 0, pathLength = 0;
	
	nameLength = strlen(frame->name) + INC;
	fwrite(&nameLength, sizeof(size_t), ONE_ELEMENT, file);
	fwrite(frame->name, sizeof(char), nameLength, file);

	fwrite(&(frame->duration), sizeof(unsigned int), ONE_ELEMENT, file);

	pathLength = strlen(frame->path) + INC;
	fwrite(&pathLength, sizeof(size_t), ONE_ELEMENT, file);
	fwrite(frame->path, sizeof(char), pathLength, file);
}

/*
	Function that saves the project in the given directory.
	Input: list - FrameNode* list of the frames data.
		   directory - a folder directory in which it is possible to save the project.
		   projectFileName - the file name of the project in which the data shall be saved.
	Output: None.
*/
void saveProject(FrameNode* list, char* directory, char* projectFileName)
{
	FrameNode* current = list; 
	FILE* file = NULL;
	char* fullPath = createFullPath(directory, projectFileName, BIN_EXTENSION);
	file = fopen(fullPath, WRITE_BINARY_MODE);

	if (!file)
	{
		printf("Could not open file!\n");
		exit(MEMORY_ALLOCATION_ERROR_CODE);
	}

	while (current) 
	{
		writeFrameToFile(current->frame, file);
		current = current->next;
	}

	fclose(file);
}

/*
	Function that loads a project and returns FrameNode* list that consists of the loaded frames data.
	Input: projectFilePath - the file path of the project to load.
	Output: FrameNode* list of the frames of the project.
*/
FrameNode* loadProject(char* projectFilePath)
{
	FrameNode* list = NULL;
	FrameNode* current = list; 
	FILE* file = NULL;
	Frame* frame = NULL;

	file = fopen(projectFilePath, READ_BINARY_MODE);
	if (!file)
	{
		printf("Could not open file!\n");
		exit(MEMORY_ALLOCATION_ERROR_CODE);
	}

	// Read the frames from the file
	while (!feof(file))
	{
		frame = (Frame*)malloc(sizeof(Frame));

		size_t nameLength = 0;
		fread(&nameLength, sizeof(size_t), 1, file);
		frame->name = malloc(nameLength);
		fread(frame->name, sizeof(char), nameLength, file);

		fread(&(frame->duration), sizeof(unsigned int), 1, file);

		size_t pathLength = 0;
		fread(&pathLength, sizeof(size_t), 1, file);
		frame->path = malloc(pathLength);
		fread(frame->path, sizeof(char), pathLength, file);

		insertFrameToList(&list, frame);
	}

	// delete the last node that contains trash information.
	deleteLastNode(&list);

	fclose(file); 
	return list; 
}

/*
	Function that prints the menu of the program.
	Input: None.
	Output: None.
*/
void printMenu(void)
{
	printf("What would you like to do ?\n");
	printf("	[0] Exit\n");
	printf("	[1] Add new Frame\n");
	printf("	[2] Remove a Frame\n");
	printf("	[3] Change frame index\n");
	printf("	[4] Change frame duration\n");
	printf("	[5] Change duration of all frames\n");
	printf("	[6] List frames\n");
	printf("	[7] Play movie!\n");
	printf("	[8] Save project\n");
}

/*
	Function that gets int user input while the input is wrong according to given range. 
	When the input is valid the function returns the given value. 
	Input: minValue - the minimum valid value of the input.
		   maxValue - the maximum valid value of the input.
		   errorMessage - the error message that the user should get on wrong input that is out the given range.
	Output: the value of the user input that is in the range from minValue to maxValue inclusive.
*/
int getIntInput(int minValue, int maxValue, char* errorMessage)
{
	int input = 0;
	do
	{
		scanf("%d", &input);
		getchar();
		if ((input > maxValue || input < minValue))
		{
			printf("%s\n", errorMessage);
		}
	} while (input > maxValue || input < minValue);

	return input;
}

/*
	Function that gets char* input from the user. 
	Input: buffer - a pointer to char* where the inputed data will be stored.
	Output: None.
*/
void stringInput(char** buffer)
{
	if (*buffer)
	{
		free(*buffer);
		*buffer = NULL;
	}
	*buffer = (char*)malloc(sizeof(char) * MAX_STRING_LENGTH);
	if (!*buffer)
	{
		printf("Memory allocation error!\n");
		exit(MEMORY_ALLOCATION_ERROR_CODE);
	}
	improvedFgets(*buffer, MAX_STRING_LENGTH, stdin);
}

/**
*	Function that runs the gif editors code.
*	Input: None.
*	Output: None.
*/
void runGifEditor(void)
{
	FrameNode* list = NULL;
	Frame* frame = NULL;
	char* path = NULL;
	char* name = NULL;
	char* folderDirectory = NULL;
	char* projectName = NULL;
	char* projectPath = NULL;
	unsigned int duration = 0;
	int input = 0;
	int index = 0;

	printf("Welcome to Magshimim Movie Maker! what would you like to do?\n [0] Create a new project\n [1] Load existing project\n");
	input = getIntInput(NEW_PROJECT_OPTION, LOAD_PROJECT_OPTION, PROJECT_OPTIONS_ERROR_MESSAGE);

	if (NEW_PROJECT_OPTION == input)
	{
		printf("Working on a new project.\n\n");
	}
	else
	{
		printf("Insert the project file path from which the data will be loaded:\n");
		stringInput(&projectPath);
		if (isFileExist(projectPath, READ_BINARY_MODE))
		{
			list = loadProject(projectPath);
		}
		else
		{
			printf("Error! Cannot open file, creating a new project!\n\n"); 
		}
	}

	do
	{
		printMenu();
		scanf("%d", &input);
		getchar();

		if (input < EXIT_OPTION || input > SAVE_PROJECT_OPTION)
		{
			printf("You should type one of the options - 0-8!\n");
		}
		else
		{
			if (ADD_FRAME_OPTION == input)
			{
				printf("*** Creating new frame ***\n");
				printf("Please insert frame path:\n");
				stringInput(&path);

				printf("Please insert frame duration (in miliseconds):\n");
				scanf("%u", &duration);
				getchar();

				printf("Please choose a name for that frame:\n");
				stringInput(&name);

				if (!isFileExist(path, FILE_READ_MODE))
				{
					printf("Can't find file! Frame will not be added\n");
					free(path);
					free(name);
				}
				else
				{
					while (isFrameNameAlreadyExistsInList(list, name, NO_NOT_FOUND_MESSAGE))
					{
						printf("The name is already taken, please enter another name:\n");
						stringInput(&name);
					}
					frame = createFrame(name, duration, path);
					insertFrameToList(&list, frame);
				}
			}
			else if (REMOVE_FRAME_OPTION == input)
			{
				printf("Enter the name of the frame to remove: \n");
				stringInput(&name);

				removeFrameNodeFromList(&list, name);
			}
			else if (CHANGE_FRAME_POSITION_OPTION == input)
			{
				printf("Enter the name of the frame: \n");
				stringInput(&name);

				if (isFrameNameAlreadyExistsInList(list, name, NOT_FOUND_MESSAGE))
				{
					printf("Enter the new index in the movie you wish to place the frame\n");
					index = getIntInput(FIRST_NODE_INDEX, frameNodeListLength(list), CHANGE_INDEX_ERROR_MESSAGE);
					changeFrameNodePosition(&list, name, index);
				}
				else
				{
					free(name);
					name = NULL;
				}

			}
			else if (CHANGE_FRAME_DURATION_OPTION == input)
			{
				printf("Enter the name of the frame: \n");
				stringInput(&name);
				
				printf("Enter the new duration:\n");
				scanf("%u", &duration);
				getchar();

				changeFrameNodeDurationInList(list, name, duration);
			}
			else if (CHANGE_ALL_FRAMES_DURATION_OPTION == input)
			{
				printf("Enter the duration for all frames:\n");
				scanf("%u", &duration);
				getchar();

				changeAllFrameNodesDurationsInList(list, duration);
			}
			else if (PRINT_ALL_FRAMES_LIST_OPTION == input)
			{
				printFrameNodeList(list);
			}
			else if (PLAY_GIF_OPTION == input)
			{
				play(list);
			}
			else if (SAVE_PROJECT_OPTION == input)
			{
				printf("Enter folder directory to save the project in it: \n");
				stringInput(&folderDirectory); 
				printf("Enter name for the project file: \n");
				stringInput(&projectName);

				saveProject(list, folderDirectory, projectName);

				free(folderDirectory); 
				folderDirectory = NULL;
				free(projectName);
				projectName = NULL;
			}
		}
		printf("\n");
	} while (input != EXIT_OPTION);

	freeFrameNodeList(&list);

	printf("Bye!\n");
}