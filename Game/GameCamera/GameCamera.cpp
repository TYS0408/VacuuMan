#include "stdafx.h"
#include "GameCamera.h"
#include"Source/Actor/Character/Player/Player.h"

namespace
{
	/** カメラの位置設定*/
	constexpr float CAMERA_DEFAULT_Y = 125.0f;
	constexpr float CAMERA_DEFAULT_Z = -250.0f;

	/** プレイヤー注視点*/
	constexpr float PLAYER_LOOK_OFFSET_Y = 80.0f;

	/** カメラの最低の高さ*/
	constexpr float MIN_CAMERA_HEIGHT = 5.0f;

	/** カメラの最高の左方向位置(x座標)*/
	constexpr float MAX_CAMERA_POS_X = -1320.0f;

	/** レイの最大距離*/
	constexpr float RAY_MAX_DISTANCE = 750.0f;

	/** 近平面の大きさ*/
	constexpr float NEARPLANE = 1.0f;

	/** 遠平面の大きさ*/
	constexpr float FARPLANE = 1000000.0f;
}

GameCamera::GameCamera()
{

}

GameCamera::~GameCamera()
{

}


bool GameCamera::Start()
{
	/** カメラの初期位置を設定*/
	m_cameraPos.Set(0.0f, CAMERA_DEFAULT_Y, CAMERA_DEFAULT_Z);

	/** 近平面を設定*/
	g_camera3D->SetNear(NEARPLANE);

	/** 遠平面を設定*/
	g_camera3D->SetFar(FARPLANE);
	return true;
}


void GameCamera::Update()
{
	m_player = FindGO<Player>("player");
	/** カメラの追従*/
	Follow();
}

void GameCamera::Follow()
{
	/** 注視点の計算*/
	Vector3 target;

	/** 注視点をプレイヤーの座標に設定*/
	target = m_player->GetPosition();

	/** プレイヤーの足元より少し高い位置を注視点にする*/
	target.y += PLAYER_LOOK_OFFSET_Y;

	/** カメラの位置を回転させる前に保存しておく*/
	Vector3 toCameraPosOld = m_cameraPos;

	/** 右スティック入力を取得してカメラを回す*/
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();

	/** y軸周りの回転*/
	Quaternion rot;

	/** 回転量はスティックに応じて変化させる*/
	rot.SetRotationDeg(Vector3::AxisY, 1.3f * x);

	/** カメラの位置を回転*/
	rot.Apply(m_cameraPos);
	/** カメラの前方向を計算*/
	Vector3 forward = m_cameraPos;

	/** 前方向が小さい場合は,デフォルトの前方向を使用して正規化する*/
	if (forward.LengthSq() > 0.0001f)
	{
		forward.Normalize();
	}
	else
	{
		forward = Vector3(0.0f, 0.0f, 1.0f);
		forward.Normalize();
	}

	/** カメラの上方向を常にY軸方向とする*/
	Vector3 up = Vector3::AxisY;

	/** カメラの右方向を算出*/
	Vector3 right;
	right.Cross(up, forward);
	right.Normalize();

	/** 上下回転*/
	rot.SetRotationDeg(right, 1.3f * y);

	/** カメラの位置を回転させる*/
	rot.Apply(m_cameraPos);

	/** カメラの前方向を再計算*/
	Vector3 dir = m_cameraPos;

	/** 前方向が極端に小さい場合は、デフォルトの前方向を使用して正規化する */
	if (dir.LengthSq() > 0.00001f)
	{
		dir.Normalize();
	}
	else
	{
		dir = Vector3(0, 0, -1);
	}

	/** cos角度による制限( = 役72°)*/
	float limit = 0.95f;

	/** カメラの前方向とY軸の内積がlimitより大きい場合はカメラの位置をもとに戻す*/
	if (fabsf(dir.Dot(Vector3::AxisY) > limit))
	{
		/** 上向きすぎ、下向き過ぎを防止*/
		m_cameraPos = toCameraPosOld;
	}

	/** 視点の計算*/
	Vector3 pos = target + m_cameraPos;

	/** カメラ位置と注視点が一致しないようにする*/
	if ((pos - target).LengthSq() < 0.0001f)
	{
		pos = target + Vector3(0.0f, 0.0f, -50.0f);
	}


	/** 地面に近すぎないようにする */
	if (pos.y < MIN_CAMERA_HEIGHT) {
		pos.y = MIN_CAMERA_HEIGHT;
	}

	/** 横方向の位置が最大値を超えないようにする */
	if (pos.x < MAX_CAMERA_POS_X)
	{
		pos.x = MAX_CAMERA_POS_X;
	}

	/** メインカメラに注視点と視点を設定 */
	g_camera3D->SetTarget(target);
	g_camera3D->SetPosition(pos);

	/** カメラの更新 */
	g_camera3D->Update();
} 




