#include "stdafx.h"
#include "Title.h"
#include"Game.h"
#include"SoundManager/SoundManager.h"
namespace
{
	/** タイトルの画像ファイルパス*/
	const char* GAMETITLE = "Assets/Sprite/GameScene/GameTitle.dds";
	const char* GAMEBACKGROUND = "Assets/Sprite/GameScene/GameTitle_BackGround.dds";
	const char* GAMETITLE_UI = "Assets/Sprite/GameScene/GameTitle_UI.dds";
	const char* VACUUM_SPRITE = "Assets/Sprite/GameScene/Title_Vacuum.dds";
	const char* FADE_SPRITE = "Assets/Sprite/GameScene/Black.dds";

	/** ゲームタイトルのロゴの大きさ*/
	const float GAMETITLE_WIDTH = 1000.0f;
	const float GAMETITLE_HEIGHT = 500.0f;

	/** ゲームタイトルの背景画像の大きさ(横幅、高さ) */
	const float GAMETITLEBACKGROUND_WIDTH = 1920;
	const float GAMETITLEBACKGROUND_HEIGHT = 1080;

	/** ゲームタイトルUIの大きさ*/
	const float GAMETITLEUI_WIDTH = 800.0f;
	const float  GAMETITLEUI_HEIGHT = 400.0f;

	/** フェード用画像の大きさ*/
	const float FADE_WIDTH = 1920.0f;
	const float FADE_HEIGHT = 1080.0f;
	/** タイトルの初期位置*/
	const Vector3 TITLE_POS = Vector3(0.0f, 200.0f, 0.0f);
	const Vector3 TITLEUI_POS = Vector3(10.0f, -250.0f, 0.0f);
	/** Aボタンを押す前の点滅周期*/
	constexpr float UI_CYCLE_TIME = 2.0f;
	/** Aボタンを押した直後の周期*/
	constexpr float PRESSED_CYCLE_START = 0.05f;
	/** 遷移直後の周期*/
	constexpr float PRESSED_CYCLE_END = 0.4f;
	/** 押してからゲームに遷移するまでの時間*/
	constexpr float PRESSED_DURATION = 1.5f;

	/** 画面内に出す掃除機の登場位置*/
	const float VACUUM_WIDTH = 850.0f;
	const float VACUUM_HEIGHT = 462.5f;

	/** 掃除機の登場位置と停止位置*/
	const Vector3 VACUUM_START_POS = Vector3(1450.0f, -50.0f, 0.0f);
	const Vector3 VACUUM_END_POS = Vector3(480.0f, -50.0f, 0.0f);

	/*Yは画像と画面で上下が逆になるのでマイナスをつける。*/
	const float NOZZLE_OFFSET_X = -272.5f;   // (122 - 340) * 1.25
	const float NOZZLE_OFFSET_Y = -43.75f;   // -(220 - 185) * 1.25

	/** 各演出の時間 */
	/**掃除機が登場する時間*/
	constexpr float VACUUM_IN_TIME = 0.6f;     
	/**吸い込み時間*/
	constexpr float SUCTION_TIME = 1.0f;  
	/** フェードアウト時間*/
	constexpr float FADEOUT_TIME = 0.5f;
	/** 吸い込み中の回転量(度)*/
	constexpr float SUCTION_ROT_DEG = 720.0f;
	/** 掃除機の振動の速さ*/
	constexpr float SHAKE_SPEED = 50.0f;
	/** 掃除機の振動の大きさ*/
	constexpr float SHAKE_POWER = 5.0f;
}

Title::Title()
{

}


Title::~Title()
{
	DeleteGO(m_titleBGM);
}

bool Title::Start()
{   
	/** タイトルロゴの初期化*/
	m_titleSpriteRender.Init(GAMETITLE,GAMETITLE_WIDTH , GAMETITLE_HEIGHT);
	m_titleSpriteRender.SetPosition(TITLE_POS);
	m_titleSpriteRender.Update();
	/** タイトル背景の初期化*/
	m_backGroundSpriteRemnder.Init(GAMEBACKGROUND, GAMETITLEBACKGROUND_WIDTH, GAMETITLEBACKGROUND_HEIGHT);
	m_backGroundSpriteRemnder.Update();

	/** タイトルUIの初期化*/
	m_titleUISpriteRender.Init(GAMETITLE_UI, GAMETITLEUI_WIDTH, GAMETITLEUI_HEIGHT);
	m_titleUISpriteRender.SetPosition(TITLEUI_POS);
	m_titleUISpriteRender.Update();

	/** 掃除機の初期化(最初は画面外に置く) */
	m_vacuumSpriteRender.Init(VACUUM_SPRITE, VACUUM_WIDTH, VACUUM_HEIGHT);
	m_vacuumSpriteRender.SetPosition(VACUUM_START_POS);
	m_vacuumSpriteRender.Update();

	/** フェード用の黒画像の初期化(最初は透明) */
	m_fadeSpriteRender.Init(FADE_SPRITE, FADE_WIDTH, FADE_HEIGHT);
	m_fadeSpriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
	m_fadeSpriteRender.Update();
	m_soundManager = FindGO<SoundManager>("soundmanager");
	m_titleBGM = m_soundManager->PlayingBGM(SoundBGM::enTitleBGM, true);
	return true;
}

