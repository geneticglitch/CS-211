/*-------------------------------------------
Program: Dynamic Food Webs
Description: Food Web Analysis with Dynamic Memory
Course: CS 211, Fall 2024, UIC
Author: Aryan Singh
-------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct Org_struct {
    char name[20];
    int* prey; //dynamic array of indices of its prey
    int numPrey;
} Org;

/**
 * TASK 3 Add a new organism to the food web
 * 
 * @param ppWeb - a pointer to a dynamically allocated array of Orgs
 * @param pNumOrg - a passed-by-pointer parameter for the number of organisms = size of web[]
 * @param newOrgName - a character array (i.e. string) for the name of the new organism 
 *
 * @return ppWeb - the Org array that has been reallocated on the heap to allow space for the new organism
 * @return pNumOrg - the number of organisms should increase by one to account for the new organism
*/
void addOrgToWeb(Org** ppWeb, int* pNumOrg, char* newOrgName) {
     
    //check if the ppWeb is an empty array if so allocate memory for the new organism
    if(*ppWeb == NULL){
        *ppWeb = (Org*)malloc(sizeof(Org));
        strcpy((*ppWeb)[0].name, newOrgName);
        *pNumOrg = 1;
    }
    else{
        //increment the number of org's then allocate a new temp Org array for the size
        (*pNumOrg)++;
        Org* tempppweb = (Org* ) malloc(sizeof(Org) * (*pNumOrg));

        //copies all the Org's into the new temp Org array
        for(int i =0 ;i< (*pNumOrg-1) ; i++){
            tempppweb[i] = (*ppWeb)[i];
        }

        strcpy(tempppweb[*pNumOrg-1].name, newOrgName);

        // Deallocate the old Org Web and replace it with the new one
        free(*ppWeb);
        *ppWeb = tempppweb;
    }
    (*ppWeb)[*pNumOrg-1].prey = NULL;
    (*ppWeb)[*pNumOrg-1].numPrey = 0;
}

/**
 * TASK 4 Adds a predator-prey relation to the food web
 *
 * @param pWeb - a dynamically allocated array of Orgs
 * @param numOrg - number of organisms = size of web[]
 * @param predInd - predator index in pWeb[]; an entry to its prey[] subitem will be added
 * @param preyInd - prey index in pWeb[]; will be added to predator's prey[] subitem
 *
 * @return bool - true if the relationship was added successfully, false otherwise
 *                If the predator or prey index is invalid or the relation already 
 *                exists the function returns false and prints an error message 
*/
bool addRelationToWeb(Org* pWeb, int numOrg, int predInd, int preyInd) {

    //Validate the provided  indices for predator and prey
    if(predInd >= numOrg || preyInd >= numOrg || preyInd < 0 || predInd < 0){
        printf("Invalid predator and/or prey index. No relation added to the food web.\n");
        return false;
    }

    //Check if the relationship already exixts
    for(int i = 0; i < pWeb[predInd].numPrey; i++){
        if(pWeb[predInd].prey[i] == preyInd){
            printf("Duplicate predator/prey relation. No relation added to the food web.\n");
            return false;
        }
    }

    //Allocate space for a tempPred array one greater than the current size of prey array of the Organism
    int* tempPred = (int*) malloc(sizeof(int) * (pWeb[predInd].numPrey + 1));
    
    // Copy existing prey indices to tempPred array
    for(int i = 0; i < pWeb[predInd].numPrey; i++){
        tempPred[i] = pWeb[predInd].prey[i];
    }
    
    // Add the new Prey index
    tempPred[pWeb[predInd].numPrey] = preyInd;

    //Free the old prey array of the organism and assign the new one
    free(pWeb[predInd].prey);
    pWeb[predInd].prey = tempPred;
    
    //increment for the new prey index
    pWeb[predInd].numPrey += 1;

    return true;
}

 /**
  * TASK 7 Remove an organsim from the food web
  * 
  * @param ppWeb - a pointer to a dynamically allocated array of Orgs
  * @param pNumOrg - a passed-by-pointer parameter for the number of organisms = size of web[]
  * @param index - organism index in the (*ppWeb)[] array to remove
  *
  * @return bool - true if organism is successfully removed, false otherwise 
 */   
