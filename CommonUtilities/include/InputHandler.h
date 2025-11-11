#pragma once

#include <Windows.h>
#include <Windef.h>
#include <bitset>


class InputHandler
{
public:
	InputHandler();
	~InputHandler();

	bool UpdateEvents(UINT message, WPARAM wParam, LPARAM lParam);
	bool UpdateMouseInput(UINT aMessage, WPARAM wParam, LPARAM lParam);

	void UpdateInput();

	bool IsKeyDown(const int aKeyCode) const;
	bool IsKeyPressed(const int aKeyCode) const;
	bool IsKeyReleased(const int aKeyCode) const;

	POINT GetMousePos() const; 
	POINT GetMouseDelta() const;
	POINT GetMousePosScreen() const; 
	int GetWheelDeltaX() const;
	int GetWheelDeltaY() const;

	bool IsMouseButtonDown(int buttonVk) const { return IsKeyDown(buttonVk); }
	bool IsMouseButtonPressed(int buttonVk) const { return IsKeyPressed(buttonVk); }
	bool IsMouseButtonReleased(int buttonVk) const { return IsKeyReleased(buttonVk); }

	void SetWindowHandle(HWND hWnd) { myHwnd = hWnd; }
	HWND GetWindowHandle() const { return myHwnd; }

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

	bool AnyMouseButtonDownTentative() const;
	void TryBeginMouseCapture();
	void TryEndMouseCaptureIfNoButtons();
};