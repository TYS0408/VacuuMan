#include "stdafx.h"
#include "Title.h"
#include"Game.h"
namespace
{
	/** タイトルの画像ファイルパス*/
	const char* GAMETITLE = "Assets/Sprite/GameScene/Title.dds";

	/** タイトルの画像の大きさ(横幅、高さ) */
	const int GAMETITLE_WIDTH = 1920;
	const int GAMETITLE_HEIGHT = 1080;
}

Title::Title()
{

}


Title::~Title()
{

}

bool Title::Start()
{
	m_titleSpriteRender.Init(GAMETITLE, GAMETITLE_WIDTH, GAMETITLE_HEIGHT);
	return true;
}

void Title::Update()
{
	InTitle();
	m_titleSpriteRender.Update();
}

void Title::InTitle()
{
	if (g_pad[0]->IsPressAnyKey() && !m_isStartButtonPressed)
	{
		/** ボタンを押したときの処理 */
		m_isStartButtonPressed = true;

	}


	if (m_isStartButtonPressed)
	{
		m_game = NewGO<Game>(0, "game");
		DeleteGO(this);
	}
}



void Title::Render(RenderContext& rc)
{
	m_titleSpriteRender.Draw(rc);
}