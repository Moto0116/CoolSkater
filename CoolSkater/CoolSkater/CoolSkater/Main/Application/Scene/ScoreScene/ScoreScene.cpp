#include "ScoreScene.h"

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
#include "ScoreText\ScoreText.h"
#include "ScoreClip\ScoreClip.h"


//----------------------------------------------------------------------
// Constructor	Destructor
//----------------------------------------------------------------------
ScoreScene::ScoreScene(int _sceneId) : 
	Lib::SceneBase(_sceneId),
	m_IsTitle(false),
	m_Counter(0)
{
	m_pScoreText = new ScoreText();
	m_pScoreClip = new ScoreClip();
}


ScoreScene::~ScoreScene()
{
	delete m_pScoreClip;
	delete m_pScoreText;
}


//----------------------------------------------------------------------
// Public Functions
//----------------------------------------------------------------------
bool ScoreScene::Initialize()
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
		"Resource\\ScoreScene\\ScoreScene.wav",
		&m_ScoreSceneSoundIndex))
	{
		OutputErrorLog("サウンドの読み込みに失敗しました");
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::SoundManager)->LoadSound(
		"Resource\\ScoreScene\\Select.wav",
		&m_SelectSoundIndex))
	{
		OutputErrorLog("サウンドの読み込みに失敗しました");
		return false;
	}

	SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_ScoreSceneSoundIndex)->SoundOperation(
		Lib::SoundManager::PLAY_LOOP);

	m_pScoreText->Initialize();
	m_pScoreClip->Initialize();

	m_IsTitle = false;
	m_Counter = 0;

	m_State = UPDATE_STATE;

	return true;
}

void ScoreScene::Finalize()
{
	m_pScoreClip->Finalize();
	m_pScoreText->Finalize();

	if (SINGLETON_INSTANCE(Lib::SoundManager) != nullptr)
	{
		SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_ScoreSceneSoundIndex)->SoundOperation(
			Lib::SoundManager::STOP_RESET);

		SINGLETON_INSTANCE(Lib::SoundManager)->ReleaseSound(m_SelectSoundIndex);
		SINGLETON_INSTANCE(Lib::SoundManager)->ReleaseSound(m_ScoreSceneSoundIndex);
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

void ScoreScene::Update()
{
	// デバイスの入力チェック.
	SINGLETON_INSTANCE(Lib::InputDeviceManager)->KeyUpdate();
	SINGLETON_INSTANCE(Lib::InputDeviceManager)->KeyCheck(DIK_Z);
	SINGLETON_INSTANCE(Lib::InputDeviceManager)->MouseUpdate();

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
			m_pScoreClip->SetIsFadeIn(false);

			SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_SelectSoundIndex)->SoundOperation(
				Lib::SoundManager::PLAY);

			m_IsTitle = true;
		}
	}
	else
	{
		if (m_IsTitle == true)
		{
			m_Counter++;
			if (m_Counter == 110)
			{
				m_State = FINAL_STATE;
				m_NextSceneID = Application::TITLE_SCENE_ID;
			}
		}
	}
}

