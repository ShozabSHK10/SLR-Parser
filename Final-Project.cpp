#include <iostream>    //provides input & output stream
#include <string.h>      //For string handling functions like strcpy.......contains functions for manipulating C-style string
#include <stdlib.h>      //For general utility functions, like memory allocation,random numbers, process control, conversions, etc. 
#include <stdio.h>      //(For standard input and output operations.) Provides functions for input and output operations.
#include <limits>       //Provides numeric limits for data types.

using namespace std;     //Use the standard namespace to avoid prefixing standard library names with std::.


//In C++, strcpy is a function from the C standard library, defined in the <cstring> header. It is used to copy a C-style string (null-terminated character array) from a source to a destination.
//strcpy(destination, source);

char terminal[100] = {};     //Array to store terminal symbols.
int count_terminals;         //Number of terminals.
char nonTerminal[100] = {};    //Array to store non-terminal symbols.
int count_nonTerminals;        //Number of non-terminals.
char gotoTable[100][100];     //The GOTO table.
char reduce[20][20];         //Table for reduce actions (unused in the provided code).
char follow[20][20];        //Array to store FOLLOW sets.
char fo_co[20][20];         //Array to store FOLLOW sets combined with something else (unused in the provided code).
char first[20][20];         //Array to store FIRST sets.
						//Arrays to store production rules and parsing-related information.
struct state             //Defines a structure to represent a state 
{ 
    int productionCount;     		 //The number of productions in the state.
    char productions[100][100];      //A 2D array to store the productions.
    state()                          //Constructor initializing all productions to null characters.
    {
        for (int i = 0; i < 100; ++i)
        {
            for (int j = 0; j < 100; ++j)
            {
                productions[i][j] = '\0';
            }
        }
    }
};

void displayWelcomePage()     //Displays a welcome message and instructions for the user. & waits for the user to press Enter.
{
    cout << "\t\t\t\t\t******************************************" << endl;
    cout << "\t\t\t\t\t*                                        *" << endl;
    cout << "\t\t\t\t\t*       Welcome to the SLR(1) Parser     *" << endl;
    cout << "\t\t\t\t\t*                                        *" << endl;
    cout << "\t\t\t\t\t******************************************" << endl;
    cout << "\t\t\t\t\t*                                        *" << endl;
    cout << "\t\t\t\t\t*    This program constructs an SLR(1)   *" << endl;
    cout << "\t\t\t\t\t*    parser table for a given grammar.   *" << endl;
    cout << "\t\t\t\t\t*                                        *" << endl;
    cout << "\t\t\t\t\t******************************************" << endl
         << endl;
    cout << "\t\t\t\t\tPress Enter to continue...";
    cin.ignore();             //cin.ignore() waits for the user to press Enter.
   //cin.ignore() is used to discard the newline character from the input buffer, waiting for the user to press Enter.
    cout << endl;
}

void addDots(struct state *I)             //Adds a dot at the beginning of each production rule in the state I.
// *I: This is a pointer to a state structure.
//When add_dots is called, it receives the address of a state structure, allowing it to modify the original structure directly.
//Accesses and modifies the prod array in the state structure to add dots at the beginning of each production.
{
    for (int i = 0; i < I->productionCount; i++)  //Iterates over each production in the state I
   //I->productionCount Accesses the productionCount member of the structure pointed to by I.
//The arrow operator (->) is used to access members of a structure through a pointer.
   
    {

        if (I->productions[i][3] != '\0')
        {
// Shift characters to the right starting from the last position (99) down to position 4. to make space for the dot at position 3. It starts from index 99 down to 4.
        
		//99> Assuming a maximum production length of 100 characters (index 0 to 99), this loop shifts characters to the right to make space for the dot.
		//3>  The dot is inserted at the fourth position (index 3). Thus, we need to shift characters from the end up to the fourth position to make space for it.
		   
		    for (int j = 99; j > 3; j--)
                I->productions[i][j] = I->productions[i][j - 1];
            I->productions[i][3] = '.';  //Places the dot at the 4th position (index 3) in each production.
        }
        else      //Ensures that there is space to insert the dot. If not, it prints a warning.
        {
            cout << "Warning: Production rule " << i << " is too long to insert a dot.\n";
        }
    }
}

