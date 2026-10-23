#include "stdafx.h"
#include "SoundPause.h"
#include"Pause.h"
#include"SoundManager/SoundManager.h"

namespace
{
	/** それぞれの画像ファイルパス*/
	/** 背景*/
	const char* BACKGROUND_PATH = "Assets/Sprite/Pause/BackGround.dds";
	
	/**ボタン */
	const char* BUTTONRETUNR_PATH = "Assets/Sprite/Pause/ReturnButton.dds";

	/** ボリューム*/
	const char* VOLUME_PATH = "Assets/Sprite/Pause/volume.dds";

	/** ボリューム無し*/
	const char* NOTVOLUME_PATH = "Assets/Sprite/Pause/notVolume.dds";

	/** BGM*/
	const char* BGM_PATH = "Assets/Sprite/Pause/BGMFont.dds";

	/** SE*/
	const char* SE_PATH = "Assets/Sprite/Pause/SEFont.dds";

	/** つまみアイコン*/
	const char* KNOB_PATH = "Assets/Sprite/Pause/Knob.dds";
	/** SE・BGMの大きさ*/
	const Vector3 FONT_SCALE = { 0.5f,0.5f,0.5f };
	/** 目盛りの画像*/
	const char* TICK_PATH = "Assets/Sprite/Pause/Tick.dds";

	/** それぞれの位置調整*/
	/** ゲームに戻る*/
	const Vector3 RETURNPOS = { 0.0f,-380.0f,0.0f };
	/** SoundPauseバー*/
	const Vector3 SOUNDPAUSE_POS = { -50.0f,-10.0f,0.0f };
	/** BGM音量アイコン*/
	const Vector3 VOLUMEPOS = { 450.0f,180.0f,0.0f };
	/** 音量が出ていない時の位置*/
	const Vector3 NOT_VOLUMEPOS = { -450.0f,180.0f,0.0f };
	/**SE音量アイコン*/
	const Vector3 SE_VOLUMEPOS = { 450.0f,0.0f,0.0f };
	/** SEが出ていないときのアイコン*/
	const Vector3 SE_NOT_VOLUMEPOS = { -450.0f,0.0f,0.0f };
	/** BGM*/
	const Vector3 BGMPOS = { 0.0f,270.0f,0.0f };
	/** SE*/
	const Vector3 SEPOS = { 0.0f,90.0f,0.0f };
	/** つまみBGMアイコン*/
	const Vector3 BGM_ICONPOS = { -370.0f,150.0f,0.0f };
	/** つまみSEアイコン*/
	const Vector3 SE_ICONPOS = { -370.0f,0.0f,0.0f };
	/** BGMのバー*/
	const Vector3 BGM_BLOWNBARPOS = { -400.0f,180.0f,0.0f };
	/** SEのバー*/
	const Vector3 SE_BLOWNBARPOS = { -400.0f, 5.0f,0.0f };	
	/** バーの大きさ*/
	const Vector3 BLOWNBARINITIALSCALE = { 0.0f,0.0f,0.0f };

    /** それぞれの大きさ*/
	/** 背景*/
	constexpr float BACKGROUND_WIDTH = 1980.;
	constexpr float BACKGROUND_HEIGHT = 1080;

	/** ボタン*/
	constexpr float BUTTON_WIDTH = 450.0f;
	constexpr float BUTTON_HEIGHT = 200.0f;

	/** ボリューム*/
	constexpr float VOLUME_WIDTH = 230.0f;
	constexpr float VOLUME_HEIGHT = 230.0f;

	/** ボリューム無し*/
	constexpr float NOTVOLUME_WIDTH = 245.0f;
	constexpr float NOTVOLUME_HEIGHT = 245.0f;

	/** BGM・SE*/
	constexpr float BGM_SE_WIDTH = 250.0f;
	constexpr float BGM_SE_HEIGHT = 250.0f;

	/** つまみアイコン*/
	constexpr float KNOB_WIDTH = 200.0f;
	constexpr float KNOB_HEIGHT = 150.0f;

