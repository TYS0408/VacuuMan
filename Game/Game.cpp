#include "stdafx.h"
#include "Game.h"


bool Game::Start()
{
	m_modelRender.Init("Assets/modelData/Player/Player.tkm");
	return true;
}

void Game::Update()
{
	// g_renderingEngine->DisableRaytracing();
	m_modelRender.Update();
	//Quaternion rotX;
	//rotX.SetRotationDegX(90.0f);
	//
	Quaternion rotY;
	rotY.SetRotationDegY(180.0f);

	m_rot = rotY;
	//m_rot.Multiply(rotY);
	m_modelRender.SetRotation(m_rot);
}

void Game::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}