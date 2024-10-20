#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "MinPopVote.h"

bool test_totalEVs()
{
    State aStates[10];
    int res;

    aStates[0].electoralVotes = 5;
    aStates[1].electoralVotes = 8;
    aStates[2].electoralVotes = 12;
    aStates[3].electoralVotes = 6;
    aStates[4].electoralVotes = 7;
    aStates[5].electoralVotes = 10;

    printf(" Checking totalEVs() for 5 States:\n");
    res = totalEVs(aStates, 5);
    if (res != 38)
    {
        printf("  individual state EVs are 5, 8, 12, 6, 7\n");
        printf("  expected total EVs = 38, actual total EVs = %d\n", res);
        return false;
    }
    printf(" Checking totalEVs() for 6 States:\n");
    res = totalEVs(aStates, 6);
    if (res != 48)
    {
        printf("  individual state EVs are 5, 8, 12, 6, 7, 10\n");
        printf("  expected total EVs = 48, actual total EVs = %d\n", res);
        return false;
    }

    //--------------------------------------------------------
    // TODO: Task 1 - perform additional testing to make this
    //                test case for totalEVs() more robust
    //--------------------------------------------------------
    printf(" Checking totalEVs() for 0 States:\n");
    res = totalEVs(aStates, 0);
    if (res != 0)
    {
        printf("  expected total EVs = 0, actual total EVs = %d\n", res);
        return false;
    }

    // Adding large values for testing
    aStates[6].electoralVotes = 12000;
    aStates[7].electoralVotes = 622;
    aStates[8].electoralVotes = 72222;
    aStates[9].electoralVotes = 10111;

    printf(" Checking totalEVs() for 10 States:\n");
    res = totalEVs(aStates, 10);
    if (res != 95003)
    {
        printf("  individual state EVs are 5, 8, 12, 6, 7, 10, 12000, 622, 72222, 10111\n");
        printf("  expected total EVs = 0, actual total EVs = %d\n", res);
        return false;
    }

    for (int i = 0; i < 6; i++)
    {
        aStates[i].electoralVotes = 0;
    }

    printf(" Checking totalEVs() for 6 States with 0 EVs:\n");
    res = totalEVs(aStates, 6);
    if (res != 0)
    {
        printf("  all states have 0 electronic Votes");
        printf("  expected total EVs = 0, actual total EVs = %d\n", res);
        return false;
    }

    return true;
}

bool test_totalPVs()
{
    //--------------------------------------------------------
    // TODO: Task 1 - write your own test case for totalPVs();
    //                use test_totalEVs() as a sample;
    //                make sure to test all components
    //--------------------------------------------------------
    State aStates[10];
    int res;

    aStates[0].popularVotes = 5;
    aStates[1].popularVotes = 8;
    aStates[2].popularVotes = 12;
    aStates[3].popularVotes = 6;
    aStates[4].popularVotes = 7;
    aStates[5].popularVotes = 10;

    printf(" Checking totalPVs() for 5 States:\n");
    res = totalPVs(aStates, 5);
    if (res != 38)
    {
        printf("  individual state PVs are 5, 8, 12, 6, 7\n");
        printf("  expected total PVs = 38, actual total PVs = %d\n", res);
        return false;
    }
    printf(" Checking totalPVs() for 6 States:\n");
    res = totalPVs(aStates, 6);
    if (res != 48)
    {
        printf("  individual state PVs are 5, 8, 12, 6, 7, 10\n");
        printf("  expected total PVs = 48, actual total PVs = %d\n", res);
        return false;
    }

    printf(" Checking totalPVs() for 0 States:\n");
    res = totalPVs(aStates, 0);
    if (res != 0)
    {
        printf("  expected total PVs = 0, actual total PVs = %d\n", res);
        return false;
    }

    // Adding large values for testing
    aStates[6].popularVotes = 12000;
    aStates[7].popularVotes = 622;
    aStates[8].popularVotes = 72222;
    aStates[9].popularVotes = 10111;

    printf(" Checking totalPVs() for 10 States:\n");
    res = totalPVs(aStates, 10);
    if (res != 95003)
    {
        printf("  individual state PVs are 5, 8, 12, 6, 7, 10, 12000, 622, 72222, 10111\n");
        printf("  expected total PVs = 0, actual total PVs = %d\n", res);
        return false;
    }

    for (int i = 0; i < 6; i++)
    {
        aStates[i].popularVotes = 0;
    }

    printf(" Checking totalPVs() for 6 States with 0 PVs:\n");
    res = totalPVs(aStates, 6);
    if (res != 0)
    {
        printf("  all states have 0 Popular Votes");
        printf("  expected total PVs = 0, actual total PVs = %d\n", res);
        return false;
    }

    return true;
}

