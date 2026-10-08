#pragma once
#include "Source/Actor/Actor.h"
#include"Trash.h"
#include<random>
class TrashManager : public Actor
{
public:
	TrashManager() {};
	~TrashManager() {};

public:
	bool Start()override;
	void Update()override;
	void Render(RenderContext& rc)override;

private:

	/** その地点にゴミを置けるか(床以外の当たり判定が無いか)*/
	bool CanPlaceTrash(float x, float z)const;

	/** 指定位置の真上から真下にレイを飛ばして、床だけにレイがあたるか調べる*/
	bool IsFloorOnly(float x, float z)const;

	/** 置いてはいけない範囲(机の上など)に入っているか*/
	//bool IsInsideObstacle(float x, float z)const;

private:
	std::vector<Trash*>m_trashList;
	std::mt19937 m_random;
};

