#include "pch.h"
#include "CppUnitTest.h"

#include <array>

#include "../CommonUtilLib/SudokuSolver.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MathTests
{
    TEST_CLASS(SudokuSolverTests)
    {
    public:
        TEST_METHOD(SolveSudoku_ValidBoard_ReturnsSolvedBoard)
        {
            CommonUtilities::SudokuSolver solver;

            std::array<int, 81> board = {
                5, 3, 0, 0, 7, 0, 0, 0, 0,
                6, 0, 0, 1, 9, 5, 0, 0, 0,
                0, 9, 8, 0, 0, 0, 0, 6, 0,
                8, 0, 0, 0, 6, 0, 0, 0, 3,
                4, 0, 0, 8, 0, 3, 0, 0, 1,
                7, 0, 0, 0, 2, 0, 0, 0, 6,
                0, 6, 0, 0, 0, 0, 2, 8, 0,
                0, 0, 0, 4, 1, 9, 0, 0, 5,
                0, 0, 0, 0, 8, 0, 0, 7, 9
            };

            const std::array<int, 81> expected = {
                5, 3, 4, 6, 7, 8, 9, 1, 2,
                6, 7, 2, 1, 9, 5, 3, 4, 8,
                1, 9, 8, 3, 4, 2, 5, 6, 7,
                8, 5, 9, 7, 6, 1, 4, 2, 3,
                4, 2, 6, 8, 5, 3, 7, 9, 1,
                7, 1, 3, 9, 2, 4, 8, 5, 6,
                9, 6, 1, 5, 3, 7, 2, 8, 4,
                2, 8, 7, 4, 1, 9, 6, 3, 5,
                3, 4, 5, 2, 8, 6, 1, 7, 9
            };

            const bool solved = solver.SolveSudoku(board);
            Assert::IsTrue(solved, L"Expected Sudoku board to be solvable.");

            for (size_t index = 0; index < board.size(); ++index)
            {
                Assert::AreEqual(expected[index], board[index], L"Solved board mismatch.");
            }
        }

        TEST_METHOD(SolveSudoku_UnsolveableBoard_ReturnsFalse)
        {
            CommonUtilities::SudokuSolver solver;

            std::array<int, 81> board = {
                5, 3, 0, 0, 7, 0, 0, 0, 0,
                6, 0, 0, 1, 9, 5, 0, 0, 0,
                0, 9, 8, 0, 0, 0, 0, 6, 0,
                8, 0, 0, 0, 6, 0, 0, 0, 3,
                4, 0, 0, 8, 0, 3, 0, 0, 1,
                7, 0, 0, 0, 2, 0, 0, 0, 6,
                5, 6, 0, 0, 0, 0, 2, 8, 0,
                0, 0, 0, 4, 1, 9, 0, 0, 5,
                0, 0, 0, 0, 8, 0, 0, 7, 9
            };

            const std::array<int, 81> original = board;

            const bool solved = solver.SolveSudoku(board);
            Assert::IsFalse(solved, L"Expected contradictory Sudoku board to be unsolveable.");

            for (size_t index = 0; index < board.size(); ++index)
            {
                Assert::AreEqual(original[index], board[index], L"Board should remain unchanged for unsolveable input.");
            }
        }
    };
}
