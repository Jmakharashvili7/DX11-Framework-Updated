#include "MouseClass.h"

#include <directxmath.h>
#include <d3d11_1.h>
#include <string>

using namespace DirectX;

void MouseClass::OnLeftClick(int x, int y)
{
	m_IsLeftClick = true;
	MouseEvent me(MouseEvent::EventType::L_CLICK, x, y);
	m_EventBuffer.push(me);
}

void MouseClass::OnLeftRelease(int x, int y)
{
	m_IsLeftClick = false;
	m_EventBuffer.push(MouseEvent(MouseEvent::EventType::L_RELEASE, x, y));
}

void MouseClass::OnRightClick(int x, int y)
{
	m_IsRightClick = true;
	MouseEvent me(MouseEvent::EventType::R_CLICK, x, y);
	m_EventBuffer.push(me);
}

void MouseClass::OnRightRelease(int x, int y)
{
	m_IsRightClick = false;
	m_EventBuffer.push(MouseEvent(MouseEvent::EventType::R_RELEASE, x, y));
}

void MouseClass::OnScrollClick(int x, int y)
{
	m_IsScrollClick = true;
	MouseEvent me(MouseEvent::EventType::SCROLL_CLICK, x, y);
	m_EventBuffer.push(me);
}

void MouseClass::OnScrollRelease(int x, int y)
{
	m_IsLeftClick = true;
	m_EventBuffer.push(MouseEvent(MouseEvent::EventType::SCROLL_RELEASE, x, y));
}

void MouseClass::OnScrollUp(int x, int y)
{
	m_EventBuffer.push(MouseEvent(MouseEvent::EventType::SCROLL_UP, x, y));
}

void MouseClass::OnScrollDown(int x, int y)
{
	m_EventBuffer.push(MouseEvent(MouseEvent::EventType::SCROLL_DOWN, x, y));
}

void MouseClass::OnMouseMove(int x, int y)
{
	dx = XMConvertToRadians(0.25f*static_cast<float>(x-m_Position.x));
	dy = XMConvertToRadians(0.25f*static_cast<float>(y-m_Position.y));


	std::string test = "dx is : ";
	test += std::to_string(dx);
	test += '\n';

	if (dx != 0.0f)
	OutputDebugStringA(test.c_str());

	std::string test2 = "dy is : ";
	test2 += std::to_string(dy);
	test2 += '\n';

	if (dy != 0.0f)
	OutputDebugStringA(test2.c_str());

	m_Position.x = x;
	m_Position.y = y;
	m_EventBuffer.push(MouseEvent(MouseEvent::EventType::MOVE, m_Position));
}

void MouseClass::OnMouseMoveRaw(int x, int y)
{
	m_EventBuffer.push(MouseEvent(MouseEvent::EventType::MOVE_RAW, x, y));
}

void MouseClass::ResetMousePos(int x, int y)
{
	// Clear the queue of all inputs
	std::queue<MouseEvent> empty;
	std::swap(m_EventBuffer, empty);

	// Set the mouse position variable to the new position
	m_Position.x = x;
	m_Position.y = y;
	
	// reset the mouse position
	SetCursorPos(x, y);
}

MouseEvent MouseClass::ReadEvent()
{
	if(m_EventBuffer.empty())
	{
		return MouseEvent();
	}
	else
	{
		MouseEvent e  = m_EventBuffer.front(); // Get the first event in the queue
		m_EventBuffer.pop(); // remove the first event from the buffer
		return e;
	}
}