//Augments the initial state with a new production starting from a special start symbol.
void augmentate(struct state *S, struct state *I)
//struct state *S and struct state *I: These are pointers to state structures.
//The function augments state S based on the first production in state I.
{
	 //Modifies the production array and production count in state S based on the first production in state I.
	//Accessing I->productions[0] to check the first production and modifying S->prod[0].
    if (I->productions[0][0] == 'S')    //Checks if the first production of I starts with S. If true, it sets S->prod[0] to "Z->.S"
        strcpy(S->productions[0], "Z->.S");
    else
    {
        strcpy(S->productions[0], "S->.");     //Otherwise, it sets S->prod[0] to "S->
        S->productions[0][4] = I->productions[0][0];     //adds the first character of I->prod[0] at position 4.
    }
    S->productionCount++;     //Increments the prod_count of state S.
}

/* This function collects the grammar from the user. Here’s a detailed explanation:

Prompt the user to input the number of productions. Validate the input to ensure it’s a number.
Prompt the user to input the number of non-terminals. Validate the input to ensure it’s a number.
Collect non-terminal symbols one by one, ensuring they are single uppercase letters.
Prompt the user to input the number of terminals. Validate the input to ensure it’s a number.
Collect terminal symbols one by one, ensuring they are single lowercase letters or symbols.
Collect productions one by one, ensuring they follow the format A->B with valid non-terminal and terminal symbols. */

//Handles user input to get the number of productions, non-terminals, and terminals, and validates the input.
void getProductions(struct state *I)
{
	//struct state *I: A pointer to a state structure to populate productions and symbol arrays.
//Populates the prod array and prod_count in the state structure I based on user input.
//It uses goto statements for error handling and input validation.
    int productionLen = 0;
a:
    cout << "\n\tPlease specify the number of productions: \n\t";
    if (!(cin >> I->productionCount))  //Modifying I->productionCount through the pointer.
    {

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\n\n\t\tPlease! Enter a number only.\n\n\t";
        goto a;
    }
b:
    cout << "\n\tPlease specify the number of non-terminals you want to include: \n\t";
    if (!(cin >> count_nonTerminals))
    {

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\n\n\t\tInvalid input! Please enter numbers only.\n\n\t";
        goto b;
    }

    cout << "\n\tInput the non-terminals one by one(Single Uppercase letter): \n\t";
    for (int i = 0; i < count_nonTerminals; i++)
    {
    c:
        if (!(cin >> nonTerminal[i]) || !(cout << "\t") || cin.peek() != '\n' || !isupper(nonTerminal[i]))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n\n\tInvalid input! you must input a Single Uppercase letter.\n\n\t";
            goto c;
        }
    }
