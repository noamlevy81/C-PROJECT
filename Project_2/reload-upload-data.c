#include "Header.h"

void uploadData(char *textFile, char *binaryFile, HistoryBList historyList, char** short_term_history, List apptList)
{
	uploadToText(textFile, historyList, short_term_history);
	uploadToBinary(binaryFile, apptList);
}

void uploadToText(char* textFile, HistoryBList historyList, char **historyArr)
{
	FILE *f;
	HistoryBNode *curr;
	int i;
	i = N - 1;
	curr = historyList.tail;
	f= fopen(textFile, "wt");
	assert(f);
	while (historyArr[i] == NULL && i>=0)
	{
		i--;
	}
	for ( i; i >=0 ; i--)
	{
		fputs(historyArr[i], f);
		fputc('\n', f);
	}
	while (curr)
	{
		fprintf(f, "%s\n", curr->command);
		curr = curr->prev;
	}
	fclose(f);
}
void uploadToBinary(char* binaryTextName, List dataList)
{
	int adressLength;
	byte data1, data2, data3;
	FILE* f = fopen(binaryTextName, "wb");
	assert(f);
	Apartment* curr = dataList.head;
	while (curr)
	{
		fwrite(&(curr->code), sizeof(int), 1, f);
		adressLength = strlen(curr->address);
		fwrite(&adressLength, sizeof(int), 1, f);
		fwrite(curr->address, sizeof(char), adressLength, f);
		fwrite(&(curr->price), sizeof(int), 1, f);

		createDataBytes(&data1, &data2, &data3,*curr, ENTRY_DATE); // create entry date by byte method


		fwrite(&data1, sizeof(byte), 1, f);
		fwrite(&data2, sizeof(byte), 1, f);
		fwrite(&data3, sizeof(byte), 1, f);

		createDataBytes(&data1, &data2, &data3, *curr, ADDING_DATE); // create entry date by byte method
		

		fwrite(&data1, sizeof(byte), 1, f);
		fwrite(&data2, sizeof(byte), 1, f);
		fwrite(&data3, sizeof(byte), 1, f);
		curr = curr->next;
	}
	fclose(f);
}

void createDataBytes(byte *d1, byte *d2, byte *d3,Apartment curr, int whichDate)
{
	*d1 = *d2 = *d3 = 0;
	if (whichDate == ENTRY_DATE)
	{
		uploadRoomsByte(d1, curr.numOfRooms);
		uploadDayByte(d1, d2, curr.enteryDate.day);
		uploadMonthByte(d2, curr.enteryDate.month);
		uploadYearByte(d2, d3, curr.enteryDate.year);
	}
	else // whichDate == ADDING_DATE
	{
		uploadDayByte(d1, d2, curr.addingDate->day);
		uploadMonthByte(d2, curr.addingDate->month);
		uploadYearByte(d2, d3, curr.addingDate->year);
	}
}

void uploadRoomsByte(byte *d1,short int numOfRooms)
{
	byte temp = (byte)numOfRooms;
	temp <<= 4;
	*d1 = *d1 | temp;
}

void uploadDayByte(byte *d1, byte *d2, short int entryDay)
{
	byte temp1, temp2;
	temp1 = temp2 = (byte)entryDay;
	temp1 >>= 1;
	*d1 = *d1 | temp1;
	temp2 <<= 7;
	*d2 = *d2 | temp2;
}

void uploadMonthByte(byte *d2, short int month)
{
	byte toReturn= (byte)month;
	toReturn <<= 3;
	*d2 |= toReturn;
}
void uploadYearByte(byte *d2, byte *d3, short int entryYear)
{
	*d2 = ((((*d2 >>3)<<3)<< 5) >> 5);
	*d2 |= (byte)(entryYear >> 4);
	*d3 |= (byte)(entryYear << 4);
}

