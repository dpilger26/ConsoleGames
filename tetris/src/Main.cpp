#include "Board.hpp"
#include "Controls.hpp"
#include "Tetrimino.hpp"

#include <functional>
#include <iostream>

tetris::controls::CallbackFunctionType leftButtonPressCallback =
[]() noexcept -> void { std::cout << "Left arrow was pressed\n"; };

tetris::controls::CallbackFunctionType rightButtonPressCallback =
[]() noexcept -> void { std::cout << "Right arrow was pressed\n"; };

tetris::controls::CallbackFunctionType downButtonPressCallback =
[]() noexcept -> void { std::cout << "Down arrow was pressed\n"; };

tetris::controls::CallbackFunctionType aButtonPressCallback =
[]() noexcept -> void { std::cout << "A button was pressed\n"; };

tetris::controls::CallbackFunctionType sButtonPressCallback =
[]() noexcept -> void { std::cout << "B button was pressed\n"; };

int main()
{
    tetris::controls::InputHandler inputHandler(leftButtonPressCallback,
        rightButtonPressCallback,
        downButtonPressCallback,
        aButtonPressCallback,
        sButtonPressCallback);

    while (true)
    {
        inputHandler.handleInput();
    }

    //auto command = tetris::controls::Command<CallbackType>(leftButtonPressCallback);
    //command.execute();

    return EXIT_SUCCESS;
}