void Title::Update()
{
	switch (m_state)
	{
	case EnTitleState::enWait:
		FadeUI();
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			ChangeState(EnTitleState::enBlink);
		}
		break;
	case EnTitleState::enBlink:
		UpdateBlink();
		break;
	case EnTitleState::enVacuumIn:
		UpdateVacuumIn();
		break;
	case EnTitleState::enSuction:
		UpdateSuction();
		break;
	case EnTitleState::enFadeOut:
		UpdateFadeOut();
		break;
	}
	m_backGroundSpriteRemnder.Update();
	m_titleSpriteRender.Update();
	m_titleUISpriteRender.Update();
	m_vacuumSpriteRender.Update();
	m_fadeSpriteRender.Update();
}
void Title::ChangeState(EnTitleState state)
{
	m_state = state;
	m_stateTimer = 0.0f;
}

void Title::FadeUI()
{
	const float dt = g_gameTime->GetFrameDeltaTime();
	/** 位相を積算(周期が変わっても連続的につながる) */
	m_fadePhase += dt / UI_CYCLE_TIME;
	m_fadePhase = fmodf(m_fadePhase, 1.0f);
	/** 三角波でアルファ値を計算 */
	float alpha = 1.0f - fabsf(m_fadePhase * 2.0f - 1.0f);
	m_titleUISpriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, alpha));
}

void Title::UpdateBlink()
{
	const float dt = g_gameTime->GetFrameDeltaTime();
	m_stateTimer += dt;

	/** 経過時間を全体の長さで割って、fminfを使って小さい方の値を返す。(これにより絶対に1を越えないようにする)*/
	float t = fminf(m_stateTimer / PRESSED_DURATION, 1.0f);

	/** 周期を START → END へ伸ばす(徐々にゆっくり = 線形補完)*/
	float cycle = PRESSED_CYCLE_START + (PRESSED_CYCLE_END - PRESSED_CYCLE_START) * t;
	/** 位相を積算 */
	/** 更新前のm_fadePhase(前のフレームの値)を記録しておく*/
	float prevPhase = m_fadePhase;
	/** m_fadePhaseにdtと一周のサイクルで割った値を足していく*/
	m_fadePhase += dt / cycle;
	/** fmodfでm_fadePhaseを1で割った余りを返す*/
	m_fadePhase = fmodf(m_fadePhase, 1.0f);
	/**(m_fadePhase * 2.0f - 1.0f)で0～1だった範囲を1～-1にする 
	1～-1になった範囲をfabsfを使って絶対値にして1～1にして谷の形に変える*/
	float alpha = 1.0f - fabsf(m_fadePhase * 2.0f - 1.0f);
	m_titleUISpriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, alpha));

	/**
	 * 時間が経過していて、かつ明るさが最大(位相0.5)を通過した瞬間に終了する。
	 * こうすると、途中で明るさが飛ばずに自然に点滅が終わる。
	 */
	/** 位相が0.5をまたいだかどうかを調べるフラグ*/
	/** 前のフレームと今のフレームを比べて0,5fを超えていたら*/
	bool passedPeak = (prevPhase < 0.5f && m_fadePhase >= 0.5f);
	/** 点滅が始まってから1描画過ぎているかつ0.5(一番明るい瞬間)だったら
	点滅を止めて、ステートを移動させる*/
	if (t >= 1.0f && passedPeak)
	{
		m_titleUISpriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		ChangeState(EnTitleState::enVacuumIn);
	}
}

