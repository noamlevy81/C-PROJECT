#include "Header.h"

void PROJECT100(HistoryBList* historyList, char** short_term_history, char* command, List* apptList)
{

	
	printf("\nPlease enter one of the following commands:\nadd-an-apt\nget-an-apt\nbuy-an-apt\ndelete-an-apt.\nFor reconstruction commands, please enter : !!\n!num\nhistory\nshort_history\n!num^str1^str\nTo exit, enter exit.\n");
	printf(">> ");
	command = getCommand();
	while (strcmp(command, "exit"))
	{
		if (strcmp(command, "\0") != 0)
		{
			if (isHistoryCommand(command)) //send command to appropriate function 
			{
				historyCommandManger(command, apptList, short_term_history, historyList);
			}
			else
			{
				addToHistory(command, historyList, short_term_history);
				commandManger(command, apptList);
			}
		}
		//printf("\nPlease enter one of the following commands:add-an-apt, get-an-apt, buy-an-apt or delete-an-apt. For reconstruction commands, please enter : !!, !num, history, short_history or !num^str1^str2 To exit, enter exit.\n");
		printf(">> ");
		command = getCommand();
	}
	puts("Good Bye!");
}

void freeAll(List* apList, HistoryBList* historyList, char** short_term_history) {
	freeApartmentList(apList);
	freeHistoryList(historyList);
	freeHistoryArr(short_term_history);
}

void freeHistoryArr(char** shortHistory)
{
	int i;
	for (i = 0; i < N; i++)
	{
		if (!shortHistory[i])
			free(shortHistory[i]);
	}
}
void freeHistoryList(HistoryBList* history)
{
	if (isEmptyHistoryBList(*history))
		return;

	HistoryBNode* curr = history->head;
	HistoryBNode* temp;
	while (curr)
	{
		temp = curr;
		curr = curr->next;
		free(temp);
	}
}
void freeApartmentList(List *apList)
{
	if (isEmpty(*apList))
		return;
	Apartment* curr = apList->head;
	Apartment* temp;
	while (curr)
	{
		temp = curr;
		curr = curr->next;
		freeNow(temp);

	}
}

char * getCommand()
{
	int lsize = 0, phsize = 1;
	char ch;
	char* str = (char *)malloc(sizeof(char));
	
	ch =getchar();
	while (ch != ENTER)
	{
		if (lsize == phsize)
		{
			phsize *= 2;
			str = (char *)realloc(str, (sizeof(char)*(phsize + 1)));
		}
		str[lsize] = ch;
		lsize++;
		ch = getchar();
	}
	if (lsize != 0)
	{
		str = (char *)realloc(str, (lsize) * sizeof(char));
	}

	str[lsize] = '\0';
	return str;
}

void commandManger(char *str, List* apptList)
{

	char* possibleCommands[4] = { "add-an-apt","get-an-apt", "delete-an-apt", "buy-an-apt" };
	char* currentCommand;
	
	currentCommand = strtok(str, " ");

	str = str + strlen(currentCommand) + 1;
	if (!strcmp(currentCommand, possibleCommands[0]))
	{
		addAnApartment(str, apptList);
	}
	else if (!strcmp(currentCommand, possibleCommands[1]))
	{
		getAnApartment(str, apptList);
	}
	else if (!strcmp(currentCommand, possibleCommands[2]))
	{
		deleteAnAp(apptList, str);
	}
	else if (!strcmp(currentCommand, possibleCommands[3]))
	{
		buyAnApartment(apptList, str);
	}

}

void makeEmptyList(List* lst)
{
	lst->head = lst->tail = NULL;
}