#include "stdafx.h"
#include "Player.h"
namespace
{
	/** プレイヤーモデルのファイルパス*/
	const char* FILEPATH = "Assets/modelData/Player/Player.tkm";
	/** プレイヤーのコントローラーの横幅*/
	constexpr float CHARACTER_CONTROLLER_WIDTH = 25.0f;
	/** プレイヤーのコントローラーの高さ*/
	constexpr float CHARACTER_CONTROLLER_HEIGHT = 75.0f;

}

Player::Player()
{
	m_playerModelRender.Init(FILEPATH);
}

Player::~Player()
{

}


bool Player::Start()
{
	/** キャラクターコントローラーを初期化する。幅・高さ・初期座標を渡す */
	m_characterController.Init(CHARACTER_CONTROLLER_WIDTH, CHARACTER_CONTROLLER_HEIGHT,m_transform.GetPosition());
	/** モデルの座標を初期座標に合わせて更新する */
	m_playerModelRender.SetPosition(m_transform.GetPosition());
	m_playerModelRender.Update();
	return true;
}


void Player::Update()
{
	Rotation();

	Move();

	m_playerModelRender.Update();
}


void Player::Move()
{

}


void Player::Rotation()
{
	//m_rotation.SetRotationDegY(180.0f);
	m_playerModelRender.SetRotation(m_rotation);
}



void Player::Render(RenderContext& rc)
{
	m_playerModelRender.Draw(rc);
}

