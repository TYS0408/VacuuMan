#include "stdafx.h"
#include "Player.h"
#include"Source/Actor/VaccumCleaner/VaccumCleaner.h"
#include"Pause/Pause.h"
namespace
{
	/** プレイヤーモデルのファイルパス*/
	const char* FILEPATH = "Assets/modelData/Player/Player.tkm";

	/** アニメーションのファイルパス*/
	const char* ANIMATION_IDLE_PATH = "Assets/animData/Player/Idle.tka";
	const char* ANIMATION_RUN_PATH = "Assets/animData/Player/Run.tka";
	/** プレイヤーのコントローラーの横幅*/
	constexpr float CHARACTER_CONTROLLER_WIDTH = 25.0f;
	/** プレイヤーのコントローラーの高さ*/
	constexpr float CHARACTER_CONTROLLER_HEIGHT = 75.0f;

	/** プレイヤーの初期座標*/
	const Vector3 PLAYER_START_POSITION = Vector3(0.0f, 0.0f, -50.0f);


	/** プレイヤーの大きさ*/
	Vector3 PLAYER_SCALE = Vector3(1.0f,1.0f, 1.0f);


	/** プレイヤーから見た掃除機の相対位置(右・上・前)*/
	const Vector3 VACCUM_OFFSET = Vector3(30.0f, 40.0f, 40.0f);
}

Player::Player()
{
	/** 立ちアニメーションを読み込む*/
	m_animationClips[enAnimaitonClip_Idle].Load(ANIMATION_IDLE_PATH);
	m_animationClips[enAnimaitonClip_Idle].SetLoopFlag(true);

	/** 走りアニメーションを読み込む*/
	m_animationClips[enAnimationClip_Run].Load(ANIMATION_RUN_PATH);
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	m_playerModelRender.Init(FILEPATH,m_animationClips,enAnimationClip_Num,enModelUpAxisZ);
}

Player::~Player()
{
	if (m_vaccumCleaner != nullptr)
	{
		DeleteGO(m_vaccumCleaner);
	}
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

	/** 掃除機を生成*/
	m_vaccumCleaner = NewGO<VaccumCleaner>(0, "vaccumcleaner");

	m_pause = FindGO<Pause>("pause");
	UpdateVaccumCleaner();

	return true;
}


void Player::Update()
{
	/** ポーズ中は処理を止める*/
	if(m_pause->GetIsPause())
	{
		return;
	}
	/** 回転*/
	Rotation();
	/** 移動*/
	Move();
	/** ステート*/
	ManageState();
	/** アニメーション更新*/
	PlayAnimation();
	m_playerModelRender.Update();

	/** 移動後の座標・回転に掃除機を合わせる*/
	UpdateVaccumCleaner();
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


void Player::UpdateVaccumCleaner()
{
	if (m_vaccumCleaner == nullptr)
	{
		return;
	}

	/** オフセットをプレイヤーの向きに合わせて回転させる*/
	Vector3 offset = VACCUM_OFFSET;
	m_rotation.Apply(offset);

	m_vaccumCleaner->SetPosition(m_transform.GetPosition() + offset);
	m_vaccumCleaner->SetRotation(m_rotation);
}

void Player::ManageState()
{
	if (fabsf(m_moveSpeed.x) >= 0.000001 || fabsf(m_moveSpeed.z )>= 0.000001)
	{
		m_state = 1;
	}
	else
	{
		m_state = 0;
	}
}

void Player::PlayAnimation()
{
	switch (m_state)
	{
	case 0:
		m_playerModelRender.PlayAnimation(enAnimaitonClip_Idle);
		break;

	case 1:
		m_playerModelRender.PlayAnimation(enAnimationClip_Run);
	default:
		break;
	}
}


void Player::Render(RenderContext& rc)
{
	m_playerModelRender.Draw(rc);
}

