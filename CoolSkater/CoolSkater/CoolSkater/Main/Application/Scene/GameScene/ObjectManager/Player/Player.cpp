/**
 * @file	Player.cpp
 * @brief	プレイヤークラス実装
 * @author	morimoto
 */

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "Player.h"

#include "Scene\GameScene\GameDataManager\GameDataManager.h"
#include "DirectX11\TextureManager\TextureManager.h"
#include "EventManager\EventManager.h"
#include "EventManager\EventBase\EventBase.h"
#include "Scene\GameScene\GameScene.h"
#include "InputDeviceManager\InputDeviceManager.h"
#include "DirectX11\AnimationManager\AnimationManager.h"
#include "Scene\GameScene\CollisionManager\CollisionManager.h"
#include "DirectX11\SoundManager\SoundManager.h"
#include "DirectX11\SoundManager\ISound\ISound.h"
#include <math.h>


//----------------------------------------------------------------------
// Static Public Variables
//----------------------------------------------------------------------
const D3DXVECTOR2 m_PlayerSize = D3DXVECTOR2(103, 103);


//----------------------------------------------------------------------
// Static Private Variables
//----------------------------------------------------------------------
const int Player::m_DefaultHp = 2;
const float Player::m_DefaultSpeed = 16.0f;
const float Player::m_DefaultSpeed2 = 18.0f;
const float Player::m_DefaultSpeed3 = 21.0f;
const float Player::m_DefaultSpeed4 = 24.0f;


//----------------------------------------------------------------------
// Constructor	Destructor
//----------------------------------------------------------------------
Player::Player() :
	m_Hp(m_DefaultHp),
	m_Speed(m_DefaultSpeed),
	m_State(PAUSE_STATE),
	m_FallTime(0),
	m_JumpTime(0),
	m_JumpSpeed(25.0f),
	m_Gravity(1.3f),
	m_IsJump(false),
	m_IsFirstJump(true),
	m_CoolPointCounter(0),
	m_HandrailCoolPointCounter(0),
	m_TimeCoolPoint(100),
	m_HandrailCoolPoint(200),
	m_Trick1CoolPoint(1000),
	m_Trick2CoolPoint(1000),
	m_Trick3CoolPoint(1000),
	m_JumpTrick1CoolPoint(1000),
	m_JumpTrick2CoolPoint(1000),
	m_JumpTrick3CoolPoint(1000),
	m_InvincibleTime(200),
	m_InvincibleColor(1.0f),
	m_IsInvincibleFlash(true),
	m_IsCollision(false),
	m_IsLanding(false),
	m_Angle(0.0f),
	m_IsHandRail(false),
	m_AnimationCounter(0)
{
	m_Pos = D3DXVECTOR2(280, 644);
	m_Size = D3DXVECTOR2(128, 128);

	m_ColliderPosOffset = D3DXVECTOR2(0, 0);
	m_ColliderSize = D3DXVECTOR2(m_Size.x+30, m_Size.y - 20);

	m_FallColliderPosOffset = D3DXVECTOR2(0, m_Size.y / 2 - 10);
	m_FallColliderSize = D3DXVECTOR2(m_Size.x - 10, 20);

	m_pUpdateFunc = &Player::PauseUpdate;
	m_pDrawFunc = &Player::PauseDraw;
}

Player::~Player()
{
}


//----------------------------------------------------------------------
// Public Functions
//----------------------------------------------------------------------
bool Player::Initialize()
{
	SINGLETON_INSTANCE(Lib::DrawTaskManager)->AddTask(m_pDrawTask);
	SINGLETON_INSTANCE(Lib::UpdateTaskManager)->AddTask(m_pUpdateTask);

	if (!CreateVertex2D())
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\GameScene\\CharactarMaptip.png",
		&m_TextureIndex))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\GameScene\\Golden_CharactarMaptip.png",
		&m_GoldenTexture))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\GameScene\\Dash.png",
		&m_DashTextureIndex))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\GameScene\\PlayerWait.png",
		&m_TextureIndex2))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::AnimationManager)->LoadAnimation(
		"Resource\\GameScene\\PlayerIdle.anim",
		&m_IdleAnimationIndex))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::AnimationManager)->LoadAnimation(
		"Resource\\GameScene\\PlayerJump.anim",
		&m_JumpAnimationIndex))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::AnimationManager)->LoadAnimation(
		"Resource\\GameScene\\PlayerLanding.anim",
		&m_LandingAnimationIndex))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::AnimationManager)->LoadAnimation(
		"Resource\\GameScene\\PlayerTrick1.anim",
		&m_Trick1AnimationIndex))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::AnimationManager)->LoadAnimation(
		"Resource\\GameScene\\PlayerTrick2.anim",
		&m_Trick2AnimationIndex))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::AnimationManager)->LoadAnimation(
		"Resource\\GameScene\\PlayerTrick3.anim",
		&m_Trick3AnimationIndex))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::AnimationManager)->LoadAnimation(
		"Resource\\GameScene\\PlayerJumpTrick1.anim",
		&m_JumpTrick1AnimationIndex))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::AnimationManager)->LoadAnimation(
		"Resource\\GameScene\\PlayerJumpTrick2.anim",
		&m_JumpTrick2AnimationIndex))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::AnimationManager)->LoadAnimation(
		"Resource\\GameScene\\PlayerJumpTrick3.anim",
		&m_JumpTrick3AnimationIndex))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::AnimationManager)->LoadAnimation(
		"Resource\\GameScene\\HandrailTrick.anim",
		&m_HandRailAnimationIndex))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::AnimationManager)->LoadAnimation(
		"Resource\\GameScene\\PlayerDash.anim",
		&m_DashAnimationIndex))
	{
		return false;
	}


	Lib::IAnimation* Animation;
	Animation = SINGLETON_INSTANCE(Lib::AnimationManager)->GetAnimation(m_IdleAnimationIndex);
	Animation->SetAnimationPattern(Lib::IAnimation::ONE_ANIMATION);
	Animation->SetAnimationSpeed(0.1f);
	Animation->AnimationStart();

	Animation = SINGLETON_INSTANCE(Lib::AnimationManager)->GetAnimation(m_JumpAnimationIndex);
	Animation->SetAnimationPattern(Lib::IAnimation::ONE_ANIMATION);
	Animation->SetAnimationSpeed(0.2f);
	Animation->AnimationStart();

	Animation = SINGLETON_INSTANCE(Lib::AnimationManager)->GetAnimation(m_LandingAnimationIndex);
	Animation->SetAnimationPattern(Lib::IAnimation::ONE_ANIMATION);
	Animation->SetAnimationSpeed(0.1f);
	Animation->AnimationStart();

	Animation = SINGLETON_INSTANCE(Lib::AnimationManager)->GetAnimation(m_Trick1AnimationIndex);
	Animation->SetAnimationPattern(Lib::IAnimation::ONE_ANIMATION);
	Animation->SetAnimationSpeed(0.15f);
	Animation->AnimationStart();

	Animation = SINGLETON_INSTANCE(Lib::AnimationManager)->GetAnimation(m_Trick2AnimationIndex);
	Animation->SetAnimationPattern(Lib::IAnimation::ONE_ANIMATION);
	Animation->SetAnimationSpeed(0.145f);
	Animation->AnimationStart();

	Animation = SINGLETON_INSTANCE(Lib::AnimationManager)->GetAnimation(m_Trick3AnimationIndex);
	Animation->SetAnimationPattern(Lib::IAnimation::ONE_ANIMATION);
	Animation->SetAnimationSpeed(0.145f);
	Animation->AnimationStart();

	Animation = SINGLETON_INSTANCE(Lib::AnimationManager)->GetAnimation(m_JumpTrick1AnimationIndex);
	Animation->SetAnimationPattern(Lib::IAnimation::ONE_ANIMATION);
	Animation->SetAnimationSpeed(0.1f);
	Animation->AnimationStart();

	Animation = SINGLETON_INSTANCE(Lib::AnimationManager)->GetAnimation(m_JumpTrick2AnimationIndex);
	Animation->SetAnimationPattern(Lib::IAnimation::ONE_ANIMATION);
	Animation->SetAnimationSpeed(0.145f);
	Animation->AnimationStart();

	Animation = SINGLETON_INSTANCE(Lib::AnimationManager)->GetAnimation(m_JumpTrick3AnimationIndex);
	Animation->SetAnimationPattern(Lib::IAnimation::ONE_ANIMATION);
	Animation->SetAnimationSpeed(0.145f);
	Animation->AnimationStart();

	Animation = SINGLETON_INSTANCE(Lib::AnimationManager)->GetAnimation(m_HandRailAnimationIndex);
	Animation->SetAnimationPattern(Lib::IAnimation::LOOP_ANIMATION);
	Animation->SetAnimationSpeed(0.145f);
	Animation->AnimationStart();

	Animation = SINGLETON_INSTANCE(Lib::AnimationManager)->GetAnimation(m_DashAnimationIndex);
	Animation->SetAnimationPattern(Lib::IAnimation::LOOP_ANIMATION);
	Animation->SetAnimationSpeed(0.145f);
	Animation->AnimationStart();


	m_CurrentAnimation = m_IdleAnimationIndex;


	if (!SINGLETON_INSTANCE(Lib::SoundManager)->LoadSound(
		"Resource\\GameScene\\Jump.wav",
		&m_JumpSoundIndex))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::SoundManager)->LoadSound(
		"Resource\\GameScene\\Damage.wav",
		&m_DamageSoundIndex))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::SoundManager)->LoadSound(
		"Resource\\GameScene\\Landing.wav",
		&m_LandingSoundIndex))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::SoundManager)->LoadSound(
		"Resource\\GameScene\\Trick1.wav",
		&m_Trick1SoundIndex))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::SoundManager)->LoadSound(
		"Resource\\GameScene\\Trick2.wav",
		&m_Trick2SoundIndex))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::SoundManager)->LoadSound(
		"Resource\\GameScene\\Trick3.wav",
		&m_Trick3SoundIndex))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::SoundManager)->LoadSound(
		"Resource\\GameScene\\Point.wav",
		&m_PointSEIndex))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::SoundManager)->LoadSound(
		"Resource\\GameScene\\Syringe.wav",
		&m_PointSEIndex2))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::SoundManager)->LoadSound(
		"Resource\\GameScene\\roll.wav",
		&m_RollSoundIndex))
	{
		return false;
	}

	m_pEventListener = new PlayerEventListener(this, &Player::ReciveEvent);
	SINGLETON_INSTANCE(Lib::EventManager)->AddEventListener(m_pEventListener);

	m_pEvent = new PlayerEvent();

	SINGLETON_INSTANCE(GameDataManager)->SetPlayerSpeed(m_Speed);

	m_AnimationCounter = 0;

	m_IsSyringe = false;
	m_SyringeCount = 0;

	m_IsGolden = false;
	m_GoldenCount = 0;

	m_JumpTrickCount = 0;

	m_IsJumpTrick = false;

	m_StageBackgroundEventType = StageBackgroundEvent::GAMEUPDATE_EVENT;

	return true;
}

