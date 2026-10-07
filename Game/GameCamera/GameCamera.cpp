#include "stdafx.h"
#include "GameCamera.h"
namespace
{
	/** 定点カメラの位置*/
	constexpr float FIXED_CAMERA_POS_X = 0.0f;
	constexpr float FIXED_CAMERA_POS_Y = 1000.0f;
	constexpr float FIXED_CAMERA_POS_Z = 500.0f;

	/** 定点カメラの注視点*/
	constexpr float FIXED_CAMERA_TARGET_X =0.0f;
	constexpr float FIXED_CAMERA_TARGET_Y = 0.0f;
	constexpr float FIXED_CAMERA_TARGET_Z = -400.0f;

	/** 近平面*/
	constexpr float NEARPLANE = 1.0f;
	/** 遠平面*/
	constexpr float FARPLANE = 10000.0f;
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
	m_fixedPos.Set(FIXED_CAMERA_POS_X, FIXED_CAMERA_POS_Y, FIXED_CAMERA_POS_Z);

	/** 定点カメラの注視点を設定*/
	m_fixedTarget.Set(FIXED_CAMERA_TARGET_X, FIXED_CAMERA_TARGET_Y, FIXED_CAMERA_TARGET_Z);

	/** 近平面を設定*/
	g_camera3D->SetNear(NEARPLANE);

	g_camera3D->SetViewAngle(Math::DegToRad(40.0f));

	/** 遠平面を設定*/
	g_camera3D->SetFar(FARPLANE);
	return true;
}


void GameCamera::Update()
{
	FixedView();
}

void GameCamera::FixedView()
{
	g_camera3D->SetPosition(m_fixedPos);
	g_camera3D->SetTarget(m_fixedTarget);
	g_camera3D->Update();
}



