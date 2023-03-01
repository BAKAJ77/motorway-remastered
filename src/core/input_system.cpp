#include <core/input_system.h>
#include <util/formatted_exception.h>
#include <util/logging_system.h>

#include <GLFW/glfw3.h>

namespace Input
{
    static glm::vec2 currentCursorPos = { 0.0f, 0.0f };
    static glm::vec2 mouseScrollOffset = { 0.0f, 0.0f };

    static void CursorPositionCallback(GLFWwindow* window, double xPos, double yPos)
    {
        currentCursorPos.x = (float)xPos;
        currentCursorPos.y = (float)yPos;
    }

    static void MouseScrollOffsetCallback(GLFWwindow* window, double xOffset, double yOffset)
    {
        mouseScrollOffset.x = (float)xOffset;
        mouseScrollOffset.y = (float)yOffset;
    }
}

InputSystem::InputSystem() :
    m_focusedWindow(nullptr)
{}

void InputSystem::SetFocusedWindow(WindowFrame& frame)
{
    m_focusedWindow = &frame;
    
    if (m_focusedWindow)
    {
        // Get the initial position of the cursor
        // This is to prevent the initial massive change in the cursor position that is fetched since the callback 
        // is only called when the cursor is moved
        double cursorPosX = 0.0, cursorPosY = 0.0;
        glfwGetCursorPos(m_focusedWindow->GetFrameStruct(), &cursorPosX, &cursorPosY);
        Input::currentCursorPos = { (float)cursorPosX, (float)cursorPosY };

        // Set custom callback functions
        glfwSetCursorPosCallback(m_focusedWindow->GetFrameStruct(), Input::CursorPositionCallback);
        glfwSetScrollCallback(m_focusedWindow->GetFrameStruct(), Input::MouseScrollOffsetCallback);
    }
}

void InputSystem::Update() const
{
    glfwPollEvents();
}

bool InputSystem::WasKeyPressed(KeyCode key) const
{
    if (!m_focusedWindow)
        throw FormattedException("Input system has no window frame set as focused.");

    return glfwGetKey(m_focusedWindow->GetFrameStruct(), (int)key) == GLFW_PRESS;
}

bool InputSystem::WasMouseButtonPressed(MouseCode button) const
{
    if (!m_focusedWindow)
        throw FormattedException("Input system has no window frame set as focused.");

    return glfwGetMouseButton(m_focusedWindow->GetFrameStruct(), (int)button) == GLFW_PRESS;
}

const glm::vec2& InputSystem::GetCursorPosition() const
{
    if (!m_focusedWindow)
        LoggingSystem::GetInstance().Output("Cursor position may be invalid, no focused window set.", LoggingSystem::Severity::WARNING);

    return Input::currentCursorPos;
}

const glm::vec2& InputSystem::GetMouseScrollOffset() const
{
    if (!m_focusedWindow)
        LoggingSystem::GetInstance().Output("Mouse scroll offset may be invalid, no focused window set.", LoggingSystem::Severity::WARNING);

    return Input::mouseScrollOffset;
}

InputSystem& InputSystem::GetInstance()
{
    static InputSystem instance;
    return instance;
}
