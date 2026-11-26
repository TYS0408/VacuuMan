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
	/** タイトルで行う処理*/
	void InTitle();

	/** タイトルUIを点滅させる関数*/
	void FadeUI();

private:
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
};
