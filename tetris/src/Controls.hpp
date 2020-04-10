#pragma once

#include <iostream>
#include <memory>
#include <type_traits>

#include <Windows.h>

namespace tetris::controls
{
    // https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
    enum Button
    {
        LEFT_ARROW = 0x25,
        RIGHT_ARROW = 0x27,
        DOWN_ARROW = 0x28,
        A = 0x41,
        S = 0x53
    };

    class CommandBase
    {
    public:
        virtual ~CommandBase() = default;
        virtual void execute() noexcept = 0;
    };

    template<typename CallbackType,
        std::enable_if_t<std::is_invocable_v<CallbackType>, int> = 0>
    class Command final : public CommandBase
    {
    public:
        Command(CallbackType callback) :
            callback_(callback)
        {}

        void execute() noexcept override
        {
            callback_();
        }

    private:
        CallbackType callback_;
    };

    template<typename CallbackType>
    class InputHandler
    {
    public:
        InputHandler(CallbackType leftArrowCallback,
            CallbackType rightArrowCallback,
            CallbackType downArrowCallback,
            CallbackType aButtonCallback,
            CallbackType sButtonCallback) :
            leftArrowCallback_(std::make_unique<Command<CallbackType>>(leftArrowCallback)),
            rightArrowCallback_(std::make_unique<Command<CallbackType>>(rightArrowCallback)),
            downArrowCallback_(std::make_unique<Command<CallbackType>>(downArrowCallback)),
            aButtonCallback_(std::make_unique<Command<CallbackType>>(aButtonCallback)),
            sButtonCallback_(std::make_unique<Command<CallbackType>>(sButtonCallback))
        {}

        void handleInput()
        {
            if (isPressed(Button::LEFT_ARROW))
            {
                leftArrowCallback_->execute();
            }

            if (isPressed(Button::RIGHT_ARROW))
            {
                rightArrowCallback_->execute();
            }

            if (isPressed(Button::DOWN_ARROW))
            {
                downArrowCallback_->execute();
            }

            if (isPressed(Button::A))
            {
                aButtonCallback_->execute();
            }

            if (isPressed(Button::S))
            {
                sButtonCallback_->execute();
            }
        }

    private:
        std::unique_ptr<CommandBase> leftArrowCallback_;
        std::unique_ptr<CommandBase> rightArrowCallback_;
        std::unique_ptr<CommandBase> downArrowCallback_;
        std::unique_ptr<CommandBase> aButtonCallback_;
        std::unique_ptr<CommandBase> sButtonCallback_;

        bool isPressed(Button button)
        {
            return (GetKeyState(button) & 0x8000) != 0;
        }
    };
}