bool test_setSettings()
{
    //-----------------------------------------------------------
    // TODO: Task 2 - write your own test case for setSettings();
    //                use test_totalEVs() as a sample;
    //                make sure to test all components
    //-----------------------------------------------------------
    int year;
    bool fastMode, quietMode;

    char *argv1[] = {"./app.exe", "-f", "-q", "-y", "2020"};
    if (!setSettings(5, argv1, &year, &fastMode, &quietMode) || year != 2020 || !fastMode || !quietMode)
    {
        printf("Test case 1 Failed\n");
        return false;
    }

    char *argv2[] = {"./app.exe", "-y", "2000"};
    if (!setSettings(3, argv2, &year, &fastMode, &quietMode) || year != 2000 || fastMode || quietMode)
    {
        printf("Test case 2 Failed\n");
        return false;
    }

    char *argv3[] = {"./app.exe", "-y", "1828"};
    if (!setSettings(3, argv3, &year, &fastMode, &quietMode) || year != 1828 || fastMode || quietMode)
    {
        printf("Test case 3 Failed\n");
        return false;
    }

    char *argv4[] = {"./app.exe", "-f", "-y", "1984"};
    if (!setSettings(4, argv4, &year, &fastMode, &quietMode) || year != 1984 || !fastMode || quietMode)
    {
        printf("Test case 4 Failed\n");
        return false;
    }

    char *argv5[] = {"./app.exe", "-y", "1800"};
    if (!setSettings(3, argv5, &year, &fastMode, &quietMode) || year != 0)
    {
        printf("Test case 5 failed\n");
        return false;
    }

    char *argv6[] = {"./app.exe", "-f", "-q"};
    if (!setSettings(3, argv6, &year, &fastMode, &quietMode) || year != 0 || !fastMode || !quietMode)
    {
        printf("Test case 6 failed\n");
        return false;
    }

    char *argv7[] = {"./app.exe", "-f", "-q", "-y", "2020", "-extra"};
    if (setSettings(6, argv7, &year, &fastMode, &quietMode))
    {
        printf("Test case 7 failed\n");
        return false;
    }

    char *argv8[] = {"./app.exe", "-y", "1999"};
    if (!setSettings(3, argv8, &year, &fastMode, &quietMode) || year != 0)
    {
        printf("Test case 8 failed\n");
        return false;
    }

    char *argv9[] = {"./app.exe", "-f", "-y"};
    if (setSettings(3, argv9, &year, &fastMode, &quietMode))
    {
        printf("Test case 9 failed\n");
        return false;
    }

    char *argv10[] = {"./app.exe", "-y", "-f", "-q"};
    if (!setSettings(4, argv10, &year, &fastMode, &quietMode) || year != 0 || fastMode || !quietMode)
    {
        printf("Test case 10 failed\n");
        return false;
    }

    char *argv11[] = {"./app.exe", "-f", "-v", "-y", "2020"};
    if (setSettings(5, argv11, &year, &fastMode, &quietMode))
    {
        printf("Test case 11 failed\n");
        return false;
    }

    char *argv12[] = {"./app.exe", "-q", "-q", "-y", "2016"};
    if (!setSettings(5, argv12, &year, &fastMode, &quietMode) || year != 2016 || !quietMode)
    {
        printf("Test case 12 failed\n");
        return false;
    }

    return true;
}

