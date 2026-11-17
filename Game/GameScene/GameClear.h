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
	/** ゲームクリアからすぐにタイトルに戻るのを防ぐために待ち時間を入れる*/
	float m_inputTimer = 1.0f;

};

