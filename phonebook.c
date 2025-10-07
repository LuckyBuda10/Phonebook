#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

/*
  Determines the operating system of the user, used in the system call to clear the terminal
 */
#if defined(_WIN32) || defined(_WIN64)
    #define OS_NAME "WINDOWS"
#elif defined(__linux__)
    #define OS_NAME "LINUX"
#else
    #define OS_NAME "UNKNOWN"
#endif

/*
  Represents a contact in a phonebook
 
  name can hold 64 bytes
  phone can hold 32 bytes
 */
typedef struct Contact {
	char * name;
	char * phone;
} Contact;

/*
  Adds a new contact to the "phonebook", the list of all contacts
  Returns true if sucessful, false otherwise

  contactList is a double pointer representing the "phonebook", and is reallocated each call to hold an additional contact
  contactCount is the number of contacts, this function increments it by 1 due to new contact being added
 */
bool addToPhoneBook(Contact ** contactList, int * contactCount);

/*
  Displays all contacts in the contactList

  contactList is a pointer holding every contact added by using -a input
  contactCount represents the total number of contacts
 */
void displayAllContacts(Contact * contactList, int contactCount);

/*
  Prints given Contact in the form: [Name]: [Phone]

  contact represents a pointer to the contact to be printed
 */
void printContactInfo(Contact * contact);

/*
  Displays a list of all possible commands, and gives a brief description of each one
 */
void helpText();

/*
  Clears the terminal based on the user's OS (see the #if section above)
 */
void clearScreen();

/*
  Attempts to search for contact in "phonebook" based on the query, looks through each name and phone

  contactList is the list of all contacts, AKA the "phonebook"
  contactCount is the total number of contacts in the "phonebook"
 */
void searchForContact(Contact * contactList, int contactCount);

int main(void) {
	printf("---------------\n");
	printf("Use -h for help\n");
	printf("---------------\n");
	
	Contact * contactList =  malloc(sizeof(Contact));
	int contactCount = 0;
	
	bool userQuit = false;
	while (!userQuit) {
		char command[8];
		printf("Phonebook> ");
		fgets(command, sizeof(command), stdin);

		if (command[0] != '-') {
			printf("Invalid Command!\n");
			continue;
		}

		switch (command[1]) {
		    case 'a':
				addToPhoneBook(&contactList, &contactCount);
				break;
		    case 's':
				searchForContact(contactList, contactCount);
				break;
		    case 'c':
				clearScreen();
				break;
		    case 'v':
				displayAllContacts(contactList, contactCount);
				break;
		    case 'q':
				printf("Closing Phonebook...\n");
				userQuit = true;
				break;
		    case 'h':
				helpText();
				break;
		    default:
				printf("Invalid Command!\n");
				break;
		}
	}

	for (int i = 0; i < contactCount; i++) {
		free(contactList[i].name);
		free(contactList[i].phone);
	}
	
	free(contactList);
}

bool addToPhoneBook(Contact ** contactList, int * contactCount) {
	char name[64];
	printf("Name: ");
	fgets(name, sizeof(name), stdin);
	name[strcspn(name, "\n")] = '\0';
	
	char phone[32];
	printf("Phone Number: ");
	fgets(phone, sizeof(phone), stdin);
	phone[strcspn(phone, "\n")] = '\0';

	(*contactCount)++;
	
	Contact * newContactList = realloc(*contactList, (*contactCount) * sizeof(Contact));
	
	if (newContactList != NULL) {
		*contactList = newContactList;
		
		Contact * contact = &((*contactList)[*contactCount - 1]);
		
		contact->name = malloc(strlen(name) + 1);
		strcpy(contact->name, name);
	
		contact->phone = malloc(strlen(phone) + 1);
		strcpy(contact->phone, phone);
		
		printf("Added ");
		printContactInfo(contact);
		printf(" to phonebook.\n");
		
		return true;
	} else {
		printf("Allocation failed for add, try again!\n");
		(*contactCount)--;

		return false;
	}
}

void printContactInfo(Contact * contact) {
	printf("%s: %s", contact->name, contact->phone);
}

void displayAllContacts(Contact * contactList, int contactCount) {
	printf("------------------------\n");
	for (int i = 0; i < contactCount; i++) {
		printContactInfo(&contactList[i]);
		printf("\n");
	}
	printf("------------------------\n");
}

void helpText() {
	printf("------------------------\n");
	printf("-a: Add to phonebook\n-v: View all contacts\n-s: Search phonebook based on query\n-c: Clear console\n-q: Quit\n");
	printf("------------------------\n");
}

void clearScreen() {
	if (strcmp(OS_NAME, "WINDOWS") == 0) {
		system("cls");
	} else if (strcmp(OS_NAME, "LINUX") == 0) {
		system("clear");
	} else {
		printf("Unable to detect your operating system.\n");
	}
}

void searchForContact(Contact * contactList, int contactCount) {
	if (contactCount == 0) {
		printf("Phonebook must contain atleast 1 contact to use -s.\n");
		return;
	}

	printf("Search query: ");
	char query[64];
	fgets(query, sizeof(query), stdin);
	query[strcspn(query, "\n")] = '\0';
	
	int contactIdx = -1;
	for (int i = 0; i < contactCount; i++) {
		Contact contact = contactList[i];

		if (strstr(contact.name, query) != NULL) {
			contactIdx = i;
			break;
		}

		if (strstr(contact.phone, query) != NULL) {
			contactIdx = i;
			break;
		}
	}

	if (contactIdx == -1) {
		printf("Could not find contact with query %s.\n", query);
	} else {
		printContactInfo(&contactList[contactIdx]);
		printf("\n");
	}
}