d:
    cout << "\n\tPlease specify the number of terminals you want to include: \n\t";
    if (!(cin >> count_terminals))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\n\n\t\tInvalid input! enter a number only.\n\n\t";
        goto d;
    }
    cout << "\n\tInput the terminals (single lowercase letter OR a Symbol) one by one: \n\t";
    for (int i = 0; i < count_terminals; i++)
    {
    e:
        cin >> terminal[i];
        cout << "\t";
        if (cin.fail() || cin.peek() != '\n')
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n\n\tInvalid input! You must enter a single letter.\n\n\t";
            goto e;
        }
        if (terminal[i] != '*' && terminal[i] != '+' && terminal[i] != '/' &&
            terminal[i] != '-' && terminal[i] != ')' && terminal[i] != '(')
        {
            if (!islower(terminal[i]))
            {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "\n\n\tInvalid input! You must enter a single lowercase letter or a Symbol.\n\n\t";
                goto e;
            }
        }
    }

    cout << "\n\tInput the productions one by one without white-spaces(eg. S->ABCd): \n\t";
    for (int i = 0; i < I->productionCount; i++)  //Ensures all productions are read from the user.
    {
    f:
        bool firstInNonTerminal = false;
        cin >> I->productions[i];     //Populating the productions array in the state structure through the pointer.
        cout << "\t";
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n\n\tInvalid production input! Enter again.\n\n\t";
            goto f;
        }
        else if (I->productions[i][1] != '-' || I->productions[i][2] != '>')
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n\n\tInvalid production input! Enter again.\n\n\t";
            goto f;
        }
        for (int j = 0; j < count_nonTerminals; j++)
        {
            if (I->productions[i][0] == nonTerminal[j])
            {
                firstInNonTerminal = true;
                break;
            }
        }
        if (!firstInNonTerminal)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n\n\tInvalid production: Left side of the production must be a single non-terminal.\n\n\t";
            goto f;
        }
    }
    cout << "\n------------------------------------------------------------------------------------------------------------------------\n";
}

//utility functions

/* This function checks if a given symbol is a non-terminal:

Loop through each non-terminal in the nonTerminal array.
If the symbol matches any non-terminal, return true.
If no match is found, return false. */

bool isNonTerminal(char a)       //Check if a character is an uppercase letter (non-terminal).
{
    return a >= 'A' && a <= 'Z';
}

/* Loop through each production in state S.
If the production matches the given production p, return true.
If no match is found, return false. */

bool inState(struct state *I, char *a)   //Checks if a production a is already in the state I
{
	//struct state *I: Pointer to a state structure.
	//char *a: Pointer to a character array (production).
	
    for (int i = 0; i < I->productionCount; i++)  //Iterates over productions in state I and compares them with the production a.
    {
        if (!strcmp(I->productions[i], a))
            return true;
    }
    return false;
}

/*Loop through each character in the production until the null character \0 is reached.
If a dot . is found, return the character immediately after it.
If no dot is found, return a space ' '. */

char charAfterDot(char a[100])      //Returns the character immediately after the dot in the production a.
    for (int i = 0; i < strlen(a); i++)
        if (a[i] == '.')
        {
            return a[i + 1];
        }
    return '\0';
}

char *moveDot(char b[100], int len)    //Move the dot one position to the right in a production.
{
    static char a[100] = {};
    strcpy(a, b);
    for (int i = 0; i < len; i++)
    {
        if (a[i] == '.')
        {
            swap(a[i], a[i + 1]);
            break;
        }
    }
    return a;
}

bool sameState(struct state *I0, struct state *I)    //Checks if two states have the same productions.

{
    if (I0->productionCount != I->productionCount)
        return false;
    for (int i = 0; i < I0->productionCount; i++)
    {
        int flag = 0;
        for (int j = 0; j < I->productionCount; j++)
            if (strcmp(I0->productions[i], I->productions[j]) == 0)
                flag = 1;
        if (flag == 0)
            return false;
    }
    return true;
}


//closure: the "closure" of a state refers to the process of adding additional productions to a state based on the grammar rules.
//To compute the closure of a state, you examine each production in the state.
//For each production, you look at the symbol immediately after the dot (.).
// If it's a non-terminal symbol, you add all productions from the grammar where that non-terminal appears as the left-hand side (LHS) to the current state.
//You continue this process recursively for each newly added production until no more new productions can be added.

void closure(struct state *I, struct state *I0)    //This function computes the closure of a state by recursively adding necessary productions.
{
	//struct state *I: Pointer to a state structure.
	//Computes the closure of a state by adding all productions for the non-terminal after the dot.
	
    char a;
    for (int i = 0; i < I0->productionCount; i++)   //Iterates through productions in I0.
    {
        a = charAfterDot(I0->productions[i]);
        if (isNonTerminal(a))
        {
            for (int j = 0; j < I->productionCount; j++)   
    //For each production, checks if the character after the dot is a non-terminal. If so, adds corresponding productions to the state.
    
            {
                if (I->productions[j][0] == a)
                {
                    if (!inState(I0, I->productions[j]))
                    {
                        strcpy(I0->productions[I0->productionCount], I->productions[j]);
                        I0->productionCount++;
                    }
                }
            }
        }
    }
}

