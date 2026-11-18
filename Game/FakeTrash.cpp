#include "stdafx.h"
#include "FakeTrash.h"
namespace
{
	/** ファイルパス*/
	const char* FILEPATH = "Assets/modelData/FakeTrash/FakeTrash_GameController.tkm";
	/** 大きさ設定*/
	const Vector3 SCALE = Vector3(1.0f, 1.0f, 1.0f);
}
bool FakeTrash::Start()
{
	m_modelRender.Init(FILEPATH);
	m_modelRender.SetScale(SCALE);
	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();
	return true;
}


void FakeTrash::Update()
{
	
}


void FakeTrash::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}