void Player::Finalize()
{
	delete m_pEvent;

	SINGLETON_INSTANCE(Lib::EventManager)->RemoveEventListener(m_pEventListener);
	delete m_pEventListener;

	SINGLETON_INSTANCE(Lib::SoundManager)->ReleaseSound(m_RollSoundIndex);
	SINGLETON_INSTANCE(Lib::SoundManager)->ReleaseSound(m_PointSEIndex2);
	SINGLETON_INSTANCE(Lib::SoundManager)->ReleaseSound(m_PointSEIndex);
	SINGLETON_INSTANCE(Lib::SoundManager)->ReleaseSound(m_Trick3SoundIndex);
	SINGLETON_INSTANCE(Lib::SoundManager)->ReleaseSound(m_Trick2SoundIndex);
	SINGLETON_INSTANCE(Lib::SoundManager)->ReleaseSound(m_Trick1SoundIndex);
	SINGLETON_INSTANCE(Lib::SoundManager)->ReleaseSound(m_LandingSoundIndex);
	SINGLETON_INSTANCE(Lib::SoundManager)->ReleaseSound(m_DamageSoundIndex);
	SINGLETON_INSTANCE(Lib::SoundManager)->ReleaseSound(m_JumpSoundIndex);

	SINGLETON_INSTANCE(Lib::AnimationManager)->ReleaseAnimation(m_DashAnimationIndex);
	SINGLETON_INSTANCE(Lib::AnimationManager)->ReleaseAnimation(m_HandRailAnimationIndex);
	SINGLETON_INSTANCE(Lib::AnimationManager)->ReleaseAnimation(m_JumpTrick3AnimationIndex);
	SINGLETON_INSTANCE(Lib::AnimationManager)->ReleaseAnimation(m_JumpTrick2AnimationIndex);
	SINGLETON_INSTANCE(Lib::AnimationManager)->ReleaseAnimation(m_JumpTrick1AnimationIndex);
	SINGLETON_INSTANCE(Lib::AnimationManager)->ReleaseAnimation(m_Trick3AnimationIndex);
	SINGLETON_INSTANCE(Lib::AnimationManager)->ReleaseAnimation(m_Trick2AnimationIndex);
	SINGLETON_INSTANCE(Lib::AnimationManager)->ReleaseAnimation(m_Trick1AnimationIndex);
	SINGLETON_INSTANCE(Lib::AnimationManager)->ReleaseAnimation(m_LandingAnimationIndex);
	SINGLETON_INSTANCE(Lib::AnimationManager)->ReleaseAnimation(m_JumpAnimationIndex);
	SINGLETON_INSTANCE(Lib::AnimationManager)->ReleaseAnimation(m_IdleAnimationIndex);

	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_TextureIndex2);
	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_DashTextureIndex);
	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_GoldenTexture);
	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_TextureIndex);
	ReleaseVertex2D();
	
	SINGLETON_INSTANCE(Lib::UpdateTaskManager)->RemoveTask(m_pUpdateTask);
	SINGLETON_INSTANCE(Lib::DrawTaskManager)->RemoveTask(m_pDrawTask);
}

void Player::Update()
{
	(this->*m_pUpdateFunc)();
}

void Player::Draw()
{
	(this->*m_pDrawFunc)();
}


//----------------------------------------------------------------------
// Private Functions
//----------------------------------------------------------------------


//----------------------------------------------------------------------
// 更新関数群
//----------------------------------------------------------------------
void Player::PauseUpdate()
{
	if (m_IsInvincibleFlash)
	{
		m_InvincibleColor -= 0.1f;
		if (m_InvincibleColor <= 0.0f)
		{
			m_IsInvincibleFlash = false;
		}
	}
	else
	{
		m_InvincibleColor += 0.1f;
		if (m_InvincibleColor >= 1.0f)
		{
			m_IsInvincibleFlash = true;
		}
	}
}

void Player::GameStartUpdate()
{
}

