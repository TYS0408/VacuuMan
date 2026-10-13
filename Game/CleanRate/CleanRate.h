#pragma once
class TrashManager;
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

	
	/** 背景の白い画像*/
	SpriteRender m_WhiteSpriteRender;	
	/** 「現在の清潔度」を表す文字の画像*/
	SpriteRender m_currentCleanSpriteRender;
	/** 「達成率」を表す文字の画像*/
	SpriteRender m_achievementSpriteRender;
	/** 紺色バー(黄色の後ろに出す)画像*/
	SpriteRender m_barBackSpriteRender;
	/** 黄色バー(紺色バーの上に出す)画像*/
	SpriteRender m_barFillSpriteRender;
	/** 「%」を表す画像*/
	SpriteRender m_percentSpriteRender;
	/** 「桁」「数字」の順番で持つ*/
	SpriteRender m_numberSpriteRender[DIGIT_PLACE][DIGIT_NUM];

	TrashManager* m_trashManager = nullptr;
	/** 表示中の%*/
	 int m_displayPercent = 0;
	 /** 表示中の割合*/
	 float m_displayRate = 0.0f;
};