bool removeOrgFromWeb(Org** ppWeb, int* pNumOrg, int index) {

    //check if the index is valid
    if(index < 0 || index >= *pNumOrg){
        printf("Invalid extinction index. No organism removed from the food web.");
        return false;
    }

    //free the prey array if the organism to be removed
    free((*ppWeb)[index].prey);

    // create a new web array, coping all orgs excluding the organism at the given index
    // if ppWeb only has one organism then set it to NULL
    Org* tempNewWeb = (*pNumOrg > 1) ? (Org*)malloc(sizeof(Org) * (*pNumOrg)) : NULL;
    int z = 0;
    for (int i = 0; i < *pNumOrg; i++) {
        if (i != index) {
            tempNewWeb[z] = (*ppWeb)[i];
            z++;
        }
    }

    // free the old array of org then replace the old web with the new one
    free(*ppWeb);
    *ppWeb = tempNewWeb;
    (*pNumOrg)--;   // decrement the num org for the removed org

    //update prey arrays of remaining organisms
    for(int i = 0; i < *pNumOrg; i++){
        // remove the extinct  organism from the prey arrays
        for(int j = 0; j < (*ppWeb)[i].numPrey ; j++){
            //check if org at i eats the now extinct org
            if((*ppWeb)[i].prey[j] == index){
                ((*ppWeb)[i].numPrey)--;
                // if numPrey for the Predator is now 0 then set the prey array to NULL
                if ((*ppWeb)[i].numPrey == 0) { 
                    free((*ppWeb)[i].prey);
                    (*ppWeb)[i].prey = NULL;
                }else{
                    // create new prey array without the extinct organism
                    int* tempPreyArray = (int *) malloc (sizeof(int) * (*ppWeb)[i].numPrey);
                    // copy all the indices except the now extinct org index
                    int l = 0;
                    for(int k = 0; k < (*ppWeb)[i].numPrey + 1; k++){
                        if(k != j){
                            tempPreyArray[l] = (*ppWeb)[i].prey[k];
                            l++;
                        }
                    }
                    // free the old prey array then replace with the new one
                    free((*ppWeb)[i].prey);
                    (*ppWeb)[i].prey = tempPreyArray;
                }
                break;
            }
        }

        // Adjust prey indices that are greater than the removed index
        for(int j = 0; j < (*ppWeb)[i].numPrey; j++){
            if((*ppWeb)[i].prey[j] > index){
                (*ppWeb)[i].prey[j]--;
            }
        }
    }

    return true; 
}

/**
 * Frees the dynamically allocated memory associated with the food web
 *
 * @param pWeb - pointer to the array of organisms
 * @param numOrg - the number of organisms in the array
*/
void freeWeb(Org* pWeb, int numOrg) {

    for(int i =0 ;i< numOrg ;i ++){
        // if the organism has a prey array free it
        if(pWeb[i].prey != NULL){
            free(pWeb[i].prey);
            pWeb[i].prey = NULL;
        }
    }

    free(pWeb);
}

/**
 * Prints the food web, listing each organism and its prey if any.
 *
 * @param pWeb - pointer to the array of organisms
 * @param numOrg - the number of organisms in the array
*/
void printWeb(Org* pWeb, int numOrg) {

    for(int i = 0;i<numOrg ;i++){
        printf("  (%d) %s",i,pWeb[i].name);

        //print "eats" if numPrey is greater than 0
        if(pWeb[i].numPrey > 0){
            printf(" eats");
        }else{
            printf("\n");
        }
        for(int j = 0; j<pWeb[i].numPrey;j++){

            // print with commas until the last one
            if(j != pWeb[i].numPrey-1){
                printf(" %s,", pWeb[pWeb[i].prey[j]].name);
            } else{
                printf(" %s\n", pWeb[pWeb[i].prey[j]].name);
            }
        }
    }

}


