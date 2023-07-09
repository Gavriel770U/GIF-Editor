/*********************************
*       GIF EDITOR PROJECT		 *
*         openCV Template     	 *
**********************************/

#define _CRTDBG_MAP_ALLOC
#define _CRT_SECURE_NO_WARNINGS
#define CV_IGNORE_DEBUG_BUILD_GUARD

#include <crtdbg.h>
#include <opencv2/imgcodecs/imgcodecs_c.h>
#include <stdio.h>
#include <stdlib.h>
#include <opencv2/core/core_c.h>
#include <opencv2/highgui/highgui_c.h>
#include <stdbool.h>
#include "linkedList.h"

#define MAX_STRING_LENGTH 1000
#define INC 1
#define ENTER "\n"
#define NULL_CHAR '\0'
#define FILE_READ_MODE "r"

#define PROJECT_OPTIONS_ERROR_MESSAGE "Invalid choice, try again:\n [0] Create a new project\n [1] Load existing project"

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
	PLAY_GIF_OPTION = 7
} Options;

bool isFileExist(const char* filePath);
/*
	Function that saves the project in the given directory.
	Input: list - FrameNode* list of the frames data.
		   directory - a folder directory in which it is possible to save the project.
		   projectFileName - the file name of the project in which the data shall be saved.
	Output: None.
*/
void saveProject(FrameNode* list, char* directory, char* projectFileName);

/*
	Function that loads a project and returns FrameNode* list that consists of the loaded frames data.
	Input: projectFilePath - the file path of the project to load.
	Output: FrameNode* list of the frames of the project. 
*/
FrameNode* loadProject(char* projectFilePath);

void printMenu(void);

int getIntInput(int minValue, int maxValue, char* errorMessage);

void runGifEditor(void);

int main(void)
{

	runGifEditor();

	/*
	char* name = "1";
	char* path = "C:/Users/Magshimim/Desktop/photos/1.png";
	Frame* frame1 = createFrame(name, 100, path);
	Frame* frame2 = createFrame("2", 50, "C:/Users/Magshimim/Desktop/photos/2.png");
	Frame* frame3 = createFrame("3", 100, "C:/Users/Magshimim/Desktop/photos/3.png");

	FrameNode* list = NULL;

	insertFrameToList(&list, frame1);
	insertFrameToList(&list, frame2); 
	insertFrameToList(&list, frame3);

	printFrameNodeList(list);

	int i = 0;
	cvNamedWindow("Display window", CV_WINDOW_AUTOSIZE); //create a window
	//create an image
	IplImage* image = cvLoadImage("C:/Users/Magshimim/Desktop/photos/1.png", 1);
	if (!image)//The image is empty.
	{
		printf("could not open image\n");
	}
	else
	{
		cvShowImage("Display window", image);
		cvWaitKey(0);
		system("pause");
		cvReleaseImage(&image);
	}
	*/

	//freeFrameNodeList(&list);

	getchar();
	return 0;
}

/*
	Function that checks if file exists. 
	Input: filePath - the path of the file to check if it exists. 
	Output: true if exists, else false.
*/
bool isFileExist(const char* filePath)
{
	FILE* file = NULL;
	file = fopen(filePath, FILE_READ_MODE);
	if (!file)
	{
		return false; 
	}
	fclose(file);
	file = NULL;
	return true; 
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

void runGifEditor(void)
{
	FrameNode* list = NULL;
	Frame* frame = NULL;
	char* path = NULL;
	char* name = NULL;
	unsigned int duration = 0;
	int input = 0;

	printf("Welcome to Magshimim Movie Maker! what would you like to do?\n [0] Create a new project\n [1] Load existing project\n");
	input = getIntInput(NEW_PROJECT_OPTION, LOAD_PROJECT_OPTION, PROJECT_OPTIONS_ERROR_MESSAGE);

	if (NEW_PROJECT_OPTION == input)
	{
		printf("Working on a new project.\n\n");
		
	}

	do
	{
		printMenu();
		scanf("%d", &input);
		getchar();

		if (input < EXIT_OPTION || input > PLAY_GIF_OPTION)
		{
			printf("You should type one of the options - 0-8!\n");
		}
		else
		{
			if (ADD_FRAME_OPTION == input)
			{
				printf("*** Creating new frame ***\n");
				printf("Please insert frame path:\n");
				path = (char*)malloc(sizeof(char) * MAX_STRING_LENGTH);
				if (!path)
				{
					printf("Memory allocation error!\n");
					exit(MEMORY_ALLOCATION_ERROR_CODE);
				}
				fgets(path, MAX_STRING_LENGTH, stdin);
				path[strcspn(path, ENTER)] = NULL_CHAR;

				printf("Please insert frame duration (in miliseconds):\n");
				scanf("%u", &duration);
				getchar();

				printf("Please choose a name for that frame:\n");
				name = (char*)malloc(sizeof(char) * MAX_STRING_LENGTH);
				if (!name)
				{
					printf("Memory allocation error!\n");
					exit(MEMORY_ALLOCATION_ERROR_CODE);
				}
				fgets(name, MAX_STRING_LENGTH, stdin);
				name[strcspn(name, ENTER)] = NULL_CHAR;

				if (!isFileExist(path))
				{
					printf("Can't find file! Frame will not be added\n");
					free(path);
					free(name);
				}
				else
				{
					frame = createFrame(name, duration, path);
					insertFrameToList(&list, frame);
				}
			}
			else if (REMOVE_FRAME_OPTION == input)
			{

			}
			else if (CHANGE_FRAME_POSITION_OPTION == input)
			{

			}
			else if (CHANGE_FRAME_DURATION_OPTION == input)
			{

			}
			else if (CHANGE_ALL_FRAMES_DURATION_OPTION == input)
			{

			}
			else if (PRINT_ALL_FRAMES_LIST_OPTION == input)
			{
				printFrameNodeList(list);
			}
			else if (PLAY_GIF_OPTION == input)
			{
				play(list);
			}
		}
		printf("\n");
	} while (input != EXIT_OPTION);


	printf("Bye!\n");
}