void Player::GameUpdate()
{
	if (m_IsJump == false)
	{
		m_IsJumpTrick = false;
	}

	if (m_IsSyringe)
	{
		switch (m_StageBackgroundEventType)
		{
		case StageBackgroundEvent::GAMEUPDATE_EVENT:
			m_Speed = m_DefaultSpeed + 30.0f;
			SINGLETON_INSTANCE(GameDataManager)->SetPlayerSpeed(m_Speed);
			break;
		case StageBackgroundEvent::STAGECHANGE_EVENT1:
			m_Speed = m_DefaultSpeed2 + 30.0f;
			SINGLETON_INSTANCE(GameDataManager)->SetPlayerSpeed(m_Speed);
			break;
		case StageBackgroundEvent::STAGECHANGE_EVENT2:
			m_Speed = m_DefaultSpeed3 + 30.0f;
			SINGLETON_INSTANCE(GameDataManager)->SetPlayerSpeed(m_Speed);
			break;
		case StageBackgroundEvent::STAGECHANGE_EVENT3:
			m_Speed = m_DefaultSpeed4 + 30.0f;
			SINGLETON_INSTANCE(GameDataManager)->SetPlayerSpeed(m_Speed);
			break;
		}

		m_CurrentAnimation = m_DashAnimationIndex;
		m_Size = D3DXVECTOR2(1024, 512);
		m_ColliderSize = D3DXVECTOR2(m_Size.x+30, m_Size.y - 20);
	}

	// プレイヤーの座標がずれている場合は既定位置に徐々に調整.
	if (m_Pos.x < 280)
	{
		m_Pos.x += 1;
		if (m_Pos.x > 280)
		{
			m_Pos.x = 280;
		}
	}

	if (m_AnimationCounter > 0)
	{
		m_AnimationCounter--;
		if (m_AnimationCounter == 0)
		{
			Lib::IAnimation* Animation;
			Animation = SINGLETON_INSTANCE(Lib::AnimationManager)->GetAnimation(m_IdleAnimationIndex);
			Animation->AnimationStart();
		}
	}

	if (SINGLETON_INSTANCE(Lib::AnimationManager)->GetAnimation(m_CurrentAnimation)->Update())
	{
		if (m_CurrentAnimation == m_LandingAnimationIndex)
		{
			// 着地アニメーションが終了していた場合.
			m_CurrentAnimation = m_IdleAnimationIndex;
		}
		else if (m_CurrentAnimation == m_IdleAnimationIndex)
		{
			m_AnimationCounter = 20;
		}
	}

	// スコア更新処理.
	PointControl();

	// 衝突判定.
	D3DXVECTOR2 OutPos, OutSize;
	float Progress = SINGLETON_INSTANCE(GameDataManager)->GetPlayerProgress();

	if (m_IsSyringe)
	{
		// 無敵状態ならギミックを破壊する
		GimmickManager::GIMMICK_TYPE Type = SINGLETON_INSTANCE(CollisionManager)->DestroyGimmick(
			&D3DXVECTOR2(m_Pos.x + m_ColliderPosOffset.x + Progress, m_Pos.y + m_ColliderPosOffset.y),
			&m_ColliderSize);
		while (Type != GimmickManager::NONE_GIMMICK)
		{
			int CurrentScore = SINGLETON_INSTANCE(GameDataManager)->GetScore();
			switch (Type)
			{
			case GimmickManager::OBSTACLE_GIMMICK:
				SINGLETON_INSTANCE(GameDataManager)->SetScore(CurrentScore + 100);
				break;
			case GimmickManager::OBSTACLECAR_GIMMICK:
				SINGLETON_INSTANCE(GameDataManager)->SetScore(CurrentScore + 500);
				break;
			}

			Type = SINGLETON_INSTANCE(CollisionManager)->DestroyGimmick(
				&D3DXVECTOR2(m_Pos.x + m_ColliderPosOffset.x + Progress, m_Pos.y + m_ColliderPosOffset.y),
				&m_ColliderSize);
		}

		int CurrentScore = SINGLETON_INSTANCE(GameDataManager)->GetScore();
		switch (Type)
		{
		case GimmickManager::OBSTACLE_GIMMICK:
			SINGLETON_INSTANCE(GameDataManager)->SetScore(CurrentScore + 100);
			break;
		case GimmickManager::OBSTACLECAR_GIMMICK:
			SINGLETON_INSTANCE(GameDataManager)->SetScore(CurrentScore + 500);
			break;
		}

		m_SyringeCount++;
		if (m_SyringeCount > 350)
		{
			switch (m_StageBackgroundEventType)
			{
			case StageBackgroundEvent::GAMEUPDATE_EVENT:
				SINGLETON_INSTANCE(GameDataManager)->SetPlayerSpeed(m_DefaultSpeed);
				break;
			case StageBackgroundEvent::STAGECHANGE_EVENT1:
				SINGLETON_INSTANCE(GameDataManager)->SetPlayerSpeed(m_DefaultSpeed2);
				break;
			case StageBackgroundEvent::STAGECHANGE_EVENT2:
				SINGLETON_INSTANCE(GameDataManager)->SetPlayerSpeed(m_DefaultSpeed3);
				break;
			case StageBackgroundEvent::STAGECHANGE_EVENT3:
				SINGLETON_INSTANCE(GameDataManager)->SetPlayerSpeed(m_DefaultSpeed4);
				break;
			}


			m_SyringeCount = 0;
			m_IsSyringe = false;

			m_Pos.y -= 100;
			m_Size = D3DXVECTOR2(128, 128);
			m_ColliderSize = D3DXVECTOR2(m_Size.x+30, m_Size.y - 20);

			m_CurrentAnimation = m_IdleAnimationIndex;

			m_State = GAME_INVINCIBLE_STATE;
			m_pUpdateFunc = &Player::GameInvincibleUpdate;
			m_pDrawFunc = &Player::GameInvincibleDraw;
		}
	}

	if (m_IsGolden)
	{
		m_GoldenCount++;
		if (m_GoldenCount > 1200)
		{
			m_IsGolden = false;
			SINGLETON_INSTANCE(GameDataManager)->SetIsGolden(false);
		}
	}

	GimmickManager::GIMMICK_TYPE Type = SINGLETON_INSTANCE(CollisionManager)->GimmickCollision(
		&D3DXVECTOR2(m_Pos.x + m_ColliderPosOffset.x + Progress, m_Pos.y + m_ColliderPosOffset.y),
		&m_ColliderSize,
		&OutPos,
		&OutSize);

	// 手すりから離れたらトリックを終了する.
	if (m_CurrentAnimation == m_HandRailAnimationIndex &&
		Type != GimmickManager::HANRAIL_GIMMICK)
	{
		m_CurrentAnimation = m_IdleAnimationIndex;
	}

	if (Type != GimmickManager::NONE_GIMMICK)
	{
		switch (Type)
		{
		case GimmickManager::JUMPRAMP_GIMMICK:
		{
			if (m_IsJumpRampThrough || m_IsJump)
			{
				break;
			}

			if (m_IsSyringe == true)
			{
				break;
			}

			// 右下座標を求める.
			float Length;
			Length = ((m_NormalPos.x + m_Size.x / 2) - (OutPos.x - OutSize.x / 2));

			float Height = Length * static_cast<float>(tan(D3DXToRadian(30)));

			// 右下座標との角度を求める.
			double radian = atan2(
				m_NormalPos.y + m_Size.y / 2 - (OutPos.y + OutSize.y / 2 - Height),
				m_NormalPos.x + m_Size.x / 2);

			if (radian > D3DXToRadian(30))
			{
				radian = D3DXToRadian(30);
			}

			m_Angle = static_cast<float>(-radian);

			// 回転後の位置調整.
			D3DXVECTOR2 PrevPos = m_Pos;
			D3DXVECTOR2 Diff = D3DXVECTOR2(m_Size.x, -m_Size.y);
			Diff = m_NormalPos - Diff;

			m_Pos.x =
				(m_Size.x * static_cast<float>(cos(-radian))) -
				(-m_Size.y * static_cast<float>(sin(-radian))) + Diff.x;

			m_Pos.y =
				(m_Size.x * static_cast<float>(sin(-radian))) +
				(-m_Size.y * static_cast<float>(cos(-radian))) + Diff.y;


			// 移動量分y座標をずらす.
			float Movement = Length - m_Size.x;
			float YMovement = static_cast<float>(tan(-radian) * (cos(-radian) * Movement));
			m_Pos.y += YMovement;

			m_IsHandRail = false;
			m_IsJumpRampCollision = true;
			break;
		}
		case GimmickManager::HANRAIL_GIMMICK:
			if (m_IsSyringe == false)
			{
				if (m_IsHandRail == false)
				{
					m_Pos.y = 595;
				}
				m_CurrentAnimation = m_HandRailAnimationIndex;

				m_IsJumpRampCollision = false;
				m_IsHandRail = true;
			}
			break;
		case GimmickManager::POINT_GIMMICK:
			if (m_IsGolden)
			{
				SINGLETON_INSTANCE(GameDataManager)->SetScore(
					SINGLETON_INSTANCE(GameDataManager)->GetScore() + 10 * 2);
			}
			else
			{
				SINGLETON_INSTANCE(GameDataManager)->SetScore(
					SINGLETON_INSTANCE(GameDataManager)->GetScore() + 10);
			}

			SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_PointSEIndex)->SoundOperation(
				Lib::SoundManager::STOP_RESET);
			SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_PointSEIndex)->SoundOperation(
				Lib::SoundManager::PLAY);
			break;
		case GimmickManager::SYRINGE_GIMMICK:
			SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_PointSEIndex2)->SoundOperation(
				Lib::SoundManager::STOP_RESET);
			SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_PointSEIndex2)->SoundOperation(
				Lib::SoundManager::PLAY);

			m_IsSyringe = true;

			break;
		case GimmickManager::GOLDEN_GIMMICK:
			SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_PointSEIndex)->SoundOperation(
				Lib::SoundManager::STOP_RESET);
			SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_PointSEIndex)->SoundOperation(
				Lib::SoundManager::PLAY);

			m_IsGolden = true;
			m_GoldenCount = 0;
			SINGLETON_INSTANCE(GameDataManager)->SetIsGolden(true);
			break;
		case GimmickManager::SCAFFOLD_GIMMICK:
			break;
		default:
			if (m_IsSyringe == false)
			{
				if (m_State != GAME_INVINCIBLE_STATE)
				{
					SINGLETON_INSTANCE(GameDataManager)->SetLife(SINGLETON_INSTANCE(GameDataManager)->GetLife() - 1);

					m_State = GAME_INVINCIBLE_STATE;
					m_pUpdateFunc = &Player::GameInvincibleUpdate;
					m_pDrawFunc = &Player::GameInvincibleDraw;

					// ダメージ音声再生.
					SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_DamageSoundIndex)->SoundOperation(
						Lib::SoundManager::PLAY);
				}
			}
			

			m_IsCollision = true;
			m_NormalPos = m_Pos;

			if (m_Angle < 0.f)
			{
				m_Angle += static_cast<float>(D3DXToRadian(2));
				if (m_Angle > 0.f)
				{
					m_Angle = 0;
				}
			}
			m_IsJumpRampCollision = false;
			m_IsHandRail = false;
			m_IsJumpRampThrough = false;

			break;
		}
	}
	else
	{
		m_IsCollision = false;
		m_NormalPos = m_Pos;

		if (m_Angle < 0.f)
		{
			m_Angle += static_cast<float>(D3DXToRadian(2));
			if (m_Angle > 0.f)
			{
				m_Angle = 0;
			}
		}
		m_IsJumpRampCollision = false;
		m_IsHandRail = false;
		m_IsJumpRampThrough = false;
	}


	KeyCheck();
	FallControl();
}

