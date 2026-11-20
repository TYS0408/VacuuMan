#include "stdafx.h"
#include "Trashbox.h"
#include"Source/Actor/Character/Player/Player.h"
#include"Source/Actor/Stage/TrashManager.h"
namespace
{
	/** ファイルパス*/
	const char* FILEPATH = "Assets/modelData/Stage/TrashBox/TrashBox.tkm";
	/** 大きさ*/
	const Vector3 SCALE = Vector3(1.0f, 1.0f, 1.0f);
	/** 位置*/
	const Vector3 POS = Vector3(-550.0f, 0.0f, 450.0f);
	/** ごみ箱に近づいたと判断する距離*/
	constexpr float DISCARD_RANGE = 100.0f;
}
bool TrashBox::Start()
{
	m_trashManager = FindGO<TrashManager>("trashmanager");
	m_modelRender.Init(FILEPATH);
	m_modelRender.SetScale(SCALE);
	m_modelRender.SetPosition(POS);
	m_modelRender.Update();
	m_physicsStaticObject.CreateFromModel(
		m_modelRender.GetModel(),
		m_modelRender.GetModel().GetWorldMatrix()
	);
	return true;
}

bool TrashBox::IsPlayerNear()const
{
	Player* player = FindGO<Player>("player");
	if (player == nullptr)
	{
		return false;
	}
	Vector3 diff = player->GetPosition() - POS;
	/** 高さは無視して水平距離で判定*/
	diff.y = 0.0f;
	return diff.Length() <= DISCARD_RANGE;
}

void TrashBox::Update()
{
	m_modelRender.Update();
	/** トラッシュマネージャーが存在しているか*/
	if (m_trashManager == nullptr)
	{
		return;
	}
	/** Aボタンを押していないか*/
	if (!g_pad[0]->IsTrigger(enButtonA))
	{
		return;
	}
	/** プレイヤーが50以内にいなかったら*/
	if (!IsPlayerNear())
	{
		return;
	}
	/** 所持しているごみが無かったら*/
	if (m_trashManager->GetHeldTrashCount() <= 0)
	{
		return;
	}

	m_trashManager->DiscardHeldTrash();

	
}

void TrashBox::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}