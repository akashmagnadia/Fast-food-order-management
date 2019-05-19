#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { FALSE = 0, TRUE, NO = 0, YES } boolean;

typedef enum { waitingInRst = 0, calledIn = 1 } status; //status of if the order is waiting or called

typedef struct node {
	char *name;
	int burgur;
	int salad;
	status status;
	struct node *next;
} waitlist;

//prototypes
void addToList(waitlist **head, char *name, int NumBurgers, int NumSalads, status status);
void displayOrdersAhead(waitlist *head, char* name);
void displayListInformation(waitlist *head);
void displayWaitingTime(waitlist *head, char* name, int debugMode);
void retrieveAndRemove(waitlist **head, int preparedBurgers, int preparedSalads, int debugMode);

int updateStatus(waitlist **head, char *name, status status);
int doesNameExist(waitlist **head, char *name);

/* forward definition of functions */
void clearToEoln();

/* Read in until the first Non-White-Space character is Read */
/* The white space characters are:
 *      space, tab \t, newline \n, vertical tab \v,
 *      form feed \f, and carriage return \r
 */
int getNextNWSChar()
{
	int ch;

	ch = getc(stdin);
	if (ch == EOF || ch == '\n')
		return ch;
	while (isspace(ch))
	{
		ch = getc(stdin);
		if (ch == EOF || ch == '\n')
			return ch;
	}
	return ch;
}

/* read in the next Positive Integer or error:    */
/*    we can count zero as a positive number */
int getPosInt()
{
	int value = -1;

	/* clear white space characters */
	int ch;
	ch = getc(stdin);
	while (!isdigit(ch))
	{
		if ('\n' == ch)  /* error \n ==> no integer given */
			return -1;
		if (!isspace(ch)) /* error non white space ==> integer not given next */
		{
			clearToEoln();
			return -1;
		}
		ch = getc(stdin);
	}

	value = ch - '0';
	ch = getc(stdin);
	while (isdigit(ch))
	{
		value = value * 10 + ch - '0';
		ch = getc(stdin);
	}

	ungetc(ch, stdin);  /* put the last read character back in input stream */

	/* Integer value less than 0 is an error in this program */
	if (0 > value)
		clearToEoln();

	return value;
}

/* read in the name until the end of the input */
char *getName()
{
	/* skip over the white space characters */
	int ch;
	ch = getc(stdin);
	while (isspace(ch))
	{
		if ('\n' == ch)  /* error \n ==> no integer given */
			return NULL;
		ch = getc(stdin);
	}

	char *word;
	int size;
	size = 10;
	word = (char *)malloc(sizeof(char) * size);

	// read in character-by-character until the newline is encountered
	int count = 0;

	while (ch != '\n')
	{
		if (count + 1 >= size)
		{
			// to grow an array to make it "dynamically sized" using malloc
			char* temp;
			int i;
			size = size * 2;
			temp = (char *)malloc(sizeof(char) * size);

			// copy the characters to the new array
			for (i = 0; i < count; i++)
				temp[i] = word[i];

			free(word);
			word = temp;
		}

		word[count] = ch;
		count++;
		word[count] = '\0';

		// read next character
		ch = getc(stdin);
	}

	if (count > 30)
	{
		count = 30;
		word[count] = '\0';
	}

	/* clear ending white space characters */
	while (isspace(word[count - 1]))
	{
		count--;
		word[count] = '\0';
	}

	return word;
}

/* Clear input until next End of Line Character - \n */
void clearToEoln()
{
	int ch;

	do {
		ch = getc(stdin);
	} while ((ch != '\n') && (ch != EOF));
}

/* Print out a list of the commands for this program */
void printCommands()
{
	printf("The commands for this program are:\n\n");
	printf("q - to quit the program\n");
	printf("? - to list the accepted commands\n");
	printf("a <# burgers> <# salads> <name> - to add your order to the order list\n");
	printf("c <# burgers> <# salads> <name> - to add a call-ahead order to the order list\n");
	printf("w <name> - to specify a call-ahead group is now waiting in the restaurant\n");
	printf("r <# burgers> <# salads> - to retrieve the first waiting group whose order matches the items on the counter\n");
	printf("l <name> - list how many orders are ahead of the named order\n");
	printf("d - display the order list information\n");
	printf("t <name> - display an estimated wait time for the given order name\n");

	/* clear input to End of Line */
	clearToEoln();
}

