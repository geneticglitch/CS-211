/*-------------------------------------------
Program: Totalistic Cellular Automaton
Description: This program simulates a one-dimensional cellular automaton based on totalistic rules.
Course: CS 211, Fall 2024, UIC
Author: Aryan Singh
------------------------------------------- */

#include <stdio.h>
#include <stdbool.h>
#include <math.h>

const int WORLD_SIZE = 65;

typedef struct cell_struct
{
    int localSum; // Sum of [left] + [me] + [right]: values 0-6
    int status;   // Current status: 0, 1, or 2
    int count;    // Accumulated count of active status
} cell;

/**
 * Task-1 Converts an integer rule ( 0-2186 ) to its base-3 representation.
 * @param valArray Array to store the base-3 representation
 * @param rule The rule number to convert
 * @return true if the input rule is valid ( 0-2186 ), false otherwise
 */
bool setValArray(int valArray[7], int rule)
{

    // validate input
    if (rule > 2186 || rule < 0)
    {
        return false;
    }

    // Convert to base-3
    for (int i = 0; i < 7; i++)
    {
        valArray[i] = rule % 3;
        rule /= 3;
    }

    return true;
}

/**
 * Task-4 Updates the localSum for each cell in the world array.
 * @param world The array of cells representing the world
 */
void setSums(cell world[WORLD_SIZE])
{
    for (int i = 0; i < WORLD_SIZE; i++)
    {
        int leftIndex = (i == 0) ? WORLD_SIZE - 1 : (i - 1);
        int rightIndex = (i + 1) % WORLD_SIZE;
        world[i].localSum = world[leftIndex].status + world[i].status + world[rightIndex].status;
    }
}

/**
 * Task-6 Evolves the world to the next generation.
 * @param world The array of cells representing the world
 * @param ruleValArray The rule value array
 * @return The total sum of all status values after evolution
 */
int evolveWorld(cell world[WORLD_SIZE], int ruleValArray[7])
{
    int worldStatusSum = 0;

    for (int i = 0; i < WORLD_SIZE; i++)
    {
        // Update cell status based on rule and update counters
        world[i].status = ruleValArray[world[i].localSum];
        worldStatusSum += world[i].status;
        world[i].count += world[i].status;
    }

    return worldStatusSum;
}

/**
 * Prints the value array in the reverse order.
 * @param valArray The array to print
 */
void printValArray(int valArray[])
{
    for (int i = 6; i >= 0; i--)
    {
        printf("%d", valArray[i]);
    }
}

/**
 * Validates user input within a given range.
 * @param prompt The prompt to display to the user
 * @param min The minimum allowed value
 * @param max The maximum allowed value
 * @return The validated input
 */
int validateInput(char *prompt, int min, int max)
{
    int userInput;
    bool isValid = false;

    while (!isValid)
    {
        printf("%s", prompt);
        scanf("%d", &userInput);

        if (userInput >= min && userInput <= max)
        {
            isValid = true;
        }
        else
        {
            printf("\n");
        }
    }
    return userInput;
}

/**
 * Initializes the world status.
 * @param world The array of cells representing the world
 * @param initMiddleVal The initial value for the middle cell
 */
void setInitWorldStatus(cell world[WORLD_SIZE], int initMiddleVal)
{
    int middle = WORLD_SIZE / 2; 

    for (int i = 0; i < WORLD_SIZE; i++)
    {
        world[i].status = (i == middle) ? initMiddleVal : 0;
        world[i].count = world[i].status;
    }
}

/**
 * Prints the current generation.
 * @param world The array of cells representing the world
 * @param sumStatus The sum of all cell statuses
 */
void printGen(cell world[WORLD_SIZE], int sumStatus)
{
    for (int i = 0; i < WORLD_SIZE; i++)
    {
        int valStatus = world[i].status;

        if (valStatus == 0)
        {
            printf(" ");
        }
        else if (valStatus == 1)
        {
            printf("-");
        }
        else
        {
            printf("+");
        }
    }
    printf(" %d \n", sumStatus);
}

/**
 * Prints the evolution states.
 * @param valArray The rule value array
 */
void printEvolutionStates(int valArray[7])
{
    printf("\nThe evolution of all possible states are as follows:\n");
    printf("Local Sum:   6       5       4       3       2       1       0      \n");
    printf("States:    |+++|   |++-|   |++ |   |+- |   |-- |   |-  |   |   |    \n");
    printf("                   |+-+|   |+ +|   |+ -|   |- -|   | - |            \n");
    printf("                   |-++|   | ++|   |-+ |   | --|   |  -|            \n");
    printf("                           |+--|   | +-|   |+  |                    \n");
    printf("                           |-+-|   |- +|   | + |                    \n");
    printf("                           |--+|   | -+|   |  +|                    \n");
    printf("                                   |---|                            \n");
    printf("New Status: ");

    for (int i = 6; i >= 0; i--)
    {
        int valStatus = valArray[i];
        printf("|");

        if (valStatus == 0)
        {
            printf(" ");
        }
        else if (valStatus == 1)
        {
            printf("-");
        }
        else
        {
            printf("+");
        }
        printf("|     ");
    }
    printf("\n\n");
}

/**
 * Prints the final cell counts.
 * @param world The array of cells representing the world
 */
void printCellCounts(cell world[WORLD_SIZE])
{
    printf("_________________________________________________________________\n");
    for (int i = 0; i < WORLD_SIZE; i++)
    {
        if (world[i].count > 9)
        {
            printf("%d", (world[i].count) / 10);
        }
        else
        {
            printf(" ");
        }
    }

    printf("\n");
    for (int i = 0; i < WORLD_SIZE; i++)
    {
        if (world[i].count == 0)
        {
            printf(" ");
        }
        else
        {
            printf("%d", world[i].count % 10);
        }
    }
}

int main() {
	cell world[WORLD_SIZE];
	int rule, valArray[7];
	int generationCount, activeCellValue, worldStatusSum;

	printf("Welcome to the Totalistic Cellular Automaton!\n");

	// Get and validate rule input
	while (true) {
		printf("Enter the rule # (0-2186): ");
		scanf("%d", &rule);

		if (setValArray(valArray, rule)) {
			break;
		} else {
			printf("\n");
		}
	}

	printf("\n\nThe value array for rule #%d is ", rule);
	printValArray(valArray);
	printf("\n");

	// Print evolution steps for all possible states
	printEvolutionStates(valArray);

	// Get user input for generation count and active cell value
	generationCount = validateInput("Enter the number of generations (1-49): ", 1, 49);     
	printf("\n");
	activeCellValue = validateInput("Enter the value (1 or 2) for the initial active cell: ", 1, 2);
	printf("\n\n");

	// Initialize world and set initial sums
	printf("Initializing world & evolving...\n");
	setInitWorldStatus(world, activeCellValue);
	setSums(world);
		 
	// Evolve and display the world through the user-specified number of generations
	worldStatusSum = activeCellValue;
	for (int i = 0; i < generationCount; i++) {
		printGen(world, worldStatusSum);
		if (i != (generationCount - 1)) {
			setSums(world);
			worldStatusSum = evolveWorld(world, valArray);  
		}
	}

	// Print the final cell count
	printCellCounts(world);

	printf("\n");

	return 0;
}
