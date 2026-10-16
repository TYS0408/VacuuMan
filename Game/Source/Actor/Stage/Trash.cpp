#include "stdafx.h"
#include "Trash.h"

namespace
{
	/** 種類ごとのデータ表*/
	struct TrashInfo
	{
		const char* filePath; /** モデルのファイルパス*/
		int cleanPoint;       /** 綺麗さのポイント*/
	};


	/** ゴミのモデルファイルパスと達成率のポイント*/
	const TrashInfo TRASH_INFO[static_cast<int>(EnTrashType::enNum)] =
	{
		
		{"Assets/modelData/Trash/Bottle/Bottle.tkm", 3},
		{"Assets/modelData/Trash/Box/Box.tkm", 20},
		{"Assets/modelData/Trash/PizzaBox/Pizza.tkm", 10},
		{"Assets/modelData/Trash/DrinkCan/SodaCan.tkm", 5},
		{"Assets/modelData/Trash/FoodCan/Foodcan.tkm", 2},
	};
}

bool Trash::Start()
{
	/** 初期化処理*/
	const TrashInfo& info = TRASH_INFO[static_cast<int>(m_type)];
	m_modelRender.Init(info.filePath);
	//m_modelRender.SetScale(Vector3(5.0f, 5.0f, 5.0f));
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