void displayAll(Org* pWeb, int numOrg, bool modified) {

    if (modified) printf("UPDATED ");
    printf("Food Web Predators & Prey:\n");
    printWeb(pWeb,numOrg); 
    printf("\n");

    if (modified) printf("UPDATED ");
    printf("Apex Predators:\n");
    // identify and print the organisms not eaten by any others
    for(int i = 0; i < numOrg ; i++){
        bool notEaten = true; // flag to check if the organism is not eaten
        for(int j = 0; j < numOrg ; j++){
            if(i != j && pWeb[j].numPrey != 0){ // check if the organism has a prey
                for(int k = 0; k < pWeb[j].numPrey ;k++){
                    if(pWeb[j].prey[k] == i){ // if organism i is prey
                        notEaten = false; // mark as eaten
                        break;
                    }
                }
            }
            if(!notEaten) break; // exit early if found
        }
        if(notEaten) printf("  %s\n", pWeb[i].name); // print the organism's name if not eaten
    }
    printf("\n");

    if (modified) printf("UPDATED ");
    printf("Producers:\n");
    // identify and print the organisms that eat no other organisms
    for(int i = 0; i < numOrg; i++){
        if(pWeb[i].prey == NULL){
            printf("  %s\n", pWeb[i].name);
        }
    }
    printf("\n");

    if (modified) printf("UPDATED ");
    printf("Most Flexible Eaters:\n");
    // identity and print the organism(s) with the most prey
    int mostFlexEaterPreyMaxCount; // Count of the max amount of prey for all the orgs in the web
    mostFlexEaterPreyMaxCount = 0;
    for(int i = 0; i < numOrg ;i ++){
        if(mostFlexEaterPreyMaxCount <= pWeb[i].numPrey){
            mostFlexEaterPreyMaxCount = pWeb[i].numPrey; //update max count
        }
    }
    for(int i = 0; i < numOrg; i++){
        if(pWeb[i].numPrey == mostFlexEaterPreyMaxCount){ // print all with max prey
            printf("  %s\n", pWeb[i].name);
        }
    }

    printf("\n");

    if (modified) printf("UPDATED ");
    printf("Tastiest Food:\n");
    // identity and print organism(s) eaten by the most other organisms
    int tempTastiestFoodArr[numOrg]; // array to keep track of how many times each org is eaten
    int tempTastiestFoodMaxCount = 0; // keeps track of the max of the tempTastiestFoodArr

    for (int i = 0; i < numOrg; i++) {
        tempTastiestFoodArr[i] = 0;
        for (int j = 0; j < numOrg; j++) {
            for (int k = 0; k < pWeb[j].numPrey; k++) {
                if (pWeb[j].prey[k] == i) { // if org i is prey of j 
                    tempTastiestFoodArr[i]++; // increment count
                }
            }
        }
        if (tempTastiestFoodArr[i] > tempTastiestFoodMaxCount) {
            tempTastiestFoodMaxCount = tempTastiestFoodArr[i]; // update max count simultaneously 
        }
    }


    for(int i = 0; i < numOrg; i++){
        if(tempTastiestFoodArr[i] == tempTastiestFoodMaxCount){
            printf("  %s\n", pWeb[i].name);
        }
    }

    printf("\n");

    if (modified) printf("UPDATED ");
    printf("Food Web Heights:\n");
    // calculate and print the length of the longest chain from any producer to each organism
    int tempHeights[numOrg]; // array to store heights of each org

    // init heights
    for(int i = 0;i < numOrg; i++){
        tempHeights[i] = 0;
    }

    bool changesWereMade = true;

    // loop until changes are made
    while(changesWereMade){
        changesWereMade = false;
        for(int i = 0; i< numOrg; i++){
            if(pWeb[i].numPrey == 0){ // skip producers
                continue;
            }
            int maxPreyHeight = -1; // track the max height
            for(int j =0 ;j<pWeb[i].numPrey ;j++){
                if(tempHeights[pWeb[i].prey[j]] > maxPreyHeight){
                    maxPreyHeight = tempHeights[pWeb[i].prey[j]]; // update max count
                }
            }

            int newHeight = maxPreyHeight + 1; // calculate max height
            if(newHeight > tempHeights[i]){ // update if new height is greater 
                tempHeights[i] = newHeight;
                changesWereMade = true; // mark change
            }
        }
    }

    // print heights of each org
    for(int i = 0; i<numOrg;i++){
        printf("  %s: %d\n",pWeb[i].name, tempHeights[i]);
    }
    printf("\n");

    if (modified) printf("UPDATED ");
    printf("Vore Types:\n");
    // classify all organisms and print each group (producers, herbivores, omnivores, & carnivores)
    int tempVoreTypesArr[numOrg]; //0 is producers, 1 is herbivores, 2 is omnivores, 3 is carnivores

    for(int i = 0; i < numOrg ;i++){
        tempVoreTypesArr[i] = 0;
        bool eatsProducers  = false;  //flags for eating producers
        bool eatsNonProducers  = false; // flag for eating non producers

        if(!(pWeb[i].numPrey == 0)) {
            for(int j = 0; j < pWeb[i].numPrey;j++){
                if(pWeb[pWeb[i].prey[j]].numPrey == 0){ //check if it eats producers 
                    eatsProducers  = true;
                }
                else{
                    eatsNonProducers  = true; // it eats other organisms
                }
            }  

            // classify based on the flags
            if (eatsProducers && !eatsNonProducers) {
                tempVoreTypesArr[i] = 1; //Herbivore
            } else if (eatsProducers && eatsNonProducers) {
                tempVoreTypesArr[i] = 2; // Ormivore
            } else if (!eatsProducers && eatsNonProducers) {
                tempVoreTypesArr[i] = 3; // Carnivore
            }
        }

    }

    printf("  Producers:\n");
    for(int i = 0; i < numOrg ;i++){
        if(tempVoreTypesArr[i] == 0){
            printf("    %s\n",pWeb[i].name);
        }
    }
    printf("  Herbivores:\n");
    for(int i = 0; i < numOrg ;i++){
        if(tempVoreTypesArr[i] == 1){
            printf("    %s\n",pWeb[i].name);
        }
    }
    printf("  Omnivores:\n");
    for(int i = 0; i < numOrg ;i++){
        if(tempVoreTypesArr[i] == 2){
            printf("    %s\n",pWeb[i].name);
        }
    }
    printf("  Carnivores:\n");
    for(int i = 0; i < numOrg ;i++){
        if(tempVoreTypesArr[i] == 3){
            printf("    %s\n",pWeb[i].name);
        }
    }
    printf("\n");

}