//Defines the GOTO function for the LR parser.

/*gotoState: Generates a new state by moving the dot over a specified character.
Loop: Iterates through productions in I.
Condition: If the character after the dot matches the specified character, the production with the dot moved is added to the new state. */

void gotoState(struct state *I, struct state *S, char a)
{
	//struct state *I: Pointer to a state structure.
    for (int i = 0; i < I->productionCount; i++)
    {
        if (charAfterDot(I->productions[i]) == a)
        {
            strcpy(S->productions[S->productionCount], moveDot(I->productions[i], strlen(I->productions[i])));
            S->productionCount++;
        }
    }
}

void printProductions(struct state *I)    //Prints all productions of a state.

{
    for (int i = 0; i < I->productionCount; i++)
        printf("\t\t\t\t\t%s\n", I->productions[i]);
    cout << endl;
}

bool inArray(char a[20], char b)      //Checks if a character is present in an array.

{
    for (int i = 0; i < strlen(a); i++)
        if (a[i] == b)
            return true;
    return false;
}

char *charsAfterDots(struct state *I)   //Populates an array with characters that follow dots in all productions of a state.

{
    static char a[20] = {};
    for (int i = 0; i < I->productionCount; i++)
    {
        if (!inArray(a, charAfterDot(I->productions[i])))
        {
            a[strlen(a)] = charAfterDot(I->productions[i]);
        }
    }
    return a;
}

void cleanupProduction(struct state *I)  //Resets the productions of a state to null characters.

{
    char a[100] = {};
    for (int i = 0; i < I->productionCount; i++)
        strcpy(I->productions[i], a);
    I->productionCount = 0;
}

int returnIndex(char a)    //Returns the index of a character in the terminals or non-terminals array.

{
    for (int i = 0; i < count_terminals; i++)
        if (terminal[i] == a)
            return i;
    for (int i = 0; i < count_nonTerminals; i++)
        if (nonTerminal[i] == a)
            return count_terminals + i;
    //     throw runtime_error("Right side of production must be terminals or non-terminals or both.");
}

void printShiftTable(int stateCount)  //Prints the shift table, showing transitions for each terminal symbol.

{
    cout << endl;

    // Print header box
    cout << "\t\t\t\t\t*******************************************************" << endl;
    cout << "\t\t\t\t\t*                                                     *" << endl;
    cout << "\t\t\t\t\t*                    Shift Actions                    *" << endl;
    cout << "\t\t\t\t\t*                                                     *" << endl;
    cout << "\t\t\t\t\t*******************************************************" << endl;
    cout << "\t\t\t\t\t                                                       " << endl;

    // Print header line with centering
    cout << "\t\t\t\t\t| State\t|";
    for (int i = 0; i < count_terminals; i++)
        cout << " " << terminal[i] << " \t|";
    for (int i = 0; i < count_nonTerminals; i++)
        cout << " " << nonTerminal[i] << " \t|";
    cout << endl;

    // Print separator line
    cout << "\t\t\t\t\t|-------";
    for (int i = 0; i < count_terminals + count_nonTerminals; i++)
        cout << "--------";
    cout << "|";
    cout << endl;

    // Print table rows
    for (int i = 0; i < stateCount; i++)
    {
        int arr[count_nonTerminals + count_terminals] = {-1};
        for (int j = 0; j < stateCount; j++)
        {
            if (gotoTable[i][j] != '~')
            {
                arr[returnIndex(gotoTable[i][j])] = j;
            }
        }

        cout << "\t\t\t\t\t| I" << i << "\t|";
        for (int j = 0; j < count_nonTerminals + count_terminals; j++)
        {
            if (i == 1 && j == count_terminals - 1)
                cout << " ACC\t|";
            else if (arr[j] == -1 || arr[j] == 0)
                cout << "\t|";
            else
            {
                if (j < count_terminals)
                    cout << " S" << arr[j] << "\t|";
                else
                    cout << " " << arr[j] << "\t|";
            }
        }
        cout << endl;

        // Print separator line between rows
        cout << "\t\t\t\t\t|-------";
        for (int j = 0; j < count_nonTerminals + count_terminals; j++)
            cout << "--------";
        cout << "|";
        cout << endl;
    }
}

