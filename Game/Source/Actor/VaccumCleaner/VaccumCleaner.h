#pragma once
#include"Source/Actor/Actor.h"
class VaccumCleaner : public Actor
{
public:

	VaccumCleaner() {};
	~VaccumCleaner() {};

public:
	bool Start()override;
	void Update()override;
	void Render(RenderContext& rc)override;

	/** 座標を設定*/
	void SetPosition(const Vector3& position)
	{
		m_position = position;
		m_modelRender.SetPosition(position);
	}

	/** 回転を設定*/
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
		m_modelRender.SetRotation(rotation);
	}

	/** 吸い込む口のワールド座標*/
	Vector3 GetSuctionPosition()const;
	/** 吸い込む方向(掃除機の前方向)*/
	Vector3 GetSuctionDirection()const;

private:
	/** 掃除機のモデル*/
	ModelRender m_modelRender;
	/** 掃除機の座標*/
	Vector3 m_position = Vector3::Zero;
	/** 掃除機の回転*/
	Quaternion m_rotation = Quaternion::Identity;

};

