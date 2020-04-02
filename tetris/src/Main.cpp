#include "Tetrimino.hpp"

#include <iostream>

int main()
{
    for (int i = 0; i < 20; ++i)
    {
        auto randTetrimino = tetris::tetrimino::randomTetrimino();
        std::cout << std::boolalpha << randTetrimino->isOccupied(0, 0) << '\n';
    }

    return EXIT_SUCCESS;
}
