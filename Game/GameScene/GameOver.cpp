#include "stdafx.h"
#include "GameOver.h"
#include"GameScene/Title.h"
#include"Game.h"
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
	if (g_pad[0]->IsPressAnyKey())
	{
		/** タイトルを生成*/
		NewGO<Title>(0, "title");
		/** ゲーム本編を丸ごと削除(デストラクタで中身も消える)*/
		Game* game = FindGO<Game>("game");
		if (game != nullptr)
		{
			DeleteGO(game);
		}

		/** クリア画面自身も削除*/
		DeleteGO(this);

	}
}

void GameOver::Render(RenderContext& rc)
{
	m_gameOverSpriteRender.Draw(rc);
}