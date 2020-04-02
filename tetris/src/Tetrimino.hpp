#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <random>
#include <stdexcept>

//====================================================================================================================

namespace tetris::tetrimino
{
    class TetrimonoBase
    {
    public:
        static constexpr wchar_t        BLANK_TILE = ' ';
        static constexpr wchar_t        OCCUPIED_TILE = 'X';

        enum class Color { 
            BLUE = 0,
            RED,
            GREEN,
            MAGENTA, 
            CYAN, 
            YELLOW,
            NUM_COLORS };

        constexpr TetrimonoBase(Color color) noexcept :
            color_(color)
        {}

        virtual ~TetrimonoBase() noexcept = default;

        void rotate90() noexcept
        {
            switch (orientation_)
            {
                case OrientationDegrees::ZERO:
                {
                    orientation_ = OrientationDegrees::NINETY;
                    break;
                }
                case OrientationDegrees::NINETY:
                {
                    orientation_ = OrientationDegrees::ONE_EIGHTY;
                    break;
                }
                case OrientationDegrees::ONE_EIGHTY:
                {
                    orientation_ = OrientationDegrees::TWO_SEVENTY;
                    break;
                }
                case OrientationDegrees::TWO_SEVENTY:
                {
                    orientation_ = OrientationDegrees::ZERO;
                    break;
                }
            }
        }

        virtual constexpr std::size_t numRows() const noexcept = 0;

        virtual constexpr std::size_t numCols() const noexcept = 0;

        virtual bool isOccupied(std::size_t row, std::size_t col) const = 0;

        virtual wchar_t operator()(std::size_t row, std::size_t col) const = 0;

    protected:
        enum class OrientationDegrees { ZERO = 0, NINETY, ONE_EIGHTY, TWO_SEVENTY };

        OrientationDegrees  orientation_{ OrientationDegrees::ZERO };
        Color               color_;
    };

    //================================================================================================================

    template<std::size_t NumRows, std::size_t NumCols>
    class Tetrimino : public TetrimonoBase
    {
    public:
        static constexpr std::size_t NUM_ROWS = NumRows;
        static constexpr std::size_t NUM_COLS = NumCols;

        constexpr Tetrimino(Color color) noexcept :
            TetrimonoBase(color)
        {
            std::fill(tiles_.begin(), tiles_.end(), BLANK_TILE);
        }

        virtual ~Tetrimino() noexcept = default;

        constexpr std::size_t numRows() const noexcept override
        {
            return NUM_ROWS;
        }

        constexpr std::size_t numCols() const noexcept override
        {
            return NUM_COLS;
        }

        bool isOccupied(std::size_t row, std::size_t col) const override
        {
            return this->operator()(row, col) == OCCUPIED_TILE;
        }

        wchar_t operator()(std::size_t row, std::size_t col) const override
        {
            std::size_t index = 0;
            switch (orientation_)
            {
                case OrientationDegrees::ZERO:
                {
                    index = NumCols * row + col;
                    break;
                }
                case OrientationDegrees::NINETY:
                {
                    index = 12 + row - NumRows * col;
                    break;
                }
                case OrientationDegrees::ONE_EIGHTY:
                {
                    index = 15 - NumCols * row - col;
                    break;
                }
                case OrientationDegrees::TWO_SEVENTY:
                {
                    index = 3 - row + NumRows * col;
                    break;
                }
            }

            return tiles_.at(index);
        }

    protected:
        std::array<wchar_t, NumRows * NumCols>    tiles_{};
    };

    //================================================================================================================

    class LongLine final : public Tetrimino<1, 4>
    {
    public:
        LongLine() noexcept :
            Tetrimino<1, 4>(Color(0))
        {
            std::fill(tiles_.begin(), tiles_.end(), OCCUPIED_TILE);
        }
    };

    //================================================================================================================

    class ShortLine final : public Tetrimino<1, 2>
    {
    public:
        ShortLine() noexcept :
            Tetrimino<1, 2>(Color(1))
        {
            std::fill(tiles_.begin(), tiles_.end(), OCCUPIED_TILE);
        }
    };

    //================================================================================================================

    class Box final : public Tetrimino<2, 2>
    {
    public:
        Box() noexcept :
            Tetrimino<2, 2>(Color(2))
        {
            std::fill(tiles_.begin(), tiles_.end(), OCCUPIED_TILE);
        }
    };

    //================================================================================================================

    class BigL final : public Tetrimino<2, 3>
    {
    public:
        BigL() noexcept :
            Tetrimino<2, 3>(Color(3))
        {
            std::fill(tiles_.begin(), tiles_.begin() + NUM_COLS, OCCUPIED_TILE);
            tiles_[NUM_COLS] = OCCUPIED_TILE;
        }
    };

    //================================================================================================================

    class LittleL final : public Tetrimino<2, 2>
    {
    public:
        LittleL() noexcept :
            Tetrimino<2, 2>(Color(4))
        {
            std::fill(tiles_.begin(), tiles_.begin() + NUM_COLS, OCCUPIED_TILE);
            tiles_[NUM_COLS] = OCCUPIED_TILE;
        }
    };

    //================================================================================================================

    class ZigZag final : public Tetrimino<2, 3>
    {
    public:
        ZigZag() noexcept :
            Tetrimino<2, 3>(Color(5))
        {
            tiles_[0] = OCCUPIED_TILE;
            tiles_[1] = OCCUPIED_TILE;
            tiles_[4] = OCCUPIED_TILE;
            tiles_[5] = OCCUPIED_TILE;
        }
    };

    //================================================================================================================

    constexpr std::uint32_t NUM_TETRIMINOS = 6;

    //================================================================================================================

    inline std::unique_ptr<TetrimonoBase> randomTetrimino()
    {
        static std::mt19937_64 generator;
        static auto dist = std::uniform_int_distribution<std::uint32_t>(1, NUM_TETRIMINOS);

        switch (dist(generator))
        {
            case 1:
            {
                return std::make_unique<LongLine>();
            }
            case 2:
            {
                return std::make_unique<ShortLine>();
            }
            case 3:
            {
                return std::make_unique<Box>();
            }
            case 4:
            {
                return std::make_unique<BigL>();
            }
            case 5:
            {
                return std::make_unique<LittleL>();
            }
            case 6:
            {
                return std::make_unique<ZigZag>();
            }
            default:
            {
                throw std::runtime_error("Tetrimino not implemented yet!");
            }
        }
    }
}
