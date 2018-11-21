#include "Header.h"


void main() {
	char *command = NULL;
	List apptList;
	HistoryBList historyList;
	char *(short_term_history[N]);
	
	initializeHistoryArr(short_term_history);
	makeEmptyList(&apptList);
	makeEmptyHistoryBList(&historyList);

	reloadData(HISTORY_FILE, APARTMETNS_FILE, &historyList, short_term_history, &apptList);

	PROJECT100(&historyList, short_term_history, command,&apptList); //getting commands until exit
	
	uploadData(HISTORY_FILE, APARTMETNS_FILE, historyList, short_term_history, apptList); 
																					 	 
	freeAll(&apptList, &historyList, short_term_history);
	
	system("pause");
}