bool test_inFilename()
{
    //----------------------------------------------------------
    // TODO: Task 3 - write your own test case for inFilename();
    //                use test_totalEVs() as a sample;
    //                make sure to test all components
    //----------------------------------------------------------
    char filename[50];

    inFilename(filename, 2020);
    if (strcmp(filename, "data/2020.csv") != 0)
    {
        printf("Test case 1 failed\n");
        return false;
    }

    inFilename(filename, 1828);
    if (strcmp(filename, "data/1828.csv") != 0)
    {
        printf("Test case 2 failed\n");
        return false;
    }

    return true; // modify this
}

bool test_outFilename()
{
    //-----------------------------------------------------------
    // TODO: Task 3 - write your own test case for outFilename();
    //                use test_totalEVs() as a sample;
    //                make sure to test all components
    //-----------------------------------------------------------
    char filename[50];
    outFilename(filename, 2020);
    if (strcmp(filename, "toWin/2020_win.csv") != 0)
    {
        return false;
    }

    // Test with an older year
    outFilename(filename, 1828);
    if (strcmp(filename, "toWin/1828_win.csv") != 0)
    {
        return false;
    }
    return true;
}

bool test_parseLine()
{
    State myState;
    char line1[] = "California,CA,55,10000000";
    char line2[] = "Texas,TX,38,8000000\n";
    char line3[] = "Florida,FL,29,6000000";
    char line4[] = "Invalid,Data,Here";
    char line5[] = "New York,NY,29,7000000";
    char line6[] = "Ohio,OH,18,5000000";
    char line7[] = "Illinois,IL,20,6000000";
    char line8[] = "Pennsylvania,PA,20,6000000";
    char line9[] = "Georgia,GA,16,4000000";
    char line10[] = "Michigan,MI,16,4000000";
    char line11[] = "Invalid,Data,123,456,789";
    char line12[] = "Invalid,Data";
    char line13[] = ",,,";
    char line14[] = "California,CA,55,not_a_number";

    bool all_passed = true;

    // Test case 1
    bool result1 = parseLine(line1, &myState);
    if (!(result1 == true && strcmp(myState.name, "California") == 0 && strcmp(myState.postalCode, "CA") == 0 && myState.electoralVotes == 55 && myState.popularVotes == 10000000))
    {
        printf("Test case 1 failed\n");
        all_passed = false;
    }

    // Test case 2
    bool result2 = parseLine(line2, &myState);
    if (!(result2 == true && strcmp(myState.name, "Texas") == 0 && strcmp(myState.postalCode, "TX") == 0 && myState.electoralVotes == 38 && myState.popularVotes == 8000000))
    {
        printf("Test case 2 failed\n");
        all_passed = false;
    }

    // Test case 3
    bool result3 = parseLine(line3, &myState);
    if (!(result3 == true && strcmp(myState.name, "Florida") == 0 && strcmp(myState.postalCode, "FL") == 0 && myState.electoralVotes == 29 && myState.popularVotes == 6000000))
    {
        printf("Test case 3 failed\n");
        all_passed = false;
    }

    // Test case 4 (Invalid data)
    bool result4 = parseLine(line4, &myState);
    if (result4 != false)
    {
        printf("Test case 4 failed\n");
        all_passed = false;
    }

    // Test case 5
    bool result5 = parseLine(line5, &myState);
    if (!(result5 == true && strcmp(myState.name, "New York") == 0 && strcmp(myState.postalCode, "NY") == 0 && myState.electoralVotes == 29 && myState.popularVotes == 7000000))
    {
        printf("Test case 5 failed\n");
        all_passed = false;
    }

    // Test case 6
    bool result6 = parseLine(line6, &myState);
    if (!(result6 == true && strcmp(myState.name, "Ohio") == 0 && strcmp(myState.postalCode, "OH") == 0 && myState.electoralVotes == 18 && myState.popularVotes == 5000000))
    {
        printf("Test case 6 failed\n");
        all_passed = false;
    }

    // Test case 7
    bool result7 = parseLine(line7, &myState);
    if (!(result7 == true && strcmp(myState.name, "Illinois") == 0 && strcmp(myState.postalCode, "IL") == 0 && myState.electoralVotes == 20 && myState.popularVotes == 6000000))
    {
        printf("Test case 7 failed\n");
        all_passed = false;
    }

    // Test case 8
    bool result8 = parseLine(line8, &myState);
    if (!(result8 == true && strcmp(myState.name, "Pennsylvania") == 0 && strcmp(myState.postalCode, "PA") == 0 && myState.electoralVotes == 20 && myState.popularVotes == 6000000))
    {
        printf("Test case 8 failed\n");
        all_passed = false;
    }

    // Test case 9
    bool result9 = parseLine(line9, &myState);
    if (!(result9 == true && strcmp(myState.name, "Georgia") == 0 && strcmp(myState.postalCode, "GA") == 0 && myState.electoralVotes == 16 && myState.popularVotes == 4000000))
    {
        printf("Test case 9 failed\n");
        all_passed = false;
    }

    // Test case 10
    bool result10 = parseLine(line10, &myState);
    if (!(result10 == true && strcmp(myState.name, "Michigan") == 0 && strcmp(myState.postalCode, "MI") == 0 && myState.electoralVotes == 16 && myState.popularVotes == 4000000))
    {
        printf("Test case 10 failed\n");
        all_passed = false;
    }

    // Test case 11 (Extra fields)
    bool result11 = parseLine(line11, &myState);
    if (result11 != false)
    {
        printf("Test case 11 failed\n");
        all_passed = false;
    }

    // Test case 12 (Missing fields)
    bool result12 = parseLine(line12, &myState);
    if (result12 != false)
    {
        printf("Test case 12 failed\n");
        all_passed = false;
    }

    // Test case 13 (Empty fields)
    bool result13 = parseLine(line13, &myState);
    if (result13 != true)
    {
        printf("Test case 13 failed\n");
        all_passed = false;
    }

    return all_passed;
}

