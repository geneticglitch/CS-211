// gameboard.h
//
// GameBoard class starter code for course project
// University of Illinois at Chicago
// CS 211 - Programming Practicum
// Original Author: Professor Scott Reckinger
//

#ifndef _GAMEBOARD_H
#define _GAMEBOARD_H

#include <cstdlib>
#include <iostream>
#include <string>
#include <ctime>
#include <stdexcept>

#include "boardcell.h"
#include "grid.h"

using namespace std;

class GameBoard
{
private:
    Grid<BoardCell *> board;
    size_t numRows;
    size_t numCols;
    size_t HeroRow; // Hero's position row
    size_t HeroCol; // Hero's position column
    int numMonsters;
    int numSuperMonsters;
    int numAbysses;
    int numBats;
    bool wonGame; // false, unless the Hero reached the exit successfully

public:
    /* default constructor */
    GameBoard()
    {
        numMonsters = 4;
        numSuperMonsters = 2;
        numAbysses = 50;
        numBats = 2;
        wonGame = false;

        this->numRows = 15;
        this->numCols = 40;

        Grid<BoardCell *> boardnew(numRows, numCols);
        board = boardnew;

        blankBoard();
    }

    /* param constructor */
    GameBoard(size_t numRows, size_t numCols)
    {
        numMonsters = 4;
        numSuperMonsters = 2;
        numAbysses = 20;
        numBats = 3;
        wonGame = false;

        this->numRows = numRows;
        this->numCols = numCols;

        Grid<BoardCell *> boardnew(numRows, numCols);
        board = boardnew;

        blankBoard();
    }

    /* destructor */
    virtual ~GameBoard()
    {
        for (size_t row = 0; row < board.numrows(); row++)
        {
            for (size_t col = 0; col < board.numcols(row); col++)
            {
                delete board(row, col);
            }
        }
    }

    void blankBoard()
    {
        for (size_t row = 0; row < board.numrows(); row++)
        {
            for (size_t col = 0; col < board.numcols(row); col++)
            {
                board(row, col) = new Nothing(row, col);
            }
        }
    }

    char getCellDisplay(size_t r, size_t c)
    {
        return board(r, c)->display();
    }

    void setCell(BoardCell *myCell, size_t r, size_t c)
    {
        board(r, c) = myCell;
    }

    void freeCell(size_t r, size_t c)
    {
        delete board(r, c);
    }

    // fills board with by randomly placing...
    //  - Hero (H) in the first three columns
    //  - EscapeLadder (*) in last three columns
    //  - 3 vertical Walls (+), each 1/2 of board height, in middle segment
    //  - Abyss cells (#), quantity set by numAbysses, in middle segment
    //  - Baddies [Monsters (m), Super Monsters (M), & Bats (~)] in middle segment;
    //    number of Baddies set by numMonsters, numSuperMonsters, & numBats
    void setupBoard(int seed)
    {
        srand(seed);
        size_t r, c;

        r = rand() % numRows;
        c = rand() % 3;
        delete board(r, c);
        board(r, c) = new Hero(r, c);
        HeroRow = r;
        HeroCol = c;

        r = rand() % numRows;
        c = numCols - 1 - (rand() % 3);
        delete board(r, c);
        board(r, c) = new EscapeLadder(r, c);

        int sizeMid = numCols - 6;

        c = 3 + (rand() % sizeMid);
        for (r = 0; r < numRows / 2; ++r)
        {
            delete board(r, c);
            board(r, c) = new Wall(r, c);
        }
        size_t topc = c;

        while (c == topc || c == topc - 1 || c == topc + 1)
        {
            c = 3 + (rand() % sizeMid);
        }
        for (r = numRows - 1; r > numRows / 2; --r)
        {
            delete board(r, c);
            board(r, c) = new Wall(r, c);
        }
        size_t botc = c;

        while (c == topc || c == topc - 1 || c == topc + 1 || c == botc || c == botc - 1 || c == botc + 1)
        {
            c = 3 + (rand() % sizeMid);
        }
        for (r = numRows / 4; r < 3 * numRows / 4; ++r)
        {
            delete board(r, c);
            board(r, c) = new Wall(r, c);
        }

        for (int i = 0; i < numMonsters; ++i)
        {
            r = rand() % numRows;
            c = 3 + (rand() % sizeMid);
            while (board(r, c)->display() != ' ')
            {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
            }
            delete board(r, c);
            board(r, c) = new Monster(r, c);
            board(r, c)->setPower(1);
        }

        for (int i = 0; i < numSuperMonsters; ++i)
        {
            r = rand() % numRows;
            c = 3 + (rand() % sizeMid);
            while (board(r, c)->display() != ' ')
            {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
            }
            delete board(r, c);
            board(r, c) = new Monster(r, c);
            board(r, c)->setPower(2);
        }

        for (int i = 0; i < numBats; ++i)
        {
            r = rand() % numRows;
            c = 3 + (rand() % sizeMid);
            while (board(r, c)->display() != ' ')
            {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
            }
            delete board(r, c);
            board(r, c) = new Bat(r, c);
        }

        for (int i = 0; i < numAbysses; ++i)
        {
            r = rand() % numRows;
            c = 3 + (rand() % sizeMid);
            while (board(r, c)->display() != ' ')
            {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
            }
            delete board(r, c);
            board(r, c) = new Abyss(r, c);
        }
    }

