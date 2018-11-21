#include"Header.h"


//declartion of function (move to header)
void getAnApartment(char* data, List *apartList);
void makeEmptyList(List *lst);
void setDate(char *evacuateDate, char *dateInput);
void printByFillters(List lst, int maxPrice, int MinimumNumOfRooms, int MaximumNumOfRooms, char dateOfEvacuation[9], int sr);
Apartment* validateMaxRooms(Apartment* apartment, int maxRooms);
Apartment* validateMinRooms(Apartment* apartment, int minRooms);
Apartment* validatePrice(Apartment* apartment, int price);
Apartment* validateDateOfEntry(Apartment* apartment, char dateOfEntry[9]);
bool evacuateInTime(Date currentDate, char commandDate[9]);

// The function passing the apartments list and print out the apartments that meet
// the requirement the user ask for.
void getAnApartment(char* data, List *apartList)
{
	int maxPrice, MinNumOfRooms, MaxNumOfRooms, sr;
	char evacuateDate[9] = { 0, 0, 0, 0, 0, 0, 0, 0 }; //initializing
	char* currentFillter;
	char* currentValue;

	maxPrice = MinNumOfRooms = MaxNumOfRooms = sr = UNDEFINED_FILLTER; //initialize the fillters

	currentFillter = strtok(data, "- ");

	if (strcmp(currentFillter, "Enter") == 0) // the user want to print the apartments by the day method
	{
		Apartment* currentAprt;
		currentValue = strtok(NULL, " ");
		currentFillter = strtok(NULL, "- ");
		if (currentFillter != NULL)
		{
			sr = 1;
			currentAprt = findByDay(apartList->tail, atoi(currentValue), apartList, sr);
		}
		else
		{
			currentAprt = findByDay(apartList->head, atoi(currentValue), apartList, sr);
		}
		
		while (currentAprt != NULL)
		{
			printf("Apartment details:\nCode: %d\nAddress: %s\nNumber of rooms: %hi\nPrice: %d\nEntry Date: %d.%d.%d\nDatabase entry date: %d.%d.%d\n\n", currentAprt->code, currentAprt->address, currentAprt->numOfRooms, currentAprt->price, currentAprt->enteryDate.day, currentAprt->enteryDate.month, currentAprt->enteryDate.year, currentAprt->addingDate->day, currentAprt->addingDate->month, currentAprt->addingDate->year);
			if (sr == 1)
				currentAprt = findByDay(currentAprt->prev, atoi(currentValue), apartList, sr);
			else
				currentAprt = findByDay(currentAprt->next, atoi(currentValue), apartList, sr);
		}
	}
	else // the user want to print the apartments by using filters
	{
		while (currentFillter != NULL)
		{
			currentValue = strtok(NULL, " ");
			if (strcmp(currentFillter, "MinimumNumRooms") == 0)
			{
				MinNumOfRooms = atoi(currentValue);
			}
			else if (strcmp(currentFillter, "MaximumNumRooms") == 0)
			{
				MaxNumOfRooms = atoi(currentValue);
			}
			else if (strcmp(currentFillter, "MaximumPrice") == 0)
			{
				maxPrice = atoi(currentValue);
			}
			else if (strcmp(currentFillter, "Date") == 0)
			{
				setDate(evacuateDate, currentValue);
			}
			else if (strcmp(currentFillter, "sr") == 0)
			{
				sr = 1;
			}
			currentFillter = strtok(NULL, "- ");
		}
		printByFillters(*apartList, maxPrice, MinNumOfRooms, MaxNumOfRooms, evacuateDate, sr);
	}
}


// The function set the evacuate date as a filter
void setDate(char *evacuateDate, char *dateInput)
{
	int i = 0;
	int dateSize = strlen(dateInput);
	for ( i = 0; i < dateSize; i++)
	{
		evacuateDate[i] = dateInput[i];
	}
	evacuateDate[dateSize] = '\0';
}

