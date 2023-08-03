#pragma once

#include <SDL.h>
#include <array>

#include "Maize/Math/Point.h"

namespace Maize {

	class InputHandler
	{
	public:
		InputHandler() = default;

		void PollEvent(const SDL_Event& event)
		{
			if (event.type == SDL_KEYDOWN) 
			{
				m_Keys[event.key.keysym.scancode] = true;
			}
			else if (event.type == SDL_KEYUP) 
			{
				m_Keys[event.key.keysym.scancode] = false;
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN) 
			{
				m_MouseButtons[event.button.button] = true;
			}
			else if (event.type == SDL_MOUSEBUTTONUP) 
			{
				m_MouseButtons[event.button.button] = false;
			}
			else if (event.type == SDL_MOUSEMOTION) 
			{
				m_MousePosition = Point(event.motion.x, event.motion.y);
			}
			else if (event.type == SDL_MOUSEWHEEL) 
			{
				m_MouseScrollDelta = event.wheel.y;
			}

			// update previous keys after handling the current event
			m_PreviousKeys = m_Keys;
			m_PreviousMouseButtons = m_MouseButtons;
		}

		bool GetKey(SDL_Scancode key) const
		{
			return m_Keys[key];
		}

		bool GetKeyDown(SDL_Scancode key) const
		{
			return m_Keys[key] && !m_PreviousKeys[key];
		}

		bool GetKeyUp(SDL_Scancode key) const
		{
			return !m_Keys[key] && m_PreviousKeys[key];
		}

		bool GetMouseButton(int button) const 
		{
			return m_MouseButtons[button];
		}

		bool GetMouseButtonDown(int button) const 
		{
			return m_MouseButtons[button] && !m_PreviousMouseButtons[button];
		}

		bool GetMouseButtonUp(int button) const 
		{
			return !m_MouseButtons[button] && m_PreviousMouseButtons[button];
		}

		Point GetMousePosition()
		{
			return m_MousePosition;
		}

		uint32_t GetMouseScrollDelta() const {
			return m_MouseScrollDelta;
		}

	private:
		std::array<bool, SDL_NUM_SCANCODES> m_Keys = { false };
		std::array<bool, SDL_NUM_SCANCODES> m_PreviousKeys = { false };
		std::array<bool, 8> m_MouseButtons = { false }; // 8 mouse buttons supported by SDL
		std::array<bool, 8> m_PreviousMouseButtons = { false };
		Point m_MousePosition = Point::zero;
		uint32_t m_MouseScrollDelta = 0;
	};

}