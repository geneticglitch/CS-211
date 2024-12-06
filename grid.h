/*-------------------------------------------
Program: Outlast the Baddies
Description: Avoid the Abyss
Course: CS 211, Fall 2024, UIC
Author: Aryan Singh
------------------------------------------- */

#pragma once

#include <iostream>
#include <exception>
#include <stdexcept>
#include <algorithm>

using namespace std;

template <typename T>
class Grid
{
private:
    struct ROW
    {
        T *Cols;        // array of column elements
        size_t NumCols; // total # of columns (0..NumCols-1)
    };

    ROW *Rows;      // array of ROWs
    size_t NumRows; // total # of rows (0..NumRows-1)

public:
    //
    // default constructor:
    //
    // Called automatically by to construct a 4x4 Grid.
    // All elements initialized to default value of T.
    //
    Grid()
    {
        Rows = new ROW[4]; // 4 rows
        NumRows = 4;

        // initialize each row to have 4 columns:
        for (size_t r = 0; r < NumRows; ++r)
        {
            Rows[r].Cols = new T[4];
            Rows[r].NumCols = 4;

            // initialize the elements to their default value:
            for (size_t c = 0; c < Rows[r].NumCols; ++c)
            {
                Rows[r].Cols[c] = T(); // default value for type T:
            }
        }
    }

    //
    // parameterized constructor:
    //
    // Called automatically to construct a Grid
    // with R rows, where each row has C columns.
    // All elements initialized to default value of T.
    //
    Grid(size_t R, size_t C)
    {
        //-------------------------------------------------
        // TODO:  Write this parameterized constructor.
        //-------------------------------------------------
        Rows = new ROW[R]; // 4 rows
        NumRows = R;

        // initialize each row to have 4 columns:
        for (size_t r = 0; r < NumRows; ++r)
        {
            Rows[r].Cols = new T[C];
            Rows[r].NumCols = C;

            // initialize the elements to their default value:
            for (size_t c = 0; c < Rows[r].NumCols; ++c)
            {
                Rows[r].Cols[c] = T(); // default value for type T:
            }
        }
    }

    //
    // destructor:
    //
    // Called automatically to free memory for this Grid.
    //
    virtual ~Grid()
    {

        //-------------------------------------------------
        // TODO:  Write this destructor.
        //-------------------------------------------------
        for (size_t i = 0; i < NumRows; i++) // loop and delete each column
        {
            delete[] Rows[i].Cols;
        }
        delete[] Rows; // delete that row
    }

    //
    // copy constructor:
    //
    // Called automatically to construct a Grid that contains a
    // copy of an existing Grid.  Example: this occurs when passing
    // Grid as a parameter by value
    //
    //   void somefunction(Grid<int> G2)  <--- G2 is a copy:
    //   { ... }
    //
    Grid(const Grid<T> &other)
    {

        //-------------------------------------------------
        // TODO:  Write this copy constructor.
        //-------------------------------------------------
        NumRows = other.NumRows;
        Rows = new ROW[NumRows];

        for (size_t i = 0; i < NumRows; i++) // loop through all the rows
        {
            Rows[i].Cols = new T[other.Rows[i].NumCols]; // create new cols for the cols to get copied
            Rows[i].NumCols = other.Rows[i].NumCols;     // copy the number of cols in that row

            for (int j = 0; j < Rows[i].NumCols; j++) // loop through row
            {
                Rows[i].Cols[j] = other.Rows[i].Cols[j]; // copy the data at the col
            }
        }
    }

    //
    // copy operator=
    //
    // Called when one Grid is assigned into another, i.e. this = other;
    //
    Grid &operator=(const Grid &other)
    {
        if (this == &other) // If the object is assigned to itself
        {
            return *this;
        }

        Grid<T> temp(other); // using the copy constructor create a temp

        this->~Grid(); // free the curret object memory by call destructor  on the object

        // copy data
        NumRows = temp.NumRows;
        Rows = temp.Rows;

        // as the data is copied nothing is there for temp to assigned to
        temp.Rows = nullptr;
        temp.NumRows = 0;

        return *this;
    }

    //
    // numrows
    //
    // Returns the # of rows in the Grid.
    // The indices for these rows are 0..numrows-1.
    //
    size_t numrows() const
    {

        //-------------------------------------------------
        // TODO:  Write this function.
        //-------------------------------------------------

        return NumRows; // TODO:  update this, it is only here so code compiles.
    }

    //
    // numcols
    //
    // Returns the # of columns in row r.
    // The indices for these columns are 0..numcols-1.
    // For now, each row has the same number of columns.
    //
    size_t numcols(size_t r) const
    {
        if (r >= NumRows || r < 0)
        {
            throw out_of_range("Row index out of range.");
        }
        //-------------------------------------------------
        // TODO:  Write this function.
        //-------------------------------------------------
        return Rows[r].NumCols; // TODO: update this, it is only here so code compiles.
    }

    //
    // size
    //
    // Returns the total # of elements in the Grid.
    //
    size_t size() const
    {

        //-------------------------------------------------
        // TODO:  Write this function.
        //-------------------------------------------------

        return NumRows * Rows[0].NumCols; // TODO: update this, it is only here so code compiles.
    }

    //
    // ()
    //
    // Returns a reference to the element at location (r, c);
    // this allows you to access or assign the element:
    //
    //    grid(r, c) = ...
    //    cout << grid(r, c) << endl;
    //
    T &operator()(size_t r, size_t c)
    {
        // TODO:  Write the parentheses overloaded operator
        //-------------------------------------------------
        if (r >= NumRows || c >= Rows[r].NumCols || r < 0 || c < 0)
        {
            throw out_of_range("Row or/and Column index out of range.");
        }
        return Rows[r].Cols[c];
    }

    //
    // _output
    //
    // Outputs the contents of the grid; for debugging purposes.
    // This is not tested.
    //
    void _output()
    {
        for (size_t r = 0; r < NumRows; ++r)
        {
            for (size_t c = 0; c < Rows[r].NumCols; ++c)
            {
                cout << Rows[r].Cols[c] << " ";
            }
            cout << endl;
        }
    }
};