void Title::UpdateVacuumIn()
{
	/** 経過時間を計る*/
	m_stateTimer += g_gameTime->GetFrameDeltaTime();

	/** 経過時間を全体の長さで割って、fminfを使って小さい方の値を返す。(これにより絶対に1を越えないようにする) */
	float t = fminf(m_stateTimer / VACUUM_IN_TIME, 1.0f);
	/** 掃除器が画面外から勢いよく飛んできてふわっと停止位置に停止するためにイーズアウトを入れている*/
	/** 最後にゆっくり止まるようにする(イーズアウト)*/
	/**  (1.0f - t) = 残りの割合*/
	float e = 1.0f - (1.0f - t) * (1.0f - t);

	/** 線形補完で開始位置から停止位置へ、x と y を別々に動かす
	線形補完の式:位置 = 開始 + (終了 - 開始) * 割合*/
	Vector3 pos;
	pos.x = VACUUM_START_POS.x + (VACUUM_END_POS.x - VACUUM_START_POS.x) * e;
	pos.y = VACUUM_START_POS.y + (VACUUM_END_POS.y - VACUUM_START_POS.y) * e;
	pos.z = 0.0f;
	m_vacuumSpriteRender.SetPosition(pos);

	/** 停止位置に着いたら吸い込みへ */
	if (t >= 1.0f)
	{
		ChangeState(EnTitleState::enSuction);
	}
}

void Title::UpdateSuction()
{
	m_stateTimer += g_gameTime->GetFrameDeltaTime();

	/** 経過時間を全体の長さで割って、fminfを使って小さい方の値を返す。(これにより絶対に1を越えないようにする) */
	float t = fminf(m_stateTimer / SUCTION_TIME, 1.0f);

	/** t*t で徐々に加速して、吸い込まれる感じを出す(イーズイン)*/
	float e = t * t;

	/** 小さくなる大きさ(1 → 0) */
	float scale = 1.0f - e;

	/** 吸い込まれる時はZ軸を中心に回転させる */
	Quaternion rot;
	rot.SetRotationDegZ(e * SUCTION_ROT_DEG);

	/** 吸い込み先(ノズル口)の位置 */
	float nozzleX = VACUUM_END_POS.x + NOZZLE_OFFSET_X;
	float nozzleY = VACUUM_END_POS.y + NOZZLE_OFFSET_Y;

	/** ロゴをノズル口へ動かす */
	/** eは何%まで進んだか*/
	Vector3 titlePos;
	titlePos.x = TITLE_POS.x + (nozzleX - TITLE_POS.x) * e;
	titlePos.y = TITLE_POS.y + (nozzleY - TITLE_POS.y) * e;
	titlePos.z = 0.0f;
	m_titleSpriteRender.SetPosition(titlePos);
	m_titleSpriteRender.SetScale(Vector3(scale, scale, 1.0f));
	m_titleSpriteRender.SetRotation(rot);

	/** UIもノズル口へ動かす */
	Vector3 uiPos;
	uiPos.x = TITLEUI_POS.x + (nozzleX - TITLEUI_POS.x) * e;
	uiPos.y = TITLEUI_POS.y + (nozzleY - TITLEUI_POS.y) * e;
	uiPos.z = 0.0f;
	m_titleUISpriteRender.SetPosition(uiPos);
	m_titleUISpriteRender.SetScale(Vector3(scale, scale, 1.0f));
	m_titleUISpriteRender.SetRotation(rot);

	/** 吸引中の掃除機を上下に小さく振動させる */
	float shake = sinf(m_stateTimer * SHAKE_SPEED) * SHAKE_POWER;
	m_vacuumSpriteRender.SetPosition(Vector3(VACUUM_END_POS.x, VACUUM_END_POS.y + shake, 0.0f));

	/** 吸い込みが終わったらフェードアウトへ */
	if (t >= 1.0f)
	{
		ChangeState(EnTitleState::enFadeOut);
	}
}

void Title::UpdateFadeOut()
{
	m_stateTimer += g_gameTime->GetFrameDeltaTime();

	/** 進行度 0〜1 */
	float t = fminf(m_stateTimer / FADEOUT_TIME, 1.0f);

	/** 黒い画像のアルファ値を 0 → 1 にして画面を暗くする */
	m_fadeSpriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, t));

	/** 真っ暗になったらゲームへ */
	if (t >= 1.0f)
	{
		m_game = NewGO<Game>(0, "game");
		DeleteGO(this);
	}
}

void Title::Render(RenderContext& rc)
{
	m_backGroundSpriteRemnder.Draw(rc);
	m_titleSpriteRender.Draw(rc);
	m_titleUISpriteRender.Draw(rc);
	m_vacuumSpriteRender.Draw(rc);
	m_fadeSpriteRender.Draw(rc);
}