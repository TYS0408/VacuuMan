#pragma once
#include "Source/Actor/Character/Character.h"
#include"Source/Actor/VaccumCleaner/VaccumCleaner.h"
/**　プレイヤークラス*/
class Pause;
class Player : public Character
{
public:
	Player();
	~Player();

public:
	virtual bool Start()override;
	virtual void Update()override;
	virtual void Render(RenderContext& rc)override;


public:
	void Move()override;

	void Rotation()override;


	/** プレイヤーのポジション取得関数 */
	const Vector3 GetPosition()const
	{
		return m_transform.GetPosition();
	};

private:
	/** 持っている掃除機*/
	VaccumCleaner* m_vaccumCleaner = nullptr;

	/** 掃除機を持つ位置(プレイヤーから見たオフセット)*/
	void UpdateVaccumCleaner();

	private:
	/** プレイヤーのモデル*/
	ModelRender m_playerModelRender;
	/** プレイヤーのコントローラー*/
	CharacterController m_characterController;

	/** ポーズ*/
	Pause* m_pause = nullptr;
	/** プレイヤーの回転*/
	Quaternion m_rotation = Quaternion::Identity;
	/** プレイヤーの移動速度*/
	Vector3 m_moveSpeed = Vector3::Zero;

};