void Player::GameInvincibleUpdate()
{
	if (m_IsJump == false)
	{
		m_IsJumpTrick = false;
	}

	if (m_IsSyringe)
	{
		switch (m_StageBackgroundEventType)
		{
		case StageBackgroundEvent::GAMEUPDATE_EVENT:
			m_Speed = m_DefaultSpeed + 30.0f;
			SINGLETON_INSTANCE(GameDataManager)->SetPlayerSpeed(m_Speed);
			break;
		case StageBackgroundEvent::STAGECHANGE_EVENT1:
			m_Speed = m_DefaultSpeed2 + 30.0f;
			SINGLETON_INSTANCE(GameDataManager)->SetPlayerSpeed(m_Speed);
			break;
		case StageBackgroundEvent::STAGECHANGE_EVENT2:
			m_Speed = m_DefaultSpeed3 + 30.0f;
			SINGLETON_INSTANCE(GameDataManager)->SetPlayerSpeed(m_Speed);
			break;
		case StageBackgroundEvent::STAGECHANGE_EVENT3:
			m_Speed = m_DefaultSpeed4 + 30.0f;
			SINGLETON_INSTANCE(GameDataManager)->SetPlayerSpeed(m_Speed);
			break;
		}

		m_CurrentAnimation = m_DashAnimationIndex;
		m_Size = D3DXVECTOR2(1024, 512);
		m_ColliderSize = D3DXVECTOR2(m_Size.x+30, m_Size.y - 20);
	}

	// プレイヤーの座標がずれている場合は既定位置に徐々に調整.
	if (m_Pos.x < 280)
	{
		m_Pos.x += 1;
		if (m_Pos.x > 280)
		{
			m_Pos.x = 280;
		}
	}

	if (m_AnimationCounter > 0)
	{
		m_AnimationCounter--;
		if (m_AnimationCounter == 0)
		{
			Lib::IAnimation* Animation;
			Animation = SINGLETON_INSTANCE(Lib::AnimationManager)->GetAnimation(m_IdleAnimationIndex);
			Animation->AnimationStart();
		}
	}

	if (SINGLETON_INSTANCE(Lib::AnimationManager)->GetAnimation(m_CurrentAnimation)->Update())
	{
		if (m_CurrentAnimation == m_LandingAnimationIndex)
		{
			// 着地アニメーションが終了していた場合.
			m_CurrentAnimation = m_IdleAnimationIndex;
		}
		else if (m_CurrentAnimation == m_IdleAnimationIndex)
		{
			m_AnimationCounter = 20;
		}
	}

	// 無敵時間の更新.
	InvincibleTimeControl();

	// スコア更新処理.
	PointControl();

	// 衝突判定.
	D3DXVECTOR2 OutPos, OutSize;
	float Progress = SINGLETON_INSTANCE(GameDataManager)->GetPlayerProgress();

	if (m_IsSyringe)
	{
		// 無敵状態ならギミックを破壊する
		GimmickManager::GIMMICK_TYPE Type = SINGLETON_INSTANCE(CollisionManager)->DestroyGimmick(
			&D3DXVECTOR2(m_Pos.x + m_ColliderPosOffset.x + Progress, m_Pos.y + m_ColliderPosOffset.y),
			&m_ColliderSize);
		while (Type != GimmickManager::NONE_GIMMICK)
		{
			int CurrentScore = SINGLETON_INSTANCE(GameDataManager)->GetScore();
			switch (Type)
			{
			case GimmickManager::OBSTACLE_GIMMICK:
				SINGLETON_INSTANCE(GameDataManager)->SetScore(CurrentScore + 100);
				break;
			case GimmickManager::OBSTACLECAR_GIMMICK:
				SINGLETON_INSTANCE(GameDataManager)->SetScore(CurrentScore + 500);
				break;
			}

			Type = SINGLETON_INSTANCE(CollisionManager)->DestroyGimmick(
				&D3DXVECTOR2(m_Pos.x + m_ColliderPosOffset.x + Progress, m_Pos.y + m_ColliderPosOffset.y),
				&m_ColliderSize);
		}

		m_SyringeCount++;
		if (m_SyringeCount > 350)
		{
			switch (m_StageBackgroundEventType)
			{
			case StageBackgroundEvent::GAMEUPDATE_EVENT:
				SINGLETON_INSTANCE(GameDataManager)->SetPlayerSpeed(m_DefaultSpeed);
				break;
			case StageBackgroundEvent::STAGECHANGE_EVENT1:
				SINGLETON_INSTANCE(GameDataManager)->SetPlayerSpeed(m_DefaultSpeed2);
				break;
			case StageBackgroundEvent::STAGECHANGE_EVENT2:
				SINGLETON_INSTANCE(GameDataManager)->SetPlayerSpeed(m_DefaultSpeed3);
				break;
			case StageBackgroundEvent::STAGECHANGE_EVENT3:
				SINGLETON_INSTANCE(GameDataManager)->SetPlayerSpeed(m_DefaultSpeed4);
				break;
			}

			m_SyringeCount = 0;
			m_IsSyringe = false;

			m_Pos.y -= 100;
			m_Size = D3DXVECTOR2(128, 128);
			m_ColliderSize = D3DXVECTOR2(m_Size.x+30, m_Size.y - 20);

			m_CurrentAnimation = m_IdleAnimationIndex;

			m_State = GAME_INVINCIBLE_STATE;
			m_pUpdateFunc = &Player::GameInvincibleUpdate;
			m_pDrawFunc = &Player::GameInvincibleDraw;
		}
	}

	if (m_IsGolden)
	{
		m_GoldenCount++;
		if (m_GoldenCount > 1200)
		{
			m_IsGolden = false;
			SINGLETON_INSTANCE(GameDataManager)->SetIsGolden(false);
		}
	}

	GimmickManager::GIMMICK_TYPE Type = SINGLETON_INSTANCE(CollisionManager)->GimmickCollision(
		&D3DXVECTOR2(m_Pos.x + m_ColliderPosOffset.x + Progress, m_Pos.y + m_ColliderPosOffset.y),
		&m_ColliderSize,
		&OutPos,
		&OutSize);

	// 手すりから離れたらトリックを終了する.
	if (m_CurrentAnimation == m_HandRailAnimationIndex &&
		Type != GimmickManager::HANRAIL_GIMMICK)
	{
		m_CurrentAnimation = m_IdleAnimationIndex;
	}

	if (Type != GimmickManager::NONE_GIMMICK)
	{
		switch (Type)
		{
		case GimmickManager::JUMPRAMP_GIMMICK:
		{
			if (m_IsJumpRampThrough)
			{
				break;
			}

			if (m_IsSyringe)
			{
				break;
			}


			// 右下座標を求める.
			float Length;
			Length = ((m_NormalPos.x + m_Size.x / 2) - (OutPos.x - OutSize.x / 2));

			float Height = Length * static_cast<float>(tan(D3DXToRadian(30)));

			// 右下座標との角度を求める.
			double radian = atan2(
				m_NormalPos.y + m_Size.y / 2 - (OutPos.y + OutSize.y / 2 - Height),
				m_NormalPos.x + m_Size.x / 2);

			if (radian > D3DXToRadian(30))
			{
				radian = D3DXToRadian(30);
			}

			m_Angle = static_cast<float>(-radian);

			// 回転後の位置調整.
			D3DXVECTOR2 Diff = D3DXVECTOR2(m_Size.x, -m_Size.y);
			Diff = m_NormalPos - Diff;

			m_Pos.x =
				(m_Size.x * static_cast<float>(cos(-radian))) -
				(-m_Size.y * static_cast<float>(sin(-radian))) + Diff.x;

			m_Pos.y =
				(m_Size.x * static_cast<float>(sin(-radian))) +
				(-m_Size.y * static_cast<float>(cos(-radian))) + Diff.y;


			// 移動量分y座標をずらす.
			float Movement = Length - m_Size.x;
			float YMovement = static_cast<float>(tan(-radian) * (cos(-radian) * Movement));
			m_Pos.y += YMovement;

			m_IsHandRail = false;
			m_IsJumpRampCollision = true;
			break;
		}
		case GimmickManager::HANRAIL_GIMMICK:
			if (m_IsSyringe == false)
			{
				if (m_IsHandRail == false)
				{
					m_Pos.y = 595;
				}
				m_CurrentAnimation = m_HandRailAnimationIndex;

				m_IsJumpRampCollision = false;
				m_IsHandRail = true;
			}
			break;
		case GimmickManager::POINT_GIMMICK:
			if (m_IsGolden)
			{
				SINGLETON_INSTANCE(GameDataManager)->SetScore(
					SINGLETON_INSTANCE(GameDataManager)->GetScore() + 10 * 2);
			}
			else
			{
				SINGLETON_INSTANCE(GameDataManager)->SetScore(
					SINGLETON_INSTANCE(GameDataManager)->GetScore() + 10);
			}

			SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_PointSEIndex)->SoundOperation(
				Lib::SoundManager::STOP_RESET);
			SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_PointSEIndex)->SoundOperation(
				Lib::SoundManager::PLAY);

			break;
		case GimmickManager::SYRINGE_GIMMICK:
			SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_PointSEIndex2)->SoundOperation(
				Lib::SoundManager::STOP_RESET);
			SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_PointSEIndex2)->SoundOperation(
				Lib::SoundManager::PLAY);

			m_IsSyringe = true;

			break;
		case GimmickManager::GOLDEN_GIMMICK:
			SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_PointSEIndex)->SoundOperation(
				Lib::SoundManager::STOP_RESET);
			SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_PointSEIndex)->SoundOperation(
				Lib::SoundManager::PLAY);

			m_IsGolden = true;
			m_GoldenCount = 0;
			SINGLETON_INSTANCE(GameDataManager)->SetIsGolden(true);
			break;
		default:
			m_IsCollision = true;
			m_NormalPos = m_Pos;

			if (m_Angle < 0.f)
			{
				m_Angle += static_cast<float>(D3DXToRadian(2));
				if (m_Angle > 0.f)
				{
					m_Angle = 0;
				}
			}
			m_IsJumpRampCollision = false;
			m_IsHandRail = false;
			m_IsJumpRampThrough = false;

			break;
		}
	}
	else
	{
		m_IsCollision = false;
		m_NormalPos = m_Pos;

		if (m_Angle < 0.f)
		{
			m_Angle += static_cast<float>(D3DXToRadian(2));
			if (m_Angle > 0.f)
			{
				m_Angle = 0;
			}
		}
		m_IsJumpRampCollision = false;
		m_IsHandRail = false;
		m_IsJumpRampThrough = false;
	}

	InvincibleKeyCheck();
	InvincibleFallControl();
}

void Player::GameTrick1Update()
{
	if (m_IsSyringe)
	{
		switch (m_StageBackgroundEventType)
		{
		case StageBackgroundEvent::GAMEUPDATE_EVENT:
			m_Speed = m_DefaultSpeed + 30.0f;
			SINGLETON_INSTANCE(GameDataManager)->SetPlayerSpeed(m_Speed);
			break;
		case StageBackgroundEvent::STAGECHANGE_EVENT1:
			m_Speed = m_DefaultSpeed2 + 30.0f;
			SINGLETON_INSTANCE(GameDataManager)->SetPlayerSpeed(m_Speed);
			break;
		case StageBackgroundEvent::STAGECHANGE_EVENT2:
			m_Speed = m_DefaultSpeed3 + 30.0f;
			SINGLETON_INSTANCE(GameDataManager)->SetPlayerSpeed(m_Speed);
			break;
		case StageBackgroundEvent::STAGECHANGE_EVENT3:
			m_Speed = m_DefaultSpeed4 + 30.0f;
			SINGLETON_INSTANCE(GameDataManager)->SetPlayerSpeed(m_Speed);
			break;
		}

		m_CurrentAnimation = m_DashAnimationIndex;
		m_Size = D3DXVECTOR2(1024, 512);
		m_ColliderSize = D3DXVECTOR2(m_Size.x+30, m_Size.y - 20);

		m_State = GAME_STATE;
		m_pUpdateFunc = &Player::GameUpdate;
		m_pDrawFunc = &Player::GameDraw;

		m_CurrentAnimation = m_IdleAnimationIndex;	// アニメーションを変更.
	}

	m_IsJumpRampThrough = true;

	if (SINGLETON_INSTANCE(Lib::AnimationManager)->GetAnimation(m_CurrentAnimation)->Update())
	{
		SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_RollSoundIndex)->SoundOperation(
			Lib::SoundManager::PLAY_LOOP);

		m_State = GAME_STATE;
		m_pUpdateFunc = &Player::GameUpdate;
		m_pDrawFunc = &Player::GameDraw;

		m_CurrentAnimation = m_IdleAnimationIndex;	// アニメーションを変更.

		// トリック成功によるスコア取得.
		int CurrentScore = SINGLETON_INSTANCE(GameDataManager)->GetScore();
		if (m_IsGolden)
		{
			SINGLETON_INSTANCE(GameDataManager)->SetScore(CurrentScore + m_Trick1CoolPoint * 2);
		}
		else
		{
			SINGLETON_INSTANCE(GameDataManager)->SetScore(CurrentScore + m_Trick1CoolPoint);
		}
	}

	// スコア更新処理.
	PointControl();
	TrickCollision();
	TrickFallControl();
}