void doAdd(waitlist **head, int debugMode)
{
	/* get number of burgers ordered from input */
	int NumBurgers = getPosInt();
	if (NumBurgers < 0)
	{
		printf("Error: Add command requires an integer value of at least 0\n");
		printf("Add command is of form: a <# burgers> <# salads> <name>\n");
		printf("  where:<# burgers> is the number of ordered burgers\n");
		printf("        <# salads> is the number of ordered salads\n");
		printf("        <name> is the name of the person putting the order\n");
		return;
	}

	/* get number of salads ordered from input */
	int NumSalads = getPosInt();
	if (NumSalads < 0)
	{
		printf("Error: Add command requires an integer value of at least 0\n");
		printf("Add command is of form: a <# burgers> <# salads> <name>\n");
		printf("  where:<# burgers> is the number of ordered burgers\n");
		printf("        <# salads> is the number of ordered salads\n");
		printf("        <name> is the name of the person putting the order\n");
		return;
	}

	/* get group name from input */
	char *name = getName();
	if (NULL == name)
	{
		printf("Error: Add command requires a name to be given\n");
		printf("Add command is of form: a <# burgers> <# salads> <name>\n");
		printf("  where:<# burgers> is the number of ordered burgers\n");
		printf("        <# salads> is the number of ordered salads\n");
		printf("        <name> is the name of the person putting the order\n");
		return;
	}

	printf("Adding In-restaurant order for \"%s\": %d burgers and %d salads\n", name, NumBurgers, NumSalads);

	// add code to perform this operation here

	if (doesNameExist(head, name)) { //check to see if someone with the same name is in the queue
		free(name);
		printf("This name exist and is in the queue. Pick a different name!!\n");
		if (debugMode == TRUE) {
			displayListInformation(*head);
		}
		return;
	}

	addToList(head, name, NumBurgers, NumSalads, waitingInRst); //waiting in restaurant 
	if (debugMode == TRUE) {
		displayListInformation(*head);
	}
}


void doCallAhead(waitlist **head, int debugMode)
{
	/* get number of burgers ordered from input */
	int NumBurgers = getPosInt();
	if (NumBurgers < 0)
	{
		printf("Error: Call-ahead command requires an integer value of at least 0\n");
		printf("Call-ahead command is of form: c <# burgers> <# salads> <name>\n");
		printf("  where:<# burgers> is the number of ordered burgers\n");
		printf("        <# salads> is the number of ordered salads\n");
		printf("        <name> is the name of the person putting the order\n");
		return;
	}

	/* get number of salads ordered from input */
	int NumSalads = getPosInt();
	if (NumSalads < 0)
	{
		printf("Error: Call-ahead command requires an integer value of at least 0\n");
		printf("Call-ahead command is of form: c <# burgers> <# salads> <name>\n");
		printf("  where:<# burgers> is the number of ordered burgers\n");
		printf("        <# salads> is the number of ordered salads\n");
		printf("        <name> is the name of the person putting the order\n");
		return;
	}


	/* get group name from input */
	char *name = getName();
	if (NULL == name)
	{
		printf("Error: Call-ahead command requires a name to be given\n");
		printf("Call-ahead command is of form: c <# burgers> <# salads> <name>\n");
		printf("  where:<# burgers> is the number of ordered burgers\n");
		printf("        <# salads> is the number of ordered salads\n");
		printf("        <name> is the name of the person putting the order\n");
		return;
	}

	printf("Adding Call-ahead order for \"%s\": %d burgers and %d salads\n", name, NumBurgers, NumSalads);

	// add code to perform this operation here

	if (doesNameExist(head, name)) { //check to see if someone with the same name is in the queue
		free(name);
		printf("This name exist and is in the queue!!");
		if (debugMode == TRUE) {
			displayListInformation(*head);
		}
		return;
	}

	addToList(head, name, NumBurgers, NumSalads, calledIn); //waiting in restaurant 
	if (debugMode == TRUE) {
		displayListInformation(*head);
	}
}

