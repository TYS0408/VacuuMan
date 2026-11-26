#include "stdafx.h"
#include "Title.h"
#include"Game.h"
#include"SoundManager/SoundManager.h"
namespace
{
	/** タイトルの画像ファイルパス*/
	const char* GAMETITLE = "Assets/Sprite/GameScene/GameTitle.dds";
	const char* GAMEBACKGROUND = "Assets/Sprite/GameScene/GameTitle_BackGround.dds";
	const char* GAMETITLE_UI = "Assets/Sprite/GameScene/GameTitle_UI.dds";

	/** ゲームタイトルのロゴの大きさ*/
	const int GAMETITLE_WIDTH = 1000.0f;
	const int GAMETITLE_HEIGHT = 500.0f;

	/** ゲームタイトルの背景画像の大きさ(横幅、高さ) */
	const int GAMETITLEBACKGROUND_WIDTH = 1920;
	const int GAMETITLEBACKGROUND_HEIGHT = 1080;

	/** ゲームタイトルUIの大きさ*/
	const int GAMETITLEUI_WIDTH = 800.0f;
	const int GAMETITLEUI_HEIGHT = 400.0f;

	/** タイトルの初期位置*/
	const Vector3 TITLE_POS = Vector3(0.0f, 200.0f, 0.0f);
	const Vector3 TITLEUI_POS = Vector3(10.0f, -250.0f, 0.0f);
	constexpr float UI_CYCLE_TIME = 2.0f;


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
	/** タイトルロゴの初期化*/
	m_titleSpriteRender.Init(GAMETITLE,GAMETITLE_WIDTH , GAMETITLE_HEIGHT);
	m_titleSpriteRender.SetPosition(TITLE_POS);
	m_titleSpriteRender.Update();
	/** タイトル背景の初期化*/
	m_backGroundSpriteRemnder.Init(GAMEBACKGROUND, GAMETITLEBACKGROUND_WIDTH, GAMETITLEBACKGROUND_HEIGHT);
	m_backGroundSpriteRemnder.Update();

	/** タイトルUIの初期化*/
	m_titleUISpriteRender.Init(GAMETITLE_UI, GAMETITLEUI_WIDTH, GAMETITLEUI_HEIGHT);
	m_titleUISpriteRender.SetPosition(TITLEUI_POS);
	m_titleUISpriteRender.Update();
	m_soundManager = FindGO<SoundManager>("soundmanager");
	m_titleBGM = m_soundManager->PlayingBGM(SoundBGM::enTitleBGM, true);
	return true;
}

void Title::Update()
{
		
	FadeUI();
	InTitle();
	m_backGroundSpriteRemnder.Update();
	m_titleSpriteRender.Update();
	m_titleUISpriteRender.Update();
}

void Title::InTitle()
{
	if (g_pad[0]->IsTrigger(enButtonA) && !m_isAButtonPressed)
	{
		/** ボタンを押したときの処理 */
		m_isAButtonPressed = true;

	}


	if (m_isAButtonPressed)
	{
		m_game = NewGO<Game>(0, "game");
		DeleteGO(this);
	}
}

void Title::FadeUI()
{
	/** 1フレーム辺りの経過時間を調べる*/
	m_fadeTimer += g_gameTime->GetFrameDeltaTime();

	/** fmodを使って少数の余りを返して、整数部分を切り捨てる*/
	float phase = fmodf(m_fadeTimer / UI_CYCLE_TIME, 1.0f);   // 0〜1
	/** 0〜1を -1〜1 に変換して、fabsfで絶対値にしてマイナスの値でもプラスになるよにする*/
	/** 最後に－1して上下反転させる*/
	float alpha = 1.0f - fabsf(phase * 2.0f - 1.0f); 

	m_titleUISpriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, alpha));
}

void Title::Render(RenderContext& rc)
{
	m_backGroundSpriteRemnder.Draw(rc);
	m_titleSpriteRender.Draw(rc);
	m_titleUISpriteRender.Draw(rc);
}