void Player::GameJumpTrick1Update()
{
	if (m_IsSyringe)
	{
		switch (m_StageBackgroundEventType)
		{
		case StageBackgroundEvent::GAMEUPDATE_EVENT:
			m_Speed = m_DefaultSpeed + 30.0f;
			SINGLETON_INSTANCE(GameDataManager)->SetPlayerSpeed(m_Speed);
			break;
		case StageBackgroundEvent::STAGECHANGE_EVENT1:
			m_Speed = m_DefaultSpeed2 + 30.0f;
			SINGLETON_INSTANCE(GameDataManager)->SetPlayerSpeed(m_Speed);
			break;
		case StageBackgroundEvent::STAGECHANGE_EVENT2:
			m_Speed = m_DefaultSpeed3 + 30.0f;
			SINGLETON_INSTANCE(GameDataManager)->SetPlayerSpeed(m_Speed);
			break;
		case StageBackgroundEvent::STAGECHANGE_EVENT3:
			m_Speed = m_DefaultSpeed4 + 30.0f;
			SINGLETON_INSTANCE(GameDataManager)->SetPlayerSpeed(m_Speed);
			break;
		}

		m_CurrentAnimation = m_DashAnimationIndex;
		m_Size = D3DXVECTOR2(1024, 512);
		m_ColliderSize = D3DXVECTOR2(m_Size.x+30, m_Size.y - 20);

		m_State = GAME_STATE;
		m_pUpdateFunc = &Player::GameUpdate;
		m_pDrawFunc = &Player::GameDraw;

		m_CurrentAnimation = m_IdleAnimationIndex;	// アニメーションを変更.
	}

	m_IsJumpRampThrough = true;

	if (SINGLETON_INSTANCE(Lib::AnimationManager)->GetAnimation(m_CurrentAnimation)->Update())
	{
		SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_RollSoundIndex)->SoundOperation(
			Lib::SoundManager::PLAY_LOOP);

		m_State = GAME_STATE;
		m_pUpdateFunc = &Player::GameUpdate;
		m_pDrawFunc = &Player::GameDraw;

		m_CurrentAnimation = m_IdleAnimationIndex;	// アニメーションを変更.

		// トリック成功によるスコア取得.
		int CurrentScore = SINGLETON_INSTANCE(GameDataManager)->GetScore();
		if (m_IsGolden)
		{
			SINGLETON_INSTANCE(GameDataManager)->SetScore(CurrentScore + m_JumpTrick1CoolPoint * 2);
		}
		else
		{
			SINGLETON_INSTANCE(GameDataManager)->SetScore(CurrentScore + m_JumpTrick1CoolPoint);
		}
	}

	// スコア更新処理.
	PointControl();
	TrickCollision();
	TrickFallControl();
}


//----------------------------------------------------------------------
// 描画関数群
//----------------------------------------------------------------------
void Player::PauseDraw()
{
	m_pVertex->SetVertex(&m_Size);
	m_pVertex->WriteVertexBuffer();

	m_pVertex->SetAnimation(nullptr);
	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex2));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&m_Pos, &D3DXVECTOR2(1, 1), m_Angle);
	m_pVertex->Draw();
}

void Player::GameStartDraw()
{
	m_pVertex->SetVertex(&m_Size);
	m_pVertex->WriteVertexBuffer();

	m_pVertex->SetAnimation(SINGLETON_INSTANCE(Lib::AnimationManager)->GetAnimation(m_CurrentAnimation));
	if (m_IsGolden)
	{
		m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_GoldenTexture));
	}
	else
	{
		m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex));
	}
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&m_Pos, &D3DXVECTOR2(1, 1), m_Angle);
	m_pVertex->Draw();
}

void Player::GameDraw()
{
	m_pVertex->SetVertex(&m_Size);
	m_pVertex->WriteVertexBuffer();

	m_pVertex->SetAnimation(SINGLETON_INSTANCE(Lib::AnimationManager)->GetAnimation(m_CurrentAnimation));

	if (m_IsSyringe == false)
	{
		if (m_IsGolden)
		{
			m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_GoldenTexture));
		}
		else
		{
			m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex));
		}
	}
	else
	{
		m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_DashTextureIndex));
	}

	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&m_Pos, &D3DXVECTOR2(1, 1), m_Angle);
	m_pVertex->Draw();
}

void Player::GameInvincibleDraw()
{
	m_pVertex->SetVertex(&m_Size);
	m_pVertex->WriteVertexBuffer();

	m_pVertex->SetColor(&D3DXCOLOR(1, 1, 1, m_InvincibleColor));
	m_pVertex->WriteVertexBuffer();

	m_pVertex->SetAnimation(SINGLETON_INSTANCE(Lib::AnimationManager)->GetAnimation(m_CurrentAnimation));

	if (m_IsSyringe == false)
	{
		if (m_IsGolden)
		{
			m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_GoldenTexture));
		}
		else
		{
			m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex));
		}
	}
	else
	{
		m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_DashTextureIndex));
	}

	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&m_Pos, &D3DXVECTOR2(1, 1), m_Angle);
	m_pVertex->Draw();
}

void Player::GameTrick1Draw()
{
	m_pVertex->SetVertex(&m_Size);
	m_pVertex->WriteVertexBuffer();

	m_pVertex->SetAnimation(SINGLETON_INSTANCE(Lib::AnimationManager)->GetAnimation(m_CurrentAnimation));

	if (m_IsSyringe == false)
	{
		if (m_IsGolden)
		{
			m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_GoldenTexture));
		}
		else
		{
			m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex));
		}
	}
	else
	{
		m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_DashTextureIndex));
	}

	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&m_Pos, &D3DXVECTOR2(1, 1), m_Angle);
	m_pVertex->Draw();
}

void Player::GameJumpTrick1Draw()
{
	m_pVertex->SetVertex(&m_Size);
	m_pVertex->WriteVertexBuffer();

	m_pVertex->SetAnimation(SINGLETON_INSTANCE(Lib::AnimationManager)->GetAnimation(m_CurrentAnimation));

	if (m_IsSyringe == false)
	{
		if (m_IsGolden)
		{
			m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_GoldenTexture));
		}
		else
		{
			m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex));
		}
	}
	else
	{
		m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_DashTextureIndex));
	}

	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&m_Pos, &D3DXVECTOR2(1, 1), m_Angle);
	m_pVertex->Draw();
}


void Player::ReciveEvent(Lib::EventBase* _pEvent)
{
	switch (_pEvent->GetEventID())
	{
	case GameScene::GAMESTART_EVENT:
		m_State = GAME_START_STATE;
		m_pUpdateFunc = &Player::GameStartUpdate;
		m_pDrawFunc = &Player::GameStartDraw;
		break;
	case GameScene::STAGEBACKGROUND_EVENT:
		switch (reinterpret_cast<StageBackgroundEvent*>(_pEvent)->GetEvent())
		{
		case StageBackgroundEvent::GAMEUPDATE_EVENT:
			SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_RollSoundIndex)->SoundOperation(
				Lib::SoundManager::PLAY_LOOP);

			m_StageBackgroundEventType = StageBackgroundEvent::GAMEUPDATE_EVENT;
			m_State = GAME_STATE;
			m_pUpdateFunc = &Player::GameUpdate;
			m_pDrawFunc = &Player::GameDraw;
			break;
		case StageBackgroundEvent::STAGECHANGE_EVENT1:
			m_StageBackgroundEventType = StageBackgroundEvent::STAGECHANGE_EVENT1;
			m_Speed = m_DefaultSpeed2;
			SINGLETON_INSTANCE(GameDataManager)->SetPlayerSpeed(m_Speed);
			break;
		case StageBackgroundEvent::STAGECHANGE_EVENT2:
			m_StageBackgroundEventType = StageBackgroundEvent::STAGECHANGE_EVENT2;
			m_Speed = m_DefaultSpeed3;
			SINGLETON_INSTANCE(GameDataManager)->SetPlayerSpeed(m_Speed);
			break;
		case StageBackgroundEvent::STAGECHANGE_EVENT3:
			m_StageBackgroundEventType = StageBackgroundEvent::STAGECHANGE_EVENT3;
			m_Speed = m_DefaultSpeed4;
			SINGLETON_INSTANCE(GameDataManager)->SetPlayerSpeed(m_Speed);
			break;
		}
		break;
	case GameScene::PLAYERDEAD_EVENT:
		if (SINGLETON_INSTANCE(GameDataManager)->GetScore() > 50000)
		{
			m_pEvent->SetEvent(PlayerEvent::PLAYERCLEAR_EVENT);
			SINGLETON_INSTANCE(Lib::EventManager)->SendEventMessage(m_pEvent);

			m_State = PAUSE_STATE;
			m_pUpdateFunc = &Player::PauseUpdate;
			m_pDrawFunc = &Player::GameInvincibleDraw;
		}
		else
		{
			m_pEvent->SetEvent(PlayerEvent::PLAYERDEADEND_EVENT);
			SINGLETON_INSTANCE(Lib::EventManager)->SendEventMessage(m_pEvent);

			m_State = PAUSE_STATE;
			m_pUpdateFunc = &Player::PauseUpdate;
			m_pDrawFunc = &Player::GameInvincibleDraw;
		}
		break;
	}
}