void doWaiting(waitlist **head, int debugMode)
{
	/* get order name from input */
	char *name = getName();
	if (NULL == name)
	{
		printf("Error: Waiting command requires a name to be given\n");
		printf("Waiting command is of form: w <name>\n");
		printf("  where: <name> is the name of the group that is now waiting\n");
		return;
	}

	printf("Call-ahead order \"%s\" is now waiting in the restaurant\n", name);

	// add code to perform this operation here
	if (!doesNameExist(head, name)) { //check to see if someone with the same name is in the queue
		printf("Not in the queue\n");
		if (debugMode == TRUE) {
			displayListInformation(*head);
		}
	}

	//update the status of the person to waiting in restaurant
	updateStatus(head, name, waitingInRst); //don't need to pass in & because not changing the head
	if (debugMode == TRUE) {
		displayListInformation(*head);
	}
}

void doRetrieve(waitlist **head, int debugMode)
{
	/* get info of prepared food ready on the counter from input */
	int PreparedBurgers = getPosInt();
	if (PreparedBurgers < 0)
	{
		printf("Error: Retrieve command requires an integer value of at least 0\n");
		printf("Retrieve command is of form: r <# burgers> <# salads>\n");
		printf("  where: <# burgers> is the number of burgers waiting on the counter for pick up\n");
		printf("         <# saladss> is the number of salads waiting on the counter for pick up\n");
		return;
	}

	int PreparedSalads = getPosInt();
	if (PreparedSalads < 0)
	{
		printf("Error: Retrieve command requires an integer value of at least 0\n");
		printf("Retrieve command is of form: r <# burgers> <# salads>\n");
		printf("  where: <# burgers> is the number of burgers waiting on the counter for pick up\n");
		printf("         <# saladss> is the number of salads waiting on the counter for pick up\n");
		return;
	}

	clearToEoln();
	printf("Retrieve (and remove) the first group that can pick up the order of %d burgers and %d salads\n", PreparedBurgers, PreparedSalads);

	// add code to perform this operation here
	printf("Before:\n");
	if (debugMode == TRUE) {
		displayListInformation(*head);
		printf("\n");
	}

	retrieveAndRemove(head, PreparedBurgers, PreparedSalads, debugMode);

	printf("\nAfter:\n");
	if (debugMode == TRUE) {
		displayListInformation(*head);
	}
}

void doList(waitlist *head)
{
	/* get order name from input */
	char *name = getName();
	int namePresent;
	namePresent = 0; //flag to see if the name is in the queue

	if (NULL == name)
	{
		printf("Error: List command requires a name to be given\n");
		printf("List command is of form: l <name>\n");
		printf("  where: <name> is the name of the order to inquire about\n");
		return;
	}

	// add code to perform this operation here
	waitlist* head2 = head; //copy of head
	while (head != NULL) {
		if (strcmp(head->name, name) == 0) {
			namePresent = 1;
		}
		head = head->next;
	}

	if (namePresent == 0) {
		printf("Name is not in the queue\n");
	}
	else {
		printf("Order for \"%s\" is behind the following orders\n", name);
		displayOrdersAhead(head2, name);
	}
}

void doDisplay(waitlist *head)
{
	clearToEoln();
	printf("Display information about all orders\n");

	// add code to perform this operation here
	displayListInformation(head);
}


