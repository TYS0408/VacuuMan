#pragma once
class Game;
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

	Game* m_game = nullptr;
};