	/** バー*/
	constexpr float BAR_WIDTH = 800.0f;
	constexpr float BAR_HEIGHT = 480.0f;

	/** 目盛り*/
	constexpr float TICK_WIDTH = 100.0f;
	constexpr float TICK_HEIGHT = 80.0f;
	constexpr float TICK_FIRST_HEIGHT = 200.0f;
	constexpr float TICK_MIDDLE_HEIGHT = 150.0f;
	constexpr float TICK_LAST_HEIGHT = 200.0f;


	/** スライダーの左端・右端のX座標*/
	constexpr float SLIDER_LEFT_X = -370.0f;
	constexpr float SLIDER_RIGHT_X = 360.0f;

	/** BGM・SEスライダーのY座標 */
	constexpr float BGM_SLIDER_Y = 180.0f;
	constexpr float SE_SLIDER_Y = 0.0f;

	
}

bool SoundPause::Start()
{
	/** それぞれの画像を初期化*/
	m_backGroundSpriteRender.Init(BACKGROUND_PATH, BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
	m_backGroundSpriteRender.Update();
	/** 「もどる」*/
	m_buttonReturnSpriteRender.Init(BUTTONRETUNR_PATH, BUTTON_WIDTH, BUTTON_HEIGHT);
	m_buttonReturnSpriteRender.SetPosition(RETURNPOS);
	m_buttonReturnSpriteRender.Update();
	/** ボイスが出ているときのマーク(BGM)*/
	m_bgmvolumeSpriteRender.Init(VOLUME_PATH, VOLUME_WIDTH, VOLUME_HEIGHT);
	m_bgmvolumeSpriteRender.SetPosition(VOLUMEPOS);
	m_bgmvolumeSpriteRender.Update();
	/** ボイスが出ていないときのマーク(BGM)*/
	m_bgmNotVolumeSpriteRender.Init(NOTVOLUME_PATH, NOTVOLUME_WIDTH, NOTVOLUME_HEIGHT);
	m_bgmNotVolumeSpriteRender.SetPosition(NOT_VOLUMEPOS);
	m_bgmNotVolumeSpriteRender.Update();
	/** ボイスが出ていないる時のマーク(SE)*/
	m_seVolumeSpriteRender.Init(VOLUME_PATH, VOLUME_WIDTH, VOLUME_HEIGHT);
	m_seVolumeSpriteRender.SetPosition(SE_VOLUMEPOS);
	m_seVolumeSpriteRender.Update();
	/** ボイスが出ていないときのマーク(SE)*/
	m_seNotVolumeSpriteRender.Init(NOTVOLUME_PATH, NOTVOLUME_WIDTH, NOTVOLUME_HEIGHT);
	m_seNotVolumeSpriteRender.SetPosition(SE_NOT_VOLUMEPOS);
	m_seNotVolumeSpriteRender.Update();
	/** 「BGM」*/
	m_bgmSpriteRender.Init(BGM_PATH, BGM_SE_WIDTH, BGM_SE_HEIGHT);
	m_bgmSpriteRender.SetScale(FONT_SCALE);
	m_bgmSpriteRender.SetPosition(BGMPOS);
	m_bgmSpriteRender.Update();
	/** 「SE」*/
	m_seSpriteRender.Init(SE_PATH, BGM_SE_WIDTH, BGM_SE_HEIGHT);
	m_seSpriteRender.SetScale(FONT_SCALE);
	m_seSpriteRender.SetPosition(SEPOS);
	m_seSpriteRender.Update();
	/** つまみ(BGM)*/
	m_bgmIconSpriteRender.Init(KNOB_PATH, KNOB_WIDTH, KNOB_HEIGHT);
	m_bgmIconSpriteRender.SetPosition(BGM_ICONPOS);
	m_bgmIconSpriteRender.Update();
	/** つまみ(SE)*/
	m_seIconSpriteRender.Init(KNOB_PATH, KNOB_WIDTH, KNOB_HEIGHT);
	m_seIconSpriteRender.SetPosition(SE_ICONPOS);
	m_seIconSpriteRender.Update();

	
	/** 目盛りの初期化 */
	for (int i = 0; i <= MAX_STEP; i++)
	{
		float x = StepToPosX(i);

		/** 位置ごとに目盛りの高さを変えて違いを持たせる*/
		float tickHeight = TICK_HEIGHT;
		if (i == MAX_STEP)
		{
			tickHeight = TICK_LAST_HEIGHT;
		}
		else if (i == MAX_STEP / 2)
		{
			tickHeight = TICK_MIDDLE_HEIGHT;
		}
		else if (i == 0)
		{
			tickHeight = TICK_FIRST_HEIGHT;
		}
		m_bgmTickSpriteRenders[i].Init(TICK_PATH, TICK_WIDTH, tickHeight);
		m_bgmTickSpriteRenders[i].SetPosition(Vector3(x, BGM_SLIDER_Y, 0.0f));
		m_bgmTickSpriteRenders[i].Update();

		m_seTickSpriteRenders[i].Init(TICK_PATH, TICK_WIDTH, tickHeight);
		m_seTickSpriteRenders[i].SetPosition(Vector3(x, SE_SLIDER_Y, 0.0f));
		m_seTickSpriteRenders[i].Update();
	}
	

	/** pauseの情報を取得*/
	m_pause = FindGO<Pause>("pause");
	/** サウンドマネージャーの情報を取得*/
	m_choiceSound = FindGO<SoundManager>("soundmanager");
	/** SoundPauseクラスが生成されたときに
	 *  SoundManagerの音量を参考にしてBGM,SEに反映 */
	m_bgmStep = static_cast<int>(m_choiceSound->m_bgmVolume * MAX_STEP + 0.5f);
	m_seStep = static_cast<int>(m_choiceSound->m_seVolume * MAX_STEP + 0.5f);

	/** UIに反映させる*/
	UpdateBGMUI();
	UpdateSEUI();
	//UpdateVoiceUI();

	return true;
}
void SoundPause::Update()
{
	/** 今ボタンが何処を指しているかを表す関数*/
	ButtonCount();
}


void SoundPause::ButtonCount()
{
	/** 下キーで選択項目を進める*/
	if (g_pad[0]->IsTrigger(enButtonDown))
	{
		/**選択番号を減らす*/
		m_Count++;
		p_chiceSE = m_choiceSound->PlayingSE(SoundSE::enChoiceSE, false);
	}
	/** 上キーで前の項目へ */
	if (g_pad[0]->IsTrigger(enButtonUp))
	{
		m_Count--;
		p_chiceSE = m_choiceSound->PlayingSE(SoundSE::enChoiceSE, false);
	}

	/** 範囲外に行ったら一番下にループ*/
	if (m_Count < 0)
	{
		m_Count = 2;
	}

	/** 音量調整モード*/
	if (m_Count == 0)
	{
		barBGM();
	}

	else if (m_Count == 1)
	{
		barSE();
	}

	

	/** 「戻る」を選択中*/
	else if (m_Count == 2)
	{
		/** スタートボタンでポーズ画面へ戻る*/
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			/** ポーズ画面を有効化*/
			m_pause->Activate();

			/** カーソル初期化*/
			m_pause->SetCountNumber(0);
			/** この画面を無効化*/
			DeleteGO(this);
			return;
		}
	}
	/** 範囲外に行ったら先頭へループ*/
	if (m_Count == 3)
	{
		m_Count = 0;
	}

}

