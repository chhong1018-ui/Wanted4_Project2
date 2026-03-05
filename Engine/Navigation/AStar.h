#pragma once

#include "Node.h"
#include <vector>

namespace Wanted
{

	class WANTED_API AStar
	{
		// 방향 처리를 위한 구조체.
		struct Direction
		{
			// 위치.
			int x = 0;
			int y = 0;

			// 이동 비용.
			float cost = 0.0f;
		};

	public:
		AStar();
		~AStar();

		// 객체 지향 이론에서 -> 메시지(Message) - 공개 멤버 함수(인터페이스).
		// 경로 검색(탐색) 함수.
		// startNode: 시작 지점.
		// goalNode: 목표 지점.
		// grid: 탐색할 맵 (2차원 배열).
		std::vector<Vector2> FindPath(
			const Vector2& startPos,
			const Vector2& goalPos,
			const std::vector<std::vector<int>>& grid
		);

		void ClearLists();

	private:
		// 객체 지향 이론에서 -> 메소드(Method) - 비공개 멤버 함수.

		// 탐색을 완료한 후에 최적 경로 반환하는 함수.
		// 부모링크를 따라가면서 역추적.
		std::vector<Vector2> ReconstructPath(Node* goalNode);

		// 휴리스틱(hCost) 계산 함수.
		float CalculateHeuristic(Vector2 currentNode, Vector2 goalNode);

		// 탐색하려는 위치(노드)가 그리그 범위 안에 있는지 확인.
		// 유효성 검증.
		bool IsInRange(Vector2 pos, const std::vector<std::vector<int>>& grid);

		// 이미 방문했는지 확인하는 함수.
		bool HasVisited(int x, int y, float gCost);

		// 탐색하려는 노드가 목표 노드인지 확인하는 함수.
		bool IsDestination(const Node* const node, Vector2 goalPos);

	private:
		// 열린 리스트.
		std::vector<Node*> openList;

		// 닫힌 리스트.
		std::vector<Node*> closedList;

		// 시작 노드.
		Node* startNode = nullptr;

		// 목표 노드.
		Node* goalNode = nullptr;
	};
}