#pragma once

#include "Tetrimino.hpp"

#include <array>
#include <cstdint>
#include <iostream>

//================================================================================================================

namespace tetris
{
    enum class Direction { RIGHT, LEFT, DOWN };

    //============================================================================================================

    template<std::size_t NumRows, std::size_t NumCols>
    class Board
    {
    public:
        static constexpr std::size_t    NUM_ROWS = NumRows;
        static constexpr std::size_t    NUM_COLS = NumCols;

        static constexpr wchar_t        BLANK_TILE = ' ';
        static constexpr wchar_t        OCCUPIED_TILE = 'X';

        struct Point
        {
            std::size_t row{ 0 };
            std::size_t col{ 0 };
        };

        Board() noexcept
        {
            std::fill(board_.begin(), board_.end(), BLANK_TILE);
        }

        bool moveTetrimino(const tetrimino::TetrimonoBase& shape,
            Direction direction,
            const Point& topLeft) noexcept
        {
            bool isFinalized = false;

            switch (direction)
            {
                case Direction::LEFT:
                {
                    isFinalized = moveDown(shape, topLeft);
                    break;
                }
                case Direction::RIGHT:
                {
                    moveRight(shape, topLeft);
                    break;
                }
                case Direction::DOWN:
                {
                    moveLeft(shape, topLeft);
                    break;
                }
            }

            return isFinalized;
        }

    private:

        wchar_t& operator()(std::size_t row, std::size_t col) noexcept
        {
            return board_.at(NumCols * row + col);
        }

        bool isOccupied(std::size_t row, std::size_t col) const noexcept
        {
            return operator()(row, col) == OCCUPIED_TILE;
        }

        void markOccupied(std::size_t row, std::size_t col) noexcept
        {
            operator()(row, col) == OCCUPIED_TILE;
        }

        bool moveDown(const tetrimino::TetrimonoBase& shape, const Point& topLeft) noexcept
        {
            if (topLeft.row + shape.numRows() == NUM_ROWS)
            {
                return true;
            }

            const auto shapeBottomRow = topLeft.row + shape.numRows() - 1;
            const auto belowShapeBottomRow = shapeBottomRow + 1;

            for (std::size_t col = 0; col < shape.numCols(); ++col)
            {
                if (shape.isOccupied(shapeBottomRow, col) &&
                    isOccupied(belowShapeBottomRow, col + topLeft.col))
                {
                    return true;
                }
            }

            placeTetrimino(shape, topLeft);

            return false;
        }

        void moveLeft(const tetrimino::TetrimonoBase& shape, const Point& topLeft) noexcept
        {
            if (topLeft.col == 0)
            {
                return;
            }

            const auto shapeLeftCol = topLeft.col;
            const auto leftOfShapeLeftCol = shapeLeftCol - 1;

            for (std::size_t row = 0; row < shape.numRows(); ++row)
            {
                if (shape.isOccupied(row, shapeLeftCol) &&
                    isOccupied(row + topLeft.row, leftOfShapeLeftCol))
                {
                    return;
                }
            }

            placeTetrimino(shape, topLeft);
        }

        void moveRight(const tetrimino::TetrimonoBase& shape, const Point& topLeft) noexcept
        {
            if (topLeft.col + shape.numCols = NUM_COLS)
            {
                return;
            }

            const auto shapeRightCol = topLeft.col + shape.numCols();
            const auto rightOfShapeRightCol = shapeRightCol + 1;

            for (std::size_t row = 0; row < shape.numRows(); ++row)
            {
                if (shape.isOccupied(row, shapeRightCol) &&
                    isOccupied(row + topLeft.row, rightOfShapeRightCol))
                {
                    return;
                }
            }

            placeTetrimino(shape, topLeft);
        }

        void placeTetrimino(const tetrimino::TetrimonoBase& shape, const Point& topLeft) noexcept
        {
            for (std::size_t row = 0; row < shape.numRows(); ++row)
            {
                for (std::size_t col = 0; col < shape.numCols(); ++col)
                {
                    if (shape.isOccupied(row, col))
                    {
                        markOccupied(row + topLeft.row, col + topLeft.col);
                    }
                }
            }
        }

        std::array<wchar_t, NumRows * NumCols>  board_;
    };

    //============================================================================================================

    template<std::size_t NumRows, std::size_t NumCols>
    std::ostream& operator<<(std::ostream& ostream, const Board<NumRows, NumCols>& board) noexcept
    {
        for (std::size_t row = 0; row < NumRows; ++row)
        {
            for (std::size_t col = 0; col < NumCols; ++col)
            {
                std::cout << board(row, col);
            }

            std::cout << '\n';
        }

        return ostream;
    }
}
