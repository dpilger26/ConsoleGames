#pragma once

#include "Controls.h"
#include "Tetrimino.hpp"

#include <array>
#include <cstdint>

//================================================================================================================

namespace tetris
{
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

        bool canMove(const tetrimino::Tetrimino& shape, controls::Direction direction, const Point& topLeft)
        {
            bool ableToMove = false;
            switch (direction)
            {
                case controls::Direction::LEFT
                {
                    ableToMove = canMoveLeft(shape, topLeft);
                }
                case controls::Direction::RIGHT
                {
                    ableToMove = canMoveRight(shape, topLeft);
                }
                case controls::Direction::DOWN
                {
                    ableToMove = canMoveDown(shape, topLeft);
                }
            }

            return ableToMove;
        }

    private:
        wchar_t& operator()(std::size_t row, std::size_t col)
        {
            return tiles_.at(NumCols * row + col);
        }

        bool isOccupied(std::size_t row, std::size_t col)
        {
            return this->operator()(row, col) == OCCUPIED_TILE;
        }

        bool canGoDown(const tetrimino::Tetrimino& shape, const Point& topLeft) const
        {
            if (topLeft.row + shape.numRows() == NUM_ROWS)
            {
                return false;
            }

            const auto lowerRow = topLeft.row + shape.numRows() - 1;
            const auto underLowerRow = lowerRow - 1;

            for (std::size_t col = 0; col < shape.numCols(); ++col)
            {
                if (shape.isOccupied(lowerRow, col) &&
                    isOccupied(underLowerRow, col + topLeft.col))
                {
                    return false;
                }
            }

            return true;
        }

        bool canGoLeft(const tetrimino::Tetrimino& shape, const Point& topLeft)
        {


            return true;
        }

        bool canGoRight(const tetrimino::Tetrimino& shape, const Point& topLeft)
        {



            return true;
        }

        std::array<wchar_t, NumRows * NumCols>  board_;
    };
}
