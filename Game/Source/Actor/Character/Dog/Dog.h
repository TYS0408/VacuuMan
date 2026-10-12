#pragma once
#include"Source/Actor/Character/Character.h"
class Dog : public Character
{
public:
	Dog() {};
	~Dog() {};

public:
	bool Start()override;
	void Update()override;
	void Render(RenderContext& rc)override;

private:
	/** 移動*/
	void Move()override;
	/** 回転*/
	void Rotation()override;
	/** 新しいランダムな目標地点を決める*/
	void DecideNextTarget();
	/** 範囲内のランダムなfloat値を返す*/
	float RandomRange(float minValue, float maxValue);

private:
	ModelRender m_modelRender;

	//CharacterController m_characterController;
	/* 移動速度**/
	Vector3 m_moveSpeed = Vector3::Zero;
	/** 現在の位置*/
	Vector3 m_position = Vector3::Zero;
	/** 目標地点*/
	Vector3 m_tagetPos = Vector3::Zero;
	/** 進行方向*/
	Vector3 m_moveDir = Vector3::Zero;
	/** 回転*/
	Quaternion m_rotation = Quaternion::Identity;
	/** 到着後の待機時間*/
	float m_waitTimer = 1.0f;


};

