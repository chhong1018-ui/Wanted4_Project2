#pragma once

#include "Level/Level.h"
#include "Interface/ICanPlayerMove.h"
#include "Navigation/Astar.h"

class SokobanLevel
	: public Wanted::Level,
	public ICanPlayerMove
{
	RTTI_DECLARATIONS(SokobanLevel, Level)

public:
	SokobanLevel();

	// 이벤트 함수 오버라이드.
	virtual void Draw() override;

	void Tick(float deltaTime);
private:

	// 게임에서 사용할 맵을 로드하는 함수.
	void LoadMap(const char* filename);

	// Inherited via ICanPlayerMove
	virtual bool CanMove(
		const Wanted::Vector2& playerPosition,
		const Wanted::Vector2& nextPosition) override;


	// 게임 클리어 확인 함수.
	bool CheckGameClear();

private:
	// 획득해야하는 목표 점수.
	int targetScore = 0;

	// 게임 클리어 여부를 알려주는 변수.
	bool isGameClear = false;

private:

	// 토글 스위치.
	bool bshowAStar = false;
	// 처음에만 방문 노드 출력하기 위함.
	bool bIsFirstSearch = true;
	// 출력 간격 조절용 타이머.
	float visualTimer = 0.0f;
	// 현재 출력중인 방문 노드 인덱스.
	int visitedIndex = 0;
	// 현재 출력중인 최단 경로 인덱스.
	int pathIndex = 0;

private:
	Wanted::AStar aStar;
	std::vector<Wanted::Vector2> shortestPath;
	Wanted::Vector2 exitPosition;

	// 맵 데이터를 A*용 2d벡터로 변환하는 함수.
	std::vector<std::vector<int>> GetGridMap();
};
