#include "Actor.h"
#include "Util/Util.h"
#include "Render/Renderer.h"

#include <iostream>
#include <Windows.h>

namespace Wanted
{
	Actor::Actor(
		const char* image, 
		const Vector2& position,
		Color color)
		: position(position), color(color)
	{
		// 문자열 복사.
		size_t length = strlen(image) + 1;
		this->image = new char[length];
		strcpy_s(this->image, length, image);
	}

	Actor::~Actor()
	{
		// 메모리 해제.
		SafeDeleteArray(image);
	}

	void Actor::BeginPlay()
	{
		// 이벤트를 받은 후에는 플래그 설정.
		hasBeganPlay = true;
	}

	void Actor::Tick(float deltaTime)
	{
	}

	void Actor::Draw()
	{
		//Renderer::Draw(position, color, image);

		// 렌더러에 데이터 제출.
		Renderer::Get().Submit(image, position, color, sortingOrder);
	}

	void Actor::SetPosition(const Vector2& newPosition)
	{
		// 렌더러에 빈칸 그리기 요청.
		//Renderer::Draw(position, ' ');

		// 변경하려는 위치가 현재 위치와 같으면 건너뀜.
		if (position == newPosition)
		{
			return;
		}

		// 새로운 위치 설정.
		position = newPosition;
	}
}