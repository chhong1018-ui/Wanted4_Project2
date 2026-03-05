#pragma once

#include "Math/Vector2.h"

namespace Wanted
{
	class Node
	{
	public:
		Node(Vector2 pos, Node* parentNode = nullptr)
			: position(pos), parentNode(parentNode), gCost(0), hCost(0), fCost(0)
		{
		}

		bool operator==(const Node& other) const
		{
			return position.x == other.position.x
				&& position.y == other.position.y;
		}

	public:
		// 노드의 그리드 위치.
		Vector2 position;

		// 시작점에서 현재 노드까지의 비용
		float gCost = 0.0f;

		// 현재 노드에서 목적지까지의 예상 비용 (휴리스틱).
		float hCost = 0.0f;

		// 총 비용(gCost + hCost).
		float fCost = 0.0f;

		// 경로 역추적을 위한 부모 노드 포인터.
		Node* parentNode = nullptr;
	};
}