bool test_readElectionData()
{
    //----------------------------------------------------------------
    // TODO: Task 6 - write your own test case for readElectionData();
    //                make sure to test all components
    //----------------------------------------------------------------
    int nStates;
    State allStates[51];
    bool result;

    nStates = 0;
    result = readElectionData("data/2020.csv", allStates, &nStates);
    if (!result || nStates == 0)
    {
        printf("Test case 1 failed: Could not read 2020 data correctly.\n");
        return false;
    }

    nStates = 0;
    result = readElectionData("data/1828.csv", allStates, &nStates);
    if (!result || nStates == 0)
    {
        printf("Test case 1 failed: Could not read 2020 data correctly.\n");
        return false;
    }

    nStates = 0;
    result = readElectionData("data/9999.csv", allStates, &nStates);
    if (result || nStates != 0)
    {
        printf("Test case 3 failed: Non-existent file should return false.\n");
        return false;
    }

    nStates = 0;
    result = readElectionData("data/empty.csv", allStates, &nStates);
    if (result || nStates != 0)
    {
        printf("Test case 5 failed: Empty file should return false.\n");
        return false;
    }

    return true; // modify this
}

bool test_minPVsSlow()
{
    //-----------------------------------------------------------------
    // TODO: Task 7 - write your own test case for minPopVoteAtLeast();
    //                make sure to test all components
    //-----------------------------------------------------------------
    State states[4] = {
        {"Alabama", "AL", 9, 600000},
        {"Georgia", "GA", 12, 800000},
        {"Illinois", "IL", 29, 1500000},
        {"Texas", "TX", 18, 3000000}};

    MinInfo result = minPopVoteToWin(states, 4);

    int expectedTotalPV = ((1500000 + 600000) / 2) + 2; //+2 for two state majority
    bool expectedSufficientEVs = true;

    // Check results
    if (result.subsetPVs != expectedTotalPV)
    {
        printf("Test failed: Expected total popular votes = %d, got = %d\n", expectedTotalPV, result.subsetPVs);
        return false;
    }

    if (result.sufficientEVs != expectedSufficientEVs)
    {
        printf("Test failed: Expected sufficient electoral votes = %d, got = %d\n", expectedSufficientEVs, result.sufficientEVs);
        return false;
    }

    // Check the states included in the result
    if (strcmp(result.someStates[0].name, states[2].name) ||
        strcmp(result.someStates[1].name, states[0].name))
    {
        printf("Test failed: Expected states do not match.\n");
        return false;
    }

    return true;
}

