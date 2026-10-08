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

	/** プレイヤーの初期座標*/
	const Vector3 PLAYER_START_POSITION = Vector3(0.0f, 0.0f, -50.0f);

	/** プレイヤーの大きさ*/
	Vector3 PLAYER_SCALE = Vector3(1.0f,1.0f, 1.0f);

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

	/** 初期座標を設定*/
	m_transform.SetPosition(PLAYER_START_POSITION);
	/** キャラクターコントローラーを初期化する。幅・高さ・初期座標を渡す */
	m_characterController.Init(CHARACTER_CONTROLLER_WIDTH, CHARACTER_CONTROLLER_HEIGHT,m_transform.GetPosition());
	/** モデルの座標を初期座標に合わせて更新する */
	m_playerModelRender.SetPosition(m_transform.GetPosition());

	m_playerModelRender.SetScale(PLAYER_SCALE);
	m_rotation.SetRotationDegY(180.0f);
	m_playerModelRender.SetRotation(m_rotation);
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
	/** 左スティックの入力量を計算*/
	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.z = g_pad[0]->GetLStickYF();

	/** カメラの前方向と右方向のベクトルを持ってくる*/
	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();

	/** 正規化*/
	right.y = 0.0f;
	forward.y = 0.0f;
	right.Normalize();
	forward.Normalize();

	/** 入力量を反映*/
	Vector3 moveDir = forward * stickL.z + right * stickL.x;
	moveDir *= 150.0f;
	m_moveSpeed = moveDir;

	/** 左スティックの入力量と150.0fを乗算する*/
	right *= stickL.x * 150.0f;
	forward *= stickL.z * 150.0f;

	/** 移動速度にカメラの前方向と右方向を加算*/
	m_moveSpeed += right + forward;

	/** キャラクターコントローラーを使って座標を移動させる*/
	m_transform.SetPosition(m_characterController.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime()));

	Vector3 position = m_transform.GetPosition();

	
	/** モデルの座標をキャラクターコントローラーの座標に合わせる*/
	m_playerModelRender.SetPosition(m_transform.GetPosition());

}


void Player::Rotation()
{
	/** 移動速度のXまたはZ成分がある程度あるときだけ回転処理を行う。
	(停止しているときは回転させない)*/
	if (fabsf(m_moveSpeed.x) >= 0.0001 || fabsf(m_moveSpeed.z)>= 0.0001f)
	{
		m_rotation.SetRotationYFromDirectionXZ(m_moveSpeed);

		m_transform.SetRotation(m_rotation);
	}

	
	m_playerModelRender.SetRotation(m_rotation);
}



void Player::Render(RenderContext& rc)
{
	m_playerModelRender.Draw(rc);
}

