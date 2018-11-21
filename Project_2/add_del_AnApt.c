#include "Header.h"

void addAnApartment(char* data, List *appList) {
	Apartment* newApartment = createAnApt(appList,data,NULL,NULL);

	if (isEmpty(*appList))
	{
		appList->head = appList->tail = newApartment;
		appList->lastApartmentCode = 1;
	}
	else//insert sorted by price
	{
		Apartment* curr = appList->head;
		while (curr != NULL && curr->price < newApartment->price)
			curr = curr->next;
		if (!curr)
			insertApToTail(appList, newApartment);
		else if (curr == appList->head)
			insertApToHead(appList, newApartment);
		else
			insertApToMiddle(appList, newApartment, curr);

		appList->lastApartmentCode++;
	}
}
void insertApToMiddle(List* lst, Apartment* newApartment, Apartment* curr)
{
	newApartment->next = curr;
	newApartment->prev = curr->prev;
	curr->prev->next = newApartment;
	curr->prev = newApartment;
}
void insertApToHead(List* lst, Apartment* newHead) {
	newHead->next = lst->head;
	lst->head->prev = newHead;
	lst->head = newHead;
}
void insertApToTail(List* lst, Apartment* newTail) {
	lst->tail->next = newTail;
	newTail->prev = lst->tail;
	lst->tail = newTail;;
}
int isEmpty(List lst)
{
	return (lst.head == NULL);
}

Apartment* createAnApt(List* lst,char* data, Apartment* next, Apartment* prev)
{
	Apartment* newApartment = (Apartment*)malloc(sizeof(Apartment));
	checkMemoryAllocation(newApartment);
	data += 1;//to eliminate first quotes
	char* token = strtok(data, "\"");

	int countStringParts = 0;

	newApartment->code = makeApartmentCode(lst);

	while (token != NULL) {
		//cases will insert the value of the input  to the list 
		switch (countStringParts)
		{

		case 0://case adress
			newApartment->address = getAdressFromString(token);
			break;
		case 1: //case price
			newApartment->price = getPriceFromString(token);
			break;
		case 2://case num of rooms
			newApartment->numOfRooms = getNumOfRoomsFromStrig(token);
			break;
		case 3://case day
			newApartment->enteryDate.day = atoi(token);
			break;
		case 4://month
			newApartment->enteryDate.month = atoi(token);
			break;
		case 5://year
			newApartment->enteryDate.year = atoi(token);
			newApartment->enteryDate.yearDay = 0;//year day isnt relevant for entery date
			break;
		}
		countStringParts++;
		token = strtok(NULL, " ");
	}

	newApartment->addingDate = getAddingDate();

	newApartment->next = next;
	newApartment->prev = prev;

	return newApartment;
}

int makeApartmentCode(List* lst)
{
	if (isEmpty(*lst))
		return 1;
	return (lst->lastApartmentCode) + 1;
}
Date* getAddingDate() {
	Date* res = (Date*)malloc(sizeof(Date));
	struct tm* info;
	checkMemoryAllocation(res);
	time_t now;
	time(&now);

	info = localtime(&now);


	res->day = info->tm_mday;
	res->month = info->tm_mon + 1;
	res->year = info->tm_year -100;
	res->yearDay = info->tm_yday;

	return res;

}
// function for delete and get
Apartment* findByDay(Apartment* head, int numOfDaysBack, List* lst, int sr)
{
	Apartment* curr = head;
	struct tm info;
	time_t range, appEpoch;
	time(&range);
	range = range - (numOfDaysBack * 24 * 60 * 60);
	while (curr != NULL)
	{
		info.tm_year = (head->addingDate->year)+100;
		info.tm_mon = (head->addingDate->month);
		info.tm_mday = head->addingDate->day;
		info.tm_isdst = -1;
		info.tm_min = info.tm_hour = info.tm_wday = info.tm_yday = info.tm_sec = 0;
		appEpoch = mktime(&info);
		if (range <= appEpoch)
			return curr;
		if (sr == UNDEFINED_FILLTER)
			curr = curr->next;
		else
			curr = curr->prev;
	}
	return NULL;


}

void deleteAnAp(List* lst, char* str)
{
	strtok(str, " ");
	int numOfDaysBack = atoi(strtok(NULL, " ,\n,\0"));
	Apartment* curr = findByDay(lst->head, numOfDaysBack, lst,0);//sr isnt relevant for this part of
	Apartment* temp;
	while (curr)
	{
		temp = curr;
		curr = findByDay(curr->next, numOfDaysBack, lst,1);
		freeApartment(temp, lst);
	}
}

void freeApartment(Apartment* toFree, List* lst)
{
	if (lst->head == lst->tail)
		lst->head = lst->tail = NULL;
	else if (lst->head == toFree)
		lst->head = toFree->next;
	else if (lst->tail == toFree)
		lst->tail = toFree->prev;
	else
	{
		toFree->next->prev = toFree->prev;
		toFree->prev->next = toFree->next;
	}
	freeNow(toFree);
}
void freeNow(Apartment* toFree) {
	free(toFree->addingDate);
	free(toFree->address);
	free(toFree);
}
int getPriceFromString(char* data)
{
	return atoi(data);
}

char* getAdressFromString(char* data) {

	char* temp = (char*)calloc(strlen(data)+1, sizeof(char));
	checkMemoryAllocation(temp);
	strcpy(temp, data);
	return temp;
}

void checkMemoryAllocation(void* ptr)
{
	if (!ptr)
	{
		printf("Memory Allocation Faild");
		exit(1);
	}
}


short int getNumOfRoomsFromStrig(char* data)
{
	return (*data - '0');
}