bool test_minPVsFast()
{
    // wrote python code to generate this for testing basically a very long long list of computations
    State states[101] = {
        {"Nevada", "NE", 27, 3722467},
        {"Oregon", "OR", 16, 8968274},
        {"Colorado", "CO", 7, 3992839},
        {"Texas", "TE", 11, 4695880},
        {"Colorado", "CO", 42, 8257073},
        {"Virginia", "VI", 32, 102043},
        {"South Dakota", "SO", 10, 5105433},
        {"Oklahoma", "OK", 32, 886777},
        {"Wisconsin", "WI", 44, 325366},
        {"Vermont", "VE", 47, 8848004},
        {"Iowa", "IO", 17, 3693739},
        {"Nevada", "NE", 9, 4583283},
        {"Nebraska", "NE", 40, 2588449},
        {"New Jersey", "NE", 30, 6692140},
        {"Massachusetts", "MA", 33, 1324024},
        {"Tennessee", "TE", 45, 3876365},
        {"Mississippi", "MI", 27, 91439},
        {"New Jersey", "NE", 28, 4361570},
        {"New Hampshire", "NE", 38, 9328180},
        {"Vermont", "VE", 24, 5757328},
        {"West Virginia", "WE", 48, 4637969},
        {"Nevada", "NE", 20, 4433148},
        {"Vermont", "VE", 4, 986486},
        {"Vermont", "VE", 16, 5809796},
        {"Delaware", "DE", 24, 9123516},
        {"South Carolina", "SO", 38, 8315723},
        {"Florida", "FL", 42, 8438937},
        {"New York", "NE", 36, 1250456},
        {"New Jersey", "NE", 1, 8866222},
        {"New Jersey", "NE", 47, 688758},
        {"Wyoming", "WY", 13, 5357518},
        {"Idaho", "ID", 46, 3357277},
        {"Wisconsin", "WI", 6, 8245900},
        {"Missouri", "MI", 21, 9423030},
        {"New Hampshire", "NE", 54, 1540969},
        {"New Mexico", "NE", 22, 4121489},
        {"Alaska", "AL", 10, 6761276},
        {"North Carolina", "NO", 12, 1750986},
        {"Wisconsin", "WI", 18, 3407297},
        {"Missouri", "MI", 15, 9920460},
        {"New York", "NE", 21, 1066502},
        {"Maryland", "MA", 12, 7589829},
        {"Georgia", "GE", 38, 7863466},
        {"North Dakota", "NO", 44, 3857280},
        {"Illinois", "IL", 1, 9408344},
        {"Oregon", "OR", 42, 1534186},
        {"Colorado", "CO", 44, 4671026},
        {"Ohio", "OH", 21, 2990736},
        {"Delaware", "DE", 11, 9868945},
        {"Michigan", "MI", 25, 1508586},
        {"Montana", "MO", 13, 5979609},
        {"Tennessee", "TE", 17, 1861445},
        {"Ohio", "OH", 17, 5263877},
        {"District of Columbia", "DI", 52, 6428764},
        {"Michigan", "MI", 26, 3992562},
        {"Utah", "UT", 45, 1925932},
        {"Hawaii", "HA", 23, 6148799},
        {"Wisconsin", "WI", 22, 5267875},
        {"Michigan", "MI", 45, 4789202},
        {"District of Columbia", "DI", 38, 3859306},
        {"Utah", "UT", 3, 3636424},
        {"Wisconsin", "WI", 9, 245662},
        {"Tennessee", "TE", 48, 9443426},
        {"New Mexico", "NE", 14, 5293521},
        {"Delaware", "DE", 4, 2614156},
        {"West Virginia", "WE", 35, 528040},
        {"Illinois", "IL", 46, 4891579},
        {"Nevada", "NE", 16, 1309068},
        {"Pennsylvania", "PE", 23, 9544992},
        {"Florida", "FL", 55, 2643032},
        {"Arkansas", "AR", 53, 2859584},
        {"Nevada", "NE", 37, 5281289},
        {"Mississippi", "MI", 49, 744751},
        {"Utah", "UT", 39, 4616936},
        {"Colorado", "CO", 8, 7099053},
        {"Utah", "UT", 47, 5610343},
        {"California", "CA", 39, 7569375},
        {"Texas", "TE", 7, 280374},
        {"Montana", "MO", 11, 1049968},
        {"Alaska", "AL", 7, 9883779},
        {"Alabama", "AL", 18, 9919419},
        {"District of Columbia", "DI", 54, 5906030},
        {"Iowa", "IO", 15, 3341517},
        {"New Jersey", "NE", 42, 1308519},
        {"Oklahoma", "OK", 35, 7739543},
        {"South Dakota", "SO", 13, 9738494},
        {"Kentucky", "KE", 31, 8969805},
        {"Oregon", "OR", 31, 3757224},
        {"Maine", "MA", 47, 4204922},
        {"Hawaii", "HA", 55, 4274360},
        {"New York", "NE", 40, 8685305},
        {"Rhode Island", "RH", 27, 4048413},
        {"North Dakota", "NO", 13, 5951496},
        {"Idaho", "ID", 20, 9274841},
        {"Alabama", "AL", 46, 8269655},
        {"Arkansas", "AR", 13, 5892607},
        {"New Hampshire", "NE", 4, 4492967},
        {"Idaho", "ID", 37, 2206028},
        {"Iowa", "IO", 20, 3353374},
        {"Mississippi", "MI", 31, 5224411}};

    MinInfo result = minPopVoteToWinFast(states, 101);

    int expectedTotalPV = 42734954;
    bool expectedSufficientEVs = true;

    // Check results
    if (result.subsetPVs != expectedTotalPV)
    {
        printf("Test failed: Expected total popular votes = %d, got = %d\n", expectedTotalPV, result.subsetPVs);
        return false;
    }

    if (result.sufficientEVs != expectedSufficientEVs)
    {
        printf("Test failed: Expected sufficient electoral votes = %d, got = %d\n", expectedSufficientEVs, result.sufficientEVs);
        return false;
    }

    return true;
    return false; // modify this
}

