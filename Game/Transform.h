#pragma once
/**
*トランスフォームクラス
*/
class Transform
{
private:
	Vector3 m_position = Vector3::Zero;
	Quaternion m_rotation = Quaternion::Identity;
	Vector3 m_scale;
	;


public:

	/** ポジションのゲッター*/
	const Vector3& GetPosition()const 
	{
		return m_position;
	}

	/** 回転のゲッター*/
	const Quaternion& GetRotation()const 
	{
		return m_rotation;
	}

    /** スケールのゲッター*/
	const Vector3& GetScale()const 
	{
		return m_scale;
	}
	/** ポジションのセッター*/
	void SetPosition(const Vector3 position)
	{
		m_position = position;
	}
	/** 回転のセッター*/
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}
	/** スケールのセッター*/
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}

};