/**
 * @file	GameScene.cpp
 * @brief	ゲームシーンクラス実装
 * @author	morimoto
 */

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "GameScene.h"

#include "Debugger\Debugger.h"
#include "DirectX11\GraphicsDevice\GraphicsDevice.h"
#include "DirectX11\ShaderManager\ShaderManager.h"
#include "DirectX11\AnimationManager\AnimationManager.h"
#include "DirectX11\TextureManager\TextureManager.h"
#include "DirectX11\SoundDevice\SoundDevice.h"
#include "DirectX11\SoundManager\SoundManager.h"
#include "InputDeviceManager\InputDeviceManager.h"
#include "TaskManager\TaskBase\UpdateTask\UpdateTask.h"
#include "TaskManager\TaskBase\DrawTask\DrawTask.h"
#include "GameDataManager\GameDataManager.h"
#include "EventManager\EventManager.h"


//----------------------------------------------------------------------
// Constructor	Destructor
//----------------------------------------------------------------------
GameScene::GameScene(int _sceneId) :
	SceneBase(_sceneId)
{
}

GameScene::~GameScene()
{
}


//----------------------------------------------------------------------
// Public Functions
//----------------------------------------------------------------------
bool GameScene::Initialize()
{
	SINGLETON_CREATE(Lib::UpdateTaskManager);
	SINGLETON_CREATE(Lib::DrawTaskManager);


	SINGLETON_CREATE(Lib::ShaderManager);
	if (!SINGLETON_INSTANCE(Lib::ShaderManager)->Initialize(SINGLETON_INSTANCE(Lib::GraphicsDevice)))
	{
		OutputErrorLog("シェーダー管理クラスの生成に失敗しました");
		return false;
	}

	SINGLETON_CREATE(Lib::AnimationManager);
	if (!SINGLETON_INSTANCE(Lib::AnimationManager)->Initialize())
	{
		OutputErrorLog("テクスチャ管理クラスの生成に失敗しました");
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


	SINGLETON_CREATE(Lib::EventManager);
	SINGLETON_CREATE(GameDataManager);

	m_pObjectManager = new ObjectManager();
	if (!m_pObjectManager->Initialize())
	{
		OutputErrorLog("オブジェクト管理クラスの生成に失敗しました");
		return false;
	}

	m_State = UPDATE_STATE;

	return true;
}

void GameScene::Finalize()
{
	if (m_pObjectManager != nullptr)
	{
		m_pObjectManager->Finalize();
		SafeDelete(m_pObjectManager);
	}

	SINGLETON_DELETE(GameDataManager);
	SINGLETON_DELETE(Lib::EventManager);


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

	if (SINGLETON_INSTANCE(Lib::AnimationManager) != nullptr)
	{
		SINGLETON_INSTANCE(Lib::AnimationManager)->Finalize();
		SINGLETON_DELETE(Lib::AnimationManager);
	}

	if (SINGLETON_INSTANCE(Lib::ShaderManager) != nullptr)
	{
		SINGLETON_INSTANCE(Lib::ShaderManager)->Finalize();
		SINGLETON_DELETE(Lib::ShaderManager);
	}

	SINGLETON_DELETE(Lib::DrawTaskManager);
	SINGLETON_DELETE(Lib::UpdateTaskManager);
}

void GameScene::Update()
{
	// デバイスの入力チェック.
	SINGLETON_INSTANCE(Lib::InputDeviceManager)->KeyUpdate();
	SINGLETON_INSTANCE(Lib::InputDeviceManager)->KeyCheck(DIK_SPACE);
	SINGLETON_INSTANCE(Lib::InputDeviceManager)->KeyCheck(DIK_Q);
	SINGLETON_INSTANCE(Lib::InputDeviceManager)->KeyCheck(DIK_W);
	SINGLETON_INSTANCE(Lib::InputDeviceManager)->KeyCheck(DIK_E);
	SINGLETON_INSTANCE(Lib::InputDeviceManager)->MouseUpdate();

	// 更新処理.
	SINGLETON_INSTANCE(Lib::UpdateTaskManager)->Run();

	// 描画処理.
	SINGLETON_INSTANCE(Lib::GraphicsDevice)->BeginScene(Lib::GraphicsDevice::BACKBUFFER_TARGET);
	SINGLETON_INSTANCE(Lib::DrawTaskManager)->Run();
	SINGLETON_INSTANCE(Lib::GraphicsDevice)->EndScene();
}
