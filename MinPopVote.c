
/*-------------------------------------------
Program: Bit Big Bug Tug
Description: Win Presidency With Only 20% Support?
Course: CS 211, Fall 2024, UIC
Author: Aryan Singh
------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "MinPopVote.h"

//----------------------------------------------
// Calculates the total number of electoral votes
// by summing the electoralVotes of each state.
// Parameters:
//  states: An array of State structures.
//  szStates: The number of states in the array.
//----------------------------------------------
int totalEVs(State *states, int szStates)
{
    int result = 0;
    for (int i = 0; i < szStates; i++)
    {
        result += states[i].electoralVotes;
    }
    return result;
}

//----------------------------------------------
// Calculates the total number of popular votes
// by summing the popularVotes of each state.
// Parameters:
//  states: An array of State structures.
//  szStates: The number of states in the array.
//----------------------------------------------
int totalPVs(State *states, int szStates)
{
    int result = 0;
    for (int i = 0; i < szStates; i++)
    {
        result += states[i].popularVotes;
    }
    return result;
}

//------------------------------------------------
// Parses command-line arguments to set the election year,
// fast mode, and quiet mode based on user input.
// Parameters:
//  argc: The count of command-line arguments.
//  argv: The command-line arguments as an array of strings.
//  year: Pointer to an integer where the year will be stored.
//  fastMode: Pointer to a boolean indicating if fast mode is enabled.
//  quietMode: Pointer to a boolean indicating if quiet mode is enabled.
//------------------------------------------------
bool setSettings(int argc, char **argv, int *year, bool *fastMode, bool *quietMode)
{
    *year = 0;
    *quietMode = false;
    *fastMode = false;

    for (int i = 1; i < argc; i++)
    {
        if (argv[i][1] == 'y')
        {
            if (i == (argc - 1))
            {
                return false;
            }
            i++;
            if (isdigit(argv[i][0]))
            {
                if (atoi(argv[i]) >= 1828 && atoi(argv[i]) <= 2020 && atoi(argv[i]) % 4 == 0)
                {
                    *year = atoi(argv[i]);
                }
            }
        }
        else if (argv[i][1] == 'f')
        {
            if (i > 1 && argv[i - 1][1] == 'y')
            {
                continue;
            }
            *fastMode = true;
        }

        else if (argv[i][1] == 'q')
        {
            if (i > 1 && argv[i - 1][1] == 'y')
            {
                continue;
            }
            *quietMode = true;
        }

        else
        {
            return false;
        }
    }

    return true;
}

//------------------------------------------------
// Generates the input filename in the format
// data/{year}.csv and stores it in the provided string buffer.
// Parameters:
//  filename: A string buffer to hold the input filename.
//  year: The year for which the filename will be generated.
//------------------------------------------------
void inFilename(char *filename, int year)
{
    sprintf(filename, "data/%d.csv", year);
    return;
}

//------------------------------------------------
// Generates the output filename in the format
// toWin/{year}_win.csv and stores it in the provided string buffer.
// Parameters:
//  filename: A string buffer to hold the output filename.
//  year: The year for which the filename will be generated.
//------------------------------------------------
void outFilename(char *filename, int year)
{
    sprintf(filename, "toWin/%d_win.csv", year);
    return;
}

//------------------------------------------------
// Parses a line of CSV data and fills in the fields
// of the provided State structure.
// Parameters:
//  line: A string containing a single line of data from the CSV file.
//  myState: A pointer to a State structure where parsed data will be stored.
//------------------------------------------------
bool parseLine(char *line, State *myState)
{
    int length = strlen(line);
    char temp[200]; // buffer
    int fieldNumber = 0;
    int tempIndex = 0;

    for (int i = 0; i <= length; i++)
    {
        if (line[i] == ',' || i == length)
        {
            temp[tempIndex] = '\0';
            if (fieldNumber == 0)
            {
                strcpy(myState->name, temp);
            }
            else if (fieldNumber == 1)
            {
                strcpy(myState->postalCode, temp);
            }
            else if (fieldNumber == 2)
            {
                myState->electoralVotes = atoi(temp);
            }
            else if (fieldNumber == 3)
            {
                myState->popularVotes = atoi(temp);
            }
            else
            {
                return false;
            }
            fieldNumber++;
            tempIndex = 0;

            for (int j = 0; j < sizeof(temp); j++)
            {
                temp[j] = '\0';
            }
        }
        else if (fieldNumber < 4)
        {
            temp[tempIndex] = line[i];
            tempIndex++;
        }
    }

    return (fieldNumber == 4);
}

//-----------------------------------------------------
// Reads election data from a file, parsing each line
// and storing the information in the provided array of State structures.
// Parameters:
//  filename: The name of the file to read.
//  allStates: An array of State structures where data will be stored.
//  nStates: A pointer to an integer where the number of states read will be stored.
//-----------------------------------------------------
bool readElectionData(char *filename, State *allStates, int *nStates)
{
    *nStates = 0; // required initialization prior to incrementing.
    FILE *file = fopen(filename, "r");
    char line[100];
    if (file == NULL)
    {
        return false;
    }
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (parseLine(line, &allStates[*nStates]))
        {
            (*nStates)++;
        }
    }
    fclose(file);
    return true;
}

//----------------------------------------------
// Recursively determines the minimum popular vote required
// to win the specified electoral votes, considering
// different combinations of states.
// Parameters:
//  states: An array of State structures.
//  szStates: The number of states in the array.
//  start: The index of the state to start processing from.
//  EVs: The number of electoral votes needed to win.
//----------------------------------------------
MinInfo minPopVoteAtLeast(State *states, int szStates, int start, int EVs)
{
    // Base case: Check if we have considered all states
    if (start >= szStates)
    {
        // If we have gone through all the states, return an empty result
        // with a check if we have enough electoral votes (EVs) to win
        MinInfo result;
        result.szSomeStates = 0;           // No states are included
        result.subsetPVs = 0;              // No popular votes since no states are selected
        result.sufficientEVs = (EVs <= 0); // True if EVs are zero or less
        return result;
    }

    // Base case: Check if we already have enough electoral votes to win
    if (EVs <= 0)
    {
        // If we have enough EVs, return an empty result
        MinInfo result;
        result.szSomeStates = 0;     // No states are included
        result.subsetPVs = 0;        // No popular votes since no states are selected
        result.sufficientEVs = true; // We have enough electoral votes to win
        return result;
    }

    // Recursive case:  including or excluding the current state
    // Option 1: Include current state
    MinInfo withState = minPopVoteAtLeast(states, szStates, start + 1, EVs - states[start].electoralVotes);
    withState.subsetPVs += (states[start].popularVotes / 2) + 1;
    if (withState.sufficientEVs || EVs - states[start].electoralVotes <= 0)
    {
        withState.someStates[withState.szSomeStates] = states[start];
        withState.szSomeStates++;
        withState.sufficientEVs = true;
    }

    // Option 2: exluding current state
    MinInfo withoutState = minPopVoteAtLeast(states, szStates, start + 1, EVs);

    // Return the better option the result
    MinInfo result;
    if (!withState.sufficientEVs && !withoutState.sufficientEVs)
    {
        result = withoutState;
    }
    else if (!withState.sufficientEVs)
    {
        result = withoutState;
    }
    else if (!withoutState.sufficientEVs)
    {
        result = withState;
    }
    else
    {
        result = (withState.subsetPVs < withoutState.subsetPVs) ? withState : withoutState;
    }

    return result;
}

MinInfo minPopVoteToWin(State *states, int szStates)
{
    int totEVs = totalEVs(states, szStates);
    int reqEVs = totEVs / 2 + 1; // required EVs to win election
    return minPopVoteAtLeast(states, szStates, 0, reqEVs);
}

//------------------------------------------------
// A fast, optimized recursive helper function
// that uses memoization to determine the minimum
// popular vote required to win the specified electoral votes.
// Parameters:
//  states: An array of State structures.
//  szStates: The number of states in the array.
//  start: The index of the state to start processing from.
//  EVs: The number of electoral votes needed to win.
//  memo: A two-dimensional array for memoization.
//------------------------------------------------
MinInfo minPopVoteAtLeastFast(State *states, int szStates, int start, int EVs, MinInfo **memo)
{
    // Base case: Check if we have considered all states
    if (start >= szStates)
    {
        // If we have gone through all the states, return an empty result
        // with a check if we have enough electoral votes (EVs) to win
        MinInfo result;
        result.szSomeStates = 0;           // No states are included
        result.subsetPVs = 0;              // No popular votes since no states are selected
        result.sufficientEVs = (EVs <= 0); // True if EVs are zero or less
        return result;
    }

    // Base case: Check if we already have enough electoral votes to win
    if (EVs <= 0)
    {
        // If we have enough EVs, return an empty result
        MinInfo result;
        result.szSomeStates = 0;     // No states are included
        result.subsetPVs = 0;        // No popular votes since no states are selected
        result.sufficientEVs = true; // We have enough electoral votes to win
        return result;
    }

    // Check if this subproblem has already been solved
    if (EVs >= 0 && memo[start][EVs].subsetPVs != -1)
    {
        return memo[start][EVs];
    }

    // Recursive case: including or excluding the current state
    // Option 1: Include current state
    MinInfo withState = minPopVoteAtLeastFast(states, szStates, start + 1, EVs - states[start].electoralVotes, memo);
    withState.subsetPVs += (states[start].popularVotes / 2) + 1;
    if (withState.sufficientEVs || EVs - states[start].electoralVotes <= 0)
    {
        withState.someStates[withState.szSomeStates] = states[start];
        withState.szSomeStates++;
        withState.sufficientEVs = true;
    }

    // Option 2: exclude current state
    MinInfo withoutState = minPopVoteAtLeastFast(states, szStates, start + 1, EVs, memo);

    // Return the better option and memoize the result
    MinInfo result;
    if (!withState.sufficientEVs && !withoutState.sufficientEVs)
    {
        result = withoutState;
    }
    else if (!withState.sufficientEVs)
    {
        result = withoutState;
    }
    else if (!withoutState.sufficientEVs)
    {
        result = withState;
    }
    else
    {
        result = (withState.subsetPVs < withoutState.subsetPVs) ? withState : withoutState;
    }

    // Store the computed result in the memo array
    memo[start][EVs] = result;

    return result;
}

MinInfo minPopVoteToWinFast(State *states, int szStates)
{
    int totEVs = totalEVs(states, szStates);
    int reqEVs = totEVs / 2 + 1; // required EVs to win election

    MinInfo **memo = (MinInfo **)malloc((szStates + 1) * sizeof(MinInfo *));
    for (int i = 0; i < szStates + 1; ++i)
    {
        memo[i] = (MinInfo *)malloc((reqEVs + 1) * sizeof(MinInfo));
        for (int j = 0; j < reqEVs + 1; ++j)
        {
            memo[i][j].subsetPVs = -1;
        }
    }
    MinInfo res = minPopVoteAtLeastFast(states, szStates, 0, reqEVs, memo);

    //----------------------------------------------
    // TODO: Task 8 - [memo] will go out of scope
    //                upon return, so free all
    //                heap-allocated memory for
    //                [memo] before return
    //----------------------------------------------
    for (int i = 0; i <= szStates; i++)
    {
        free(memo[i]);
    }
    free(memo);
    return res;
}

bool writeSubsetData(char *filenameW, int totEVs, int totPVs, int wonEVs, MinInfo toWin)
{
    //-----------------------------------------------------
    // TODO: Task 9 - write the writeSubsetData() function
    //-----------------------------------------------------
    FILE *file = fopen(filenameW, "w");
    if (file == NULL)
    {
        return false;
    }

    fprintf(file, "%d,%d,%d,%d\n", totEVs, totPVs, wonEVs, toWin.subsetPVs);

    for (int i = toWin.szSomeStates - 1; i >= 0; i--)
    {
        fprintf(file, "%s,%s,%d,%d\n", toWin.someStates[i].name, toWin.someStates[i].postalCode, toWin.someStates[i].electoralVotes, (toWin.someStates[i].popularVotes / 2) + 1);
    }

    fclose(file);

    return true;
}
