#include "Header.h"


void makeEmptyHistoryBList(HistoryBList* lst) // The function make empty history list
{
	lst->head = lst->tail = NULL;
}

bool isEmptyHistoryBList(HistoryBList lst) //The function get list and return true if it empty, else i'll return false
{
	return (lst.head == NULL);
}

HistoryBNode* createHistoryBNode(char *str, HistoryBNode *next, HistoryBNode *prev)
{
	HistoryBNode *res = (HistoryBNode *)malloc(sizeof(HistoryBNode));
	checkMemoryAllocation(res);
	res->command = (char*)calloc((strlen(str)+1),sizeof(char));
	checkMemoryAllocation(res->command);
	strcpy(res->command,str);
	res->numOfCommand = UNDEFINED_FILLTER; //initializing
	res->next = next;
	res->prev = prev;
	return res;
}

void freeList(HistoryBList *lst) // The function free the allocations from the memory
{
	HistoryBNode *curr, *after;
	curr = lst->head;
	after = curr->next;
	while (after != NULL)
	{
		free((curr->command));
		free(curr);
		curr = after;
		after = after->next;
	}
	curr = after = NULL;
}

void insertDataToEndList(HistoryBList *lst, char *command) //The function get pointer to list and char and insert it to the end of the list
{
	HistoryBNode *new_tail;
	new_tail = createHistoryBNode(command, NULL, NULL);
	insertNodeToTail(lst, new_tail);
}

void insertNodeToTail(HistoryBList *lst, HistoryBNode *node)
{
	if (isEmptyHistoryBList(*lst))
	{
		lst->head = lst->tail = node;
		node->numOfCommand = 1;
	}
	else
	{
		node->numOfCommand = (lst->tail->numOfCommand) + 1;
		lst->tail->next = node;
		node->prev = lst->tail;
		lst->tail = node;
	}
}

void addToHistory(char*command, HistoryBList* lst, char** short_term_history)
{
	int i = 0;
	while (*(short_term_history+i) != NULL && i<N)
		i++;
	if (i == N) {//first add short_term_history[0] to 2nd database and then add new command to arr

		addToHistoryList(*short_term_history, lst);
		addCommanToHistoryArr(command, short_term_history);
	}
	else
		addToHistoryArr(command, short_term_history + i);

}

void addCommanToHistoryArr(char* command, char** commandArr)
{
	int i;
	for (i = 1; i < N; i++)
		commandArr[i - 1] = commandArr[i];
	i -= 1;//i=6
	commandArr[i] = (char*)calloc(strlen(command) + 1, sizeof(char));
	checkMemoryAllocation(commandArr[i]);
	strcpy(commandArr[i], command);
	
	
}
void addToHistoryList(char* command, HistoryBList* lst)
{
	insertDataToEndList(lst,command);
}
void addToHistoryArr(char* command, char** historyArr) 
{
	*historyArr = (char*)calloc((strlen(command) + 1), sizeof(char));
	checkMemoryAllocation(*historyArr);
	strcpy(*historyArr, command);

}

void initializeHistoryArr(char **arr)
{
	int i = 0;
	for (i; i < N; i++)
		(arr)[i] = NULL;
	
}

char* applyByNumOfCommand(HistoryBList* lst, char** historyA, char* userInput)
{
	char* temp = NULL;
	int numOfCommand = atoi(userInput);
	HistoryBNode* curr = lst->head;
	// if userInput is in the list
	if ((!isEmptyHistoryBList(*lst)) && numOfCommand >= lst->head->numOfCommand && numOfCommand <= lst->tail->numOfCommand)
	{
		while (curr->numOfCommand != numOfCommand)
			curr = curr->next;
		temp = (char*)calloc(strlen(curr->command) + 1, sizeof(char));
		checkMemoryAllocation(temp);
		strcpy(temp, curr->command);
	}
	
	else // userInput is in the array
	{
		if (!isEmptyHistoryBList(*lst))// if user input is greater then 7
		{
			numOfCommand -= lst->tail->numOfCommand;
		}
		temp = (char*)calloc(strlen(historyA[numOfCommand - 1]) + 1, sizeof(char));
		checkMemoryAllocation(temp);
		strcpy(temp, historyA[numOfCommand - 1]);

		
	
	}
	return temp;
}

