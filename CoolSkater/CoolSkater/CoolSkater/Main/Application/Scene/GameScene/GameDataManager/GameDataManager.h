/**
 * @file	GameDataManager.h
 * @brief	ゲームのデータ管理クラス定義
 * @author	morimoto
 */
#ifndef GAMEDATAMANAGER_H
#define GAMEDATAMANAGER_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "SingletonBase\SingletonBase.h"


class GameDataManager : public Lib::SingletonBase<GameDataManager>
{
public:
	friend Lib::SingletonBase<GameDataManager>;

	/**
	 * コンストラクタ 
	 */
	GameDataManager();

	/**
	 * デストラクタ
	 */
	~GameDataManager();

	void SetPlayerSpeed(float _speed) { m_PlayerSpeed = _speed; }
	float GetPlayerSpeed() { return m_PlayerSpeed; }

	void SetScore(int _score) { m_Score = _score; }
	int GetScore() { return m_Score; }

	void SetLife(int _life) { m_Life = _life; }
	int GetLife() { return m_Life; }

private:
	float	m_PlayerSpeed;	//!< プレイヤーのスピード.
	int		m_Score;		//!< スコア.
	int		m_Life;			//!< 残機.

};


#endif // !GAMEDATAMANAGER_H
