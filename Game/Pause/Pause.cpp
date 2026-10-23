#include "stdafx.h"
#include "Pause.h"
#include"GameScene/Title.h"
#include"GameScene/Game.h"
#include"SoundPause.h"
#include"SoundManager/SoundManager.h"
namespace
{
	/** Pauseに使うファイルパス*/
	/** Pause中背景*/
	const char* PAUSE_BACKGROUND_PATH = "Assets/Sprite/Pause/BackGround2.dds";
	/** 「ゲームをやめる」*/
	const char * PAUSE_QUITE_PATH = "Assets/Sprite/Pause/QuitGame.dds";
	/** 「ゲームに戻る」*/
	const char * PAUSE_RETURN_PATH = "Assets/Sprite/Pause/ReturnGame.dds";
	/**	「音量」*/
	const char* PAUSE_VOLUME_PATH = "Assets/Sprite/Pause/SoundIcon.dds";
	/** それぞれの位置設定*/
	const Vector3 QUITEPOS = { 0.0f, 80.0f,0.0f };
	const Vector3 RETURNPOS = { 0.0f, 200.0f,0.0f };
	const Vector3 SOUNDPOS = { 0.0f,-40.0f,0.0f };



	/** それぞれの画像の大きさ*/
	/** 背景*/
	const int BACKGROUND_WIDTH = 1980;
	const int BACKGROUND_HEIGHT = 1080;
	/** 「音量」・「ゲームに戻る」・「ゲームをやめる」*/
	const int BUTTON_WIDTH = 300.0f;
	const int BUTTON_HEIGHT= 200.0f;
	/** 「音量」黒い背景*/
	const int VOLUMEBLACK_WIDTH = 470.0f;
	const int VOLUMEBLACK_HEIGHT = 370.0f;
	
}

bool Pause::Start()
{
	/** それぞれの画像を初期化*/
	/** 背景*/
	m_pauseBackSpriteRender.Init(PAUSE_BACKGROUND_PATH, BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
	m_pauseBackSpriteRender.Update();

	/** 「ゲームをやめる」*/
	m_quitGameSpriteRender.Init(PAUSE_QUITE_PATH, BUTTON_WIDTH, BUTTON_HEIGHT);
	m_quitGameSpriteRender.SetPosition(QUITEPOS);
	m_quitGameSpriteRender.Update();

	/** 「ゲームに戻る」*/
	m_returnGameSpriteRender.Init(PAUSE_RETURN_PATH, BUTTON_WIDTH, BUTTON_HEIGHT);
	m_returnGameSpriteRender.SetPosition(RETURNPOS);
	m_returnGameSpriteRender.Update();
	/** 音量*/
	m_volumeSpriteRender.Init(PAUSE_VOLUME_PATH, BUTTON_WIDTH, BUTTON_HEIGHT);
	m_volumeSpriteRender.SetPosition(SOUNDPOS);
	m_volumeSpriteRender.Update();

	/** ゲームの情報を取得*/
	m_game = FindGO<Game>("game");
	m_choiceSound = FindGO<SoundManager>("soundmanager");
	m_isPause = false;
	return true;
}


void Pause::Update()
{
	if (!m_isPause)
	{
		if (g_pad[0]->IsTrigger(enButtonStart))

		{
			Open();
		}
		return;
	}

	/** 上下の入力で選択している場所の番号を変える*/
	if (g_pad[0]->IsTrigger(enButtonDown))
	{
		m_countNumber++;
		p_chiceSE = m_choiceSound->PlayingSE(SoundSE::enChoiceSE, false);
	}
	if (g_pad[0]->IsTrigger(enButtonUp))
	{
		m_countNumber--;
		p_chiceSE = m_choiceSound->PlayingSE(SoundSE::enChoiceSE, false);
	}
	/** 選択*/
	Choice();
	/** 決定*/
	Select();
}


void Pause::Choice()
{
	/** 範囲外に行ったら一番下に戻る*/
	if (m_countNumber == -1)
	{
		m_countNumber = 2;
	}

	/** 範囲外に行ったら一番上に戻る*/
	if (m_countNumber >= 3)
	{
		m_countNumber = 0;
	}

}


void Pause::Select()
{
	/** 決定ボタンが押された場合の処理*/
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		/** 選択している場所の番号が0だったら*/
		if (m_countNumber == 0)
		{
			/** ゲームに戻る*/
			Close();
			return;

		}
		/**選択している場所の番号が1だったら*/
		else if (m_countNumber == 1)
		{
			/** タイトルに戻る*/
			NewGO<Title>(0, "title");
			DeleteGO(m_game);
			return;
		}

		/** 選択している場所の番号が2の時は音量設定画面に行く*/
		else if (m_countNumber == 2)
		{
			Deactivate();

			/** ポーズ画面の音量設定クラスを生成 */
			m_soundPause = NewGO<SoundPause>(0, "soundpause");
			
		}
	}
}

void Pause::Open()
{
	/** Pause中はフラグをtrueにしてm_countNumberを初期化する*/
	m_isPause = true;
	m_countNumber = 0;
}

void Pause::Close()
{
	m_isPause = false;
}

void Pause::Render(RenderContext& rc)
{

	if (!m_isPause)
	{
		return;
	}
	/** 選択中は明るく、それ以外は暗くする */
	const Vector4 SELECT_COLOR(1.0f, 1.0f, 1.0f, 1.0f);
	const Vector4 NOT_SELECT_COLOR(0.5f, 0.5f, 0.5f, 1.0f);

	m_returnGameSpriteRender.SetMulColor(m_countNumber == 0 ? SELECT_COLOR : NOT_SELECT_COLOR);
	m_quitGameSpriteRender.SetMulColor(m_countNumber == 1 ? SELECT_COLOR : NOT_SELECT_COLOR);
	m_volumeSpriteRender.SetMulColor(m_countNumber == 2 ? SELECT_COLOR : NOT_SELECT_COLOR);

	m_pauseBackSpriteRender.Draw(rc);
	m_returnGameSpriteRender.Draw(rc);
	m_quitGameSpriteRender.Draw(rc);
	m_volumeSpriteRender.Draw(rc);
	}