/**
 * TASK 2 Processes command-line arguments to set program modes: basicMode, debugMode, quietMode
 * 
 * @param argc - the number of command-line arguments
 * @param argv - an array of command-line argument strings
 * @param pBasicMode - a pointer to the boolean that will be set to TRUE if basic mode is enabled
 * @param pDebugMode - a pointer to the boolean that will be set to TRUE if debug mode is enabled
 * @param pQuietMode - a pointer to the boolean that will be set to TRUE if quiet mode is enabled
 * 
 * @return bool - return true if all arguments are valid and the modes are set correctly
 *                return false if there are invalid or duplicate command-line arguments
*/
bool setModes(int argc, char* argv[], bool* pBasicMode, bool* pDebugMode, bool* pQuietMode) {

    // return false if too many arguments are passed
    if(argc > 4){
        return false;
    }

    // flags to detect duplicate arguments for each mode
    bool vDupQm = false, vDupBm = false, vDupDm = false;

    // loop through command-line arguments starting from index 1 (skippinf over the program name)
    for (int i = 1; i < argc; i++) {

        // check if the arguments starts with 'q' (quiet mode)
        if(argv[i][1] == 'q'){
            if(vDupQm){ // if quiet mode has already been set, return false
                return false;
            }
            *pQuietMode = true; // set quiet mode to true
            vDupQm = true; // mark quiet mode as processed
        }
        else if(argv[i][1] == 'b'){
            if(vDupBm){
                return false;
            }
            *pBasicMode = (vDupBm == false) ? true : false;
            vDupBm = true;
        }
        else if(argv[i][1] == 'd'){
            if(vDupDm){
                return false;
            }
            *pDebugMode = (vDupDm == false) ? true : false;
            vDupDm = true;
        }

        // if an invalid argument is provided return false
        else{
            return false;
        }
    }
    
    return true; 
}


