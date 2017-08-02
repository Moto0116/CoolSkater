/**
 * @file	Player.h
 * @brief	プレイヤークラス定義
 * @author	morimoto
 */
#ifndef PLAYER_H
#define PLAYER_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "Main\Object2DBase\Object2DBase.h"

#include "PlayerEventListener\PlayerEventListener.h"


/**
 * プレイヤークラス
 */
class Player : public Object2DBase
{
public:
	/**
	 * コンストラクタ
	 */
	Player();

	/**
	 * デストラクタ
	 */
	virtual ~Player();

	/**
	 * 初期化処理
	 * @return 初期化に成功したか
	 */
	virtual bool Initialize();

	/**
	 * 終了処理
	 */
	virtual void Finalize();

	/**
	 * オブジェクトの更新
	 */
	virtual void Update();

	/**
	 * オブジェクトの描画
	 */
	virtual void Draw();

private:
	/**
	 * プレイヤーのステート 
	 */
	enum STATE
	{
		PAUSE_STATE,
		GAME_START_STATE,
		GAME_STATE,
		STATE_MAX
	};

	static const int m_DefaultHp;
	static const float m_DefaultSpeed;

	// 更新関数.
	void PauseUpdate();
	void GameStartUpdate();
	void GameUpdate();

	// 描画関数.
	void PauseDraw();
	void GameStartDraw();
	void GameDraw();

	// イベント受信関数.
	void ReciveEvent(Lib::EventBase* _pEvent);

	void KeyCheck();
	void FallControl();

	void(Player::*m_pUpdateFunc)();
	void(Player::*m_pDrawFunc)();
	int		m_Hp;
	float	m_Speed;

	STATE m_State;
	PlayerEventListener* m_pEventListener;

	int		m_FallTime;
	int		m_JumpTime;
	float	m_JumpSpeed;
	float	m_Gravity;
	bool	m_IsJump;
	bool	m_IsTrick1;
	bool	m_IsTrick2;
	bool	m_IsTrick3;

	int		m_CurrentAnimation;
	int		m_IdleAnimationIndex;
	int		m_Trick1AnimationIndex;
	int		m_Trick2AnimationIndex;
	int		m_Trick3AnimationIndex;

};


#endif // !PLAYER_H
