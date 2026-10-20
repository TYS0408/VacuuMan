#pragma once
class Pause;
class SoundManager;
class SoundPause : public IGameObject
{
public:
	SoundPause() {};
	~SoundPause() {};

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/** 選択番号設定*/
	void SetCount(int count)
	{
		m_Count = count;
	}

private:
	/** どこにカーソルがあるかを判断する関数*/
	void ButtonCount();

	void barBGM();

	void barSE();

	//void barVoice();

	void UpdateBGMUI();

	void UpdateSEUI();

	//void UpdateVoiceUI();


private:
	/**どこを選択しているかを表すカウント変数*/
	int m_Count = 0;

	/** 音量の段階数*/
	static const int MAX_STEP = 10;

	/** BGM・SEの現在の段階 */
	int m_bgmStep = 3;
	int m_seStep = 8;
	

	/** Voiceの音量*/
	float m_voiceVolume = 0.8f;

	SpriteRender m_bgmTickSpriteRenders[MAX_STEP + 1];
	SpriteRender m_seTickSpriteRenders[MAX_STEP + 1];
	/** 段階→X座標*/
	float StepToPosX(int step)const;

	/** Pasue画面に出てくる背景*/
	SpriteRender m_backGroundSpriteRender;

	/** BGMやSEを調整する画面の外枠*/
	SpriteRender m_settingSpriteRender;

	/** 音量アイコン*/
	SpriteRender m_bgmvolumeSpriteRender;

	/** 音量が出ていないときのアイコン*/
	SpriteRender m_bgmNotVolumeSpriteRender;

	/** 「BGM」とテキストで表示するアイコン*/
	SpriteRender m_bgmSpriteRender;

	/**「SE」とテキストで表示するアイコン*/
	SpriteRender m_seSpriteRender;

	/**SEの音量アイコン */
	SpriteRender m_seVolumeSpriteRender;

	/** SEで音量が出ていないことを表すアイコン*/
	SpriteRender m_seNotVolumeSpriteRender;

	/** Voiceの音量アイコン */
	SpriteRender m_voiceVolumeSpriteRender;

	/** Voiceの音量が出ていないことを表すアイコン*/
	SpriteRender m_voiceNotVolumeSpriteRender;

	/** 「もどる」を文字で表すアイコン*/
	SpriteRender m_buttonReturnSpriteRender;

	/** BGMの調整をするための茶色い丸いスライドバー*/
	SpriteRender m_bgmIconSpriteRender;

	/** SEの調整をするための黄色い丸いスライドバー*/
	SpriteRender m_seIconSpriteRender;

	/** Voiceの調整をするためのミルク色の丸いスライドバー*/
	SpriteRender m_voiceIconSpriteRender;

	/** BGM・SE・VoiceUIを調整するための細長いバー*/
	SpriteRender m_bgmBlownBarSpriteRender;
	SpriteRender m_seBlownBarSpriteRender;
	SpriteRender m_voiceBlownBarSpriteRender;

	/** 丸いスライドバーが選択されていないときに出すスライドバー*/
	SpriteRender m_bgmBlackIcon;
	SpriteRender m_seBlackIcon;
	SpriteRender m_voiceBlackIcon;

	/** 選ばれていないときに出す黒い画像*/
	SpriteRender m_blackSpriteRender;

	/** 灰色のバー */
	SpriteRender m_grayBarSpriteRender;

	/** VoiceUIの画像 */
	SpriteRender m_voiceUISpriteRender;

	/** サウンドマネージャーのポインタ*/
	SoundManager* m_choiceSound = nullptr;
	/** 選択する時のSEのサウンドソースのポインタ*/
	SoundSource* p_chiceSE = nullptr;
	/** 決定する時のSEのサウンドソースのポインタ*/
	SoundSource* p_decisionSE = nullptr;

	/** サウンドマネージャーのポインタ */
	SoundManager* m_soundManager = nullptr;

	/** ゲームのポインタ*/
	Pause* m_pause = nullptr;

};

