#include "Controls.hpp"
#include "Tetrimino.hpp"

#include <functional>
#include <iostream>

using CallbackType = std::function<void()>;

CallbackType leftButtonPressCallback = []() noexcept -> void { std::cout << "Left arrow was pressed\n"; };
CallbackType rightButtonPressCallback = []() noexcept -> void { std::cout << "Right arrow was pressed\n"; };
CallbackType downButtonPressCallback = []() noexcept -> void { std::cout << "Down arrow was pressed\n"; };
CallbackType aButtonPressCallback = []() noexcept -> void { std::cout << "A button was pressed\n"; };
CallbackType sButtonPressCallback = []() noexcept -> void { std::cout << "B button was pressed\n"; };

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
