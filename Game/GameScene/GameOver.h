#pragma once
class Title;
class GameOver :public IGameObject
{
public:
	GameOver();
	~GameOver();

	bool Start()override;
	void Update()override;
	void Render(RenderContext& rc)override;


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

