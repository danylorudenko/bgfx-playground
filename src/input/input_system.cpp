#include <input/input_system.h>

#include <utility>
#include <cstdint>
#include <cstring>
#include <iostream>

namespace pg::input
{

InputSystem::InputSystem()
    : m_PendingMouseState{}
    , m_MouseState{}
    , m_PrevMouseState{}
    , m_PendingKeyboardState{}
    , m_KeyboardState{}
    , m_PrevKeyboardState{}
{
}

InputSystem::InputSystem(HWND windowHandle)
    : m_PendingMouseState{}
    , m_MouseState{}
    , m_PrevMouseState{}
    , m_PendingKeyboardState{}
    , m_KeyboardState{}
    , m_PrevKeyboardState{}
{
    UINT inputDeviceCount = 0;
    {
        UINT err = GetRawInputDeviceList(NULL, &inputDeviceCount, sizeof(RAWINPUTDEVICELIST));

        if (err == (UINT)-1) {
            DWORD lastError = GetLastError();
            std::cerr << "InputSystem: failed to retrieve RAWINPUTDEVICELIST(0). Error code: " << lastError << std::endl;
            return;
        }
    }

    RAWINPUTDEVICELIST* list = NULL;
    if (inputDeviceCount > 0) {
        list = (RAWINPUTDEVICELIST*)std::malloc(sizeof(RAWINPUTDEVICELIST) * inputDeviceCount);
        {
            UINT err = GetRawInputDeviceList(list, &inputDeviceCount, sizeof(RAWINPUTDEVICELIST));
            if (err == (UINT)-1) {
                DWORD lastError = GetLastError();
                std::cerr << "InputSystem: failed to retrieve RAWINPUTDEVICELIST(1). Error code: " << lastError << std::endl;
                return;
            }
        }
    }

    bool keyboardConnected = false;
    bool mouseConnected = false;

    for (std::uint32_t i = 0; i < inputDeviceCount; ++i) {
        if (list[i].dwType == RIM_TYPEMOUSE) {
            mouseConnected = true;
        }
        else if (list[i].dwType == RIM_TYPEKEYBOARD) {
            keyboardConnected = true;
        }
    }

    UINT rawDevicesCount = 0;
    RAWINPUTDEVICE rawDevices[2];
    if (mouseConnected) {
        rawDevices[rawDevicesCount].usUsagePage = 1;
        rawDevices[rawDevicesCount].usUsage = 2;
        rawDevices[rawDevicesCount].dwFlags = 0;
        rawDevices[rawDevicesCount].hwndTarget = windowHandle; // TODO: let's check how that works

        rawDevicesCount += 1;
    }

    if (keyboardConnected) {
        rawDevices[rawDevicesCount].usUsagePage = 1;
        rawDevices[rawDevicesCount].usUsage = 6;
        rawDevices[rawDevicesCount].dwFlags = 0;
        rawDevices[rawDevicesCount].hwndTarget = windowHandle; // TODO: let's check how that works

        rawDevicesCount += 1;
    }

    if (RegisterRawInputDevices(rawDevices, rawDevicesCount, sizeof(*rawDevices)) != TRUE) {
        UINT err = GetLastError();
        std::cerr << "Input System: Failed to register raw input devices. Error code: " << err << std::endl;
    }

    if (list != NULL)
        std::free(list);
}

InputSystem::InputSystem(InputSystem&& rhs)
{
    operator=(std::move(rhs));
}

InputSystem& InputSystem::operator=(InputSystem&& rhs)
{
    return *this;
}

InputSystem::~InputSystem()
{
    
}

InputSystem::MouseState const& InputSystem::GetMouseState() const
{
    return m_MouseState;
}

bool InputSystem::GetLeftMouseButtonPressed() const
{
    return m_MouseState.mouseButtonStates_ & 1 << (int)MouseState::Left;
}

bool InputSystem::GetRightMouseButtonPressed() const
{
    return m_MouseState.mouseButtonStates_ & 1 << (int)MouseState::Right;
}

bool InputSystem::GetMiddleMouseButtonPressed() const
{
    return m_MouseState.mouseButtonStates_ & 1 << (int)MouseState::Middle;
}

bool InputSystem::GetLeftMouseButtonJustPressed() const
{
    bool prevValue = m_PrevMouseState.mouseButtonStates_ & 1 << (int)MouseState::Left;
    return !prevValue && GetLeftMouseButtonPressed();
}

bool InputSystem::GetRightMouseButtonJustPressed() const
{
    bool prevValue = m_PrevMouseState.mouseButtonStates_ & 1 << (int)MouseState::Right;
    return !prevValue && GetRightMouseButtonPressed();
}

bool InputSystem::GetMiddleMouseButtonJustPressed() const
{
    bool prevValue = m_PrevMouseState.mouseButtonStates_ & 1 << (int)MouseState::Middle;
    return !prevValue && GetMiddleMouseButtonPressed();
}

bool InputSystem::GetLeftMouseButtonJustReleased() const
{
    bool prevValue = m_PrevMouseState.mouseButtonStates_ & 1 << (int)MouseState::Left;
    return prevValue && !GetLeftMouseButtonPressed();
}

bool InputSystem::GetRightMouseButtonJustReleased() const
{
    bool prevValue = m_PrevMouseState.mouseButtonStates_ & 1 << (int)MouseState::Right;
    return prevValue && !GetRightMouseButtonPressed();
}

bool InputSystem::GetMiddleMouseButtonJustReleased() const
{
    bool prevValue = m_PrevMouseState.mouseButtonStates_ & 1 << (int)MouseState::Middle;
    return prevValue && !GetMiddleMouseButtonPressed();
}

bool InputSystem::GetKeyboardButtonDown(Keys key) const
{
    return GetKeysBitflagValue(m_KeyboardState.keysBits, key);
}

bool InputSystem::GetKeyboardButtonJustPressed(Keys key) const
{
    bool prevValue = GetKeysBitflagValue(m_PrevKeyboardState.keysBits, key);
    return !prevValue && GetKeyboardButtonDown(key);
}

bool InputSystem::GetKeyboardButtonJustReleased(Keys key) const
{
    bool prevValue = GetKeysBitflagValue(m_PrevKeyboardState.keysBits, key);
    return prevValue && !GetKeyboardButtonDown(key);
}

void InputSystem::SetKeysBitflagValue(std::uint64_t* bitflag, Keys key, bool value)
{
    std::uint32_t member = (std::uint32_t)key / 64;
    std::uint32_t bitOffset = (std::uint32_t)key - 64 * member;

    if (value) {
        bitflag[member] |= 1ULL << bitOffset;
    }
    else {
        bitflag[member] &= ~(1ULL << bitOffset);
    }
    
}

bool InputSystem::GetKeysBitflagValue(std::uint64_t const* bitflag, Keys key)
{
    std::uint32_t member = (std::uint32_t)key / 64;
    std::uint32_t bitOffset = (std::uint32_t)key - 64 * member;

    return bitflag[member] & (1ULL << bitOffset);
}

std::uint32_t InputSystem::GetCharFromKeys(Keys key)
{
    return MapVirtualKeyW(KeysToVKey(key), MAPVK_VK_TO_CHAR);
}

void InputSystem::Update(float dt)
{
    m_PrevMouseState = m_MouseState;
    m_MouseState = m_PendingMouseState;

    m_PrevKeyboardState = m_KeyboardState;
    m_KeyboardState = m_PendingKeyboardState;
}

void InputSystem::ProcessSystemInput(HWND handle, WPARAM wparam, LPARAM lparam)
{
    UINT dataSize = 0;

    UINT result = GetRawInputData((HRAWINPUT)lparam, RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER));
    if (result != 0) {
        DWORD err = GetLastError();
        std::cerr << 
            "InputSystem::ProcessSystemInput: GetRawInputData failed. Can't retrieve system input. "
            "Error code: " << err << std::endl;
        return;
    }