float SoundPause::StepToPosX(int step) const
{
	/** floatに変換して小数点以下を切り捨てられないようにする*/
	float rate = static_cast<float>(step) / MAX_STEP;
	/** 全体の距離から割合をかけて最後に左端の距離を足すことで座標を出している*/
	return SLIDER_LEFT_X + (SLIDER_RIGHT_X - SLIDER_LEFT_X) * rate;
}


void SoundPause::barBGM()
{
	/** 右キーが押されてまだMAXの段階まで行っていなかったら*/
	if (g_pad[0]->IsTrigger(enButtonRight) && m_bgmStep < MAX_STEP)
	{
		/** ステップを増やす*/
		m_bgmStep++;
	}

	/** 左キーが押されて0の段階まで行っていなかったら*/
	if (g_pad[0]->IsTrigger(enButtonLeft) && m_bgmStep > 0)
	{
		/** ステップを減らす*/
		m_bgmStep--;
	}
	/** 音量をfloatに変換してSoundManagerに反映させている*/
	m_choiceSound->m_bgmVolume = static_cast<float>(m_bgmStep) / MAX_STEP;
	/** つまみの表示を更新*/
	UpdateBGMUI();
}


void SoundPause::barSE()
{

	if (g_pad[0]->IsTrigger(enButtonRight) && m_seStep < MAX_STEP)
	{
		m_seStep++;
	}
	if (g_pad[0]->IsTrigger(enButtonLeft) && m_seStep > 0)
	{
		m_seStep--;
	}
	/** 音量をfloatに変換してSoundManagerに反映させている*/
	m_choiceSound->m_seVolume = static_cast<float>(m_seStep) / MAX_STEP;
	/** つまみの表示を更新*/
	UpdateSEUI();


	
}