void reloadData( char *textFile, char *binaryFile, HistoryBList *historyList, char *(historyArr[N]), List *apptList)
{
	if (!(isFileExists(HISTORY_FILE) && isFileExists(APARTMETNS_FILE)))
		return;
	reloadApartments(binaryFile, apptList);
	reloadCommandsHistory(textFile, historyList, historyArr);
}
void reloadApartments(char *binaryFile, List *dataList)
{
	FILE *f;
	Apartment *currentApart;
	int fileSize;
	int adressLength;
	short int numOfRooms, day;
	byte month,year;
	byte data1, data2, data3;

	dataList->lastApartmentCode = 0; //initializing
	f = fopen(binaryFile, "rb");
	assert(f);
	fseek(f, 0, SEEK_END);
	fileSize = ftell(f);
	rewind(f);

	while (ftell(f) < fileSize)
	{
		currentApart = (Apartment*)malloc(sizeof(Apartment));
		checkMemoryAllocation(currentApart);

		fread(&(currentApart->code), sizeof(int), 1, f);
		fread(&adressLength, sizeof(int), 1, f);
		currentApart->address = (char*)malloc((adressLength + 1)* sizeof(char));
		checkMemoryAllocation(currentApart->address);

		fread(currentApart->address, sizeof(char), adressLength, f);
		(currentApart->address)[adressLength] = '\0';

		fread(&(currentApart->price), sizeof(int), 1, f);
		//next following 3 fread responsible fir reading number of rooms and entery date
		fread(&data1, sizeof(byte), 1, f);
		fread(&data2, sizeof(byte), 1, f);
		fread(&data3, sizeof(byte), 1, f);

		//converting bytes to data
		numOfRooms = (short int)(data1 >> 4);
		currentApart->numOfRooms = numOfRooms;
		data1 <<= 4;
		data1 >>= 3;
		day = (short int)(data1 | (data2 >> 7));
		month = data2;
		month <<= 1;
		month >>= 4;

		year = data2;
		year <<= 5;
		year >>= 1;
		year |= (data3 >> 4);

		currentApart->enteryDate.day = day;
		currentApart->enteryDate.month = (short int)month;
		currentApart->enteryDate.year= (short int)year;

		/*next 3 following fread responsible for reading adding date*/
		fread(&data1, sizeof(byte), 1, f);
		fread(&data2, sizeof(byte), 1, f);
		fread(&data3, sizeof(byte), 1, f);
	
		data1 <<= 4;
		data1 >>= 3;
		day = (short int)(data1 | (data2 >> 7));

		month = data2;
		month <<= 1;
		month >>= 4;

		year = (data2 << 5);
		year = year >> 1;
		year |= (data3 >> 4);

		currentApart->addingDate = (Date *)malloc(sizeof(Date));
		checkMemoryAllocation(currentApart->addingDate);
		currentApart->addingDate->day = day;
		currentApart->addingDate->month = (short int)month;
		currentApart->addingDate->year = (short int)year;

		currentApart->next = currentApart->prev = NULL;
		if (isEmpty(*dataList))
		{
			dataList->head = dataList->tail = currentApart;
		}
		else
		{
			insertApToTail(dataList, currentApart);
		}
		dataList->lastApartmentCode = max(dataList->lastApartmentCode, currentApart->code);
		
	}
	fclose(f);
}
void reloadCommandsHistory(char *historyFile, HistoryBList *historyList, char *(historyArr[N]))
{
	FILE* f = fopen(historyFile, "rt");
	assert(f);

	char* temp = (char*)malloc(2);
	checkMemoryAllocation(temp);
	char c;
	int fileSize = 0;
	int lsize, psize=2;
	fseek(f, 0, SEEK_END);
	fileSize = ftell(f);
	rewind(f);

	while (fileSize > ftell(f))
	{
		lsize = 0;
		
		c = getc(f);

		while (c != '\n')
		{
			if (lsize == psize)
			{
				psize *= 2;
				temp = (char*)realloc(temp, psize);
				checkMemoryAllocation(temp);
			}
			temp[lsize] = c;
			lsize++;
			c = getc(f);
		}
		temp = realloc(temp, lsize + 1);
		temp[lsize] = '\0';
		reloadToHistory(temp, historyList, historyArr);
		psize = lsize;
	}
	moveElemLeft(historyArr);
	addNumber(historyList);
	free(temp);
	fclose(f);
}

void addNumber(HistoryBList* lst)
{
	HistoryBNode* curr = lst->head;
	int i = 1;
	while (curr)
	{
		curr->numOfCommand = i++;
		curr = curr->next;
	}
}
void reloadToHistory(char* temp, HistoryBList *lst, char** arr)
{
	int i = N-1;
	while (arr[i] != NULL && i>=0)
		i--;
	if (i == 0)
		insertStrToHead(lst, temp);
	else {
		arr[i] = (char*)calloc(strlen(temp) + 1, sizeof(char));
		checkMemoryAllocation(arr[i]);
		strcpy(arr[i], temp);
	}
}

void insertStrToHead(HistoryBList *lst, char *temp)
{
	HistoryBNode *node = createHistoryBNode(temp, NULL, NULL);
	if (isEmptyHistoryBList(*lst))
	{
		lst->head = lst->tail = node;
		node->numOfCommand = 1;
	}
	else
	{
		node->numOfCommand = (lst->tail->numOfCommand) + 1;
		lst->head->prev = node;
		node->next = lst->head;
		lst->head = node;
	}
}
/*in case that we read from history file less then 7 commands*/
void moveElemLeft(char **historyArr)
{
	int i=0;
	int readArr;

	while (historyArr[i] == NULL)
	{
		i++;
	}

	for ( readArr = 0; i < N; i++ ,readArr++)
	{
		historyArr[readArr] = historyArr[i];
	}
	for ( readArr; readArr < N; readArr++)
	{
		historyArr[readArr] = NULL;
	}
}

int isFileExists(char *fname)
{
	FILE *file;
	if ((file = fopen(fname, "r")))
	{
		fclose(file);
		return 1;
	}
	return 0;
}
