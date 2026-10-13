#pragma once
class TrashManager;
class GameTimer : public IGameObject
{
public:

	GameTimer();
	~GameTimer();


	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	/** タイマーの処理*/
	void TextTimer();

	/** タイムアップした際の処理*/
	void CheckTimeUp();

private:
	/** タイマーのスプライトレンダー*/
	/** 三桁描画するため2次元配列を使う*/
	SpriteRender m_digitSpriteRender[3][10];
	/** 制限時間*/
	float m_timer = 120.0f;
	/** 前のフレームの時間 */
	int m_prevTime = -1;

	/** 部屋の綺麗さを取得するためのTrashManager*/
	TrashManager* m_trashManager = nullptr;

	/** タイムアップ判定が行われたか*/
	bool m_isTimeUpChecked = false;

};