    void* data = std::malloc(dataSize);
    result = GetRawInputData((HRAWINPUT)lparam, RID_INPUT, data, &dataSize, sizeof(RAWINPUTHEADER));
    if (result < 0 || result != dataSize) {
        DWORD err = GetLastError();
        std::cerr <<
            "InputSystem::ProcessSystemInput: GetRawInputData failed. Can't retrieve system input. "
            "Error code: " << err << std::endl;
    }

    RAWINPUT* rawInput = (RAWINPUT*)data;
    RAWINPUTHEADER& header = rawInput->header;
    if (header.dwType == RIM_TYPEMOUSE) {
        RAWMOUSE& mouse = rawInput->data.mouse;
        if(mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN)
            m_PendingMouseState.mouseButtonStates_ |= 1 << static_cast<std::uint32_t>(MouseState::MouseButtonOffsets::Left);

        if(mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN)
            m_PendingMouseState.mouseButtonStates_ |= 1 << static_cast<std::uint32_t>(MouseState::MouseButtonOffsets::Right);

        if(mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN)
            m_PendingMouseState.mouseButtonStates_ |= 1 << static_cast<std::uint32_t>(MouseState::MouseButtonOffsets::Middle);

        if(mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP)
            m_PendingMouseState.mouseButtonStates_ &= ~(1 << static_cast<std::uint32_t>(MouseState::MouseButtonOffsets::Left));

        if (mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP)
            m_PendingMouseState.mouseButtonStates_ &= ~(1 << static_cast<std::uint32_t>(MouseState::MouseButtonOffsets::Right));

        if (mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_UP)
            m_PendingMouseState.mouseButtonStates_ &= ~(1 << static_cast<std::uint32_t>(MouseState::MouseButtonOffsets::Middle));

        if (mouse.usButtonFlags & RI_MOUSE_WHEEL)
            m_PendingMouseState.mouseWheelDelta_ = static_cast<float>(mouse.usButtonData);

        m_PendingMouseState.xDelta_ = static_cast<float>(mouse.lLastX);
        m_PendingMouseState.yDelta_ = static_cast<float>(mouse.lLastY);
    }
    else if (header.dwType == RIM_TYPEKEYBOARD) {
        // handle keyboard input?
        RAWKEYBOARD& keyboard = rawInput->data.keyboard;
        if (keyboard.Flags & RI_KEY_BREAK) {
            // up
            SetKeysBitflagValue(m_PendingKeyboardState.keysBits, VKeyToKeys(keyboard.VKey), false);
        }
        // RI_KEY_MAKE defined as 0 sooo
        else if (keyboard.Flags == RI_KEY_MAKE) {
            // down
            SetKeysBitflagValue(m_PendingKeyboardState.keysBits, VKeyToKeys(keyboard.VKey), true);
        }
    }

    if (data != NULL)
        std::free(data);

}

} // namespace pg::input
