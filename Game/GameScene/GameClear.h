#pragma once
class GameClear : public IGameObject
{
public:
	GameClear();
	~GameClear();

	bool Start();
	void Update();
	void Render(RenderContext& rc);
	

private:
	/** ゲームクリア時に行う処理*/
	void InGameClear();
private:
	/** ゲームクリアのスプライトレンダー*/
	SpriteRender m_gameClearSpriteRender;

};