int main()
{
    printf("Welcome to the Popular Vote Minimizer Testing Suite!\n\n");

    printf("Testing totalEVs()...\n");
    if (test_totalEVs())
    {
        printf("  All tests PASSED!\n");
    }
    else
    {
        printf("  test FAILED.\n");
    }

    printf("Testing totalPVs()...\n");
    if (test_totalPVs())
    {
        printf("  All tests PASSED!\n");
    }
    else
    {
        printf("  test FAILED.\n");
    }

    printf("Testing setSettings()...\n");
    if (test_setSettings())
    {
        printf("  All tests PASSED!\n");
    }
    else
    {
        printf("  test FAILED.\n");
    }

    printf("Testing inFilename()...\n");
    if (test_inFilename())
    {
        printf("  All tests PASSED!\n");
    }
    else
    {
        printf("  test FAILED.\n");
    }

    printf("Testing outFilename()...\n");
    if (test_outFilename())
    {
        printf("  All tests PASSED!\n");
    }
    else
    {
        printf("  test FAILED.\n");
    }

    printf("Testing parseLine()...\n");
    if (test_parseLine())
    {
        printf("  All tests PASSED!\n");
    }
    else
    {
        printf("  test FAILED.\n");
    }

    printf("Testing readElectionData()...\n");
    if (test_readElectionData())
    {
        printf("  All tests PASSED!\n");
    }
    else
    {
        printf("  test FAILED.\n");
    }

    printf("Testing minPopVoteToWin()...\n");
    if (test_minPVsSlow())
    {
        printf("  All tests PASSED!\n");
    }
    else
    {
        printf("  test FAILED.\n");
    }

    printf("Testing minPopVoteToWinFast()...\n");
    if (test_minPVsFast())
    {
        printf("  All tests PASSED!\n");
    }
    else
    {
        printf("  test FAILED.\n");
    }

    return 0;
}
