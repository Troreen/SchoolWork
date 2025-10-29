#pragma once

#include <Windows.h>
#include <Windef.h>
#include <bitset>


class InputHandler
{
public:
	InputHandler();
	~InputHandler();

	// Feed keyboard-related messages (and focus changes)
	bool UpdateEvents(UINT message, WPARAM wParam, LPARAM lParam);
	// Feed mouse-related messages
	bool UpdateMouseInput(UINT aMessage, WPARAM wParam, LPARAM lParam);

	// Per-frame commit (call once per frame before reading states)
	void UpdateInput();

	// Keyboard queries
	bool IsKeyDown(const int aKeyCode) const;
	bool IsKeyPressed(const int aKeyCode) const;
	bool IsKeyReleased(const int aKeyCode) const;

	// Mouse queries
	POINT GetMousePos() const; 
	POINT GetMouseDelta() const;
	POINT GetMousePosScreen() const; // absolute on screen
	int GetWheelDeltaX() const;
	int GetWheelDeltaY() const;

	// Mouse button convenience (use VK codes 0x01..0x06 or your Keys enum values)
	bool IsMouseButtonDown(int buttonVk) const { return IsKeyDown(buttonVk); }
	bool IsMouseButtonPressed(int buttonVk) const { return IsKeyPressed(buttonVk); }
	bool IsMouseButtonReleased(int buttonVk) const { return IsKeyReleased(buttonVk); }

	// Window handle for optional features (mouse capture, screen coord transforms)
	void SetWindowHandle(HWND hWnd) { myHwnd = hWnd; }
	HWND GetWindowHandle() const { return myHwnd; }

	// Mouse capture controls (auto-capture on button down and release when all up)
	void SetAutoMouseCapture(bool enabled) { myAutoMouseCapture = enabled; }
	bool GetAutoMouseCapture() const { return myAutoMouseCapture; }
	bool IsMouseCaptured() const { return myMouseCaptured; }

private:
	std::bitset<256> myCurrentState;
	std::bitset<256> myPreviousState;
	std::bitset<256> myTentativeState;

	POINT myCurrentMousePos;
	POINT myPreviousMousePos;
	POINT myTentativeMousePos;

	int myWheelX = 0;
	int myWheelY = 0;
	int myTentativeWheelX = 0;
	int myTentativeWheelY = 0;

	HWND myHwnd = nullptr;
	bool myAutoMouseCapture = true;
	bool myMouseCaptured = false;

	// Helpers
	bool AnyMouseButtonDownTentative() const;
	void TryBeginMouseCapture();
	void TryEndMouseCaptureIfNoButtons();
};