#pragma once
class TrashManager;
class Pause;


class CleanRate : public IGameObject
{
	public:
	CleanRate();
	~CleanRate();


	bool Start();
	void Update();
	void Render(RenderContext& rc);


private:
	/** 0～９*/
	static const int DIGIT_NUM = 10;
	/** 百・十・一*/
	static const int DIGIT_PLACE = 3;

	/** ゲージの分割数(1％刻み)*/
	static constexpr int GAUGE_SEGMENT = 100;
	/** 円形ゲージの土台*/
	SpriteRender m_circleBackGroundSpriteRender;
	/** 円形ゲージ*/
	SpriteRender m_gaugeSegmentSpriteRender[GAUGE_SEGMENT];

	/** 掃除機のアイコン*/
	SpriteRender m_vaccumClenerIconSpriteRender;
	/** 「達成率」を表す文字の画像*/
	SpriteRender m_achievementSpriteRender;
	/** 「%」を表す画像*/
	SpriteRender m_percentSpriteRender;
	/** 「桁」「数字」の順番で持つ*/
	SpriteRender m_numberSpriteRender[DIGIT_PLACE][DIGIT_NUM];
	/** トラッシュマネージャー*/
	TrashManager* m_trashManager = nullptr;
	/** ポーズ*/
	Pause* m_pause = nullptr;
	/** 表示中の%*/
	 int m_displayPercent = 0;
	 /** 表示中の割合*/
	 float m_displayRate = 0.0f;

	 float m_gaugeRate = 0.0f;
};

