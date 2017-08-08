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
#include "..\..\Event\PlayerEvent\PlayerEvent.h"
#include "..\..\Event\StageBackgroundEvent\StageBackgroundEvent.h"


/**
 * プレイヤークラス
 */
class Player : public Object2DBase
{
public:
	static const D3DXVECTOR2 m_PlayerSize;

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
		GAME_INVINCIBLE_STATE,
		GAME_TRICK1_STATE,
		GAME_TRICK2_STATE,
		GAME_TRICK3_STATE,
		GAME_JUMPTRICK1_STATE,
		GAME_JUMPTRICK2_STATE,
		GAME_JUMPTRICK3_STATE,
		STATE_MAX
	};

	static const int m_DefaultHp;
	static const float m_DefaultSpeed;
	static const float m_DefaultSpeed2;
	static const float m_DefaultSpeed3;
	static const float m_DefaultSpeed4;

	// 更新関数.
	void PauseUpdate();
	void GameStartUpdate();
	void GameUpdate();
	void GameInvincibleUpdate();
	void GameTrick1Update();
	void GameJumpTrick1Update();


	// 描画関数.
	void PauseDraw();
	void GameStartDraw();
	void GameDraw();
	void GameInvincibleDraw();
	void GameTrick1Draw();
	void GameJumpTrick1Draw();

	// イベント受信関数.
	void ReciveEvent(Lib::EventBase* _pEvent);

	void KeyCheck();
	void InvincibleKeyCheck();
	void FallControl();
	void InvincibleTimeControl();
	void InvincibleFallControl();
	void PointControl();
	void TrickCollision();
	void TrickFallControl();

	void(Player::*m_pUpdateFunc)();
	void(Player::*m_pDrawFunc)();
	int		m_Hp;
	float	m_Speed;

	STATE m_State;
	PlayerEventListener*	m_pEventListener;
	PlayerEvent*			m_pEvent;

	int		m_FallTime;
	int		m_JumpTime;
	float	m_JumpSpeed;
	float	m_Gravity;
	bool	m_IsJump;
	bool	m_IsFirstJump;

	int		m_CurrentAnimation;
	int		m_IdleAnimationIndex;
	int		m_JumpAnimationIndex;
	int		m_LandingAnimationIndex;
	int		m_Trick1AnimationIndex;
	int		m_Trick2AnimationIndex;
	int		m_Trick3AnimationIndex;
	int		m_JumpTrick1AnimationIndex;
	int		m_JumpTrick2AnimationIndex;
	int		m_JumpTrick3AnimationIndex;

	D3DXVECTOR2 m_ColliderPosOffset;	//!< 通常の当たり判定の座標オフセット値.
	D3DXVECTOR2 m_ColliderSize;			//!< 通常の当たり判定のサイズ.

	D3DXVECTOR2 m_FallColliderPosOffset;//!< 落下時の当たり判定の座標オフセット値.
	D3DXVECTOR2 m_FallColliderSize;		//!< 落下時の当たり判定のサイズ.

	int			m_CoolPointCounter;
	int			m_HandrailCoolPointCounter;
	int			m_TimeCoolPoint;	// 時間によって取得するスコア.
	int			m_HandrailCoolPoint;	// 手すりトリックによって取得するスコア.

	int			m_InvincibleTime;
	bool		m_IsInvincibleFlash;// 点滅時に透明にしていくか不透明にしていくかのフラグ.
	float		m_InvincibleColor;

	bool		m_IsCollision;	// 通常の衝突判定が行われていた場合.

	int			m_Trick1CoolPoint;	// トリックによって取得するスコア.
	int			m_Trick2CoolPoint;	// トリックによって取得するスコア.
	int			m_Trick3CoolPoint;	// トリックによって取得するスコア.
	int			m_JumpTrick1CoolPoint;	// トリックによって取得するスコア.
	int			m_JumpTrick2CoolPoint;	// トリックによって取得するスコア.
	int			m_JumpTrick3CoolPoint;	// トリックによって取得するスコア.

	bool		m_IsLanding;

	int			m_JumpSoundIndex;
	int			m_DamageSoundIndex;
	int			m_LandingSoundIndex;
	int			m_Trick1SoundIndex;
	int			m_Trick2SoundIndex;
	int			m_Trick3SoundIndex;

	D3DXVECTOR2 m_NormalPos;
	float		m_Angle;
	bool		m_IsJumpRampThrough;
	bool		m_IsJumpRampCollision;

	bool		m_IsHandRail;
	int			m_HandRailAnimationIndex;

	StageBackgroundEvent::EVENT_TYPE m_StageBackgroundEventType;

	int m_TextureIndex2;
	int m_PointSEIndex;

	int m_AnimationCounter;

	int m_PointSEIndex2;

	bool m_IsSyringe;
	int m_SyringeCount;

	int m_RollSoundIndex;

	bool m_IsGolden;
	int m_GoldenCount;

	int m_DashTextureIndex;
	int m_DashAnimationIndex;

	int m_JumpTrickCount;

	int m_GoldenTexture;

	bool m_IsJumpTrick;

};


#endif // !PLAYER_H