void Player::KeyCheck()
{
	const Lib::KeyDevice::KEYSTATE* pKey = SINGLETON_INSTANCE(Lib::InputDeviceManager)->GetKeyState();

	if (pKey[DIK_X] == Lib::KeyDevice::KEY_PUSH && m_IsHandRail == false && m_IsSyringe == false)
	{
		SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_RollSoundIndex)->SoundOperation(
			Lib::SoundManager::STOP_RESET);

		m_CurrentAnimation = m_JumpAnimationIndex;
		SINGLETON_INSTANCE(Lib::AnimationManager)->GetAnimation(m_CurrentAnimation)->AnimationStart();
		m_IsJump = true;
		m_IsJumpRampThrough = true;
		m_IsFirstJump = true;
		if (m_IsJumpRampCollision && m_Angle <= D3DXToRadian(-15))
		{
			if (m_IsGolden)
			{
				SINGLETON_INSTANCE(GameDataManager)->SetScore(
					SINGLETON_INSTANCE(GameDataManager)->GetScore() + 1000 * 2);
			}
			else
			{
				SINGLETON_INSTANCE(GameDataManager)->SetScore(
					SINGLETON_INSTANCE(GameDataManager)->GetScore() + 1000);
			}
			m_JumpSpeed = 31.f;
		}
		else
		{
			m_JumpSpeed = 25.f;
		}
	}

	if (m_IsSyringe == false)
	{
		if (pKey[DIK_Z] == Lib::KeyDevice::KEY_PUSH && m_IsHandRail == false && m_IsJumpRampCollision == false && m_IsJumpTrick == false)
		{
			m_IsJumpTrick = true;

			Lib::IAnimation* pAnimation;

			SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_RollSoundIndex)->SoundOperation(
				Lib::SoundManager::STOP_RESET);

			m_JumpTrickCount = 0;
			if (m_IsJump)
			{
				m_State = GAME_JUMPTRICK1_STATE;
				m_pUpdateFunc = &Player::GameJumpTrick1Update;
				m_pDrawFunc = &Player::GameJumpTrick1Draw;
				m_CurrentAnimation = m_JumpTrick1AnimationIndex;	// アニメーションを変更.
			}
			else
			{
				m_State = GAME_TRICK1_STATE;
				m_pUpdateFunc = &Player::GameTrick1Update;
				m_pDrawFunc = &Player::GameTrick1Draw;
				m_CurrentAnimation = m_Trick1AnimationIndex;	// アニメーションを変更.

				SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_Trick1SoundIndex)->SoundOperation(
					Lib::SoundManager::PLAY);
			}

			pAnimation = SINGLETON_INSTANCE(Lib::AnimationManager)->GetAnimation(m_CurrentAnimation);
			pAnimation->AnimationStart();

			return;
		}
	}
}

void Player::InvincibleKeyCheck()
{
	const Lib::KeyDevice::KEYSTATE* pKey = SINGLETON_INSTANCE(Lib::InputDeviceManager)->GetKeyState();

	if (pKey[DIK_X] == Lib::KeyDevice::KEY_PUSH && m_IsHandRail == false && m_IsSyringe == false)
	{
		SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_RollSoundIndex)->SoundOperation(
			Lib::SoundManager::STOP_RESET);

		m_CurrentAnimation = m_JumpAnimationIndex;
		SINGLETON_INSTANCE(Lib::AnimationManager)->GetAnimation(m_CurrentAnimation)->AnimationStart();
		m_IsJump = true;
		m_IsJumpRampThrough = true;
		m_IsFirstJump = true;
		if (m_IsJumpRampCollision && m_Angle <= D3DXToRadian(-15))
		{
			if (m_IsGolden)
			{
				SINGLETON_INSTANCE(GameDataManager)->SetScore(
					SINGLETON_INSTANCE(GameDataManager)->GetScore() + 1000 * 2);
			}
			else
			{
				SINGLETON_INSTANCE(GameDataManager)->SetScore(
					SINGLETON_INSTANCE(GameDataManager)->GetScore() + 1000);
			}
			m_JumpSpeed = 31.f;
		}
		else
		{
			m_JumpSpeed = 25.f;
		}
	}
}

void Player::FallControl()
{
	if (m_IsSyringe == false && m_IsHandRail == false)
	{
		if (m_IsJump)
		{
			if (m_IsFirstJump)
			{
				SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_JumpSoundIndex)->SoundOperation(
					Lib::SoundManager::PLAY);
				m_IsFirstJump = false;
			}
			float Speed = (m_JumpSpeed + (-m_Gravity * m_JumpTime));	// ジャンプ処理.
			m_Pos.y -= Speed;
			m_JumpTime++;
		}
		else
		{
			m_Pos.y += m_Gravity * m_FallTime;	// 通常落下処理.
		}
	}


	D3DXVECTOR2 OutPos, OutSize;
	float Progress = SINGLETON_INSTANCE(GameDataManager)->GetPlayerProgress();

	if (m_IsSyringe)
	{
		m_SyringeCount++;
		if (m_SyringeCount > 350)
		{
			switch (m_StageBackgroundEventType)
			{
			case StageBackgroundEvent::GAMEUPDATE_EVENT:
				SINGLETON_INSTANCE(GameDataManager)->SetPlayerSpeed(m_DefaultSpeed);
				break;
			case StageBackgroundEvent::STAGECHANGE_EVENT1:
				SINGLETON_INSTANCE(GameDataManager)->SetPlayerSpeed(m_DefaultSpeed2);
				break;
			case StageBackgroundEvent::STAGECHANGE_EVENT2:
				SINGLETON_INSTANCE(GameDataManager)->SetPlayerSpeed(m_DefaultSpeed3);
				break;
			case StageBackgroundEvent::STAGECHANGE_EVENT3:
				SINGLETON_INSTANCE(GameDataManager)->SetPlayerSpeed(m_DefaultSpeed4);
				break;
			}

			m_SyringeCount = 0;
			m_IsSyringe = false;

			m_Pos.y -= 100;
			m_Size = D3DXVECTOR2(128, 128);
			m_ColliderSize = D3DXVECTOR2(m_Size.x+30, m_Size.y - 20);

			m_CurrentAnimation = m_IdleAnimationIndex;

			m_State = GAME_INVINCIBLE_STATE;
			m_pUpdateFunc = &Player::GameInvincibleUpdate;
			m_pDrawFunc = &Player::GameInvincibleDraw;
		}
	}

	if (m_IsGolden)
	{
		m_GoldenCount++;
		if (m_GoldenCount > 1200)
		{
			m_IsGolden = false;
			SINGLETON_INSTANCE(GameDataManager)->SetIsGolden(false);
		}
	}

	GimmickManager::GIMMICK_TYPE Type;
	Type = SINGLETON_INSTANCE(CollisionManager)->GimmickCollision(
		&D3DXVECTOR2(m_Pos.x + Progress + m_FallColliderPosOffset.x, m_Pos.y + m_FallColliderPosOffset.y),
		&m_FallColliderSize,
		&OutPos,
		&OutSize);

	if (Type == GimmickManager::JUMPRAMP_GIMMICK)
	{
		if (m_IsJump)
		{
			m_IsJumpRampThrough = true;
		}
	}
	else if (Type == GimmickManager::HANRAIL_GIMMICK)
	{
	}
	else if (Type == GimmickManager::POINT_GIMMICK)
	{
		if (m_IsGolden)
		{
			SINGLETON_INSTANCE(GameDataManager)->SetScore(
				SINGLETON_INSTANCE(GameDataManager)->GetScore() + 10 * 2);
		}
		else
		{
			SINGLETON_INSTANCE(GameDataManager)->SetScore(
				SINGLETON_INSTANCE(GameDataManager)->GetScore() + 10);
		}
		SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_PointSEIndex)->SoundOperation(
			Lib::SoundManager::STOP_RESET);
		SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_PointSEIndex)->SoundOperation(
			Lib::SoundManager::PLAY);

	}
	else if (Type == GimmickManager::SYRINGE_GIMMICK)
	{
		SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_PointSEIndex2)->SoundOperation(
			Lib::SoundManager::STOP_RESET);
		SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_PointSEIndex2)->SoundOperation(
			Lib::SoundManager::PLAY);

		m_IsSyringe = true;

	}
	else if (Type == GimmickManager::GOLDEN_GIMMICK)
	{
		SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_PointSEIndex)->SoundOperation(
			Lib::SoundManager::STOP_RESET);
		SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_PointSEIndex)->SoundOperation(
			Lib::SoundManager::PLAY);

		m_IsGolden = true;
		m_GoldenCount = 0;
		SINGLETON_INSTANCE(GameDataManager)->SetIsGolden(true);
	}
	else if (Type != GimmickManager::NONE_GIMMICK &&
		m_IsCollision == false)
	{
		if (m_IsSyringe == false)
		{
			m_Pos.y = OutPos.y - OutSize.y / 2 - m_Size.y / 2;

			if (m_IsJump)
			{
				m_IsJump = false;
				m_CurrentAnimation = m_LandingAnimationIndex;///@todo 着地アニメーションに変更.
				SINGLETON_INSTANCE(Lib::AnimationManager)->GetAnimation(m_CurrentAnimation)->AnimationStart();
				m_JumpTime = 0;

				SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_LandingSoundIndex)->SoundOperation(
					Lib::SoundManager::PLAY);

				SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_RollSoundIndex)->SoundOperation(
					Lib::SoundManager::PLAY_LOOP);
			}

			m_FallTime = 0;
		}
	}
	else if (m_Pos.y + m_Size.y / 2 >= 708)	// 床との判定.
	{
		m_Pos.y = 708 - m_Size.y / 2;

		if (m_IsJump)
		{
			m_IsJump = false;
			m_CurrentAnimation = m_LandingAnimationIndex;///@todo 着地アニメーションに変更.
			SINGLETON_INSTANCE(Lib::AnimationManager)->GetAnimation(m_CurrentAnimation)->AnimationStart();
			m_JumpTime = 0;

			SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_LandingSoundIndex)->SoundOperation(
				Lib::SoundManager::PLAY);

			SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_RollSoundIndex)->SoundOperation(
				Lib::SoundManager::PLAY_LOOP);
		}

		m_FallTime = 0;
	}
	else
	{
		m_FallTime++;
	}
}

void Player::InvincibleTimeControl()
{
	// 無敵時間の更新.
	m_InvincibleTime--;
	if (m_InvincibleTime <= 0)
	{
		m_InvincibleTime = 200;

		m_State = GAME_STATE;
		m_pUpdateFunc = &Player::GameUpdate;
		m_pDrawFunc = &Player::GameDraw;

		// カラー値は初期値に.
		m_pVertex->SetColor(&D3DXCOLOR(1, 1, 1, 1));
		m_pVertex->WriteVertexBuffer();
	}

	// 点滅処理.
	if (m_IsInvincibleFlash)
	{
		m_InvincibleColor -= 0.1f;
		if (m_InvincibleColor <= 0.0f)
		{
			m_IsInvincibleFlash = false;
		}
	}
	else
	{
		m_InvincibleColor += 0.1f;
		if (m_InvincibleColor >= 1.0f)
		{
			m_IsInvincibleFlash = true;
		}
	}
}

