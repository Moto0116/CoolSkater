/**
 * @file	GameScene.cpp
 * @brief	ゲームシーンクラス実装
 * @author	morimoto
 */

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "GameScene.h"

#include "Application.h"
#include "Debugger\Debugger.h"
#include "DirectX11\GraphicsDevice\GraphicsDevice.h"
#include "DirectX11\ShaderManager\ShaderManager.h"
#include "DirectX11\AnimationManager\AnimationManager.h"
#include "DirectX11\TextureManager\TextureManager.h"
#include "DirectX11\SoundDevice\SoundDevice.h"
#include "DirectX11\SoundManager\SoundManager.h"
#include "DirectX11\SoundManager\ISound\ISound.h"
#include "InputDeviceManager\InputDeviceManager.h"
#include "TaskManager\TaskBase\UpdateTask\UpdateTask.h"
#include "TaskManager\TaskBase\DrawTask\DrawTask.h"
#include "GameDataManager\GameDataManager.h"
#include "CollisionManager\CollisionManager.h"
#include "EventManager\EventManager.h"
#include "EventManager\EventBase\EventBase.h"
#include "Event\PlayerEvent\PlayerEvent.h"


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

	if (!SINGLETON_INSTANCE(Lib::SoundManager)->LoadSound(
		"Resource\\GameScene\\GameSceneBGM.wav",
		&m_GameSoundIndex))
	{
		OutputErrorLog("サウンドの読み込みに失敗しました");
		return false;
	}

	SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_GameSoundIndex)->SoundOperation(
		Lib::SoundManager::PLAY_LOOP);


	SINGLETON_CREATE(Lib::EventManager);
	SINGLETON_CREATE(GameDataManager);
	SINGLETON_CREATE(CollisionManager);

	m_pObjectManager = new ObjectManager();
	if (!m_pObjectManager->Initialize())
	{
		OutputErrorLog("オブジェクト管理クラスの生成に失敗しました");
		return false;
	}

	m_pEventListener = new GameSceneEventListener(this, &GameScene::ReciveEvent);
	SINGLETON_INSTANCE(Lib::EventManager)->AddEventListener(m_pEventListener);

	m_State = UPDATE_STATE;

	return true;
}

void GameScene::Finalize()
{
	delete m_pEventListener;

	if (m_pObjectManager != nullptr)
	{
		m_pObjectManager->Finalize();
		SafeDelete(m_pObjectManager);
	}

	SINGLETON_DELETE(CollisionManager);
	SINGLETON_DELETE(GameDataManager);
	SINGLETON_DELETE(Lib::EventManager);


	if (SINGLETON_INSTANCE(Lib::SoundManager) != nullptr)
	{
		SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_GameSoundIndex)->SoundOperation(
			Lib::SoundManager::STOP_RESET);

		SINGLETON_INSTANCE(Lib::SoundManager)->ReleaseSound(m_GameSoundIndex);
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

	m_State = INIT_STATE;
}

void GameScene::Update()
{
	// デバイスの入力チェック.
	SINGLETON_INSTANCE(Lib::InputDeviceManager)->KeyUpdate();
	SINGLETON_INSTANCE(Lib::InputDeviceManager)->KeyCheck(DIK_Z);
	SINGLETON_INSTANCE(Lib::InputDeviceManager)->KeyCheck(DIK_X);
	SINGLETON_INSTANCE(Lib::InputDeviceManager)->MouseUpdate();

	// 更新処理.
	SINGLETON_INSTANCE(Lib::UpdateTaskManager)->Run();

	// 描画処理.
	SINGLETON_INSTANCE(Lib::GraphicsDevice)->BeginScene(Lib::GraphicsDevice::BACKBUFFER_TARGET);
	SINGLETON_INSTANCE(Lib::DrawTaskManager)->Run();
	SINGLETON_INSTANCE(Lib::GraphicsDevice)->EndScene();
}

//----------------------------------------------------------------------
// Private Functions
//----------------------------------------------------------------------
void GameScene::ReciveEvent(Lib::EventBase* _pEvent)
{
	switch (_pEvent->GetEventID())
	{
	case GameScene::GAMESTART_EVENT:
		break;
	case GameScene::STAGEBACKGROUND_EVENT:
		break;
	case GameScene::PLAYERDEAD_EVENT:
		break;
	case GameScene::PLAYER_EVENT:
		switch (reinterpret_cast<PlayerEvent*>(_pEvent)->GetEvent())
		{
		case PlayerEvent::PLAYERCLEAR_EVENT:
			m_IsClear = true;
			break;
		case PlayerEvent::PLAYERDEADEND_EVENT:
			m_IsClear = false;
			break;
		}
		break;
	case GameScene::CLIP_EVENT:
		if (m_IsClear)
		{
			ScoreSave();
			NewScoreSave();
			m_State = FINAL_STATE;
			m_NextSceneID = Application::CLEAR_SCENE_ID;
		}
		else
		{
			ScoreSave();
			NewScoreSave();
			m_State = FINAL_STATE;
			m_NextSceneID = Application::OVER_SCENE_ID;
		}
		break;
	}
}

void GameScene::ScoreSave()
{
	// ファイル読み込み.
	std::vector<int> ScoreData;
	FILE* pFile = nullptr;
	long FileSize = 0;
	char* pBuffer = nullptr;
	char* pContext = nullptr;
	char* pDataStr = nullptr;

	fopen_s(&pFile, "Resource\\Score.csv", "r");

	// ファイルサイズの取得.
	fseek(pFile, 0, SEEK_END);
	FileSize = ftell(pFile) + 1;
	fseek(pFile, 0, SEEK_SET);

	// ファイルのデータを格納するバッファ.
	pBuffer = new char[FileSize];
	ZeroMemory(pBuffer, FileSize);

	// ファイルの読み込み.
	fread(pBuffer, FileSize, 1, pFile);
	pBuffer[FileSize - 1] = '\0';

	// ファイル内のデータを見つけて取得.
	pBuffer = strstr(pBuffer, "{");
	strcpy_s(pBuffer, FileSize, strtok_s(pBuffer, "{}", &pContext));
	pDataStr = strtok_s(pBuffer, "{},\n", &pContext);

	while (1)
	{
		if (pDataStr == nullptr)
		{
			break;	// データ全てを取り出すまでループ
		}
		else
		{
			ScoreData.push_back(atoi(pDataStr));
		}

		pDataStr = strtok_s(nullptr, "{},\n", &pContext);
	}

	fclose(pFile);

	delete[] pBuffer;


	fopen_s(&pFile, "Resource\\Score.csv", "w");

	fprintf_s(pFile, "{\n");

	for (unsigned int i = 0; i < ScoreData.size(); i++)
	{
		fprintf_s(pFile, "%d\n", ScoreData[i]);
	}
	
	fprintf_s(pFile, "%d\n", SINGLETON_INSTANCE(GameDataManager)->GetScore());

	fprintf_s(pFile, "}");

	fclose(pFile);
}

void GameScene::NewScoreSave()
{
	// ファイル読み込み.
	std::vector<int> ScoreData;
	FILE* pFile = nullptr;

	fopen_s(&pFile, "Resource\\NewScore.csv", "w");

	fprintf_s(pFile, "{\n");

	for (unsigned int i = 0; i < ScoreData.size(); i++)
	{
		fprintf_s(pFile, "%d\n", ScoreData[i]);
	}

	fprintf_s(pFile, "%d\n", SINGLETON_INSTANCE(GameDataManager)->GetScore());

	fprintf_s(pFile, "}");

	fclose(pFile);
}