int getIndex(char c, char *a)    //Returns the index of a character in a string.

{
    for (int i = 0; i < strlen(a); i++)
        if (a[i] == c)
            return i;
    // throw runtime_error("Invalid character in get_index.");
}

void addDotAtEnd(struct state *I)   //Adds a dot at the end of a production.
/* for identifying reduced states so that we can match it with original states
with the dots at their end and if match then reduced */
{
    for (int i = 0; i < I->productionCount; i++)
    {
        strcat(I->productions[i], ".");
    }
}

void addToFirst(int n, char b)
{
    for (int i = 0; i < strlen(first[n]); i++)
        if (first[n][i] == b)
            return;
    first[n][strlen(first[n])] = b;
}

void addToFirst(int m, int n)
{
    for (int i = 0; i < strlen(first[n]); i++)
    {
        int flag = 0;
        for (int j = 0; j < strlen(first[m]); j++)
        {
            if (first[n][i] == first[m][j])
                flag = 1;
        }
        if (flag == 0)
            addToFirst(m, first[n][i]);
    }
}

void addToFollow(int n, char b)
{
    for (int i = 0; i < strlen(follow[n]); i++)
        if (follow[n][i] == b)
            return;
    follow[n][strlen(follow[n])] = b;
}

void addToFollow(int m, int n)
{
    for (int i = 0; i < strlen(follow[n]); i++)
    {
        int flag = 0;
        for (int j = 0; j < strlen(follow[m]); j++)
        {
            if (follow[n][i] == follow[m][j])
                flag = 1;
        }
        if (flag == 0)
            addToFollow(m, follow[n][i]);
    }
}

void addToFollowFirst(int m, int n)
{
    for (int i = 0; i < strlen(first[n]); i++)
    {
        int flag = 0;
        for (int j = 0; j < strlen(follow[m]); j++)
        {
            if (first[n][i] == follow[m][j])
                flag = 1;
        }
        if (flag == 0)
            addToFollow(m, first[n][i]);
    }
}


void findFirst(struct state *I)  //First Set: Determines the set of terminals that begin the strings derivable from a non-terminal.

{
    for (int i = 0; i < count_nonTerminals; i++)
    {
        for (int j = 0; j < I->productionCount; j++)
        {
            if (I->productions[j][0] == nonTerminal[i])
            {
                if (!isNonTerminal(I->productions[j][3]))
                {
                    addToFirst(i, I->productions[j][3]);
                }
            }
        }
    }
}

void findFollow(struct state *I)  //Follow Set: Determines the set of terminals that can appear immediately to the right of a non-terminal in some "sentential" form.

{
    for (int i = 0; i < count_nonTerminals; i++)
    {
        for (int j = 0; j < I->productionCount; j++)
        {
            for (int k = 3; k < strlen(I->productions[j]); k++)
            {
                if (I->productions[j][k] == nonTerminal[i])
                {
                    if (I->productions[j][k + 1] != '\0')
                    {
                        if (!isNonTerminal(I->productions[j][k + 1]))
                        {
                            addToFollow(i, I->productions[j][k + 1]);
                        }
                    }
                }
            }
        }
    }
}

