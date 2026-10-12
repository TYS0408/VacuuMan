#include "stdafx.h"
#include "VaccumCleaner.h"

namespace
{
	/** ファイルパス*/
	const char* FILEPATH = "Assets/modelData/VaccumCleaner/VaccumCleaner.tkm";
	Vector3 SCALE = Vector3(1.0f, 1.0f, 1.0f);

	/** 掃除機モデルから見た吸い込み口の相対位置*/
	const Vector3 NOZZLE_OFFSET = Vector3(0.0f, 0.0f, 60.0f);
}
bool VaccumCleaner::Start()
{
	m_modelRender.Init(FILEPATH);
	m_modelRender.SetScale(SCALE);
	m_modelRender.Update();
	return true;
}


void VaccumCleaner::Update()
{
	m_modelRender.Update();
}

Vector3 VaccumCleaner::GetSuctionPosition()const
{
	Vector3 offset = NOZZLE_OFFSET;
	m_rotation.Apply(offset);
	return m_position + offset;
}



Vector3 VaccumCleaner::GetSuctionDirection()const
{
	Vector3 dir = Vector3(0.0f, 0.0f, 1.0f);
	m_rotation.Apply(dir);
	return dir;
}

void VaccumCleaner::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