    // neatly displaying the game board
    void display()
    {
        cout << '-';
        for (size_t col = 0; col < board.numcols(0); col++)
        {
            cout << '-';
        }
        cout << '-';
        cout << endl;
        for (size_t row = 0; row < board.numrows(); row++)
        {
            cout << '|';
            for (size_t col = 0; col < board.numcols(row); col++)
            {
                cout << board(row, col)->display();
            }
            cout << '|';
            cout << endl;
        }
        cout << '-';
        for (size_t col = 0; col < board.numcols(0); col++)
        {
            cout << '-';
        }
        cout << '-';
        cout << endl;
    }

    bool getWonGame()
    {
        return wonGame;
    }

    // distributing total number of monsters so that
    //  ~1/3 of num are Super Monsters (M), and
    //  ~2/3 of num are Regular Monsters (m)
    void setNumMonsters(int num)
    {
        numSuperMonsters = num / 3;
        numMonsters = num - numSuperMonsters;
    }

    void setNumAbysses(int num)
    {
        numAbysses = num;
    }

    void setNumBats(int num)
    {
        numBats = num;
    }

    size_t getNumRows()
    {
        return numRows;
    }

    size_t getNumCols()
    {
        return numCols;
    }

    //---------------------------------------------------------------------------------
    // void getHeroPosition(size_t& row, size_t& col)
    //
    // getter for Hero's position, which are private data members
    //      int HeroRow;
    //      int HeroCol;
    // note: row and col are passed-by-reference
    //---------------------------------------------------------------------------------
    void getHeroPosition(size_t &row, size_t &col)
    {

        row = HeroRow;
        col = HeroCol;
    }

    //---------------------------------------------------------------------------------
    // void setHeroPosition(size_t row, size_t col)
    //
    // setter for Hero's position, which are private data members
    //      int HeroRow;
    //      int HeroCol;
    //---------------------------------------------------------------------------------
    void setHeroPosition(size_t row, size_t col)
    {

        //---------------------------------
        // TODO: write this setter function
        //---------------------------------
        HeroRow = row;
        HeroCol = col;
    }

    //---------------------------------------------------------------------------------
    // findHero()
    //
    // updater for Hero's position, which are private data members
    //      int HeroRow;
    //      int HeroCol;
    // this function should find Hero in board and update
    //      HeroRow and HeroCol with the Hero's updated position;
    // if Hero cannot be found in board, then set Hero's position to (-1,-1)
    //---------------------------------------------------------------------------------
    void findHero()
    {

        //----------------------------------
        // TODO: write this updater function
        //----------------------------------
        bool found = false;

        // Iterate over all cells in the board
        for (size_t row = 0; row < numRows; ++row)
        {
            for (size_t col = 0; col < numCols; ++col)
            {
                // Check if the current cell contains the Hero
                if (board(row, col)->isHero())
                {
                    // Use the setter to update Hero's position
                    setHeroPosition(row, col);
                    found = true;
                    break;
                }
            }
            if (found)
                break;
        }

        // If Hero is not found, set position to (-1, -1) using setter
        if (!found)
        {
            setHeroPosition(-1, -1);
        }
    }

