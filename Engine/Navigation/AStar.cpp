#include "AStar.h"
#include "Util/Util.h"

#include <algorithm>
#include <cmath>
#include <Windows.h>
#include <iostream>

namespace Wanted
{

	AStar::AStar()
	{
	}

AStar::~AStar()
{
// 메모리 정리.
for (Node* node : openList)
{
SafeDelete(node);
}
openList.clear();

for (Node* node : closedList)
{
SafeDelete(node);
}
closedList.clear();

// goalNode는 open/closed 리스트에 들어가지 않으므로 별도 해제.
SafeDelete(goalNode);
goalNode = nullptr;
startNode = nullptr;
}

void AStar::ClearLists()
{
// 벡터에 담긴 모든 노드 객체를 메모리에서 해제.
for (Node* node : openList)
{
SafeDelete(node);
}

for (Node* node : closedList)
{
SafeDelete(node);
}

// goalNode는 open/closed 리스트에 들어가지 않으므로 별도 해제.
SafeDelete(goalNode);

// 벡터 자체를 비움.
openList.clear();
closedList.clear();
startNode = nullptr;
goalNode = nullptr;
}

	std::vector<Vector2> AStar::FindPath(
		const Vector2& startPos,
		const Vector2& goalPos,
		const std::vector<std::vector<int>>& grid)
	{
		// 탐색 시작 전 이전 데이터 청소.
		ClearLists();
		visitedNodes.clear();

		if (grid.empty() || grid[0].empty())
			return {};
		// 시작/목표 노드 저장.

		startNode = new Node(startPos);
		// goalNode는 경로 탐색에 사용하지 않으므로 생성하지 않음.
		goalNode = nullptr;

		// 시작 노드를 열린리스트에 추가 및 탐색 시작.
		openList.push_back(startNode);

		// 비용 계산에 사용할 변수 값 설정.
		std::vector<Direction> directions =
		{
			// 하상우좌 이동.
			{ 0, 1, 1.0f }, { 0, -1, 1.0f }, {1, 0, 1.0f }, {-1, 0, 1.0f}
		};

		// 탐색 가능한 위치가 있으면 계속 진행.
		while (!openList.empty())
		{
			// 현재 열린 리스트에 있는 노드 중 fCost가 가장 낮은 노드 검색.
			Node* lowestNode = openList[0];
			int currentIndex = 0;
			// 가장 비용이 작은 노드 검색 (선형 탐색).
			for (int ix = 0; ix < openList.size(); ix++)
			{
				// 비용이 더 작은 노드가 있으면 설정.
				if (openList[ix]->fCost < lowestNode->fCost)
				{
					lowestNode = openList[ix];
					currentIndex = ix;
				}
			}

			// fCost가 가장 낮은 노드를 현재 노드로 설정.
			Node* currentNode = lowestNode;

			visitedNodes.push_back(currentNode->position);

			// 현재 노드가 목표 노드인지 확인.
			if (currentNode->position.x == goalPos.x && currentNode->position.y == goalPos.y)
			{
				// 여기에 goalNode를 메모리 정리용도로 목록에 추가 가능.

				// 경로 반환 후 종료.
				std::vector<Vector2> path = ReconstructPath(currentNode);
				// 탐색용 노드 메모리 해제.
				ClearLists();
				// 최종 좌표 경로 반환.
				return path;
			}

			// 현재 노드를 열린 리스트에서 닫힌 리스트로 이동
			openList.erase(openList.begin() + currentIndex);
			closedList.push_back(currentNode);

			// 방문 처리용으로 열린 리스트에서 제거.
			//for (auto iterator = openList.begin();
			//	iterator != openList.end();
			//	++iterator)
			//{
			//	// iterator 위치의 노드가 currentNode인지 확인.
			//	if ((*iterator) == currentNode)
			//	{
			//		openList.erase(iterator);
			//		break;
			//	}
			//}

			// 현재 노드를 닫힌 리스트에 추가.
			//closedList.emplace_back(currentNode);


			// 이웃 노드 방문(탐색).
			for (const Direction& direction : directions)
			{
				Vector2 neighborPos(currentNode->position.x + direction.x, currentNode->position.y + direction.y);

				if (!IsInRange(neighborPos, grid) || grid[(int)neighborPos.y][(int)neighborPos.x] == 1)
				{
					continue;
				}

				// 현재 노드를 기준으로 새 gCost 계산.
				float newGCost = currentNode->gCost + direction.cost;

				// 갈 수는 있지만, 이미 방문한 곳인지 확인.
				if (HasVisited((int)neighborPos.x, (int)neighborPos.y, newGCost))
				{
					continue;
				}

				// 방문을 위한 이웃 노드 생성.
				Node* neighborNode = new Node(neighborPos, currentNode);
				// 비용 계산.
				neighborNode->gCost = newGCost;
				neighborNode->hCost = CalculateHeuristic(neighborPos, goalPos);
				neighborNode->fCost = neighborNode->gCost + neighborNode->hCost;

				// 이웃 노드가 열린 리스트에 있는지 확인.
				bool isDuplicate = false;
				for (Node* node : openList)
				{
					// 위치만 비교해서 열린 리스트에 넣을지 여부 확인.
					if (*node == *neighborNode)
					{
						isDuplicate = true;
						break;
					}
				}

				if (!isDuplicate)
				{
					openList.push_back(neighborNode);
				}

				else
				{
					// 중복이면 생성한 노드 삭제.
					delete neighborNode;
				}

			}
		}
		// 경로를 찾지 못한 경우 (openList가 빈 상태로 루프 종료).
		ClearLists();
		return {};
	}

