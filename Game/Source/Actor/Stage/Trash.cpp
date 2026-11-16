#include "stdafx.h"
#include "Trash.h"

namespace
{
	/** 種類ごとのデータ表*/
	struct TrashInfo
	{
		const char* filePath; /** モデルのファイルパス*/
		int cleanPoint;       /** 綺麗さのポイント*/
		float scale;          /** モデルの拡大率*/
	};

	/** 大きいゴミ用の大きさ設定*/
	const float TRASH_SCALE_NORMAL = 1.3f;
	/** 小さいゴミ用の大きさ設定*/
	const float TRASH_SCALE_SMOLL = 1.5f;
	/** ゴミのモデルファイルパスと達成率のポイント*/
	const TrashInfo TRASH_INFO[static_cast<int>(EnTrashType::enNum)] =
	{
		
		{"Assets/modelData/Trash/Bottle/Bottle.tkm", 3,TRASH_SCALE_SMOLL},
		{"Assets/modelData/Trash/Box/Box.tkm", 20,TRASH_SCALE_NORMAL},
		{"Assets/modelData/Trash/PizzaBox/Pizza.tkm", 10,TRASH_SCALE_NORMAL},
		{"Assets/modelData/Trash/DrinkCan/SodaCan.tkm", 5,TRASH_SCALE_SMOLL},
		{"Assets/modelData/Trash/FoodCan/Foodcan.tkm", 2,TRASH_SCALE_SMOLL},
	};
	
}

bool Trash::Start()
{
	/** 初期化処理*/
	const TrashInfo& info = TRASH_INFO[static_cast<int>(m_type)];
	m_modelRender.Init(info.filePath);
	m_modelRender.SetScale(Vector3(info.scale, info.scale, info.scale));
	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();
	return true;

}

void Trash::Update()
{
	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();
}


int Trash::GetCleanPoint()const
{
	return TRASH_INFO[static_cast<int>(m_type)].cleanPoint;
}

void Trash::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}