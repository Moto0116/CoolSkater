/**
 * @file	TitleScene.cpp
 * @brief	タイトルシーンクラス実装
 * @author	morimoto
 */

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "TitleScene.h"

#include "..\..\Application.h"
#include "Debugger\Debugger.h"
#include "DirectX11\GraphicsDevice\GraphicsDevice.h"
#include "DirectX11\ShaderManager\ShaderManager.h"
#include "DirectX11\TextureManager\TextureManager.h"
#include "DirectX11\SoundDevice\SoundDevice.h"
#include "DirectX11\SoundManager\SoundManager.h"
#include "DirectX11\SoundManager\ISound\ISound.h"
#include "InputDeviceManager\InputDeviceManager.h"
#include "TaskManager\TaskBase\DrawTask\DrawTask.h"
#include "TaskManager\TaskBase\UpdateTask\UpdateTask.h"
#include "TitleBackground\TitleBackground.h"
#include "TitleLogo\TitleLogo.h"
#include "TitleText\TitleText.h"
#include "TitleClip\TitleClip.h"


//----------------------------------------------------------------------
// Constructor	Destructor
//----------------------------------------------------------------------
TitleScene::TitleScene(int _sceneId) :
	SceneBase(_sceneId),
	m_IsGameStart(false),
	m_IsScoreStart(false),
	m_Counter(0)
{
}

TitleScene::~TitleScene()
{
}


//----------------------------------------------------------------------
// Public Functions
//----------------------------------------------------------------------
bool TitleScene::Initialize()
{
	SINGLETON_CREATE(Lib::UpdateTaskManager);
	SINGLETON_CREATE(Lib::DrawTaskManager);


	SINGLETON_CREATE(Lib::ShaderManager);
	if (!SINGLETON_INSTANCE(Lib::ShaderManager)->Initialize(SINGLETON_INSTANCE(Lib::GraphicsDevice)))
	{
		OutputErrorLog("シェーダー管理クラスの生成に失敗しました");
		return false;
	}

	SINGLETON_CREATE(Lib::TextureManager);
	if (!SINGLETON_INSTANCE(Lib::TextureManager)->Initialize(SINGLETON_INSTANCE(Lib::GraphicsDevice)))
	{
		OutputErrorLog("テクスチャ管理クラスの生成に失敗しました");
		return false;
	}

	SINGLETON_CREATE(Lib::SoundDevice);
	if (!SINGLETON_INSTANCE(Lib::SoundDevice)->Initialize(
		SINGLETON_INSTANCE(Lib::GraphicsDevice)->GetMainWindowHandle()))
	{
		OutputErrorLog("サウンドデバイスの生成に失敗しました");
		return false;
	}

	SINGLETON_CREATE(Lib::SoundManager);
	if (!SINGLETON_INSTANCE(Lib::SoundManager)->Initialize(SINGLETON_INSTANCE(Lib::SoundDevice)))
	{
		OutputErrorLog("サウンド管理クラスの生成に失敗しました");
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::SoundManager)->LoadSound(
		"Resource\\TitleScene\\TitleSceneBGM.wav",
		&m_TitleSoundIndex))
	{
		OutputErrorLog("サウンドの読み込みに失敗しました");
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::SoundManager)->LoadSound(
		"Resource\\TitleScene\\Select.wav",
		&m_TitleSelectIndex))
	{
		OutputErrorLog("サウンドの読み込みに失敗しました");
		return false;
	}

	SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_TitleSoundIndex)->SoundOperation(
		Lib::SoundManager::PLAY_LOOP);

	m_pBackground = new TitleBackground();
	if (!m_pBackground->Initialize())
	{
		return false;
	}

	m_pTitleLogo = new TitleLogo();
	if (!m_pTitleLogo->Initialize())
	{
		return false;
	}

	m_pTitleText = new TitleText();
	if (!m_pTitleText->Initialize())
	{
		return false;
	}

	m_pTitleClip = new TitleClip();
	if (!m_pTitleClip->Initialize())
	{
		return false;
	}

	m_IsGameStart = false;
	m_IsScoreStart = false;
	m_Counter = 0;

	m_State = UPDATE_STATE;

	return true;
}

