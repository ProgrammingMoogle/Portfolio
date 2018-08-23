/******************************************************************************/
/*!
\file   Sudoku.cpp
\author Kacey Lei Yeo
\par    email: yeo.k\@digipen.edu
\par    DigiPen login: yeo.k
\par    Course: CS280
\par    Assignment #3
\date   6/26/2017
\brief
  This is the implementation file for all member functions
  of a class called Sudoku, as specified in specification
  file Sudoku.h.

  Operations include:

  - SetupBoard  - this sets up the board for sudoku
  - Solve       - Solves the entire board
  - GetBoard    - Gets the current board
  - GetStats    - Gets the stats of the current board
  - Clash       - Deals with any conflicting values
  - FindCell    - finds the next empty cell to place a number in
  - PlaceNum    - places the number on the board
  - CheckRow    - checks the row for any conflicts
  - CheckColumn - checks the column for any conflicts
  - CheckBlock  - checks the black it resides for any conflicts

*/
/******************************************************************************/
#include "Sudoku.h"

/******************************************************************************/
/*!

  \brief
    A non defualt constructor of Sudoku.

  \param basesize
    the base size of the board

  \param stype
    defines what type of sudoku we are going to be using

  \param callback
    a callback to the function

*/
/******************************************************************************/
Sudoku::Sudoku(int basesize, SymbolType stype, SUDOKU_CALLBACK callback) :
  mStats(), mLength(basesize * basesize), mBoard(new char[mLength * mLength]),
  mCall(callback), mDone(false)
{
  //defines what kind of board it will be
  //Is it 1 to 9
  if (stype == SYM_NUMBER)
  {
    mStart = '1';
    mEnd = static_cast<char>('1' + (mLength - 1));
  }
  //Or is it A to I
  else
  {
    mStart = 'A';
    mEnd = static_cast<char>('A' + (mLength - 1));
  }

  //tells the stats the base size of the board
  mStats.basesize = basesize;
}

/******************************************************************************/
/*!

  \brief
    Deconstructor.

*/
/******************************************************************************/
Sudoku::~Sudoku()
{
  delete[] mBoard;
}

/******************************************************************************/
/*!

  \brief
    Sets up the board.
  
  \param values
    the values to be used such as 1 to 9 or A to I
  
  \param size
    the size of the board
  
*/
/******************************************************************************/
void Sudoku::SetupBoard(const char * values, int size)
{
  for (int i = 0; i < size; i++)
  {
    if (values[i] == '.')
    {
      mBoard[i] = EMPTY_CHAR;
    }
    else
    {
      mBoard[i] = values[i];
    }
  } 
}

/******************************************************************************/
/*!

  \brief
    Solves the board.

*/
/******************************************************************************/
void Sudoku::Solve()
{
  //Calls back to the function telling it that it is about to start
  mCall(*this, mBoard, MSG_STARTING, mStats.moves, mStats.basesize, 0,
        mBoard[0], 0);

  //searches for the next empty and then it stores the positon of the cell
  mFECell = FindCell(0);
  //Places a number in the spot after finding a valid value to be placed in it
  PlaceNum(mFECell);
}

/******************************************************************************/
/*!

  \brief
    Solves the board.

  \return
    returns the board

*/
/******************************************************************************/
const char * Sudoku::GetBoard() const
{
  return mBoard;
}

/******************************************************************************/
/*!

  \brief
    Gets the stats.

  \return 
    returns the stats.

*/
/******************************************************************************/
Sudoku::SudokuStats Sudoku::GetStats() const
{
  return mStats;
}

/******************************************************************************/
/*!

  \brief
    Checks if the number in the index clashes with any number in the board.

  \param index
    the index that is being checked on.

*/
/******************************************************************************/
void Sudoku::Clash(int index)
{
  vector<int> dups(3);
  dups[0] = CheckRow(index);
  dups[1] = CheckColumn(index);
  dups[2] = CheckBlock(index);

  sort(dups.begin(), dups.end());

  auto check = unique(dups.begin(), dups.end());

  for (; check != dups.end(); check++)
  {
    *check = -1;
  }
  for (size_t i = 0; i < 3; i++)
  {
    mDuplicates[i] = dups[i];
  }
}

