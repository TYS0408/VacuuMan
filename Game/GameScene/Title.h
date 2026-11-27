#pragma once
class Game;
class SoundManager;

class Title : public IGameObject
{
public:
	Title();
	~Title();
	bool Start()override;
	void Update()override;
	void Render(RenderContext& rc)override;

public:
	///** タイトルで行う処理*/
	//void InTitle();

	/** タイトルUIを点滅させる関数*/
	void FadeUI();

private:
	enum class EnTitleState
	{
		/** 待機中(UIの点滅)*/
		enWait,
		/** Aボタンの点滅後*/
		enBlink,
		/** 掃除機の登場*/
		enVacuumIn,
		/** 吸い込み*/
		enSuction,
		/** フェードアウト*/
		enFadeOut,
	};
	/** タイトルで掃除機の吸い込み演出をするために必要な変数*/
	EnTitleState m_state = EnTitleState::enWait;
private:
	/** ステートが切り替えるときの関数*/
	void ChangeState(EnTitleState state);
	/** 点滅処理*/
	void UpdateBlink();
	/** 掃除機登場の関数*/
	void UpdateVacuumIn();
	/** 吸い込み演出用の関数*/
	void UpdateSuction();
	/** フェード処理関数*/
	void UpdateFadeOut();
private:
	/** ステートごとに入ってから何秒経過したかを計る変数*/
	float m_stateTimer = 0.0f;
	/** 吸い込む演出用のスプライトレンダー*/
	SpriteRender m_vacuumSpriteRender;
	/** フェード処理用のスプライトレンダー*/
	SpriteRender m_fadeSpriteRender;
	/** タイトルのスプライトレンダー*/
	SpriteRender m_titleSpriteRender;
	/** 背景のスプライトレンダー*/
	SpriteRender m_backGroundSpriteRemnder;
	/** UIのスプライトレンダー*/
	SpriteRender m_titleUISpriteRender;
	/** ボタンを押したときの処理*/
	bool m_isAButtonPressed = false;
	/** 入力を受け付けるまでの待ち時間(秒)*/
	float m_inputWaitTimer = 1.0f;
	Game* m_game = nullptr;
	/** タイトルのBGMとSEを管理するサウンドマネージャー */
	SoundManager* m_soundManager = nullptr;
	/** タイトルのBGM*/
	SoundSource* m_titleBGM = nullptr;
	/** フェードインタイマー*/
	float m_fadeTimer = 0.0f;
	/** 点滅の位相*/
	float m_fadePhase = 0.0f;
	/** Aボタンを押してからの経過時間*/
	float m_pressedTimer = 0.0f;
};