// The function print out the apartments that meet the requirement the user ask for
void printByFillters(List lst, int maxPrice, int MinimumNumOfRooms, int MaximumNumOfRooms, char dateOfEvacuation[9], int sr)
{
	Apartment *currentAprt;
	Apartment *filterCriteria;

	if (sr == UNDEFINED_FILLTER) // print the list from the cheapest to the most expensive
	{
		currentAprt = lst.head;
		while (currentAprt != NULL)
		{
			printf("\n");
			filterCriteria = validateDateOfEntry(validateMaxRooms(validateMinRooms(validatePrice(currentAprt, maxPrice), MinimumNumOfRooms), MaximumNumOfRooms),dateOfEvacuation);
			if (filterCriteria != NULL)
			{
				printf("Apartment details:\nCode: %d\nAddress: %s\nNumber of rooms: %hi\nPrice: %d\nEntry Date: %d.%d.%d\nDatabase entry date: %d.%d.%d\n", currentAprt->code, currentAprt->address, currentAprt->numOfRooms, currentAprt->price, currentAprt->enteryDate.day, currentAprt->enteryDate.month, currentAprt->enteryDate.year, currentAprt->addingDate->day, currentAprt->addingDate->month, currentAprt->addingDate->year);
			}
			currentAprt = currentAprt->next;
		}
	}
	else // print the list from the most expensive to the cheapest 
	{
		currentAprt = lst.tail;
		while (currentAprt != NULL)
		{
			printf("\n");
			filterCriteria = validateDateOfEntry(validateMaxRooms(validateMinRooms(validatePrice(currentAprt, maxPrice), MinimumNumOfRooms), MaximumNumOfRooms), dateOfEvacuation);
			if (filterCriteria != NULL)
			{
				printf("Apartment details:\nCode: %d\nAddress: %s\nNumber of rooms: %hi\nPrice: %d\nEntry Date: %d.%d.%d\nDatabase entry date: %d.%d.%d\n", currentAprt->code, currentAprt->address, currentAprt->numOfRooms, currentAprt->price,currentAprt->enteryDate.day, currentAprt->enteryDate.month,currentAprt->enteryDate.year,currentAprt->addingDate->day,currentAprt->addingDate->month,currentAprt->addingDate->year);
			}
			currentAprt = currentAprt->prev;
		}
	}
}

// The function return the apartment node if the current apartment passing the maximum room filter
// else it'll return NULL
Apartment* validateMaxRooms(Apartment* apartment, int maxRooms)
{
	if (apartment == NULL)
		return NULL;
	if (maxRooms == UNDEFINED_FILLTER)
		return apartment;
	if (apartment->numOfRooms <= maxRooms)
		return apartment;
	else
		return NULL;
}

// The function return the apartment node if the current apartment passing the minimum room filter
// else it'll return NULL
Apartment* validateMinRooms(Apartment* apartment, int minRooms)
{
	if (apartment == NULL)
		return NULL;
	if (minRooms == UNDEFINED_FILLTER)
		return apartment;
	if (apartment->numOfRooms >= minRooms)
		return apartment;
	else
		return NULL;
}

// The function return the apartment node if the current apartment passing the apartment price filter
// else it'll return NULL
Apartment* validatePrice(Apartment* apartment, int price)
{
	if (apartment == NULL)
		return NULL;
	if (price == UNDEFINED_FILLTER)
		return apartment;
	if (apartment->price <= price)
		return apartment;
	else
		return NULL;
}

// The function return the apartment node if the current apartment passing the date of entry filter
// else it'll return NULL
Apartment* validateDateOfEntry(Apartment* apartment, char dateOfEntry[9])
{
	if (apartment == NULL)
		return NULL;
	if (strcmp(dateOfEntry,"\0") == 0)
		return apartment;
	if (evacuateInTime(apartment->enteryDate, dateOfEntry) == true)
		return apartment;
	else
		return NULL;
}

// help function to validate the date of entry
// The function return true if the current apartment evacuate in time the user ask for
// else it'll return false
bool evacuateInTime(Date currentDate, char commandDate[9])
{
	char commandDay[3];
	char commandMonth[3];
	char commandYear[5];

	commandDay[0] = commandDate[0];
	commandDay[1] = commandDate[1];
	commandDay[2] = '\0';
	commandMonth[0] = commandDate[2];
	commandMonth[1] = commandDate[3];
	commandMonth[2] = '\0';
	commandYear[0] = commandDate[4];
	commandYear[1] = commandDate[5];
	commandYear[2] = commandDate[6];
	commandYear[3] = commandDate[7];
	commandYear[4] = '\0';

	if (currentDate.year < (atoi(commandYear) - 2000))
		return true;
	else if (currentDate.year == (atoi(commandYear) - 2000))
		if (currentDate.month < atoi(commandMonth))
			return true;
		else if (currentDate.month == atoi(commandMonth))
			if (currentDate.day <= atoi(commandDay))
				return true;
	return false;
}