void printReduceTable(int state_count, int *no_re, struct state *temp1)
{
    cout << endl;

    // Print header box
    cout << "\t\t\t\t\t*******************************************************" << endl;
    cout << "\t\t\t\t\t*                                                     *" << endl;
    cout << "\t\t\t\t\t*                    Reduce Actions                   *" << endl;
    cout << "\t\t\t\t\t*                                                     *" << endl;
    cout << "\t\t\t\t\t*******************************************************" << endl;
    cout << "\t\t\t\t\t                                                       " << endl;

    // Print header line with centering
    cout << "\t\t\t\t\t| State\t|";
    for (int i = 0; i < count_terminals; i++)
        cout << " " << terminal[i] << " \t|";
    cout << endl;

    // Print separator line
    cout << "\t\t\t\t\t|-------";
    for (int i = 0; i < count_terminals; i++)
        cout << "--------";
    cout << "|";
    cout << endl;

    // Create the table content
    int arr[temp1->productionCount][count_terminals];
    memset(arr, -1, sizeof(arr));

    for (int i = 0; i < temp1->productionCount; i++)
    {
        int n = no_re[i];
        for (int j = 0; j < strlen(follow[returnIndex(temp1->productions[i][0]) - count_terminals]); j++)
        {
            for (int k = 0; k < count_terminals; k++)
            {
                if (follow[returnIndex(temp1->productions[i][0]) - count_terminals][j] == terminal[k])
                    arr[i][k] = i + 1;
            }
        }

        cout << "\t\t\t\t\t| I" << n << "\t|";
        for (int j = 0; j < count_terminals; j++)
        {
            if (arr[i][j] != -1 && arr[i][j] != 0 && arr[i][j] < state_count)
                cout << " R" << arr[i][j] << "\t|";
            else
                cout << "\t|";
        }
        cout << endl;

        // Print separator line between rows
        cout << "\t\t\t\t\t|-------";
        for (int j = 0; j < count_terminals; j++)
            cout << "--------";
        cout << "|";
        cout << endl;
    }
}

/* Main function of the program.
 The main logic for building the SLR(1) parser.
Initialization: Displays the welcome page and initializes states.
Production Handling: Gets productions from the user and adds dots.
State Machine Construction: Constructs the DFA states for the SLR(1) parser.
Loops: Nested loops handle the generation and closure of states.
Conditionals: Check if a new state is equivalent to an existing state.
First and Follow Sets: Calculate first and follow sets for non-terminals.
Tables: Print shift and reduce tables.
Restart: Allows the user to parse another grammar by restarting the process. */

