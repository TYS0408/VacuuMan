#include "stdafx.h"
#include "Title.h"
#include"Game.h"
#include"SoundManager/SoundManager.h"
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
	DeleteGO(m_titleBGM);
}

bool Title::Start()
{
	m_titleSpriteRender.Init(GAMETITLE, GAMETITLE_WIDTH, GAMETITLE_HEIGHT);
	m_soundManager = FindGO<SoundManager>("soundmanager");
	m_titleBGM = m_soundManager->PlayingBGM(SoundBGM::enTitleBGM, true);
	return true;
}

void Title::Update()
{
	/** ゲームが終わって再度ゲームを始める際に、
	/*タイトルとゲームオーバー・クリアがAnyPressKeyなのでそのままスキップされてしまうのを防ぐため
	生成直後は入力を受け付けない*/
	if (m_inputWaitTimer > 0.0f)
	{
		m_inputWaitTimer -= g_gameTime->GetFrameDeltaTime();
		return;
	}

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