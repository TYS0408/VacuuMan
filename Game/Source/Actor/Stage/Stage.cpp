#include "stdafx.h"
#include "Stage.h"
namespace
{
	/** ステージのファイルパス*/
	const char* STAGE_FILE_PATH = "Assets/modelData/Stage/Stage.tkm";

	/** ステージの座標*/
	Vector3 STAGE_POS = Vector3(0.0f, 0.0f, 0.0f);
}

bool Stage::Start()
{
	//ステージのモデルをロード。
	m_stageModelRender.Init(STAGE_FILE_PATH);

	/** ステージの位置を設定 */
	m_stageModelRender.SetPosition(STAGE_POS);

	/** 当たり判定*/
	m_stagePhysicsObject.CreateFromModel(
		m_stageModelRender.GetModel(),
		m_stageModelRender.GetModel().GetWorldMatrix());
	return true;
}


void Stage::Update()
{
	m_stageModelRender.Update();
}

void Stage::Render(RenderContext& rc)
{
	m_stageModelRender.Draw(rc);
}