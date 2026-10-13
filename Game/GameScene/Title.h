#pragma once
//#include"Sound/SoundSource.h"
class Game;
class SoundManager;

class Title : public IGameObject
{
public:
	Title();
	~Title();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

public:
	/** タイトルで行う処理*/
	void InTitle();

private:
	/** タイトルのスプライトレンダー*/
	SpriteRender m_titleSpriteRender;

	/** ボタンを押したときの処理*/
	bool m_isStartButtonPressed = false;

	/** 入力を受け付けるまでの待ち時間(秒)*/
	float m_inputWaitTimer = 1.0f;
	Game* m_game = nullptr;

	/** タイトルのBGMとSEを管理するサウンドマネージャー */
	SoundManager* m_soundManager = nullptr;
	/** タイトルのBGM*/
	SoundSource* m_titleBGM = nullptr;
};
