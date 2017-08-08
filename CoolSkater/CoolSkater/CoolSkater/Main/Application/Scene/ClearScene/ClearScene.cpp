/**
 * @file	ClearScene.cpp
 * @brief	クリアシーンクラス実装
 * @author	morimoto
 */

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "ClearScene.h"

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
#include "ClearBackground\ClearBackground.h"
#include "ClearLogo\ClearLogo.h"
#include "ClearText\ClearText.h"
#include "ClearClip\ClearClip.h"


//----------------------------------------------------------------------
// Constructor	Destructor
//----------------------------------------------------------------------
ClearScene::ClearScene(int _sceneId) : 
	SceneBase(_sceneId)
{
}

ClearScene::~ClearScene()
{
}


//----------------------------------------------------------------------
// Public Functions
//----------------------------------------------------------------------
bool ClearScene::Initialize()
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


	m_pClearBackground = new ClearBackground();
	if (!m_pClearBackground->Initialize())
	{
		return false;
	}

	m_pClearLogo = new ClearLogo();
	if (!m_pClearLogo->Initialize())
	{
		return false;
	}

	m_pClearText = new ClearText();
	if (!m_pClearText->Initialize())
	{
		return false;
	}

	m_pClearClip = new ClearClip();
	if (!m_pClearClip->Initialize())
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::SoundManager)->LoadSound(
		"Resource\\ClearScene\\ClearScene.wav",
		&m_SoundIndex))
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

	SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_SoundIndex)->SoundOperation(
		Lib::SoundManager::PLAY_LOOP);

	m_Counter = 0;
	m_IsTitle = false;

	m_State = UPDATE_STATE;

	return true;
}

void ClearScene::Finalize()
{
	SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_SoundIndex)->SoundOperation(
		Lib::SoundManager::STOP_RESET);

	SINGLETON_INSTANCE(Lib::SoundManager)->ReleaseSound(m_SelectIndex);
	SINGLETON_INSTANCE(Lib::SoundManager)->ReleaseSound(m_SoundIndex);

	m_pClearClip->Finalize();
	delete m_pClearClip;

	m_pClearText->Finalize();
	delete m_pClearText;

	m_pClearLogo->Finalize();
	delete m_pClearLogo;

	m_pClearBackground->Finalize();
	delete m_pClearBackground;


	if (SINGLETON_INSTANCE(Lib::SoundManager) != nullptr)
	{
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

void ClearScene::Update()
{
	// デバイスの入力チェック.
	SINGLETON_INSTANCE(Lib::InputDeviceManager)->KeyUpdate();
	SINGLETON_INSTANCE(Lib::InputDeviceManager)->KeyCheck(DIK_Z);

	// 更新処理.
	SINGLETON_INSTANCE(Lib::UpdateTaskManager)->Run();

	// 描画処理.
	SINGLETON_INSTANCE(Lib::GraphicsDevice)->BeginScene(Lib::GraphicsDevice::BACKBUFFER_TARGET);
	SINGLETON_INSTANCE(Lib::DrawTaskManager)->Run();
	SINGLETON_INSTANCE(Lib::GraphicsDevice)->EndScene();


	if (m_IsTitle == false)
	{
		if (SINGLETON_INSTANCE(Lib::InputDeviceManager)->GetKeyState()[DIK_Z] == Lib::KeyDevice::KEY_PUSH)
		{
			m_IsTitle = true;

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
			m_NextSceneID = Application::TITLE_SCENE_ID;
		}
	}
}