/******************************************************************************/
/*!

  \brief
    Finds the next empty cell on the board.

  \param index
    the index that is being checked on

  \return
    returns the index of the next cell or -1 if their are no more cells.

*/
/******************************************************************************/
int Sudoku::FindCell(int index)
{
  for (int i = index; i < mLength * mLength; ++i)
  {
    if (mBoard[i]==' ')
    {
      return i;
    }
  }

  return -1;
}

/******************************************************************************/
/*!

  \brief
    Places a number on the board after checking if their is any conflicts

  \param index
    the index that the number is being placed in

*/
/******************************************************************************/
void Sudoku::PlaceNum(int index)
{
  for (char i = mStart; i <= mEnd; ++i)
  {
    ++mStats.moves;

    mBoard[index] = i;

    Clash(index);

    ++mStats.placed;

    mCall(*this, mBoard, MSG_ABORT_CHECK, mStats.moves, mStats.basesize, index, mBoard[index], mDuplicates);

    mCall(*this, mBoard, MSG_PLACING, mStats.moves, mStats.basesize, index, mBoard[index], mDuplicates);

    if (mDuplicates[0] == -1 && mDuplicates[1] == -1 && mDuplicates[2] == -1)
    {
      int nexcell = FindCell(index);
      if (nexcell == -1)
      {
        mCall(*this, mBoard, MSG_FINISHED_OK, mStats.moves, mStats.basesize, index, mBoard[index], mDuplicates);

        mDone = true;

        return;
      }
      else
      {
        PlaceNum(nexcell);
      }
      if (mDone)
      {
        return;
      }
    }

    mBoard[index] = ' ';
    --mStats.placed;

    mCall(*this, mBoard, MSG_ABORT_CHECK, mStats.moves, mStats.basesize, index, i, mDuplicates);

    mCall(*this, mBoard, MSG_REMOVING, mStats.moves, mStats.basesize, index, i, mDuplicates);
  }

  mBoard[index] = ' ';
  ++mStats.backtracks;

  return;
}

/******************************************************************************/
/*!

  \brief
    Checks the row for any conflicts.

  \param index
    the index that is being checked on for any conflicts

  \return
    returns the index if there is a dupelicate or 
    -1 if there is no dupelicate.

*/
/******************************************************************************/
int Sudoku::CheckRow(int index)
{
  int row = index / mLength;

  for (int i = row * mLength; i < mLength * (row + 1); ++i)
  {
    if (i != index && mBoard[i] == mBoard[index]) 
    {
      return i;
    }
  }

  //duplicate? Nope.
  return -1;
}

/******************************************************************************/
/*!

  \brief
    Checks the column for any conflicts.

  \param index
    the index that is being checked on for any conflicts

  \return
    returns the index if there is a dupelicate or
    -1 if there is no dupelicate.

*/
/******************************************************************************/
int Sudoku::CheckColumn(int index)
{
  int column = index % mLength;

  int count = 0;

  for (int i = column; count < mLength; ++count)
  {
    if (i != index && mBoard[i] == mBoard[index])
    {
      return mLength * count + column;
    }

    i += mLength;
  }
  
  //duplicate? Nope.
  return -1;
}

/******************************************************************************/
/*!

  \brief
    Checks the entire block for any conflicts.

  \param index
    the index that is being checked on for any conflicts

  \return
    returns the index if there is a dupelicate or
    -1 if there is no dupelicate.

*/
/******************************************************************************/
int Sudoku::CheckBlock(int index)
{
  int row = index / mLength;
  int column = index % mLength;

  int TpLtRw = mStats.basesize * (row / mStats.basesize);
  int TpLtCm = mStats.basesize * (column / mStats.basesize);

  int MxR = mStats.basesize * (TpLtRw / mStats.basesize + 1);
  int MxC = mStats.basesize * ((TpLtCm / mStats.basesize) + 1);

  for (int i = TpLtRw; i < MxR; ++i)
  {
    for (int j = TpLtCm; j < MxC; j++)
    {
      int indCheck = i * mLength + j;

      if (indCheck != index && mBoard[indCheck] == mBoard[index]) 
      {
        return i * mLength + j;
      }
    }
  }

  return -1;
}