void Player::InvincibleFallControl()
{
	if (m_IsHandRail == false)
	{
		if (m_IsJump)
		{
			if (m_IsFirstJump)
			{
				SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_JumpSoundIndex)->SoundOperation(
					Lib::SoundManager::PLAY);
				m_IsFirstJump = false;
			}
			float Speed = (m_JumpSpeed + (-m_Gravity * m_JumpTime));	// ジャンプ処理.
			m_Pos.y -= Speed;
			m_JumpTime++;
		}
		else
		{
			m_Pos.y += m_Gravity * m_FallTime;	// 通常落下処理.
		}
	}


	D3DXVECTOR2 OutPos, OutSize;
	float Progress = SINGLETON_INSTANCE(GameDataManager)->GetPlayerProgress();

	if (m_IsSyringe)
	{
		m_SyringeCount++;
		if (m_SyringeCount > 350)
		{
			switch (m_StageBackgroundEventType)
			{
			case StageBackgroundEvent::GAMEUPDATE_EVENT:
				SINGLETON_INSTANCE(GameDataManager)->SetPlayerSpeed(m_DefaultSpeed);
				break;
			case StageBackgroundEvent::STAGECHANGE_EVENT1:
				SINGLETON_INSTANCE(GameDataManager)->SetPlayerSpeed(m_DefaultSpeed2);
				break;
			case StageBackgroundEvent::STAGECHANGE_EVENT2:
				SINGLETON_INSTANCE(GameDataManager)->SetPlayerSpeed(m_DefaultSpeed3);
				break;
			case StageBackgroundEvent::STAGECHANGE_EVENT3:
				SINGLETON_INSTANCE(GameDataManager)->SetPlayerSpeed(m_DefaultSpeed4);
				break;
			}

			m_SyringeCount = 0;
			m_IsSyringe = false;

			m_Pos.y -= 100;
			m_Size = D3DXVECTOR2(128, 128);
			m_ColliderSize = D3DXVECTOR2(m_Size.x+30, m_Size.y - 20);

			m_CurrentAnimation = m_IdleAnimationIndex;

			m_State = GAME_INVINCIBLE_STATE;
			m_pUpdateFunc = &Player::GameInvincibleUpdate;
			m_pDrawFunc = &Player::GameInvincibleDraw;
		}
	}

	if (m_IsGolden)
	{
		m_GoldenCount++;
		if (m_GoldenCount > 1200)
		{
			m_IsGolden = false;
			SINGLETON_INSTANCE(GameDataManager)->SetIsGolden(false);
		}
	}

	GimmickManager::GIMMICK_TYPE Type;
	Type = SINGLETON_INSTANCE(CollisionManager)->GimmickCollision(
		&D3DXVECTOR2(m_Pos.x + Progress + m_FallColliderPosOffset.x, m_Pos.y + m_FallColliderPosOffset.y),
		&m_FallColliderSize,
		&OutPos,
		&OutSize);

	if (Type == GimmickManager::JUMPRAMP_GIMMICK)
	{
		if (m_IsJump)
		{
			m_IsJumpRampThrough = true;
		}
	}
	else if (Type == GimmickManager::HANRAIL_GIMMICK)
	{
	}
	else if (Type == GimmickManager::POINT_GIMMICK)
	{
		if (m_IsGolden)
		{
			SINGLETON_INSTANCE(GameDataManager)->SetScore(
				SINGLETON_INSTANCE(GameDataManager)->GetScore() + 10 * 2);
		}
		else
		{
			SINGLETON_INSTANCE(GameDataManager)->SetScore(
				SINGLETON_INSTANCE(GameDataManager)->GetScore() + 10);
		}
		SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_PointSEIndex)->SoundOperation(
			Lib::SoundManager::STOP_RESET);
		SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_PointSEIndex)->SoundOperation(
			Lib::SoundManager::PLAY);

	}
	else if (Type == GimmickManager::SYRINGE_GIMMICK)
	{
		SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_PointSEIndex2)->SoundOperation(
			Lib::SoundManager::STOP_RESET);
		SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_PointSEIndex2)->SoundOperation(
			Lib::SoundManager::PLAY);

		m_IsSyringe = true;

	}
	else if (Type == GimmickManager::GOLDEN_GIMMICK)
	{
		SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_PointSEIndex)->SoundOperation(
			Lib::SoundManager::STOP_RESET);
		SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_PointSEIndex)->SoundOperation(
			Lib::SoundManager::PLAY);

		m_IsGolden = true;
		m_GoldenCount = 0;
		SINGLETON_INSTANCE(GameDataManager)->SetIsGolden(true);
	}
	else if (Type == GimmickManager::SCAFFOLD_GIMMICK)
	{
		if (m_IsSyringe == false)
		{
			m_Pos.y = OutPos.y - OutSize.y / 2 - m_Size.y / 2;

			if (m_IsJump)
			{
				m_IsJump = false;
				m_CurrentAnimation = m_LandingAnimationIndex;///@todo 着地アニメーションに変更.
				SINGLETON_INSTANCE(Lib::AnimationManager)->GetAnimation(m_CurrentAnimation)->AnimationStart();
				m_JumpTime = 0;

				SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_LandingSoundIndex)->SoundOperation(
					Lib::SoundManager::PLAY);

				SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_RollSoundIndex)->SoundOperation(
					Lib::SoundManager::PLAY_LOOP);
			}


			m_FallTime = 0;
		}
	}
	else if (Type != GimmickManager::NONE_GIMMICK &&
		m_IsCollision == false)
	{
		if (m_IsSyringe == false)
		{
			m_Pos.y = OutPos.y - OutSize.y / 2 - m_Size.y / 2;

			if (m_IsJump)
			{
				m_IsJump = false;
				m_CurrentAnimation = m_LandingAnimationIndex;///@todo 着地アニメーションに変更.
				SINGLETON_INSTANCE(Lib::AnimationManager)->GetAnimation(m_CurrentAnimation)->AnimationStart();
				m_JumpTime = 0;

				SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_LandingSoundIndex)->SoundOperation(
					Lib::SoundManager::PLAY);

				SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_RollSoundIndex)->SoundOperation(
					Lib::SoundManager::PLAY_LOOP);
			}


			m_FallTime = 0;
		}
	}
	else if (m_Pos.y + m_Size.y / 2 >= 708)	// 床との判定.
	{
		m_Pos.y = 708 - m_Size.y / 2;

		if (m_IsJump)
		{
			m_IsJump = false;
			m_CurrentAnimation = m_LandingAnimationIndex;///@todo 着地アニメーションに変更.
			SINGLETON_INSTANCE(Lib::AnimationManager)->GetAnimation(m_CurrentAnimation)->AnimationStart();
			m_JumpTime = 0;

			SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_LandingSoundIndex)->SoundOperation(
				Lib::SoundManager::PLAY);

			SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_RollSoundIndex)->SoundOperation(
				Lib::SoundManager::PLAY_LOOP);
		}

		m_FallTime = 0;
	}
	else
	{
		m_FallTime++;
	}
}

void Player::PointControl()
{
	// 移動によるスコア制御.
	m_CoolPointCounter++;
	if (m_CoolPointCounter >= 30)	// 0.5秒経過していたら.
	{
		int CurrentScore = SINGLETON_INSTANCE(GameDataManager)->GetScore();
		m_CoolPointCounter = 0;

		if (m_IsGolden)
		{
			SINGLETON_INSTANCE(GameDataManager)->SetScore(CurrentScore + m_TimeCoolPoint * 2);
		}
		else
		{
			SINGLETON_INSTANCE(GameDataManager)->SetScore(CurrentScore + m_TimeCoolPoint);
		}
	}


	if (m_IsHandRail)
	{
		m_HandrailCoolPointCounter++;
		if (m_HandrailCoolPointCounter >= 4)
		{
			int CurrentScore = SINGLETON_INSTANCE(GameDataManager)->GetScore();
			m_HandrailCoolPointCounter = 0;
			if (m_IsGolden)
			{
				SINGLETON_INSTANCE(GameDataManager)->SetScore(CurrentScore + m_HandrailCoolPoint * 2);
			}
			else
			{
				SINGLETON_INSTANCE(GameDataManager)->SetScore(CurrentScore + m_HandrailCoolPoint);
			}
		}
	}
	else
	{
		m_HandrailCoolPointCounter = 0;
	}
}