void printONorOFF(bool mode) {
    if (mode) {
        printf("ON\n");
    } else {
        printf("OFF\n");
    }
}


int main(int argc, char* argv[]) {  

    // -----------------------------------------------------------------------------------------------------
    // TODO (Task 1): add comments to the main application to show understanding
    //  DO NOT MODIFY THE CODE BENEATH THIS COMMENT (except for debugging purposes and adding comments).
    //  The code in main() represents the primary application, which has been provided for you.
    //  There are many calls to functions that you must write above. Read through the code below, understand  
    //  how it works, check how functions are called, and add relevant comments to show your understanding.
    //  However, no changes should be made to the code in main(). You can put in debugging print statements, 
    //  but those should be removed or commented out prior to submitting to the autograder.
    // -----------------------------------------------------------------------------------------------------
    
    // flags for the mode selector
    bool basicMode = false; // indicates if in basic mode
    bool debugMode = false; // indicates if in debug mode
    bool quietMode = false; // indicates if in quiet mode

    //set modes based on command line ars, terminate if invalid
    if (!setModes(argc, argv, &basicMode, &debugMode, &quietMode)) {
        printf("Invalid command-line argument. Terminating program...\n");
        return 1;
    }

    // display program settings
    printf("Program Settings:\n");
    printf("  basic mode = ");
    printONorOFF(basicMode);
    printf("  debug mode = ");
    printONorOFF(debugMode);
    printf("  quiet mode = ");
    printONorOFF(quietMode);
    printf("\n");

    
    int numOrgs = 0; // init the num of organisms
    printf("Welcome to the Food Web Application\n\n");
    printf("--------------------------------\n\n");

    Org* pWeb = NULL; // creating a new pointer of food web structure

    printf("Building the initial food web...\n");
    
    if (!quietMode) printf("Enter the name for an organism in the web (or enter DONE): ");
    char tempName[20] = ""; // temp storage for organism names
    scanf("%s",tempName); 
    if (!quietMode) printf("\n");

    // loop to add organisms until the user enters "DONE"
    while (strcmp(tempName,"DONE") != 0) {
        addOrgToWeb(&pWeb,&numOrgs,tempName); // add the organism to the food web
        if (debugMode) {
            printf("DEBUG MODE - added an organism:\n");
            printWeb(pWeb,numOrgs); // printing the current state if debug mode is on
            printf("\n");
        }
        if (!quietMode) printf("Enter the name for an organism in the web (or enter DONE): ");
        scanf("%s",tempName); 
        if (!quietMode) printf("\n");
    }
    if (!quietMode) printf("\n");

    // asking the user to enter predator/prey relationships
    if (!quietMode) printf("Enter the pair of indices for a predator/prey relation.\n");
    if (!quietMode) printf("Enter any invalid index when done (-1 2, 0 -9, 3 3, etc.).\n");
    if (!quietMode) printf("The format is <predator index> <prey index>: ");

    // scanning for for the pred/ prey indices
    int predInd, preyInd;
    scanf("%d %d",&predInd, &preyInd);
    if (!quietMode) printf("\n");

    // loop to add predator/prey relationships until an invalid index is entered
    while (predInd >= 0 && preyInd >= 0 && predInd < numOrgs &&  preyInd < numOrgs && predInd != preyInd) {
        addRelationToWeb(pWeb,numOrgs,predInd,preyInd); // add relationships
        if (debugMode) {
            printf("DEBUG MODE - added a relation:\n");
            printWeb(pWeb,numOrgs); // printing the current state if debug mode is on
            printf("\n");
        }
        if (!quietMode) printf("Enter the pair of indices for a predator/prey relation.\n");
        if (!quietMode) printf("Enter any invalid index when done (-1 2, 0 -9, 3 3, etc.).\n");
        if (!quietMode) printf("The format is <predator index> <prey index>: ");
        
        scanf("%d %d",&predInd, &preyInd);  
        if (!quietMode) printf("\n");
    }
    printf("\n");

    // Display completion message of init food web setup
    printf("--------------------------------\n\n");
    printf("Initial food web complete.\n");
    printf("Displaying characteristics for the initial food web...\n");
    
    // call the function to display all characteristics of the food web
    displayAll(pWeb,numOrgs,false);

    // if not in basic mode provide options to modify the food web
    if (!basicMode) {
        printf("--------------------------------\n\n");
        printf("Modifying the food web...\n\n");
        char opt = '?';

        // loop for user options until they choose to quit
        while (opt != 'q') {
            if (!quietMode) {
                printf("Web modification options:\n");
                printf("   o = add a new organism (expansion)\n");
                printf("   r = add a new predator/prey relation (supplementation)\n");
                printf("   x = remove an organism (extinction)\n");
                printf("   p = print the updated food web\n");
                printf("   d = display ALL characteristics for the updated food web\n");
                printf("   q = quit\n");
                printf("Enter a character (o, r, x, p, d, or q): ");
            }
            scanf(" %c", &opt); // scan for user input
            printf("\n\n");

            // handle each options based on user's input
            if (opt == 'o') {
                char newName[20];
                if (!quietMode) printf("EXPANSION - enter the name for the new organism: ");
                scanf("%s",newName);
                if (!quietMode) printf("\n");
                printf("Species Expansion: %s\n", newName);
                addOrgToWeb(&pWeb,&numOrgs,newName); // add new org to the web 
                printf("\n");

                if (debugMode) {
                    printf("DEBUG MODE - added an organism:\n");
                    printWeb(pWeb,numOrgs); // printing the current state if debug mode is on
                    printf("\n");
                }

            } else if (opt == 'x') {
                int extInd;
                if (!quietMode) printf("EXTINCTION - enter the index for the extinct organism: ");
                scanf("%d",&extInd);
                if (!quietMode) printf("\n");
                // validate the index add remove the org if valid
                if (extInd >= 0 && extInd < numOrgs) {
                    printf("Species Extinction: %s\n", pWeb[extInd].name);
                    removeOrgFromWeb(&pWeb,&numOrgs,extInd); // remove the org from the web 
                } else {
                    printf("Invalid index for species extinction\n");
                }
                printf("\n");
                
                if (debugMode) {
                    printf("DEBUG MODE - removed an organism:\n");
                    printWeb(pWeb,numOrgs); // printing the current state if debug mode is on
                    printf("\n");
                }

            } else if (opt == 'r') {
                int extInd;
                if (!quietMode) printf("SUPPLEMENTATION - enter the pair of indices for the new predator/prey relation.\n");
                if (!quietMode) printf("The format is <predator index> <prey index>: ");
                scanf("%d %d",&predInd, &preyInd);
                if (!quietMode) printf("\n");

                if (addRelationToWeb(pWeb,numOrgs,predInd,preyInd)) {  // add the new relationship as a new food source for a predator
                    printf("New Food Source: %s eats %s\n", pWeb[predInd].name, pWeb[preyInd].name);
                };
                printf("\n");
                if (debugMode) {
                    printf("DEBUG MODE - added a relation:\n");
                    printWeb(pWeb,numOrgs); // printing the current state if debug mode is on
                    printf("\n");
                }

            } else if (opt == 'p') {
                // print the updated web 
                printf("UPDATED Food Web Predators & Prey:\n");
                printWeb(pWeb,numOrgs);
                printf("\n");
                
            } else if (opt == 'd') {
                // display all characteristics for the updated food web
                printf("Displaying characteristics for the UPDATED food web...\n\n");
                displayAll(pWeb,numOrgs,true);

            }
            printf("--------------------------------\n\n");
        
        }
        
    }

    // Call the freeWeb function to free alloacted space for each org and the org array it self
    freeWeb(pWeb,numOrgs);

    printf("\n\n");

    return 0;
}