    //---------------------------------------------------------------------------------
    // bool makeMoves(char HeroNextMove)
    //
    // This is the primary gameplay operation for a single round of the game.
    // A LOT happens in this function...
    // General steps:
    //  - Allow user to input their next move
    //  - Get the attempted move position for the Hero
    //  - Move the hero, catching/resolving any potential collision exceptions...
    //      - attempted move out-of-bounds: change row &/or col to stay on board
    //      - attempted move into a barrier: change row &/or col to stay off barrier
    //      - attempted move to the exit: remove hero from board, hero escaped!
    //      - attempted move into a hole: remove hero from board, hero did not escape
    //      - attempted move to a baddie: remove hero from board, hero did not escape
    //      - attempted move to an empty space: actual move is the attempted move
    //  - For each baddie (regular Monster, super Monster, or Bat) on the board...
    //      - check that this baddies hasn't already moved this round
    //      - get its attempted move position
    //      - move the baddie, catching/resolving any potential collision exceptions...
    //          - attempted move out-of-bounds: change row &/or col to stay on board
    //          - attempted move into a barrier: change row &/or col to stay off barrier
    //          - attempted move to the exit: change row &/or col to stay off exit
    //          - attempted move into a hole: remove baddie from board
    //          - attempted move to hero: remove hero from board, hero did not escape
    //          - attempted move to a baddie: ignore attempted move, no position change
    //          - attempted move to an empty space: actual move is the attempted move
    //
    // Note: all baddies (and the hero) fall into holes if their attempted
    //       move ENDs on a hole (i.e. even Bats are drawn into the
    //       Abyss if their attempted move takes them to an Abyss cell);
    //       BUT, baddies can travel over holes, as long as their attempted
    //       move does NOT END on a hole; this only applies, in practice,
    //       to super monsters and bats, since their step sizes can be more
    //       than 1 (monsters and the hero are limited to steps of size 1)
    //
    // Note: also, whereas baddies (and the hero) can never move onto a
    //       barrier (i.e. a wall), they can step over barriers as long
    //       as the attempted move does NOT END on a barrier; this only
    //       applies, in practice, to super monsters and bats, since their
    //       step sizes can be more than 1 (monsters and the hero are limited
    //       to steps of size 1)
    //
    // Note: to prevent a single baddie from making multiple moves
    //       during a single round of the game, whenever a baddie
    //       has moved, it should be marked as "already moved" in
    //       some way; this can be done using the [moved] data member
    //       of the BoardCell base class, which has setter/getter
    //       functions provided. The [moved] data members must be
    //       reset for all BoardCells at the beginning of each round.
    //
    // Note: the [myRow] and [myCol] data members for BoardCell derived
    //       class objects must be updated whenever a move is made;
    //       additionally, [HeroRow] and [HeroCol] data members for the
    //       GameBoard must be updated whenever the Hero has moved,
    //       which can be done easily with a call to findHero()
    //
    // Note: many actual moves made by non-static board cell objects
    //       (i.e. Heros, Monsters, Bats) involve constructing and/or
    //       destructing objects; be careful with memory management;
    //       specifically, make sure to free (delete) the memory for
    //       BoardCell derived class objects when you are done with it
    //
    // return true if Hero is still on board at the end of the round
    // return false if Hero is NOT on board at the end of the round
    //---------------------------------------------------------------------------------
    bool makeMoves(char HeroNextMove)
    {
        // Reset all cells' moved status at start of round
        for (int row = 0; row < numRows; row++)
        {
            for (int col = 0; col < numCols; col++)
            {
                board(row, col)->setMoved(false);
            }
        }
        size_t newR, newC;
        board(HeroRow, HeroCol)->setNextMove(HeroNextMove);
        board(HeroRow, HeroCol)->attemptMoveTo(newR, newC, HeroRow, HeroCol);

        try
        {
            // Check for out-of-bounds row
            if (newR < 0 || newR >= numRows)
            {
                throw runtime_error("Hero trying to move out-of-bounds in rows");
            }
        }
        catch (runtime_error &excpt)
        {
            cout << excpt.what() << endl;
            newR = HeroRow; // Stay within bounds
            cout << "Adjusting row for Hero to stay in-bounds" << endl;
        }

        try
        {
            // Check for out-of-bounds column
            if (newC < 0 || newC >= numCols)
            {
                throw runtime_error("Hero trying to move out-of-bounds in columns");
            }
        }
        catch (runtime_error &excpt)
        {
            cout << excpt.what() << endl;
            newC = HeroCol; // Stay within bounds
            cout << "Adjusting column for Hero to stay in-bounds" << endl;
        }

        // Check for barrier collision and adjust
        if (board(newR, newC)->isBarrier())
        {
            if (!board(newR, HeroCol)->isBarrier())
            {
                newC = HeroCol; // Step back in columns
            }
            else if (!board(HeroRow, newC)->isBarrier())
            {
                newR = HeroRow; // Step back in rows
            }
            else
            {
                newR = HeroRow;
                newC = HeroCol; // Stay in place
            }
            cout << "Hero attempted to move into a barrier; adjusted position" << endl;
        }

        // Check for exit
        try
        {
            // Check if the hero found the exit
            if (board(newR, newC)->isExit())
            {
                // Throw an exception when the hero escapes
                throw runtime_error("Hero found the exit and escaped the board!");
            }
        }
        catch (const runtime_error &excpt)
        {
            cout << excpt.what() << endl;
            freeCell(HeroRow, HeroCol);
            board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
            wonGame = true;
            return false;
        }

        // Check for abyss (hole)
        if (board(newR, newC)->isHole() || board(newR, newC)->isBaddie())
        {
            cout << "Hero fell into an abyss/baddie and didn't escape." << endl;
            freeCell(HeroRow, HeroCol);
            board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
            return false; // Hero is removed
        }

        // Check for baddie
        if (board(newR, newC)->isBaddie())
        {
            cout << "Hero encountered a baddie and didn't escape." << endl;
            return false; // Hero is removed
        }

        // If the target cell is an empty space, update Hero's position
        if (board(newR, newC)->isSpace())
        {
            ;
            BoardCell *heroPtr = board(HeroRow, HeroCol);
            // Update hero's position data
            heroPtr->setRow(newR);
            heroPtr->setCol(newC);
            // Create new Nothing cell at old position
            board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
            // Place hero at new position
            freeCell(newR, newC);
            board(newR, newC) = heroPtr;
        }
        findHero();
        bool heroKilled = false;

        for (int row = 0; row < numRows; row++)
        {
            for (int col = 0; col < numCols; col++)
            {
                if (!board(row, col)->isBaddie() || board(row, col)->getMoved())
                {
                    continue;
                }

                size_t badNewR = row, badNewC = col;
                BoardCell *baddiePtr = board(row, col);
                baddiePtr->attemptMoveTo(badNewR, badNewC, HeroRow, HeroCol);
                baddiePtr->setMoved(true);

                // Check for out-of-bounds row
                // Checking for out-of-bounds for Baddie
                try
                {
                    if (badNewR < 0 || badNewR >= numRows)
                    {
                        throw runtime_error("Baddie trying to move out-of-bounds in rows");
                    }
                }
                catch (const runtime_error &excpt)
                {
                    cout << excpt.what() << endl;
                    badNewR = row; // Stay within bounds
                    cout << "Adjusting row for Baddie to stay in-bounds" << endl;
                }

                try
                {
                    if (badNewC < 0 || badNewC >= numCols)
                    {
                        throw runtime_error("Baddie trying to move out-of-bounds in columns");
                    }
                }
                catch (const runtime_error &excpt)
                {
                    cout << excpt.what() << endl;
                    badNewC = col; // Stay within bounds
                    cout << "Adjusting column for Baddie to stay in-bounds" << endl;
                }

                // After adjusting position, actually move the baddie
                if (!board(badNewR, badNewC)->isBaddie() &&
                    !board(badNewR, badNewC)->isBarrier() &&
                    !board(badNewR, badNewC)->isExit())
                {

                    // If moving to a hole
                    if (board(badNewR, badNewC)->isHole())
                    {
                        delete board(row, col);
                        board(row, col) = new Nothing(row, col);
                        continue;
                    }

                    // If moving to hero position
                    if (board(badNewR, badNewC)->isHero())
                    {
                        // Delete hero and put Nothing in its place
                        freeCell(HeroRow, HeroCol);

                        // Update monster's position and move it
                        baddiePtr->setRow(badNewR);
                        baddiePtr->setCol(badNewC);
                        board(badNewR, badNewC) = baddiePtr;

                        // Create Nothing in monster's original position
                        board(row, col) = new Nothing(row, col);

                        heroKilled = true;
                    }

                    // Moving to empty space
                    if (board(badNewR, badNewC)->isSpace())
                    {
                        freeCell(badNewR, badNewC);
                        baddiePtr->setRow(badNewR);
                        baddiePtr->setCol(badNewC);
                        board(badNewR, badNewC) = baddiePtr;
                        board(row, col) = new Nothing(row, col);
                    }
                }
            }
        }
        if (heroKilled)
        {
            return false;
        }
        findHero();
        return true; // Hero is still on the board
    }
};

#endif //_GAMEBOARD_H