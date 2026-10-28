#include "stdafx.h"
#include "CleanRate.h"
#include"Source/Actor/Stage/TrashManager.h"
#include"Pause/Pause.h"
namespace
{
	
	/** 数字の画像をロード*/
	const char* DIGIT_PATHS[10] =
	{
		"Assets/Sprite/CleanRate/Number/Number_Zero.dds",
		"Assets/Sprite/CleanRate/Number/Number_One.dds",
		"Assets/Sprite/CleanRate/Number/Number_Two.dds",
		"Assets/Sprite/CleanRate/Number/Number_Three.dds",
		"Assets/Sprite/CleanRate/Number/Number_Four.dds",
		"Assets/Sprite/CleanRate/Number/Number_Five.dds",
		"Assets/Sprite/CleanRate/Number/Number_Six.dds",
		"Assets/Sprite/CleanRate/Number/Number_Seven.dds",
		"Assets/Sprite/CleanRate/Number/Number_Eight.dds",
		"Assets/Sprite/CleanRate/Number/Number_Nine.dds",
	};

	/** 円形ゲージ用の背景ファイルパス*/
	const char* CIRCLE_BACKGROUND_FILEPATH = "Assets/Sprite/CleanRate/Circular_BackGround_Square.dds";
	/** 円形ゲージ用のファイルパス*/
	const char* CIRCLE_WEDGE_FILEPATH = "Assets/Sprite/CleanRate/Circular_Wedge.dds";
	/** 掃除機のアイコン*/
	const char* VACCUMCLEANAR_FILEPATH = "Assets/Sprite/CleanRate/VaccumCleaner_Image.dds";
	/** 「%」を表す画像ファイルパス*/
	const char* PERCENTRATE_FILEPATH = "Assets/Sprite/CleanRate/Rate.dds";
	/** 「達成率」を表す画像ファイルパス*/
	const char* ACHIEVEMENTRATE_FILEPATH = "Assets/Sprite/CleanRate/AchievementRate.dds";

	/** 円の大きさ*/
	constexpr float CIRCLE_SIZE = 200.0f;
	/** 中心位置*/
	const Vector3 CIRCLE_POS = {-860.0f, 440.0f, 0.0f};
	/** 掃除機アイコン(円の上半分)*/
	constexpr float ICON_W = 150.0f;
	constexpr float ICON_H = 100.0f;
	const Vector3 ICON_POS(-860.0f, 490.0f, 0.0f);

	/** 「数字」*/
	const Vector3 ONES_POS(-860.0f, 390.0f, 0.0f);
	/** 横幅*/
	constexpr float DIGIT_W = 70.0f;
	/** 縦幅*/
	constexpr float DIGIT_H = 70.0f;
	/** 間隔*/
	constexpr float DIGIT_SPACING = 25.0f;

	/** 「％」*/
	const Vector3 PERCENT_POS = { -840.0f,390.0f,0.0f };
	constexpr float PERCENT_SIZE_W = 50.0f;
	constexpr float PERCENT_SIZE_H = 50.0f;

	/** 「達成率」*/
	const Vector3 ACHIEVEMENT_POS(-860.0f, 440.0f, 0.0f);
	constexpr float ACHIEVEMENT_W = 150.0f;
	constexpr float ACHIEVEMENT_H = 100.0f;
	/** 表示値が目標に追いつく速さ*/
	constexpr float FOLLOW_SPEED = 3.0f;
	
}
CleanRate::CleanRate()
{

}

CleanRate::~CleanRate()
{

}

