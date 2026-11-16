#include "stdafx.h"
#include "Score.h"
#include"Pause/Pause.h"
namespace
{
	/** 数字の画像ファイルパス*/
	const char* DIGIT_PATHS[10] =
	{
		"Assets/Sprite/Score/ScoreNumber_Zero.dds",
		"Assets/Sprite/Score/ScoreNumber_One.dds",
		"Assets/Sprite/Score/ScoreNumber_Two.dds",
		"Assets/Sprite/Score/ScoreNumber_Three.dds",
		"Assets/Sprite/Score/ScoreNumber_Four.dds",
		"Assets/Sprite/Score/ScoreNumber_Five.dds",
		"Assets/Sprite/Score/ScoreNumber_Six.dds",
		"Assets/Sprite/Score/ScoreNumber_Seven.dds",
		"Assets/Sprite/Score/ScoreNumber_Eight.dds",
		"Assets/Sprite/Score/ScoreNumber_Nine.dds",
	};

	/** SCOREアイコン*/
	const char* ICON_FILEPATH = "Assets/Sprite/Score/ScoreIcon.dds";

	/** アイコン(横長)*/
	constexpr float ICON_W = 400.0f;
	/** アイコン(縦幅)*/
	constexpr float ICON_H = 100.0f;
	/** 位置*/
	const Vector3 ICON_POS= Vector3(-770.0f, -450.0f, 0.0f);

	/** 数字の大きさ*/
	constexpr float DIGIT_W = 100.0f;
	constexpr float DIGIT_H = 100.0f;
	/** 数字の間隔*/
	constexpr float DIGIT_SPACING = 50.0f;
	/** 一の位の位置(ここから左へ並べる)*/
	const Vector3 ONES_POS =Vector3(-370.0f, -450.0f, 0.0f);

	/** 表示値が目標に追いつく速さ*/
	constexpr float FOLLOW_SPEED = 4.0f;
}

bool Score::Start()
{
	m_pause = FindGO<Pause>("pause");
	/** アイコン*/
	m_iconSpriteRender.Init(ICON_FILEPATH, ICON_W, ICON_H);
	m_iconSpriteRender.SetPosition(ICON_POS);
	m_iconSpriteRender.Update();
	
	/** 数字を並べる*/
	for (int place = 0; place < DIGIT_PLACE; place++)
	{
		for (int number = 0; number < DIGIT_NUM; number++)
		{
			/** 数字を読み込む*/
			m_numberSpriteRender[place][number].Init(DIGIT_PATHS[number], DIGIT_W, DIGIT_H);
			Vector3 pos = ONES_POS;
			/** 左に並べる*/
			pos.x -= DIGIT_SPACING * place;
			m_numberSpriteRender[place][number].SetPosition(pos);
			m_numberSpriteRender[place][number].Update();
		}
	}
	return true;
}


void Score::AddScore(const int value)
{
	m_score += value;
	if (m_score > MAX_SCORE)
	{
		m_score = MAX_SCORE;
	}
}

void Score::ResetScore()
{
	m_score = 0;
	m_displayScore = 0.0f;
	m_displayInt = 0;
}

void Score::Update()
{
	/** ポーズ中は処理を止める*/
	if (m_pause != nullptr && m_pause->GetIsPause())
	{
		return;
	}


	/** 目標に向かってカウントアップ*/
	const float dt = g_gameTime->GetFrameDeltaTime();
	m_displayScore += (static_cast<float>(m_score) - m_displayScore) * min(1.0f, FOLLOW_SPEED * dt);

	/** 近づきすぎて止まらないよう、差が小さければ一致させる*/
	if (fabsf(m_score - m_displayScore) < 0.5f)
	{
		m_displayScore = static_cast<float>(m_score);
	}
	m_displayInt = static_cast<int>(m_displayScore + 0.5f);


	
}
void Score::Render(RenderContext& rc)
{
	/** ポーズ中は描画を止める*/
	if (m_pause != nullptr && m_pause->GetIsPause())
	{
		return;
	}
	m_iconSpriteRender.Draw(rc);

	/** 全桁は常に描画。各桁の数字は表示値から取り出す*/
	int value = m_displayInt;
	for (int place = 0; place < DIGIT_PLACE; place++)
	{
		/**一の位から順に一つ取り出してその数字スプライトを描画*/
		const int digit = value % 10;
		m_numberSpriteRender[place][digit].Draw(rc);
		value /= 10;
	}

}