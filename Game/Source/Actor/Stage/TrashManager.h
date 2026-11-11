#pragma once
#include "Source/Actor/Actor.h"
#include"Trash.h"
#include<array>
#include<random>
class TrashManager : public Actor
{
	/** ステージの区画*/
	enum class EnArea
	{
		/** 左上*/
		enUpperLeft
		/** 右上*/,
		enUpperRight
		/** 左下*/,
		enLowerLeft,
		/** 右下*/
		enLowerRight,
		enNum
	};
public:
	TrashManager();
	~TrashManager();

public:
	bool Start()override;
	void Update()override;
	void Render(RenderContext& rc)override {};


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
	/** ゴミとそのゴミが沸いた区画のペア*/
	struct TrashEntry
	{
		Trash* trash;
		EnArea area;
	};
private:
	
	/** その地点にゴミを置けるか(床以外の当たり判定が無いか)*/
	bool CanPlaceTrash(float x, float z,float&outY)const;
	/** 指定位置の真上から真下にレイを飛ばして、床だけにレイがあたるか調べる*/
	bool IsFloorOnly(float x, float z,float&outY)const;
	/** 指定位置が他のゴミから十分離れているか*/
	bool IsFarFromOtherTrash(float x, float z)const;
	/** プレイヤーがいるエリアを返す*/
	EnArea GetPlayerArea(const Vector3& playerPos)const;
	/** プレイヤーがエリア内にいるかどうか*/
	bool IsPlayerInErea(const Vector3& playerpos)const;
	/** 指定区画にゴミを一個配置*/
	bool SpawnTrash(EnArea area);
	/**ゴミが吸引範囲(距離 + 扇形)に入っているか*/
	bool IsInSuctionRange(const Vector3 & toTrash, float distance, const Vector3 & suctionDir)const;
	/** エリアが確保されたことをステージに知らせる関数*/
	void NotifyAreaSecured(EnArea area);
	/** 吸引処理全体*/
	void UpdateSuction();
	/** 距離に応じた引き寄せ速度を計算*/
	float CalcSuctionSpeed(float distance)const;
	/** ゴミを吸い込み口に向けて1フレーム動かす*/
	void PullTrash(Trash* trash, const Vector3& nozzlePos, float speed, float deltaTime)const;

private:

	static constexpr int  AREA_NUM = static_cast<int>(EnArea::enNum);

	std::vector<TrashEntry> m_trashList;
	std::mt19937 m_random;
	/** 各区画のごみの残り数*/
	std::array<int, AREA_NUM>m_areaTrashCount = {};
	/** 配置したゴミのポイント合計*/
	int m_totalCleanPoint = 0;
	/** 吸い込んだ分のポイント*/
	int m_currentCleanPoint = 0; 
};

