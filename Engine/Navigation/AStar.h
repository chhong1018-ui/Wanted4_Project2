#pragma once

#include "Node.h"
#include <vector>

template<typename T>
void SafeDelete(T*& t)
{
	if (t)
	{
		delete t;
		t = nullptr;
	}
}

namespace Wanted
{
	class AStar
	{
		// ���� ó���� ���� ����ü.
		struct Direction
		{
			// ��ġ.
			int x = 0;
			int y = 0;

			// �̵� ���.
			float cost = 0.0f;
		};

	public:
		AStar();
		~AStar();

		// ��ü ���� �̷п��� -> �޽���(Message) - ���� ��� �Լ�(�������̽�).
		// ��� �˻�(Ž��) �Լ�.
		// startNode: ���� ����.
		// goalNode: ��ǥ ����.
		// grid: Ž���� �� (2���� �迭).
		std::vector<Node*> FindPath(
			Node* startNode, Node* goalNode,
			std::vector<std::vector<int>>& grid
		);

		// ��(�׸���) ��� �Լ�.
		void DisplayGridWithPath(
			std::vector<std::vector<int>>& grid,
			const std::vector<Node*>& path
		);

	private:
		// ��ü ���� �̷п��� -> �޼ҵ�(Method) - ����� ��� �Լ�.

		// Ž���� �Ϸ��� �Ŀ� ���� ��� ��ȯ�ϴ� �Լ�.
		// �θ�ũ�� ���󰡸鼭 ������.
		std::vector<Node*> ConstructPath(Node* goalNode);

		// �޸���ƽ(hCost) ��� �Լ�.
		float CalculateHeuristic(Node* currentNode, Node* goalNode);

		// Ž���Ϸ��� ��ġ(���)�� �׸��� ���� �ȿ� �ִ��� Ȯ��.
		// ��ȿ�� ����.
		bool IsInRange(int x, int y, const std::vector<std::vector<int>>& grid);

		// �̹� �湮�ߴ��� Ȯ���ϴ� �Լ�.
		bool HasVisited(int x, int y, float gCost);

		// Ž���Ϸ��� ��尡 ��ǥ ������� Ȯ���ϴ� �Լ�.
		bool IsDestination(const Node* const node);

		// �׸��� ��� �Լ�.
		void DisplayGrid(std::vector<std::vector<int>>& grid);

	private:
		// ���� ����Ʈ.
		std::vector<Node*> openList;

		// ���� ����Ʈ.
		std::vector<Node*> closedList;

		// ���� ���.
		Node* startNode = nullptr;

		// ��ǥ ���.
		Node* goalNode = nullptr;
	};
}