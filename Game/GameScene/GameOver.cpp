#include "stdafx.h"
#include "GameOver.h"
#include"GameScene/Title.h"
namespace
{
	/** ゲームオーバー画像のファイルパス*/
	const char* FILEPATH = "Assets/Sprite/GameScene/GameOver.dds";

	/** ゲームオーバーの画像の大きさ(横幅、高さ) */
	const int GAMEOVER_WIDTH = 1920;
	const int GAMEOVER_HEIGHT = 1080;
}

GameOver::GameOver()
{

}


GameOver::~GameOver()
{

}


bool GameOver::Start()
{
	m_gameOverSpriteRender.Init(FILEPATH,GAMEOVER_WIDTH,GAMEOVER_HEIGHT);
	return true;
}


void GameOver::Update()
{
	InGameOver();
}


void GameOver::InGameOver()
{
	if (g_pad[0]->IsPressAnyKey() && !m_isGameOverToTitleButtonPressed)
	{
		m_isGameOverToTitleButtonPressed = true;
	}

	if (m_isGameOverToTitleButtonPressed)
	{
		m_title = NewGO<Title>(0, "title");
		DeleteGO(this);
	}
}

void GameOver::Render(RenderContext& rc)
{
	m_gameOverSpriteRender.Draw(rc);
}