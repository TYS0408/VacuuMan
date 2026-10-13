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


	/** 清潔度(0.0〜1.0)*/
	float GetCleanRate() const
	{
		if (m_totalCleanPoint <= 0)
		{
			return 0.0f;
		}
		return static_cast<float>(m_currentCleanPoint) / m_totalCleanPoint;
	}
private:

	/** その地点にゴミを置けるか(床以外の当たり判定が無いか)*/
	bool CanPlaceTrash(float x, float z)const;

	/** 指定位置の真上から真下にレイを飛ばして、床だけにレイがあたるか調べる*/
	bool IsFloorOnly(float x, float z)const;

	/** 吸引処理全体*/
	void UpdateSuction();

	/** ゴミが吸引範囲(距離+扇形)に入っているか*/
	bool IsInSuctionRange(const Vector3& toTrash, float distance, const Vector3& suctionDir)const;

	/** 距離に応じた引き寄せ速度を計算*/
	float CalcSuctionSpeed(float distance)const;

	/** ゴミを吸い込み口に向けて1フレーム動かす*/
	void PullTrash(Trash* trash, const Vector3& nozzlePos, float speed, float deltaTime)const;

private:
	std::vector<Trash*>m_trashList;
	std::mt19937 m_random;
	/** 配置したゴミのポイント合計*/
	int m_totalCleanPoint = 0;
	/** 吸い込んだ分のポイント*/
	int m_currentCleanPoint = 0; 
};

