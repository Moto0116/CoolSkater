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


//----------------------------------------------------------------------
// Static Private Variables
//----------------------------------------------------------------------
const int Player::m_DefaultHp = 2;
const float Player::m_DefaultSpeed = 9.0f;


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
	m_IsTrick1(false),
	m_IsTrick2(false),
	m_IsTrick3(false)
{
	m_Pos = D3DXVECTOR2(200, 700);
	m_Size = D3DXVECTOR2(128, 256);

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
		"Resource\\GameScene\\Player.png",
		&m_TextureIndex))
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


	Lib::IAnimation* Animation;
	Animation = SINGLETON_INSTANCE(Lib::AnimationManager)->GetAnimation(m_Trick1AnimationIndex);
	Animation->SetAnimationPattern(Lib::IAnimation::LOOP_ANIMATION);
	Animation->SetAnimationSpeed(0.1f);
	Animation->AnimationStart();

	Animation = SINGLETON_INSTANCE(Lib::AnimationManager)->GetAnimation(m_Trick2AnimationIndex);
	Animation->SetAnimationPattern(Lib::IAnimation::LOOP_ANIMATION);
	Animation->SetAnimationSpeed(0.1f);
	Animation->AnimationStart();

	Animation = SINGLETON_INSTANCE(Lib::AnimationManager)->GetAnimation(m_Trick3AnimationIndex);
	Animation->SetAnimationPattern(Lib::IAnimation::LOOP_ANIMATION);
	Animation->SetAnimationSpeed(0.1f);
	Animation->AnimationStart();

	m_CurrentAnimation = m_IdleAnimationIndex;


	m_pEventListener = new PlayerEventListener(this, &Player::ReciveEvent);
	SINGLETON_INSTANCE(Lib::EventManager)->AddEventListener(m_pEventListener);

	SINGLETON_INSTANCE(GameDataManager)->SetPlayerSpeed(m_Speed);

	return true;
}

void Player::Finalize()
{
	SINGLETON_INSTANCE(Lib::EventManager)->RemoveEventListener(m_pEventListener);
	delete m_pEventListener;

	SINGLETON_INSTANCE(Lib::AnimationManager)->ReleaseAnimation(m_Trick3AnimationIndex);
	SINGLETON_INSTANCE(Lib::AnimationManager)->ReleaseAnimation(m_Trick2AnimationIndex);
	SINGLETON_INSTANCE(Lib::AnimationManager)->ReleaseAnimation(m_Trick1AnimationIndex);
	SINGLETON_INSTANCE(Lib::AnimationManager)->ReleaseAnimation(m_IdleAnimationIndex);

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
void Player::PauseUpdate()
{
}

void Player::GameStartUpdate()
{
}

void Player::GameUpdate()
{
	SINGLETON_INSTANCE(Lib::AnimationManager)->GetAnimation(m_CurrentAnimation)->Update();

	KeyCheck();
	FallControl();
}

void Player::PauseDraw()
{
	m_pVertex->SetAnimation(SINGLETON_INSTANCE(Lib::AnimationManager)->GetAnimation(m_CurrentAnimation));
	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&m_Pos);
	m_pVertex->Draw();
}

void Player::GameStartDraw()
{
	m_pVertex->SetAnimation(SINGLETON_INSTANCE(Lib::AnimationManager)->GetAnimation(m_CurrentAnimation));
	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&m_Pos);
	m_pVertex->Draw();
}

void Player::GameDraw()
{
	m_pVertex->SetAnimation(SINGLETON_INSTANCE(Lib::AnimationManager)->GetAnimation(m_CurrentAnimation));
	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&m_Pos);
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
	case GameScene::GAMEUPDATE_EVENT:
		m_State = GAME_STATE;
		m_pUpdateFunc = &Player::GameUpdate;
		m_pDrawFunc = &Player::GameDraw;
		break;
	}
}

void Player::KeyCheck()
{
	const Lib::KeyDevice::KEYSTATE* pKey = SINGLETON_INSTANCE(Lib::InputDeviceManager)->GetKeyState();

	if (pKey[DIK_SPACE] == Lib::KeyDevice::KEY_PUSH)
	{
		m_IsJump = true;
	}

	if (pKey[DIK_Q] == Lib::KeyDevice::KEY_PUSH)
	{
		m_IsTrick1 = true;
	}

	if (pKey[DIK_W] == Lib::KeyDevice::KEY_PUSH)
	{
		m_IsTrick2 = true;
	}

	if (pKey[DIK_E] == Lib::KeyDevice::KEY_PUSH)
	{
		m_IsTrick3 = true;
	}
}

void Player::FallControl()
{
	if (m_IsJump)	
	{
		float Speed = (m_JumpSpeed + (-m_Gravity * m_JumpTime));	// ジャンプ処理.
		m_Pos.y -= Speed;
		m_JumpTime++;
	}
	else
	{
		m_Pos.y += m_Gravity * m_FallTime;	// 通常落下処理.
	}

	if (m_Pos.y + m_Size.y / 2 >= 828)	// 床との判定.
	{
		m_Pos.y = 828 - m_Size.y / 2;

		m_JumpTime = 0;
		m_IsJump = false;

		m_FallTime = 0;
	}
	else
	{
		m_FallTime++;
	}
}