int main()   //Calls functions like get_prods, add_dots, augument, and closure with pointers to states S and I.
{

    displayWelcomePage();
start:             //start: is a label used for jumping back to this point in the code later.

    struct state init;
    struct state temp;
    struct state temp1;
//These lines declare three instances of the state struct: init, temp, and temp1.

    int stateCount = 1;
// STEP 1: Get user input
    getProductions(&init); //This line calls a function named getProductions() and passes a pointer to the init struct as an argument.
    
    temp = init;
    temp1 = temp;
    //These lines copy the content of the init struct to the temp and temp1 structs.

//STEP 2: Add dots at the beginning of them all
    addDots(&init);   //This line calls a function named addDots() and passes a pointer to the init struct as an argument.
    
//This nested loop initializes a 2D array named gotoTable with the character '~'.
    for (int i = 0; i < 100; i++)
        for (int j = 0; j < 100; j++)
            gotoTable[i][j] = '~';

    struct state I[50];  //These lines declare an array of state structs named I with size 50.

//STEP 3: augmentation  
    augmentate(&I[0], &init);

//STEP 4: closure

    closure(&init, &I[0]);
    //They call functions augmentate() and closure() and pass pointers to the init and I[0] structs as arguments.
    
//STEP 5: prints state I0
    cout << "\nI0:\n";
    printProductions(&I[0]); //This line prints the label "I0:" followed by the productions of the I[0] struct.
    
    char characters[20] = {}; //This declares a character array characters with a size of 20 and initializes it with empty characters.

//STEP 6: states construction and print
    for (int i = 0; i < stateCount; i++)  //Main Loop for State Construction
    //It starts a loop iterating over the states, using stateCount as the limit.
    
    {
        char characters[20] = {};  // Collects characters immediately after the dot in all productions of the current state.
        //This line reinitializes the characters array to empty, as it is intended to collect characters immediately after the dot in each iteration of the outer loop.
        
        for (int z = 0; z < I[i].productionCount; z++)  //This loop iterates over the productions of the current state (I[i]).
            if (!inArray(characters, charAfterDot(I[i].productions[z])))   //It checks if the character immediately after the dot in each production is not already present in the characters array using the inArray() function.
            
                characters[strlen(characters)] = charAfterDot(I[i].productions[z]);   //If the character is not present, it adds it to the characters array using charAfterDot() to extract the character after the dot.
                
        for (int j = 0; j < strlen(characters); j++)   //This loop iterates over the characters collected in the characters array.
        
        {
    /* For each unique character after the dot:
gotoState: Moves the dot over the character to create a new state.
closure: Expands the new state. */

            gotoState(&I[i], &I[stateCount], characters[j]);
            closure(&init, &I[stateCount]);
//These lines call the gotoState() function to move the dot over the current character and create a new state, then call the closure() function to expand the new state.

            int flag = 0;
            for (int k = 0; k < stateCount - 1; k++)
//This initializes a flag variable flag to 0 and starts a loop iterating over existing states (I[k]) up to the current state count minus one (stateCount - 1).

            {
          /* 	Check for Existing State: If the new state matches an existing state, mark it and update the goto table.
New State Handling: If the state is unique, increment the state count and add the state to the table. */

                if (sameState(&I[k], &I[stateCount]))  //This condition checks if the newly created state is the same as any existing state.
        
                {
                    cleanupProduction(&I[stateCount]);
                    flag = 1;
                    cout << "I" << i << " on reading the symbol " << characters[j] << " goes to I" << k << ".\n";
                    gotoTable[i][k] = characters[j];
                    break;
        //If the state matches an existing state, it cleans up the production of the new state, sets the flag to 1, prints a message indicating the transition between states, updates the gotoTable, and breaks out of the loop.
        
                }
            }
            if (flag == 0)
        //If the flag remains 0, indicating that the state is unique, it increments the state count, prints a message indicating the transition to the new state, updates the gotoTable, and prints the productions of the new state.
        
            {
                stateCount++;
                cout << "I" << i << " on reading the symbol " << characters[j] << " goes to I" << stateCount - 1 << ":\n";
                gotoTable[i][stateCount - 1] = characters[j];
                printProductions(&I[stateCount - 1]);
            }
        }
    }
/* This section of the code focuses on state construction, where each state represents a set of items in LR(0) parsing.
 The code iterates over the states, collects characters after the dot in each state's productions, creates new states
 by moving the dot over each unique character, and updates the transition table accordingly. */
 
//STEP 7: finding reduced states

    int noReduced[temp.productionCount] = {-1};  //This line initializes an array noReduced with a size equal to the number of productions in the temp state and sets all elements to -1

    terminal[count_terminals] = '$';
    count_terminals++;   //It adds the end-of-input marker ('$') to the terminal array and increments the count of terminals.
    
    addDotAtEnd(&temp1);  //This line adds a dot at the end of each production in the temp1 state. This operation is necessary for handling reductions in LR(0) parsing.

    for (int i = 0; i < stateCount; i++)  //These nested loops iterate over each state and each production within that state.

    {
        for (int j = 0; j < I[i].productionCount; j++)
            for (int k = 0; k < temp1.productionCount; k++)
                if (inState(&I[i], temp1.productions[k]))
                    noReduced[k] = i;
/* They compare each production in temp1 with the productions in each state (I[i]).
If a production from temp1 is found in a state, it updates the corresponding index in the noReduced array with the index of that state. */
    }

//STEP 8: first of non-terminals
    findFirst(&temp);  //This code finds the first set for each non-terminal symbol in the temp state.
    
    for (int l = 0; l < count_nonTerminals; l++)
    {
    /* It iterates over each production in temp and checks if the symbol after the arrow ('>') is a non-terminal.
If it is, it adds the first set of the non-terminal to the first set of the production's left-hand side symbol. */

        for (int i = 0; i < temp.productionCount; i++)
        {
            if (isNonTerminal(temp.productions[i][3]))
            {
                addToFirst(returnIndex(temp.productions[i][0]) - count_terminals, returnIndex(temp.productions[i][3]) - count_terminals);
            }
        }
    }
//STEP 9: follow of non-terminals
    findFollow(&temp);
    addToFollow(0, '$');

/* This code finds the follow set for each non-terminal symbol in the temp state.
It initializes the follow set of the start symbol with the end-of-input marker ('$').
It iterates over each production in temp and checks for occurrences of non-terminal symbols.
If a non-terminal symbol is found, it adds the first set of the next symbol (if it's a non-terminal) to the follow set of the current non-terminal.
Additionally, if the non-terminal is at the end of a production, it adds the follow set of the left-hand side symbol to its follow set. */

    for (int l = 0; l < count_nonTerminals; l++)
    {
        for (int i = 0; i < temp.productionCount; i++)
        {
            for (int k = 3; k < strlen(temp.productions[i]); k++)
            {
                if (temp.productions[i][k] == nonTerminal[l])
                {
                    if (isNonTerminal(temp.productions[i][k + 1]))
                    {
                        addToFollowFirst(l, returnIndex(temp.productions[i][k + 1]) - count_terminals);
                    }
                    if (temp.productions[i][k + 1] == '\0')
                        addToFollow(l, returnIndex(temp.productions[i][0]) - count_terminals);
                }
            }
        }
    }

//STEP 10: shift table
    printShiftTable(stateCount);  //This line prints the shift table, which represents the transitions between states when shifting input symbols during parsing.
    
    cout << endl
         << endl;
//STEP 11: reduce table
    printReduceTable(stateCount, &noReduced[0], &temp1);  //This line prints the reduce table, which contains information about reduction actions to be taken when certain conditions are met during parsing.
    
    cout << endl
         << endl;
         
/* This code prompts the user if they want to generate output for another context-free grammar (CFG).
If the user chooses to continue ('y' or 'Y'), the program jumps back to the start label and starts over.
If the user chooses not to continue, a farewell message is displayed, and the program terminates. */

//STEP 12: choice for more CFG
    char choice;
    cout << "\nDo you want to generate output for another CFG? (y/n): ";
    cin >> choice;

    if (choice == 'y' || choice == 'Y')
    {
        goto start;
    }
    else
    {
        cout << "\n\t\t\t\t\t*************************************************\n";
        cout << "\t\t\t\t\t*                                               *\n";
        cout << "\t\t\t\t\t*   Thank you for using the SLR(1) Parser.      *\n";
        cout << "\t\t\t\t\t*                Goodbye!                       *\n";
        cout << "\t\t\t\t\t*                                               *\n";
        cout << "\t\t\t\t\t*************************************************\n";
    }

    return 0;
}


/* 100: Used as the maximum size for various arrays to handle states and productions. This is a common practice to ensure the arrays can store a reasonably large number of elements.
	20: Used for arrays like reduce, follow, fo_co, and first, assuming that the number of non-terminals is relatively small.
	99: Used in loops to handle arrays of size 100, as the index ranges from 0 to 99.
	struct state *I: A pointer to a state structure. Used to pass the state by reference to functions.
	The pointer is dereferenced using the -> operator to access members of the state structure.
	//gotostate moves the dot over the character and after that closure expands that gotostate by adding its productions	
	//through struct pointers changings at the states of struct are occuring and passing to them through accessing the address