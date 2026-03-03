#pragma once

#include <array>
#include <iostream>
#include "DoublyLinkedList.hpp"

namespace CommonUtilities
{
    class SudokuSolver
    {
    public:
        // return true if a solution is found, false otherwise. The solved board will be stored in aBoard.
        static bool SolveSudoku(std::array<int, 81>& aBoard);

        static void PrintSudokuBoard(const std::array<int, 81>& aBoard, std::ostream& aOutput = std::cout);

    private:
        struct CellPosition
        {
            int row;
            int col;

            bool operator==(const CellPosition& aOther) const
            {
                return row == aOther.row && col == aOther.col;
            }
        };

        static int GetIndex(const int aRow, const int aCol);

        static bool IsValueInRange(const int aValue);

        static bool IsValidPlacement(const std::array<int, 81>& aBoard, const int aRow, const int aCol, const int aValue);

        static bool IsBoardStateValid(const std::array<int, 81>& aBoard);

        static void BuildEmptyCellList(const std::array<int, 81>& aBoard, DoublyLinkedList<CellPosition>& aOutEmptyCells);

        static bool SolveFromNode(std::array<int, 81>& aBoard, DoublyLinkedListNode<CellPosition>* aCurrentNode);
    };

    bool SudokuSolver::SolveSudoku(std::array<int, 81>& aBoard)
    {
        if (!IsBoardStateValid(aBoard))
        {
            return false;
        }

        DoublyLinkedList<CellPosition> emptyCells;
        BuildEmptyCellList(aBoard, emptyCells);
        if (emptyCells.GetSize() == 0)
        {
            return true;
        }

        return SolveFromNode(aBoard, emptyCells.GetFirst());
    }

    void SudokuSolver::PrintSudokuBoard(const std::array<int, 81>& aBoard, std::ostream& aOutput)
    {
        aOutput << "+-------+-------+-------+\n";

        for (int row = 0; row < 9; ++row)
        {
            if (row > 0 && row % 3 == 0)
            {
                aOutput << "+-------+-------+-------+\n";
            }

            for (int col = 0; col < 9; ++col)
            {
                if (col % 3 == 0)
                {
                    aOutput << "| ";
                }

                const int value = aBoard[GetIndex(row, col)];
                if (value == 0)
                {
                    aOutput << ". ";
                }
                else
                {
                    aOutput << value << ' ';
                }
            }

            aOutput << "|\n";
        }

        aOutput << "+-------+-------+-------+\n";
    }

    int SudokuSolver::GetIndex(const int aRow, const int aCol)
    {
        return aRow * 9 + aCol;
    }

    bool SudokuSolver::IsValueInRange(const int aValue)
    {
        return aValue >= 0 && aValue <= 9;
    }

    bool SudokuSolver::IsValidPlacement(const std::array<int, 81>& aBoard, const int aRow, const int aCol, const int aValue)
    {
        for (int col = 0; col < 9; ++col)
        {
            if (col != aCol && aBoard[GetIndex(aRow, col)] == aValue)
            {
                return false;
            }
        }

        for (int row = 0; row < 9; ++row)
        {
            if (row != aRow && aBoard[GetIndex(row, aCol)] == aValue)
            {
                return false;
            }
        }

        const int boxStartRow = (aRow / 3) * 3;
        const int boxStartCol = (aCol / 3) * 3;
        for (int row = boxStartRow; row < boxStartRow + 3; ++row)
        {
            for (int col = boxStartCol; col < boxStartCol + 3; ++col)
            {
                if ((row != aRow || col != aCol) && aBoard[GetIndex(row, col)] == aValue)
                {
                    return false;
                }
            }
        }

        return true;
    }

    bool SudokuSolver::IsBoardStateValid(const std::array<int, 81>& aBoard)
    {
        for (int row = 0; row < 9; ++row)
        {
            for (int col = 0; col < 9; ++col)
            {
                const int value = aBoard[GetIndex(row, col)];
                if (!IsValueInRange(value))
                {
                    return false;
                }

                if (value != 0 && !IsValidPlacement(aBoard, row, col, value))
                {
                    return false;
                }
            }
        }

        return true;
    }

    void SudokuSolver::BuildEmptyCellList(const std::array<int, 81>& aBoard, DoublyLinkedList<CellPosition>& aOutEmptyCells)
    {
        for (int row = 0; row < 9; ++row)
        {
            for (int col = 0; col < 9; ++col)
            {
                if (aBoard[GetIndex(row, col)] == 0)
                {
                    aOutEmptyCells.InsertLast({ row, col });
                }
            }
        }
    }

    bool SudokuSolver::SolveFromNode(std::array<int, 81>& aBoard, DoublyLinkedListNode<CellPosition>* aCurrentNode)
    {
        if (aCurrentNode == nullptr)
        {
            return true;
        }

        const CellPosition& position = aCurrentNode->GetValue();
        for (int candidate = 1; candidate <= 9; ++candidate)
        {
            if (IsValidPlacement(aBoard, position.row, position.col, candidate))
            {
                aBoard[GetIndex(position.row, position.col)] = candidate;

                if (SolveFromNode(aBoard, aCurrentNode->GetNext()))
                {
                    return true;
                }

                aBoard[GetIndex(position.row, position.col)] = 0;
            }
        }

        return false;
    }
}