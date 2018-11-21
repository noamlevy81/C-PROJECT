#include "Header.h"

// The function remove the apartment from the apartments list by the apartment code
void buyAnApartment(List *apartList, char *apartmentCode)
{
	strtok(apartmentCode, " ");
	int apartCode = atoi(apartmentCode);
	Apartment *current = apartList->head;
	while (current != NULL)
	{
		if (current->code == apartCode)
		{
			freeApartment(current, apartList);
			return;
		}
		current = current->next;
	}
}