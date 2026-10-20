#pragma once
class Game;
class SoundPause;
class SoundManager;
class Pause : public IGameObject
{
public:
	Pause() {};
	~Pause() {};
	bool Start();
	void Update();
	void Render(RenderContext& rc);

public:
	/** ポーズ中に選んでいる場所の番号を設定する関数*/
	void SetCountNumber(int countNumber)
	{
		m_countNumber = countNumber;
	}

	/** ポーズ中に選んでいる場所の番号を取得する関数*/
	 int GetCountNumber()const
	{
		return m_countNumber;
	}


	/** ポーズ中かどうかのフラグ設定関数*/
	void SetIsPause(bool flag)
	{
		m_isPause = flag;
	}

	/** ポーズ中かどうかのフラグを取得する関数*/
	 bool GetIsPause()const
	{
		return m_isPause;
	}

private:
	/** 選択する時の処理*/
	void Select();
	/** 選択された後の処理*/
	void Choice();

private:
	/** 選択中の画面を表示させる番号*/
	int m_countNumber = 0;

	/** Pause中に使用する画像一覧*/
	/** Paus背景*/
	SpriteRender m_pauseBackSpriteRender;

	/**「ゲームをやめる」を表示するアイコン*/
	SpriteRender m_quitGameSpriteRender;

	/** 「ゲームに戻る」を表示するアイコン*/
	SpriteRender m_returnGameSpriteRender;

	/** 「音量」を表示するアイコン*/
	SpriteRender m_volumeSpriteRender;

	/** 選択していないときに出す黒い背景*/
	SpriteRender m_quitGameBlackSpriteRender;
	SpriteRender m_returnGameBlackSpriteRender;
	SpriteRender m_volumeBlackSpriteRender;
	
	/** ゲーム*/
	Game* m_game = nullptr;

	/**	サウンドポーズ*/
	SoundPause* m_soundPause = nullptr;
	/**サウンドマネージャー*/
	SoundManager* m_choiceSound = nullptr;

	SoundSource* p_chiceSE = nullptr;
	SoundSource* p_DecisionSE = nullptr;

	/** ポーズ中かどうかのフラグ*/
	bool m_isPause = false;
};

