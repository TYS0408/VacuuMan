#include "stdafx.h"
#include "GameTimer.h"
#include"Source/Actor/Stage/TrashManager.h"
#include"GameScene/GameClear.h"
#include"GameScene/GameOver.h"
namespace
{
	/** 数字の画像をロード*/
	const char* m_digitPaths[10] =
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

	/** 三桁を描画する画像の大きさ*/
	const Vector3 DIGIT_SCALE = { 1.0f,1.0f, 1.0f };

	/** ゲームクリアに必要な達成率*/
	constexpr float CLEAR_CLEAN_RATE = 0.8f;
}


GameTimer::GameTimer()
{

}


GameTimer::~GameTimer()
{

}

bool GameTimer::Start()
{
	/** ここでトラッシュマネージャーの情報を取得する*/
	m_trashManager = FindGO<TrashManager>("trashmanager");

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			m_digitSpriteRender[i][j].Init(m_digitPaths[j], 150, 150);
			m_digitSpriteRender[i][j].SetScale({ 0.0f, 0.0f, 0.0f }); // 最初は非表示
		}
		
	}
	return true;
}


void GameTimer::Update()
{
	/** タイムアップ時で判定していたらタイマーを進めない*/
	if (m_isTimeUpChecked)
	{
		return;
	}

	TextTimer();

	/** 全ての画像スプライト(30個)を更新 */
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			m_digitSpriteRender[i][j].Update();
		}
	}

	/** 時間切れになったらCheckTimeUpを呼ぶ*/
	if (m_timer <= 0.0f)
	{
		CheckTimeUp();
	}
}

void GameTimer::TextTimer()
{
	m_timer -= g_gameTime->GetFrameDeltaTime();

	/** 小数点の切り捨て*/
	int time = (int)m_timer;

	/**0以下にならないようにする*/
	if (time < 0)
	{
		time = 0;
	}

	/** 桁分解*/
		/** (例)123÷100 = 1 */
	int hundreds = time / 100;

	/** (例)(123÷10) %10 = 12 %10 =1あまり2 */
	int tens = (time / 10) % 10;

	/** (例)123÷10 = 12あまり3 */
	int ones = time % 10;

	/** 秒数が変わっていなければ何もしない*/
	if (time != m_prevTime)
	{
		m_prevTime = time;
		/** 全て非表示*/
		/** 全部非表示 */
		for (int i = 0; i < 10; i++)
		{
			m_digitSpriteRender[0][i].SetScale({ 0,0,0, });
			m_digitSpriteRender[1][i].SetScale({ 0,0,0, });
			m_digitSpriteRender[2][i].SetScale({ 0,0,0, });
		}


			
			/** タイマーの表示*/
			m_digitSpriteRender[0][hundreds].SetScale(DIGIT_SCALE);
			m_digitSpriteRender[1][tens].SetScale(DIGIT_SCALE);
			m_digitSpriteRender[2][ones].SetScale(DIGIT_SCALE);


			/** 位置設定 */
			m_digitSpriteRender[0][hundreds].SetPosition({ -50.0f,460.0f,0.0f });
			m_digitSpriteRender[1][tens].SetPosition({ 0.0f,460.0f,0.0f });
			m_digitSpriteRender[2][ones].SetPosition({ 50.0f,460.0f,0.0f });
	}
}

void GameTimer::CheckTimeUp()
{
	/** タイムアップフラグをtrueにする*/
	m_isTimeUpChecked = true;
	/** トラッシュマネージャーが存在していないと判定できないので
	/*存在していない場合はこの下の処理を飛ばす。*/
	if (m_trashManager == nullptr)
	{
		return;
	}
	/** 部屋の綺麗さが80%以上だったら*/
	if (m_trashManager->GetCleanRate() >= CLEAR_CLEAN_RATE)
	{
		/** ゲームクリアを生成*/
		NewGO<GameClear>(0, "gameclear");
	}

	else
	{
		/** 80%以下だったらゲームオーバー*/
		NewGO<GameOver>(0, "gameover");
	}

}


void GameTimer::Render(RenderContext& rc)
{
	/** 制限時間の描画*/
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			m_digitSpriteRender[i][j].Draw(rc);
		}
	}
}