	std::vector<Vector2> AStar::ReconstructPath(Node* goalNode)
	{
		// 경로를 저장할 배열 선언.
		std::vector<Vector2> path;

		// 역추적하면서 path에 채우기.
		Node* currentNode = goalNode;
		while (currentNode)
		{
			path.push_back(currentNode->position);
			currentNode = currentNode->parentNode;
		}

		// 이렇게 얻은 결과는 순서가 거꾸로.
		// 그래서 거꾸로 다시 정렬이 필요함.
		std::reverse(path.begin(), path.end());
		return path;
	}

	float AStar::CalculateHeuristic(Vector2 currentNode, Vector2 goalNode)
	{
		// 추정 비용 계산 함수.
		// 고민해볼 계산 방식.
		// 현재노드에서 목표 노드까지의 비용 계산.
		// 단순 거리를 휴리스틱 비용으로 계산.
		float xDistance = static_cast<float>(std::abs(currentNode.x - goalNode.x));
		float yDistance = static_cast<float>(std::abs(currentNode.y - goalNode.y));

		return xDistance + yDistance;
	}

	bool AStar::IsInRange(
		Vector2 pos, const std::vector<std::vector<int>>& grid)
	{
		// 예외처리.
		if (grid.empty() || grid[0].empty())
		{
			return false;
		}

		int ix = static_cast<int>(pos.x);
		int iy = static_cast<int>(pos.y);

		// y축 범위 체크 후 x축 범위 체크
		return (iy >= 0 && iy < (int)grid.size())
			&& (ix >= 0 && ix < (int)grid[0].size());
	}

	bool AStar::HasVisited(int x, int y, float gCost)
	{
		// 열린 리스트에 이미 같은 위치가 있고,
		// 기존 위치가 비용이 더 낮으면 방문했다고 판단 (중복 방지).
		for (Node* const node : openList)
		{
			if (node->position.x == x && node->position.y == y
				&& gCost >= node->gCost)
			{
				return true;
			}
		}

		// 닫힌 리스트에 이미 갖은 위치가 있고, 
		// 비용이 더 낮으면 방문했다고 판단.
		for (Node* const node : closedList)
		{
			if (node->position.x == x && node->position.y == y
				/*&& gCost >= node->gCost*/)
			{
				return true;
			}
		}

		return false;
	}

	bool AStar::IsDestination(const Node* const node, Vector2 goalPos)
	{
		// 두 노드가 같은지 비교.
		return node->position == goalPos;
	}
}
