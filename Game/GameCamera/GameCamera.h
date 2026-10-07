#pragma once
class Player;
class GameCamera : public  IGameObject
{
public:
	GameCamera();
	~GameCamera();

public:
	bool Start();
	void Update();


public:
	/** 定点カメラの更新*/
	void FixedView();

	/** カメラの位置を取得*/
	Vector3 GetCameraPosition()const
	{
		return g_camera3D->GetPosition();
	}
	/** カメラの前方ベクトルを取得*/
	Vector3 GetCameraForward()const
	{
		return g_camera3D->GetForward();
	}

private:
	

private:
	/** カメラの位置*/
	Vector3 m_cameraPos = Vector3::Zero;

	/** プレイヤーのポインタ*/
	Player* m_player = nullptr;

	/** 定点カメラの位置*/
	Vector3 m_fixedPos = Vector3::Zero;
	Vector3 m_fixedTarget = Vector3::Zero;

};



