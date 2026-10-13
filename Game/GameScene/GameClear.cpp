#include "stdafx.h"
#include "GameClear.h"
#include"GameScene/Title.h"
#include"Game.h"
namespace
{
	/** ゲームクリアの画像ファイルパス*/
	const char* GAMECLEAR_FILEPATH = "Assets/Sprite/GameScene/GameClear.dds";

	/** ゲームクリアの画像の大きさ */
	constexpr int GAMECLEAR_WIDTH = 1920;
	constexpr int GAMECLEAR_HEIGHT = 1080;
}
GameClear::GameClear()
{

}


GameClear::~GameClear()
{

}


bool GameClear::Start()
{
	m_gameClearSpriteRender.Init(GAMECLEAR_FILEPATH, GAMECLEAR_WIDTH, GAMECLEAR_HEIGHT);

	return true;
}


void GameClear::Update()
{
	InGameClear();
}


void GameClear::InGameClear()
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

void GameClear::Render(RenderContext& rc)
{
	m_gameClearSpriteRender.Draw(rc);
}


