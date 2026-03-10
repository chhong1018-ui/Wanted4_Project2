#include "Input.h"
#include <Windows.h>
#include <iostream>

// Ctrl + Home키로 파일 제일 위로 이동 가능.
// Rider는 헤더 자동 추가 해줌.
namespace Wanted
{
	// 전역 변수 초기화.
	Input* Input::instance = nullptr;

	Input::Input()
	{
		// 객체가 초기화되면 자기 자신의 주소를 저장.
		instance = this;

		// 콘솔 마우스 입력 활성화 (Quick Edit가 켜져 있으면 클릭이 먹통될 수 있음)
		inputHandle = GetStdHandle(STD_INPUT_HANDLE);
		if (inputHandle && inputHandle != INVALID_HANDLE_VALUE)
		{
			DWORD mode = 0;
			if (GetConsoleMode((HANDLE)inputHandle, &mode))
			{
				previousConsoleMode = mode;
				hasPreviousConsoleMode = true;

				DWORD newMode = mode;
				newMode |= ENABLE_MOUSE_INPUT;
				newMode |= ENABLE_EXTENDED_FLAGS;
				newMode &= ~ENABLE_QUICK_EDIT_MODE;
				SetConsoleMode((HANDLE)inputHandle, newMode);
			}
		}
	}

	Input::~Input()
	{
		if (hasPreviousConsoleMode && inputHandle && inputHandle != INVALID_HANDLE_VALUE)
		{
			SetConsoleMode((HANDLE)inputHandle, previousConsoleMode);
		}
	}

	bool Input::GetKeyDown(int keyCode)
	{
		return keyStates[keyCode].isKeyDown
			&& !keyStates[keyCode].wasKeyDown;
	}

	bool Input::GetKeyUp(int keyCode)
	{
		return !keyStates[keyCode].isKeyDown
			&& keyStates[keyCode].wasKeyDown;
	}

	bool Input::GetKey(int keyCode)
	{
		return keyStates[keyCode].isKeyDown;
	}

	Vector2 Input::GetMousePosition() const
	{
		return mousePosition;
	}

	bool Input::GetMouseLeftDown() const
	{
		return isMouseLeftDown && !wasMouseLeftDown;
	}

	Input& Input::Get()
	{
		// 싱글턴(Singleton).
		// 이 함수는 콘텐츠 프로젝트에서 접근함.
		// 따라서 엔진은 이미 초기화 완료 상태.
		if (!instance)
		{
			//return *nullptr;
			std::cout << "Error: Input::Get(). instance is null\n";

			// 디버그 모드에서만 동작함.
			// 자동으로 중단점 걸림.
			__debugbreak();
		}

		// Lazy-Pattern.
		// 이펙티브 C++에 나옴.
		//static Input instance;
		return *instance;
	}

	void Input::ProcessInput()
	{
		// 키 마다의 입력 읽기.
		// !!! 운영체제가 제공하는 기능을 사용할 수 밖에 없음.
		for (int ix = 0; ix < 255; ++ix)
		{
			keyStates[ix].isKeyDown
				= (GetAsyncKeyState(ix) & 0x8000) > 0 ? true : false;
		}

		// 콘솔 입력 이벤트를 drain 하면서 마우스 상태 갱신.
		if (!inputHandle || inputHandle == INVALID_HANDLE_VALUE)
		{
			return;
		}

		DWORD eventCount = 0;
		if (!GetNumberOfConsoleInputEvents((HANDLE)inputHandle, &eventCount))
		{
			return;
		}

		while (eventCount > 0)
		{
			INPUT_RECORD record = {};
			DWORD readCount = 0;
			if (!ReadConsoleInputA((HANDLE)inputHandle, &record, 1, &readCount) || readCount == 0)
			{
				break;
			}

			if (record.EventType == MOUSE_EVENT)
			{
				const MOUSE_EVENT_RECORD& mouse = record.Event.MouseEvent;
				mousePosition = Vector2((int)mouse.dwMousePosition.X, (int)mouse.dwMousePosition.Y);
				isMouseLeftDown = (mouse.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) != 0;
			}

			if (!GetNumberOfConsoleInputEvents((HANDLE)inputHandle, &eventCount))
			{
				break;
			}
		}
	}

	void Input::SavePreviousInputStates()
	{
		// 현재 입력 값을 이전 입력 값으로 저장.
		for (int ix = 0; ix < 255; ++ix)
		{
			keyStates[ix].wasKeyDown
				= keyStates[ix].isKeyDown;
		}

		wasMouseLeftDown = isMouseLeftDown;
	}
}