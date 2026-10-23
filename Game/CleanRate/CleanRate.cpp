#include "stdafx.h"
#include "CleanRate.h"
#include"Source/Actor/Stage/TrashManager.h"
#include"Pause/Pause.h"
namespace
{

	/** 数字の画像をロード*/
	const char* DIGIT_PATHS[10] =
	{
		"Assets/Sprite/Number/Number_Zero.dds",
		"Assets/Sprite/Number/Number_One.dds",
		"Assets/Sprite/Number/Number_Two.dds",
		"Assets/Sprite/Number/Number_Three.dds",
		"Assets/Sprite/Number/Number_Four.dds",
		"Assets/Sprite/Number/Number_Five.dds",
		"Assets/Sprite/Number/Number_Six.dds",
		"Assets/Sprite/Number/Number_Seven.dds",
		"Assets/Sprite/Number/Number_Eight.dds",
		"Assets/Sprite/Number/Number_Nine.dds",
	};

	/** 背景の白い画像ファイルパス*/
	const char* WHITE_FILEPATH = "Assets/Sprite/CleanRate/White.dds";
	/** パラメーターで後ろに出す画像ファイルパス*/
	const char* BACKBAR_FILEPATH = "Assets/Sprite/CleanRate/DarkBlue_Bar.dds";
	/** パラメーターで前に出すファイルパス*/
	const char* FILLBAR_FILEPATH = "Assets/Sprite/CleanRate/Yellow_Bar.dds";
	/** 「%」を表す画像ファイルパス*/
	const char* PERCENTRATE_FILEPATH = "Assets/Sprite/CleanRate/Rate.dds";
	/** 「現在の清潔度」を表す画像ファイルパス*/
	const char* CURRENTCLEAN_FILEPATH = "Assets/Sprite/CleanRate/CurrentCleanRate.dds";
	/** 「達成率」を表す画像ファイルパス*/
	const char* ACHIEVEMENTRATE_FILEPATH = "Assets/Sprite/CleanRate/AchievementRate.dds";
	/** 横幅*/
	constexpr float WIDTH = 128;
	/** 縦幅*/
	constexpr float HIGHT = 128;


	/** バーの左端の位置と大きさ*/
	const Vector3 BAR_LEFT_POS(-800.0f, 420.0f, 0.0f);
	constexpr float BAR_W = 300.0f;
	constexpr float BAR_H = 50.0f;
	/** 一の位の数字の位置と、桁の間隔*/
	const Vector3 ONES_POS(-550.0f, 450.0f, 0.0f);
	constexpr float DIGIT_W = 50.0f;
	constexpr float DIGIT_H = 50.0f;
	constexpr float DIGIT_SPACING = 20.0f;
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
	/** 白い背景画像の初期化*/
	m_WhiteSpriteRender.Init(WHITE_FILEPATH, 600, 200);
	/** 「現在の清潔度」画像の初期化*/
	m_currentCleanSpriteRender.Init(CURRENTCLEAN_FILEPATH, 300, 70);
	/** 「達成率」画像の初期化*/
	m_achievementSpriteRender.Init(ACHIEVEMENTRATE_FILEPATH, 200, 150);
	/** 後ろに出すバー画像の初期化*/
	m_barBackSpriteRender.Init(BACKBAR_FILEPATH, BAR_W, BAR_H);
	/** 前に出すバー画像の初期化*/
	m_barFillSpriteRender.Init(FILLBAR_FILEPATH, BAR_W, BAR_H);
	/** 「%」画像の初期化*/
	m_percentSpriteRender.Init(PERCENTRATE_FILEPATH, 50, 50);


	/** バーは左端を基準にする(右へ伸ばすために必須)*/
	m_barBackSpriteRender.SetPivot(Vector2(0.0f, 0.5f));
	m_barBackSpriteRender.SetPosition(BAR_LEFT_POS);
	m_barFillSpriteRender.SetPivot(Vector2(0.0f, 0.5f));
	m_barFillSpriteRender.SetPosition(BAR_LEFT_POS);
	/** 最初は空にしておく*/
	m_barFillSpriteRender.SetScale(Vector3(0.0f, 1.0f, 1.0f)); 

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
	

	/** 残りのパーツの位置*/
	m_WhiteSpriteRender.SetPosition(Vector3(-730.0f, 465.0f, 0.0f));
	m_currentCleanSpriteRender.SetPosition(Vector3(-800.0f, 510.0f, 0.0f));
	m_achievementSpriteRender.SetPosition(Vector3(-880.0f, 445.0f, 0.0f));
	m_percentSpriteRender.SetPosition(Vector3(-530.0f, 450.0f, 0.0f));

	m_WhiteSpriteRender.Update();
	m_currentCleanSpriteRender.Update();
	m_achievementSpriteRender.Update();
	m_barBackSpriteRender.Update();
	m_barFillSpriteRender.Update();
	m_percentSpriteRender.Update();

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
	// 目標値に向かって滑らかに近づける
	/** 清潔度を反映させる*/
	const float target = m_trashManager->GetCleanRate();
	const float dt = g_gameTime->GetFrameDeltaTime();
	/**(target - m_displayRate)  = 目標まであとどれくらい残っているか*/
	/**OLLOW_SPEED * dt → 1フレームで差の何割を埋めるか */
	m_displayRate += (target - m_displayRate) * min(1.0f, FOLLOW_SPEED * dt);


	/** 0〜1に収める*/
	const float rate = max(0.0f, min(1.0f, m_displayRate));


	/** スケールをかける*/
	m_barFillSpriteRender.SetScale(Vector3(rate, 1.0f, 1.0f));
	/** バーを右へ伸ばす*/ 
	m_barFillSpriteRender.SetScale(Vector3(rate, 1.0f, 1.0f));
	m_barFillSpriteRender.Update();
	/** 小数を「四捨五入して整数%」に変換する*/
	/**数字は実際の値(整数%)*/
	m_displayPercent = static_cast<int>(target * 100.0f + 0.5f);
}


void CleanRate::Render(RenderContext& rc)
{
	/** ポーズ中は描画もしない*/
	if (m_pause->GetIsPause())
	{
		return;
	}
	m_WhiteSpriteRender.Draw(rc);
	m_currentCleanSpriteRender.Draw(rc);
	m_achievementSpriteRender.Draw(rc);
	m_barBackSpriteRender.Draw(rc);
	m_barFillSpriteRender.Draw(rc);
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