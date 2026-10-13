#pragma once
class Title;
class GameOver :public IGameObject
{
public:
	GameOver();
	~GameOver();

	bool Start();
	void Update();
	void Render(RenderContext& rc);


private:
	/** ゲームオーバー処理*/
	void InGameOver();
private:
	/** ゲームオーバーのスプライトレンダー*/
	SpriteRender m_gameOverSpriteRender;

	/** ボタンを押したときのフラグ */
	bool m_isGameOverToTitleButtonPressed = false;
	/** タイトル*/
	Title* m_title = nullptr;
};