char* applyChangedCommand(char** history, HistoryBList* lst, char* str)
{
	char* token = strtok(str, "^");
	char* toReplace = strtok(NULL, "^");
	char* with = strtok(NULL, "^");
	return str_replace(applyByNumOfCommand(lst, history, token), toReplace, with);
}

char *str_replace(char *orig, char *rep, char *with) {
	char *result; 
	char *ins;    // the next insert point
	char *tmp;    // varies
	int len_rep;  // length of the string to remove
	int len_with; // length of the string to replace rep with
	int len_front; // distance between rep and end of last rep
	int count=0;    // number of replacements
	char* helper = orig;
	while (*helper != ' ') {//getting the first part of the command size
		helper++;
		count++;
	}
	helper = (char*)calloc(count + 1, sizeof(char));
	checkMemoryAllocation(helper);
	
	strncpy(helper, orig, count);

	if (!orig || !rep)
		return NULL;
	len_rep = strlen(rep);
	if (len_rep == 0)
		return NULL; 
	if (!with)
		with = "";
	len_with = strlen(with);

	// count the number of replacements needed
	ins = orig;
	for (count = 0; tmp = strstr(ins, rep); ++count) 
		ins = tmp + len_rep;
	
	tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);
	checkMemoryAllocation(result);
	
	while (count--) {
		ins = strstr(orig, rep);
		len_front = ins - orig;
		tmp = strncpy(tmp, orig, len_front) + len_front;
		tmp = strcpy(tmp, with) + len_with;
		orig += len_front + len_rep; // move to next "end of rep"
	}
	strcpy(tmp, orig);
	strncpy(result, helper, strlen(helper));
	free(helper);
	return result;
}

void printAllHistory(char** history, HistoryBList* historyB)
{

	int commandIndex = 0;
	int i;
	HistoryBNode* curr = historyB->head;
	printf("\nYour Commands History is:\n");
	while (curr)
	{
		printf("\n%3d. %s", ++commandIndex, curr->command);
		curr = curr->next;
	}
	for (i = 0; i < N; i++) {
		if (history[i] != NULL)
			printf("\n%3d. %s", ++commandIndex, history[i]);
	}
	printf("\n");
}
void printShortHistory(char** history, HistoryBList* historyB)
{
	int i = 0;
	printf("\nYour 7 last commands are:\n");
	for (i = 0; i < N; i++) {
		if (history[i] != NULL)
			printf("\n%d. %s",historyB->tail->numOfCommand+ i + 1, history[i]);
	}
	printf("\n");
}

void applyLastCommand(char** historyA, List *apartList, HistoryBList* historyB)
{
	int i=0;
	char* temp;
	while (historyA[i] != NULL && i<N)
	{
		i++;
	}
	i--;
	addToHistory(historyA[i], historyB, historyA);
	temp = (char*)calloc(strlen(historyA[i]) + 1, sizeof(char));
	checkMemoryAllocation(temp);
	strcpy(temp, historyA[i]);
	commandManger(temp, apartList); 
	free(temp);
}

bool isHistoryCommand(char *command) // The function return true if the command is history command type, else it'll return false
{
	if (!strcmp(command, "!!") || (command[0] == '!' && isdigit(command[1])) || !strcmp(command, "short_history") || !strcmp(command, "history"))
		return true;
	else
		return false;
}

void historyCommandManger(char *command, List* apptList, char **historyA, HistoryBList *historyB)
{
	if (!strcmp(command, "!!"))
	{
		applyLastCommand(historyA, apptList,historyB);
	}
	else if (!strcmp(command, "short_history"))
	{
		printShortHistory(historyA,historyB);
	}
	else if (!strcmp(command, "history"))
	{
		printAllHistory(historyA, historyB);
	}
	else
	{
		int num;
		char ch;
		char *currentCommand;
		sscanf(command + 1, "%d %c", &num, &ch);
		if (ch == '^')
		{
			currentCommand = applyChangedCommand(historyA, historyB, command+1);
		}
		else
		{
			currentCommand = applyByNumOfCommand( historyB, historyA, command+1);
		}
		addToHistory(currentCommand, historyB, historyA);
		commandManger(currentCommand, apptList);
		free(currentCommand);
	}
}

