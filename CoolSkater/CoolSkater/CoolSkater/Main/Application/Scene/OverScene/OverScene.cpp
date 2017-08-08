/**
 * @file	OverScene.cpp
 * @brief	ゲームオーバーシーン実装
 * @author	morimoto
 */

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "OverScene.h"

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
#include "OverBackground\OverBackground.h"
#include "OverLogo\OverLogo.h"
#include "OverText\OverText.h"
#include "OverClip\OverClip.h"


//----------------------------------------------------------------------
// Constructor	Destructor
//----------------------------------------------------------------------
OverScene::OverScene(int _sceneId) : 
	SceneBase(_sceneId)
{
}


OverScene::~OverScene()
{
}


//----------------------------------------------------------------------
// Public Functions
//----------------------------------------------------------------------
bool OverScene::Initialize()
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
		"Resource\\OverScene\\OverScene.wav",
		&m_OverSoundIndex))
	{
		OutputErrorLog("サウンドの読み込みに失敗しました");
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::SoundManager)->LoadSound(
		"Resource\\TitleScene\\Select.wav",
		&m_SelectIndex))
	{
		OutputErrorLog("サウンドの読み込みに失敗しました");
		return false;
	}

	SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_OverSoundIndex)->SoundOperation(
		Lib::SoundManager::PLAY_LOOP);


	m_pOverBackground = new OverBackground();
	if (!m_pOverBackground->Initialize())
	{
		return false;
	}

	m_pOverLogo = new OverLogo();
	if (!m_pOverLogo->Initialize())
	{
		return false;
	}

	m_pOverText = new OverText();
	if (!m_pOverText->Initialize())
	{
		return false;
	}

	m_pOverClip = new OverClip();
	if (!m_pOverClip->Initialize())
	{
		return false;
	}

	m_Counter = 0;
	m_IsSceneChange = false;

	m_State = UPDATE_STATE;

	return true;
}

void OverScene::Finalize()
{
	m_pOverClip->Finalize();
	delete m_pOverClip;

	m_pOverText->Finalize();
	delete m_pOverText;

	m_pOverLogo->Finalize();
	delete m_pOverLogo;

	m_pOverBackground->Finalize();
	delete m_pOverBackground;


	if (SINGLETON_INSTANCE(Lib::SoundManager) != nullptr)
	{
		SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_OverSoundIndex)->SoundOperation(
			Lib::SoundManager::STOP_RESET);

		SINGLETON_INSTANCE(Lib::SoundManager)->ReleaseSound(m_SelectIndex);
		SINGLETON_INSTANCE(Lib::SoundManager)->ReleaseSound(m_OverSoundIndex);
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

void OverScene::Update()
{
	// デバイスの入力チェック.
	SINGLETON_INSTANCE(Lib::InputDeviceManager)->KeyUpdate();
	SINGLETON_INSTANCE(Lib::InputDeviceManager)->KeyCheck(DIK_Z);
	SINGLETON_INSTANCE(Lib::InputDeviceManager)->KeyCheck(DIK_LSHIFT);

	// 更新処理.
	SINGLETON_INSTANCE(Lib::UpdateTaskManager)->Run();

	// 描画処理.
	SINGLETON_INSTANCE(Lib::GraphicsDevice)->BeginScene(Lib::GraphicsDevice::BACKBUFFER_TARGET);
	SINGLETON_INSTANCE(Lib::DrawTaskManager)->Run();
	SINGLETON_INSTANCE(Lib::GraphicsDevice)->EndScene();


	if (m_IsSceneChange == false)
	{
		if (SINGLETON_INSTANCE(Lib::InputDeviceManager)->GetKeyState()[DIK_Z] == Lib::KeyDevice::KEY_PUSH)
		{
			m_NextSceneID = Application::TITLE_SCENE_ID;
			m_IsSceneChange = true;

			SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_SelectIndex)->SoundOperation(
				Lib::SoundManager::PLAY);
		}
		else if (SINGLETON_INSTANCE(Lib::InputDeviceManager)->GetKeyState()[DIK_LSHIFT] == Lib::KeyDevice::KEY_PUSH)
		{
			m_NextSceneID = Application::GAME_SCENE_ID;
			m_IsSceneChange = true;

			SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_SelectIndex)->SoundOperation(
				Lib::SoundManager::PLAY);
		}
	}
	else
	{
		m_Counter++;
		if (m_Counter > 150)
		{
			m_State = FINAL_STATE;
		}
	}
}

