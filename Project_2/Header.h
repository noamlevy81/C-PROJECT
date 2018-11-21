#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>

#define ENTER '\n'
#define UNDEFINED_FILLTER -1
#define N 7
#define byte unsigned char 
#define ENTRY_DATE 1
#define ADDING_DATE 0
#define HISTORY_FILE "ComamndHistory.txt"
#define APARTMETNS_FILE "Apartments.bin"


typedef struct date {
	short int day;
	short int month;
	short int year;
	short int yearDay;
}Date;
typedef struct apartment {

	int code;
	char* address;
	int price;
	short int numOfRooms;
	Date enteryDate;
	Date* addingDate;
	struct apartment* next;
	struct apartment* prev;
}Apartment;

typedef struct list {
	Apartment* head;
	Apartment* tail;
	int lastApartmentCode;
}List;


typedef struct HistoryBNode {
	char* command;
	int numOfCommand;
	struct HistoryBNode *next;
	struct HistoryBNode *prev;
}HistoryBNode;

typedef struct HistoryBList {
	HistoryBNode *head;
	HistoryBNode *tail;
}HistoryBList;


void makeEmptyList(List* lst);

/*4 major functions*/
void addAnApartment(char* data, List *appList);
void deleteAnAp(List* lst, char* str);
void getAnApartment(char* data, List *apartList);
void buyAnApartment(List *apartList, char *apartmentCode);


Apartment* findByDay(Apartment* head, int numOfDaysBack, List* lst, int sr);


//free prototype
void freeApartment(Apartment* toFree, List* lst);
void freeNow(Apartment* toFree);
void freeApartmentList(List *apList);
void freeHistoryList(HistoryBList* history);
void freeHistoryArr(char** shortHistory);
void freeAll(List *apptList, HistoryBList* historyList, char** short_term_history);

//history prototype
bool isEmptyHistoryBList(HistoryBList lst);
void makeEmptyHistoryBList(HistoryBList* lst);
HistoryBNode* createHistoryBNode(char *str, HistoryBNode *next, HistoryBNode *prev);
void insertDataToEndList(HistoryBList *lst, char *command);
void insertNodeToTail(HistoryBList *lst, HistoryBNode *node);
void initializeHistoryArr(char  **arr);
void addToHistoryArr(char* command, char** historyArr);
void addToHistory(char*command, HistoryBList* lst, char** short_term_history);
void addCommanToHistoryArr(char* command, char** commandArr);
void applyLastCommand(char** historyA, List *apartList, HistoryBList* historyB);
void addToHistoryList(char* command, HistoryBList* lst);
char* applyByNumOfCommand(HistoryBList* lst, char** historyA, char* userInput);
void printShortHistory(char** history, HistoryBList* historyB);
void printAllHistory(char** history, HistoryBList* historyB);
char* applyChangedCommand(char** history, HistoryBList* lst, char* str);
char *str_replace(char *orig, char *rep, char *with);

//global prototypes
void commandManger(char *str, List* apptList);
void historyCommandManger(char *command, List* apptList, char **historyA, HistoryBList *historyB);
void checkMemoryAllocation(void* ptr);
bool isHistoryCommand(char *command);
void PROJECT100(HistoryBList* historyList, char** short_term_history, char* command, List* apptList);
char* getCommand();
void checkMemoryAllocation(void* ptr);

// reload-upload functions
int isFileExists(char *fname);
void uploadData(char *textFile, char *binaryFile, HistoryBList historyList, char** short_term_history, List apptList);
void createDataBytes(byte *d1, byte *d2, byte *d3, Apartment curr, int whichDate);
void uploadRoomsByte(byte *d1, short int numOfRooms);
void uploadDayByte(byte *d1, byte *d2, short int entryDay);
void uploadMonthByte(byte *d2,short int entryMonth);
void uploadYearByte(byte *d2, byte *d3, short int entryYear);
void uploadToBinary(char* binaryTextName, List dataList);
void uploadToText(char* textFile, HistoryBList historyList, char **historyArr);

void reloadData(char *textFile, char *binaryFile, HistoryBList *historyList, char *(historyArr[N]), List *apptList);
void reloadApartments(char *binaryFile, List *dataList);
void reloadCommandsHistory(char *historyFile, HistoryBList *historyList, char *(historyArr[N]));
void reloadToHistory(char* temp, HistoryBList *lst, char** arr);
void insertStrToHead(HistoryBList *lst, char *temp);
void moveElemLeft(char **historyArr);
void addNumber(HistoryBList* lst);



//sub-functions
char* getAdressFromString(char* data);
int getPriceFromString(char* data);
short int getNumOfRoomsFromStrig(char* data);
Apartment* createAnApt(List* lst, char* data, Apartment* next, Apartment* prev);
Apartment* findByDay(Apartment* head, int numOfDaysBack, List* lst, int sr);
Date* getAddingDate();
void insertApToMiddle(List* lst, Apartment* newApartment, Apartment* curr);
void insertApToHead(List* lst, Apartment* newHead);
void insertApToTail(List* lst, Apartment* newTail);
int makeApartmentCode(List* lst);
void makeEmptyList(List* lst);
void freeApartment(Apartment* toFree, List* lst);
void freeNow(Apartment* toFree);
int isEmpty(List lst);