void TitleScene::Finalize()
{
	m_pTitleClip->Finalize();
	delete m_pTitleClip;

	m_pTitleText->Finalize();
	delete m_pTitleText;

	m_pTitleLogo->Finalize();
	delete m_pTitleLogo;

	m_pBackground->Finalize();
	delete m_pBackground;

	if (SINGLETON_INSTANCE(Lib::SoundManager) != nullptr)
	{
		SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_TitleSoundIndex)->SoundOperation(
			Lib::SoundManager::STOP_RESET);

		SINGLETON_INSTANCE(Lib::SoundManager)->ReleaseSound(m_TitleSelectIndex);
		SINGLETON_INSTANCE(Lib::SoundManager)->ReleaseSound(m_TitleSoundIndex);
		SINGLETON_INSTANCE(Lib::SoundManager)->Finalize();
		SINGLETON_DELETE(Lib::SoundManager);
	}

	if (SINGLETON_INSTANCE(Lib::SoundDevice) != nullptr)
	{
		SINGLETON_INSTANCE(Lib::SoundDevice)->Finalize();
		SINGLETON_DELETE(Lib::SoundDevice);
	}

	if (SINGLETON_INSTANCE(Lib::TextureManager) != nullptr)
	{
		SINGLETON_INSTANCE(Lib::TextureManager)->Finalize();
		SINGLETON_DELETE(Lib::TextureManager);
	}

	if (SINGLETON_INSTANCE(Lib::ShaderManager) != nullptr)
	{
		SINGLETON_INSTANCE(Lib::ShaderManager)->Finalize();
		SINGLETON_DELETE(Lib::ShaderManager);
	}

	SINGLETON_DELETE(Lib::DrawTaskManager);
	SINGLETON_DELETE(Lib::UpdateTaskManager);

	m_State = INIT_STATE;
}

void TitleScene::Update()
{	
	// デバイスの入力チェック.
	SINGLETON_INSTANCE(Lib::InputDeviceManager)->KeyUpdate();
	SINGLETON_INSTANCE(Lib::InputDeviceManager)->KeyCheck(DIK_Z);
	SINGLETON_INSTANCE(Lib::InputDeviceManager)->KeyCheck(DIK_A);
	SINGLETON_INSTANCE(Lib::InputDeviceManager)->KeyCheck(DIK_LSHIFT);
	SINGLETON_INSTANCE(Lib::InputDeviceManager)->MouseUpdate();

	// 更新処理.
	SINGLETON_INSTANCE(Lib::UpdateTaskManager)->Run();

	// 描画処理.
	SINGLETON_INSTANCE(Lib::GraphicsDevice)->BeginScene(Lib::GraphicsDevice::BACKBUFFER_TARGET);
	SINGLETON_INSTANCE(Lib::DrawTaskManager)->Run();
	SINGLETON_INSTANCE(Lib::GraphicsDevice)->EndScene();



	if (m_IsGameStart == false && m_IsScoreStart == false)
	{
		if (SINGLETON_INSTANCE(Lib::InputDeviceManager)->GetKeyState()[DIK_Z] == Lib::KeyDevice::KEY_PUSH)
		{
			SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_TitleSelectIndex)->SoundOperation(
				Lib::SoundManager::PLAY);

			m_IsGameStart = true;

			m_pTitleClip->SetIsCliping(true);
			m_pTitleClip->SetIsFadeIn(false);
		}
		else if (SINGLETON_INSTANCE(Lib::InputDeviceManager)->GetKeyState()[DIK_LSHIFT] == Lib::KeyDevice::KEY_PUSH)
		{
			SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_TitleSelectIndex)->SoundOperation(
				Lib::SoundManager::PLAY);

			m_IsScoreStart = true;

			m_pTitleClip->SetIsCliping(true);
			m_pTitleClip->SetIsFadeIn(false);
		}
	}
	else
	{
		if (m_IsGameStart == true)
		{
			m_Counter++;
			if (m_Counter == 110)
			{
				m_State = FINAL_STATE;
				m_NextSceneID = Application::GAME_SCENE_ID;
			}
		}
		else if (m_IsScoreStart == true)
		{
			m_Counter++;
			if (m_Counter == 110)
			{
				m_State = FINAL_STATE;
				m_NextSceneID = Application::SCORE_SCENE_ID;
			}
		}
	}
}