bool CleanRate::Start()
{
	/** トラッシュマネージャーの情報を取得*/
	m_trashManager = FindGO<TrashManager>("trashmanager");
	/** ポーズの情報を取得*/
	m_pause = FindGO<Pause>("pause");

	/** 円形ゲージの土台*/
	m_circleBackGroundSpriteRender.Init(CIRCLE_BACKGROUND_FILEPATH, CIRCLE_SIZE, CIRCLE_SIZE);
	m_circleBackGroundSpriteRender.SetPosition(CIRCLE_POS);
	m_circleBackGroundSpriteRender.Update();

	/** 円形ゲージの塗り(扇形を回転させて100枚並べる)*/
	for (int i = 0; i < GAUGE_SEGMENT; i++)
	{
		m_gaugeSegmentSpriteRender[i].Init(CIRCLE_WEDGE_FILEPATH, CIRCLE_SIZE, CIRCLE_SIZE);
		m_gaugeSegmentSpriteRender[i].SetPosition(CIRCLE_POS);
		/** プラスだと反時計回りになってしまうためマイナスをつけて時計回りにする*/
		Quaternion rot;
		rot.SetRotationDegZ(-360.0f / GAUGE_SEGMENT * i);
		m_gaugeSegmentSpriteRender[i].SetRotation(rot);
		m_gaugeSegmentSpriteRender[i].Update();
	}
		/** 掃除機のアイコン*/
		m_vaccumClenerIconSpriteRender.Init(VACCUMCLEANAR_FILEPATH, ICON_W, ICON_H);
		m_vaccumClenerIconSpriteRender.SetPosition(ICON_POS);
		m_vaccumClenerIconSpriteRender.Update();

		/** 達成率*/
		m_achievementSpriteRender.Init(ACHIEVEMENTRATE_FILEPATH, ACHIEVEMENT_W, ACHIEVEMENT_H);
		m_achievementSpriteRender.SetPosition(ACHIEVEMENT_POS);
		m_achievementSpriteRender.Update();
		/** 「%」画像の初期化*/
		m_percentSpriteRender.Init(PERCENTRATE_FILEPATH, PERCENT_SIZE_W, PERCENT_SIZE_H);
		m_percentSpriteRender.SetPosition(PERCENT_POS);
		m_percentSpriteRender.Update();

		// ---- 数字(3桁×10種類) ----
		for (int place = 0; place < DIGIT_PLACE; place++)
		{
			for (int n = 0; n < DIGIT_NUM; n++)
			{
				m_numberSpriteRender[place][n].Init(DIGIT_PATHS[n], DIGIT_W, DIGIT_H);
				Vector3 pos = ONES_POS;
				/** 左ヘ並べる*/
				pos.x -= DIGIT_SPACING * place;
				m_numberSpriteRender[place][n].SetPosition(pos);
				m_numberSpriteRender[place][n].Update();
			}
		}

		return true;
	}


void CleanRate::Update()
{
	/** ポーズ中は処理を止める*/
	if (m_pause->GetIsPause())
	{
		return;
	}

	if (m_trashManager == nullptr)
	{
		return;
	}
	
	/** 目標に向かって滑らかに近づける*/
	const float target = m_trashManager->GetCleanRate();
	/** 同じ速さでゲージを動かすためにフレーム数によって動かす*/
	const float dt = g_gameTime->GetFrameDeltaTime();
	/** ここで残りが大きい時は早く、近づくほどゆっくりになるようにする*/
	m_displayRate += (target - m_displayRate) * min(1.0f, FOLLOW_SPEED * dt);

	/** 0～1に収める*/
	const float rate = max(0.0f, min(1.0f, m_displayRate));
	m_gaugeRate = rate;

	/** 数字は実際の値(整数%)*/
	m_displayPercent = static_cast<int>(m_displayRate * 100.0f + 0.5f);
}


void CleanRate::Render(RenderContext& rc)
{
	/** ポーズ中は描画もしない*/
	if (m_pause->GetIsPause())
	{
		return;
	}
	/** 奥から手前の順に描画させる*/
	/** 背景*/
	m_circleBackGroundSpriteRender.Draw(rc);
	/** 塗りの部分*/
	/** m_gaugeRateに応じた分だけ描画させる*/
	/**m_gaugeRateが0ならdrawCountも0にする */
	const int drawCount = static_cast<int>(m_gaugeRate * GAUGE_SEGMENT + 0.5f);
	for (int i = 0; i < drawCount; i++)
	{
		m_gaugeSegmentSpriteRender[i].Draw(rc);
	}
	/** 掃除機のアイコン*/
	m_vaccumClenerIconSpriteRender.Draw(rc);
	/** 達成率*/
	m_achievementSpriteRender.Draw(rc);
	/**％*/
	m_percentSpriteRender.Draw(rc);

	/**一の位は常に表示 */ 
	m_numberSpriteRender[0][m_displayPercent % 10].Draw(rc);
	/**十の位は10%以上になったら表示*/ 
	if (m_displayPercent >= 10)
	{
		m_numberSpriteRender[1][(m_displayPercent / 10) % 10].Draw(rc);
	}
	/**百の位は100%のみ表示する */
	if (m_displayPercent >= 100)
	{
		m_numberSpriteRender[2][(m_displayPercent / 100) % 10].Draw(rc);
	}
}