void SoundPause::UpdateBGMUI()
{
	/** つまみをStepToPosXと同じ位置に持ってくる*/
	m_bgmIconSpriteRender.SetPosition(Vector3(StepToPosX(m_bgmStep), BGM_SLIDER_Y, 0.0f));
	m_bgmIconSpriteRender.Update();
}


void SoundPause::UpdateSEUI()
{
	/** つまみをStepToPosXと同じ位置に持ってくる*/
	m_seIconSpriteRender.SetPosition(Vector3(StepToPosX(m_seStep), SE_SLIDER_Y, 0.0f));
	m_seIconSpriteRender.Update();
}


void SoundPause::Render(RenderContext& rc)
{
	const Vector4 SELECT_COLOR(1.0f, 1.0f, 1.0f, 1.0f);
	const Vector4 NOT_SELECT_COLOR(0.5f, 0.5f, 0.5f, 1.0f);

	/** 背景*/
	m_backGroundSpriteRender.Draw(rc);
	
	/** ボタン*/
	/** 選択中は明るく、それ以外は暗くする */
	
	/** ２を選んでいる時は明るく、それ以外は暗くする*/
	m_buttonReturnSpriteRender.SetMulColor(m_Count == 2 ? SELECT_COLOR : NOT_SELECT_COLOR);
	m_buttonReturnSpriteRender.Draw(rc);
	/** アイコン類*/
	m_bgmvolumeSpriteRender.Draw(rc);
	m_bgmNotVolumeSpriteRender.Draw(rc);
	m_seVolumeSpriteRender.Draw(rc);
	m_seNotVolumeSpriteRender.Draw(rc);
	//m_voiceVolumeSpriteRender.Draw(rc);
	//m_voiceNotVolumeSpriteRender.Draw(rc);
	m_bgmSpriteRender.Draw(rc);
	m_seSpriteRender.Draw(rc);
	/** 目盛り描画 */
	for (int i = 0; i <= MAX_STEP; i++)
	{
		m_bgmTickSpriteRenders[i].Draw(rc);
		m_seTickSpriteRenders[i].Draw(rc);
	}

	/** BGMつまみ:常に描画。選択中だけ明るく */
	m_bgmIconSpriteRender.SetMulColor(m_Count == 0 ? SELECT_COLOR : NOT_SELECT_COLOR);
	m_bgmIconSpriteRender.Draw(rc);

	/** SEつまみ:常に描画。選択中だけ明るく */
	m_seIconSpriteRender.SetMulColor(m_Count == 1 ? SELECT_COLOR : NOT_SELECT_COLOR);
	m_seIconSpriteRender.Draw(rc);
	
	
}