void doEstimateTime(waitlist **head, int debugMode)
{
	/* get order name from input */

	char *name = getName();
	if (name == NULL)
	{
		printf("Error: List command requires a name to be given\n");
		printf("List command is of form: t <name>\n");
		printf("  where: <name> is the name of the order to inquire about\n");
		return;
	}


	// add code to perform this operation here  
	if (!doesNameExist(head, name)) {
		printf("Name not in the queue\n");
		return;
	}

	displayWaitingTime(*head, name, debugMode);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void addToList(waitlist **head, char *name, int NumBurgers, int NumSalads, status status) {
	waitlist *new = (waitlist *)malloc(sizeof(waitlist)); //make memory for the info
	new->burgur = NumBurgers; //assign burgers
	new->salad = NumSalads; //assign salads
	new->name = name; //assign name
	new->next = NULL; //next node is null since its at the end
	new->status = status; //physically in restaurant so making it waiting in restaurant

	//check to see if there is anything in the queue
	if (*head == NULL) {
		*head = new;
	}
	else { //if there is someone in the queue already then add it to the end
		waitlist *temp = NULL;
		temp = *head;
		while (temp->next != NULL) { //traverse to get to the end
			temp = temp->next;
		}

		temp->next = new;
	}
}

int doesNameExist(waitlist **head, char *name) {
	waitlist *temp = NULL;
	temp = *head;

	while (temp != NULL) { //go through everyone in the queue
		if (strcmp(temp->name, name) == 0) { //check the names
			return TRUE; //name found
		}
		temp = temp->next;
	}

	return FALSE; //if the name is not found
}

int updateStatus(waitlist **head, char *name, status status) {
	waitlist *temp = NULL;
	temp = *head;

	while (temp != NULL) {
		if (strcmp(temp->name, name) == 0) { //if the person with same name is found
			if (temp->status == waitingInRst) {
				printf("Already in the restaurant");
				return FALSE; //return false if the person is already checked in
			}

			temp->status = status; //change the status to status
			return TRUE;
		}

		temp = temp->next; //go to next node
	}

	return FALSE; //make the compiler happy
}

void retrieveAndRemove(waitlist **head, int preparedBurgers, int preparedSalads, int debugMode) {
	waitlist *temp = *head;
	waitlist *deleting = NULL;

	if (temp == NULL) { //no elements or group in queue 
		printf("No one in the restaurant to pick up the order\n");
		return;
	}
	else { // check first node
		if (temp->status == waitingInRst) {
			if (temp->burgur <= preparedBurgers) {
				if (temp->salad <= preparedSalads) {
					printf("Serving %s\n", temp->name);
					*head = (*head)->next;
					free(temp->name);
					free(temp); //free up the memory
					return;
				}
				else {
					// not enough salads
					if (debugMode == TRUE) {
						printf("%s has ordered more salads than prepared salads\n", temp->name);
					}
				}
			}
			else {
				// not enough burgers
				if (debugMode == TRUE) {
					printf("%s has ordered more burgers than prepared burgers\n", temp->name);
				}
			}
		}
		else {
			// not in resturant
			if (debugMode == TRUE) {
				printf("%s is not in the resturant\n", temp->name);
			}
		}

		//traverse the list until valid element is found
		while (temp->next != NULL) {
			if (temp->next->status == waitingInRst) {
				if (temp->next->burgur <= preparedBurgers) {
					if (temp->next->salad <= preparedSalads) {
						printf("Serving %s\n", temp->next->name);
						deleting = temp->next;
						temp->next = temp->next->next; //temp->next = deleting->next;
						free(deleting);
						return;
					}
					else {
						// not enough salads
						if (debugMode == TRUE) {
							printf("%s has ordered more salads than prepared salads\n", temp->next->name);
						}
						temp = temp->next; //move to the next node
					}
				}
				else {
					// not enough burgers
					if (debugMode == TRUE) {
						printf("%s has ordered more burgers than prepared burgers\n", temp->next->name);
					}
					temp = temp->next; //move to the next node
				}
			}
			else {
				// not in restaurant
				if (debugMode == TRUE) {
					printf("%s is not in the resturant\n", temp->next->name);
				}
				temp = temp->next; //move to the next node
			}
		}
	}

	//get to the end of the list without serving anything
	printf("Sorry no group found eligible to pick up the order\n");
	return;
}

void displayOrdersAhead(waitlist *head, char *name) {

	if (head == NULL) {
		printf("No one is in the queue\n");
		return;
	}

	if (strcmp(head->name, name) == 0) { //if the name is first in place
		printf("You are up next\n");
		return;
	}

	while (head != NULL) {
		//printf("%d", strcmp(head->name, name));
		if (strcmp(head->name, name) != 0) {
			printf("Name: %s, Burgers: %d, Salads: %d, ", head->name, head->burgur, head->salad);
			if (head->status == waitingInRst) {
				printf("Status: In restaurant\n");
			}
			else {
				printf("Status: Not in restaurant\n");
			}
		}
		else {
			return;
		}
		head = head->next;
	}
	return;
}

void displayListInformation(waitlist *head) {
	if (head == NULL) {
		printf("No one is in the queue\n");
		return;
	}

	printf("Followings are all the orders in queue:\n");
	while (head != NULL) {
		printf("Name: %s, Burgers: %d, Salads: %d, ", head->name, head->burgur, head->salad);
		if (head->status == waitingInRst) {
			printf("Status: In restaurant\n");
		}
		else {
			printf("Status: Not in restaurant\n");
		}
		head = head->next;
	}
	return;
}

void displayWaitingTime(waitlist *head, char* name, int debugMode) {
	int waitTime;
	waitTime = 0;

	if (debugMode == TRUE) {
		while (head != NULL) {
			waitTime = waitTime + ((head->burgur * 10) + (head->salad * 5));
			if (strcmp(head->name, name) == 0) {
				printf("Requested: ");
			}
			printf("Estimated waiting time for %s is %d minutes\n", head->name, waitTime);
			head = head->next;
		}
	}
	else {
		while (head != NULL) {
			if (strcmp(head->name, name) != 0) {
				waitTime = waitTime + ((head->burgur * 10) + (head->salad * 5));
			}
			else {
				break;
			}
			head = head->next;
		}

		printf("Estimated waiting time for %s is %d minutes\n", name, waitTime);
	}
}

int countOrdersAhead(waitlist *head, char*name) {
	int counter;
	counter = 0;
	while (head != NULL) {
		//printf("%d", strcmp(head->name, name));
		if (strcmp(head->name, name) != 0) {
			counter++;
		}
		else {
			return counter;
		}
		head = head->next;
	}
	return counter;
}

void clean(waitlist **head) {
	waitlist *temp = *head;
	while (temp != NULL) {
		free(temp->name);
		*head = (*head)->next;
		free(temp);
		temp = *head;
	}
	*head = NULL;
}

int main(int argc, char **argv)
{
	//add debuggung stuff
	int debugMode, i;
	debugMode = FALSE;
	for (i = 0; i < argc; i++) {
		if (strcmp(argv[i], "-d") == 0) {
			debugMode = TRUE;
		}
	}
	//debugMode = TRUE; //to manually set to debug mode

	int ch;
	waitlist *head = NULL;

	printf("Starting Fast Food Order Management Program\n\n");
	if (debugMode == TRUE) {
		printf("Debugging Mode Activated\n");
	}
	printf("Enter command: ");

	while ((ch = getNextNWSChar()) != EOF)
	{
		/* check for the various commands */
		if ('q' == ch)
		{
			printf("Quitting Program\n");
			clean(&head);
			printf("\n");
			return (0);
		}
		else if ('?' == ch)
		{
			printCommands();
		}
		else if ('a' == ch)
		{
			doAdd(&head, debugMode); // & since you want to be able to change the value of head
		}
		else if ('c' == ch)
		{
			doCallAhead(&head, debugMode); // & since you want to be able to change the value of head
		}
		else if ('w' == ch)
		{
			doWaiting(&head, debugMode);
		}
		else if ('r' == ch)
		{
			doRetrieve(&head, debugMode);
		}
		else if ('l' == ch)
		{
			doList(head);
		}	
		else if ('d' == ch)
		{
			doDisplay(head);
		}

		else if ('t' == ch)
		{
			doEstimateTime(&head, debugMode);
		}

		else if ('\n' == ch)
		{
			/* nothing entered on input line     *
			 * do nothing, but don't give error  */
		}
		else
		{
			printf("%c - in not a valid command\n", ch);
			printf("For a list of valid commands, type ?\n");
			clearToEoln();
		}

		printf("\nEnter command: ");
	}

	clean(&head); //free up all the memory
	printf("Quiting Program - EOF reached\n");
	printf("\n");
	return 1;
}
