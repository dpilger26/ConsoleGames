#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <type_traits>

#include <Windows.h>

namespace tetris::controls
{
    using CallbackFunctionType = std::function<void()>;

    //============================================================================================

    class ButtonCallbackBase
    {
    public:
        virtual ~ButtonCallbackBase() = default;
        virtual void operator()(bool isKeyDown) = 0;
    };

    //============================================================================================

    template<typename CallbackFunction,
        std::enable_if_t<std::is_invocable_v<CallbackFunction>, int> = 0>
    class ButtonCallback final : public ButtonCallbackBase
    {
    public:
        ButtonCallback(CallbackFunction callbackFunction) noexcept:
            callbackFunction_(callbackFunction)
        {}

        void operator()(bool isKeyDown) override
        {
            if (isKeyDownPrevious_ && isKeyDown)
            {
                // pass
            }
            else if (isKeyDown)
            {
                isKeyDownPrevious_ = true;
                callbackFunction_();
            }
            else if (isKeyDownPrevious_)
            {
                isKeyDownPrevious_ = false;
            }
        }

    private:
        bool isKeyDownPrevious_{ false };
        CallbackFunction callbackFunction_{};
    };

    //============================================================================================

    class CommandBase
    {
    public:
        virtual ~CommandBase() = default;
        virtual void execute(bool isPressed) = 0;
    };

    //============================================================================================

    template<typename CallbackType,
        std::enable_if_t<std::is_invocable_v<CallbackType, bool>, int> = 0>
    class Command final : public CommandBase
    {
    public:
        Command(CallbackType callback) noexcept:
            callback_(callback)
        {}

        void execute(bool isPressed) override
        {
            callback_(isPressed);
        }

    private:
        CallbackType callback_;
    };

    //============================================================================================

    // https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
    enum class Button
    {
        LEFT_ARROW = 0x25,
        RIGHT_ARROW = 0x27,
        DOWN_ARROW = 0x28,
        A = 0x41,
        S = 0x53
    };

    //============================================================================================

    template<typename CallbackFunction>
    class InputHandler
    {
    public:
        InputHandler(CallbackFunction leftArrowCallback,
            CallbackFunction rightArrowCallback,
            CallbackFunction downArrowCallback,
            CallbackFunction aButtonCallback,
            CallbackFunction sButtonCallback) :
            leftArrowCallback_(std::make_unique<Command<ButtonCallback<CallbackFunction>>>(leftArrowCallback)),
            rightArrowCallback_(std::make_unique<Command<ButtonCallback<CallbackFunction>>>(rightArrowCallback)),
            downArrowCallback_(std::make_unique<Command<ButtonCallback<CallbackFunction>>>(downArrowCallback)),
            aButtonCallback_(std::make_unique<Command<ButtonCallback<CallbackFunction>>>(aButtonCallback)),
            sButtonCallback_(std::make_unique<Command<ButtonCallback<CallbackFunction>>>(sButtonCallback))
        {}

        void handleInput()
        {
            leftArrowCallback_->execute(isPressed(Button::LEFT_ARROW));
            rightArrowCallback_->execute(isPressed(Button::RIGHT_ARROW));
            downArrowCallback_->execute(isPressed(Button::DOWN_ARROW));
            aButtonCallback_->execute(isPressed(Button::A));
            sButtonCallback_->execute(isPressed(Button::S));
        }

    private:
        std::unique_ptr<CommandBase> leftArrowCallback_;
        std::unique_ptr<CommandBase> rightArrowCallback_;
        std::unique_ptr<CommandBase> downArrowCallback_;
        std::unique_ptr<CommandBase> aButtonCallback_;
        std::unique_ptr<CommandBase> sButtonCallback_;

        bool isPressed(Button button)
        {
            return (GetKeyState(static_cast<int>(button)) & 0x8000) != 0;
        }
    };
}
