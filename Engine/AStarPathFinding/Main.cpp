#include "Navigation/Node.h"
#include "Navigation/AStar.h"

#include <iostream>
#include <vector>
#include <Windows.h>

// 맵.
// 0: 이동 가능.
// 1: 이동 불가.
// 2: 시작 위치.
// 3: 목표 위치.
std::vector<std::vector<int>> grid =
{
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 3, 1 },
	{ 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 },
	{ 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 },
	{ 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1 },
	{ 1, 2, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
};

// 시작 노드 및 목표 노드 검색 함수.
void FindStartAndGoalNode(
	const std::vector<std::vector<int>>& grid,
	Node*& outStartNode,
	Node*& outGoalNode
);

int main()
{
	// 커서 감추기.
	CONSOLE_CURSOR_INFO info = {};
	info.dwSize = 1;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);

	// 시작 위치 및 목표 위치.
	Node* startNode = nullptr;
	Node* goalNode = nullptr;

	// 시작/목표 위치 검색.
	FindStartAndGoalNode(
		grid, startNode, goalNode
	);

	// 못찾은 경우.
	if (!startNode || !goalNode)
	{
		// 생성됐을 수 있는 노드 삭제.
		SafeDelete(startNode);
		SafeDelete(goalNode);

		// 콘솔 복구 (넘어감).
		return 0;
	}

	// 길찾기.
	AStar aStar;

	// 경로 탐색.
	auto path = aStar.FindPath(startNode, goalNode, grid);

	// 성공한 경우.
	if (!path.empty())
	{
		std::cout << "\n경로 탐색 성공.\n";
		aStar.DisplayGridWithPath(grid, path);
	}

	// 실패한 경우.
	else
	{
		std::cout << "\n경로 탐색 실패.\n";
	}

	// 목표 노드 메모리 해제.
	SafeDelete(goalNode);

	// 콘솔 정리(넘어감).
	COORD coord = { 0, 30 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

	std::cin.get();
}

void FindStartAndGoalNode(
	const std::vector<std::vector<int>>& grid,
	Node*& outStartNode,
	Node*& outGoalNode)
{
	// 빈 그리드는 검색 불가.
	if (grid.empty() || grid[0].empty())
	{
		return;
	}

	for (int y = 0; y < static_cast<int>(grid.size()); ++y)
	{
		// 다 찾았으면 종료.
		if (outStartNode && outGoalNode)
		{
			break;
		}

		for (int x = 0; x < static_cast<int>(grid[0].size()); ++x)
		{
			// 다 찾았으면 종료.
			if (outStartNode && outGoalNode)
			{
				break;
			}

			// 시작 노드.
			if (grid[y][x] == 2)
			{
				outStartNode = new Node(x, y);
				continue;
			}

			// 목표 노드.
			if (grid[y][x] == 3)
			{
				outGoalNode = new Node(x, y);
				continue;
			}
		}
	}
}