#pragma once
class Pause;
class Score : public IGameObject
{
public:
	Score() {};
	~Score() {};

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/** スコアを加算*/
	void AddScore(const int value);
	/** スコアを取得*/
	int GetScore()const
	{
		return m_score;
	}
	/** スコアをリセットする*/
	void ResetScore();

private:
	/** 桁数*/
	static const int DIGIT_PLACE = 5;
	/** 数字の種類*/
	static const int DIGIT_NUM = 10;
	/** 表示できる最大値*/
	static const int  MAX_SCORE = 99999;
	/** Pause*/
	Pause* m_pause = nullptr;
	/** 実際のスコア*/
	int m_score = 0;
	/** 画面に表示している値(カウントアップ用)*/
	float m_displayScore = 0.0f;
	/** 表示用の整数*/
	int m_displayInt = 0;
	/** スコアアイコン*/
	SpriteRender m_iconSpriteRender;
	/** スコア数字*/
	SpriteRender m_numberSpriteRender[DIGIT_PLACE][DIGIT_NUM];

};