void Player::TrickCollision()
{
	// 衝突判定.
	float Progress = SINGLETON_INSTANCE(GameDataManager)->GetPlayerProgress();

	if (m_IsSyringe)
	{
		// 無敵状態ならギミックを破壊する
		GimmickManager::GIMMICK_TYPE Type = SINGLETON_INSTANCE(CollisionManager)->DestroyGimmick(
			&D3DXVECTOR2(m_Pos.x + m_ColliderPosOffset.x + Progress, m_Pos.y + m_ColliderPosOffset.y),
			&m_ColliderSize);
		while (Type != GimmickManager::NONE_GIMMICK)
		{
			int CurrentScore = SINGLETON_INSTANCE(GameDataManager)->GetScore();
			switch (Type)
			{
			case GimmickManager::OBSTACLE_GIMMICK:
				SINGLETON_INSTANCE(GameDataManager)->SetScore(CurrentScore + 100);
				break;
			case GimmickManager::OBSTACLECAR_GIMMICK:
				SINGLETON_INSTANCE(GameDataManager)->SetScore(CurrentScore + 500);
				break;
			}

			Type = SINGLETON_INSTANCE(CollisionManager)->DestroyGimmick(
				&D3DXVECTOR2(m_Pos.x + m_ColliderPosOffset.x + Progress, m_Pos.y + m_ColliderPosOffset.y),
				&m_ColliderSize);
		}

		int CurrentScore = SINGLETON_INSTANCE(GameDataManager)->GetScore();
		switch (Type)
		{
		case GimmickManager::OBSTACLE_GIMMICK:
			SINGLETON_INSTANCE(GameDataManager)->SetScore(CurrentScore + 100);
			break;
		case GimmickManager::OBSTACLECAR_GIMMICK:
			SINGLETON_INSTANCE(GameDataManager)->SetScore(CurrentScore + 500);
			break;
		}

		m_SyringeCount++;
		if (m_SyringeCount > 350)
		{
			switch (m_StageBackgroundEventType)
			{
			case StageBackgroundEvent::GAMEUPDATE_EVENT:
				SINGLETON_INSTANCE(GameDataManager)->SetPlayerSpeed(m_DefaultSpeed);
				break;
			case StageBackgroundEvent::STAGECHANGE_EVENT1:
				SINGLETON_INSTANCE(GameDataManager)->SetPlayerSpeed(m_DefaultSpeed2);
				break;
			case StageBackgroundEvent::STAGECHANGE_EVENT2:
				SINGLETON_INSTANCE(GameDataManager)->SetPlayerSpeed(m_DefaultSpeed3);
				break;
			case StageBackgroundEvent::STAGECHANGE_EVENT3:
				SINGLETON_INSTANCE(GameDataManager)->SetPlayerSpeed(m_DefaultSpeed4);
				break;
			}

			m_SyringeCount = 0;
			m_IsSyringe = false;

			m_Pos.y -= 100;
			m_Size = D3DXVECTOR2(128, 128);
			m_ColliderSize = D3DXVECTOR2(m_Size.x+30, m_Size.y - 20);

			m_CurrentAnimation = m_IdleAnimationIndex;

			m_State = GAME_INVINCIBLE_STATE;
			m_pUpdateFunc = &Player::GameInvincibleUpdate;
			m_pDrawFunc = &Player::GameInvincibleDraw;
		}
	}

	if (m_IsGolden)
	{
		m_GoldenCount++;
		if (m_GoldenCount > 1200)
		{
			m_IsGolden = false;
			SINGLETON_INSTANCE(GameDataManager)->SetIsGolden(false);
		}
	}

	GimmickManager::GIMMICK_TYPE Type;
	Type = SINGLETON_INSTANCE(CollisionManager)->GimmickCollision(
		&D3DXVECTOR2(m_Pos.x + m_ColliderPosOffset.x + Progress, m_Pos.y + m_ColliderPosOffset.y),
		&m_ColliderSize);

	if (Type != GimmickManager::NONE_GIMMICK)
	{
		switch (Type)
		{
		case GimmickManager::JUMPRAMP_GIMMICK:
			break;
		case GimmickManager::POINT_GIMMICK:
			if (m_IsGolden)
			{
				SINGLETON_INSTANCE(GameDataManager)->SetScore(
					SINGLETON_INSTANCE(GameDataManager)->GetScore() + 10 * 2);
			}
			else
			{
				SINGLETON_INSTANCE(GameDataManager)->SetScore(
					SINGLETON_INSTANCE(GameDataManager)->GetScore() + 10);
			}

			SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_PointSEIndex)->SoundOperation(
				Lib::SoundManager::STOP_RESET);
			SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_PointSEIndex)->SoundOperation(
				Lib::SoundManager::PLAY);

			break;
		case GimmickManager::SYRINGE_GIMMICK:
			SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_PointSEIndex2)->SoundOperation(
				Lib::SoundManager::STOP_RESET);
			SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_PointSEIndex2)->SoundOperation(
				Lib::SoundManager::PLAY);

			m_IsSyringe = true;

			break;
		case GimmickManager::GOLDEN_GIMMICK:
			SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_PointSEIndex)->SoundOperation(
				Lib::SoundManager::STOP_RESET);
			SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_PointSEIndex)->SoundOperation(
				Lib::SoundManager::PLAY);

			m_IsGolden = true;
			m_GoldenCount = 0;
			SINGLETON_INSTANCE(GameDataManager)->SetIsGolden(true);
			break;
		case GimmickManager::SCAFFOLD_GIMMICK:
			break;
		case GimmickManager::HANRAIL_GIMMICK:
			break;
		default:
			if (m_IsSyringe == false)
			{
				if (m_State != GAME_INVINCIBLE_STATE)
				{
					SINGLETON_INSTANCE(GameDataManager)->SetLife(SINGLETON_INSTANCE(GameDataManager)->GetLife() - 1);

					m_State = GAME_INVINCIBLE_STATE;
					m_pUpdateFunc = &Player::GameInvincibleUpdate;
					m_pDrawFunc = &Player::GameInvincibleDraw;

					// ダメージ音声再生.
					SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_DamageSoundIndex)->SoundOperation(
						Lib::SoundManager::PLAY);
				}
			}

			m_CurrentAnimation = m_IdleAnimationIndex;	///@todo ダメージ時のアニメーションがないので代用.

			m_IsCollision = true;
			break;
		}
	}
	else
	{
		m_IsCollision = false;
	}
}

void Player::TrickFallControl()
{
	if (m_IsJump)
	{
		if (m_IsFirstJump)
		{
			SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_JumpSoundIndex)->SoundOperation(
				Lib::SoundManager::PLAY);
			m_IsFirstJump = false;
		}
		float Speed = (m_JumpSpeed + (-m_Gravity * m_JumpTime));	// ジャンプ処理.
		if (Speed < 0 && m_JumpTrickCount < 10)
		{
			m_JumpTrickCount++;
		}
		else
		{
			m_Pos.y -= Speed;
			m_JumpTime++;
		}
	}
	else
	{
		m_Pos.y += m_Gravity * m_FallTime;	// 通常落下処理.
	}

	D3DXVECTOR2 OutPos, OutSize;
	float Progress = SINGLETON_INSTANCE(GameDataManager)->GetPlayerProgress();

	if (m_IsSyringe)
	{
		m_SyringeCount++;
		if (m_SyringeCount > 350)
		{
			switch (m_StageBackgroundEventType)
			{
			case StageBackgroundEvent::GAMEUPDATE_EVENT:
				SINGLETON_INSTANCE(GameDataManager)->SetPlayerSpeed(m_DefaultSpeed);
				break;
			case StageBackgroundEvent::STAGECHANGE_EVENT1:
				SINGLETON_INSTANCE(GameDataManager)->SetPlayerSpeed(m_DefaultSpeed2);
				break;
			case StageBackgroundEvent::STAGECHANGE_EVENT2:
				SINGLETON_INSTANCE(GameDataManager)->SetPlayerSpeed(m_DefaultSpeed3);
				break;
			case StageBackgroundEvent::STAGECHANGE_EVENT3:
				SINGLETON_INSTANCE(GameDataManager)->SetPlayerSpeed(m_DefaultSpeed4);
				break;
			}

			m_SyringeCount = 0;
			m_IsSyringe = false;

			m_Pos.y -= 100;
			m_Size = D3DXVECTOR2(128, 128);
			m_ColliderSize = D3DXVECTOR2(m_Size.x+30, m_Size.y - 20);

			m_CurrentAnimation = m_IdleAnimationIndex;

			m_State = GAME_INVINCIBLE_STATE;
			m_pUpdateFunc = &Player::GameInvincibleUpdate;
			m_pDrawFunc = &Player::GameInvincibleDraw;
		}
	}

	if (m_IsGolden)
	{
		m_GoldenCount++;
		if (m_GoldenCount > 1200)
		{
			m_IsGolden = false;
			SINGLETON_INSTANCE(GameDataManager)->SetIsGolden(false);
		}
	}

	GimmickManager::GIMMICK_TYPE Type;
	Type = SINGLETON_INSTANCE(CollisionManager)->GimmickCollision(
		&D3DXVECTOR2(m_Pos.x + Progress + m_FallColliderPosOffset.x, m_Pos.y + m_FallColliderPosOffset.y),
		&m_FallColliderSize,
		&OutPos,
		&OutSize);

	if (Type == GimmickManager::JUMPRAMP_GIMMICK)
	{
		if (m_IsJump)
		{
			m_IsJumpRampThrough = true;
		}
	}
	else if (Type == GimmickManager::HANRAIL_GIMMICK)
	{
	}
	else if (Type == GimmickManager::POINT_GIMMICK)
	{
		if (m_IsGolden)
		{
			SINGLETON_INSTANCE(GameDataManager)->SetScore(
				SINGLETON_INSTANCE(GameDataManager)->GetScore() + 10 * 2);
		}
		else
		{
			SINGLETON_INSTANCE(GameDataManager)->SetScore(
				SINGLETON_INSTANCE(GameDataManager)->GetScore() + 10);
		}

		SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_PointSEIndex)->SoundOperation(
			Lib::SoundManager::STOP_RESET);
		SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_PointSEIndex)->SoundOperation(
			Lib::SoundManager::PLAY);

	}
	else if (Type == GimmickManager::SYRINGE_GIMMICK)
	{
		SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_PointSEIndex2)->SoundOperation(
			Lib::SoundManager::STOP_RESET);
		SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_PointSEIndex2)->SoundOperation(
			Lib::SoundManager::PLAY);

		m_IsSyringe = true;

	}
	else if (Type == GimmickManager::GOLDEN_GIMMICK)
	{
		SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_PointSEIndex)->SoundOperation(
			Lib::SoundManager::STOP_RESET);
		SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_PointSEIndex)->SoundOperation(
			Lib::SoundManager::PLAY);

		m_IsGolden = true;
		m_GoldenCount = 0;
		SINGLETON_INSTANCE(GameDataManager)->SetIsGolden(true);
	}
	else if (Type != GimmickManager::NONE_GIMMICK &&
		m_IsCollision == false)
	{
		if (m_IsSyringe == false)
		{
			m_Pos.y = OutPos.y - OutSize.y / 2 - m_Size.y / 2;

			m_IsJump = false;
			m_JumpTime = 0;

			m_FallTime = 0;

			SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_RollSoundIndex)->SoundOperation(
				Lib::SoundManager::PLAY_LOOP);
		}
	}
	else if (m_Pos.y + m_Size.y / 2 >= 708)	// 床との判定.
	{
		m_Pos.y = 708 - m_Size.y / 2;

		m_IsJump = false;
		m_JumpTime = 0;

		m_FallTime = 0;

		SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_RollSoundIndex)->SoundOperation(
			Lib::SoundManager::PLAY_LOOP);
	}
	else
	{
		m_FallTime++;
	}
}

