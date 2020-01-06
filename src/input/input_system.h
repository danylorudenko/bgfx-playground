#pragma once

#include <class_features/class_features.hpp>
#include <input/keyboard.h>
#include <Windows.h>

namespace pg::input
{

class InputSystem
    : public class_features::NonCopyable
    , public class_features::StaticSingleton<InputSystem>
{
private:
    struct MouseState
    {
        enum MouseButtonOffsets { Left, Right, Middle };
        std::uint32_t mouseButtonStates_;
        float xDelta_;
        float yDelta_;
        float mouseWheelDelta_;
    };

    struct KeyboardState
    {
        // so we have enough bits for the whole Keys enum
        std::uint64_t keysBits[(std::uint32_t)Keys::END / 64 + 1];
    };

    MouseState m_PendingMouseState;
    MouseState m_MouseState;
    MouseState m_PrevMouseState;

    KeyboardState m_PendingKeyboardState;
    KeyboardState m_PrevKeyboardState;
    KeyboardState m_KeyboardState;


public:
    InputSystem();
    InputSystem(HWND windowHandle);
    InputSystem(InputSystem&& rhs);
    InputSystem& operator=(InputSystem&& rhs);

    ~InputSystem();

    void Update(float dt);

    void ProcessSystemInput(HWND handle, WPARAM wparam, LPARAM lparam);

    MouseState const& GetMouseState() const;

    bool GetLeftMouseButtonPressed() const;
    bool GetRightMouseButtonPressed() const;
    bool GetMiddleMouseButtonPressed() const;
    bool GetLeftMouseButtonJustPressed() const;
    bool GetRightMouseButtonJustPressed() const;
    bool GetMiddleMouseButtonJustPressed() const;
    bool GetLeftMouseButtonJustReleased() const;
    bool GetRightMouseButtonJustReleased () const;
    bool GetMiddleMouseButtonJustReleased() const;

    bool GetKeyboardButtonDown(Keys key) const;
    bool GetKeyboardButtonJustPressed(Keys key) const;
    bool GetKeyboardButtonJustReleased(Keys key) const;

    static std::uint32_t GetCharFromKeys(Keys key);

private:
    static void SetKeysBitflagValue(std::uint64_t* bitflag, Keys key, bool value);
    static bool GetKeysBitflagValue(std::uint64_t const* bitflag, Keys key);


};

} // namespace pg::input
