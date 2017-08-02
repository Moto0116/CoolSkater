﻿/**
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
#include "InputDeviceManager\InputDeviceManager.h"
#include "TaskManager\TaskBase\DrawTask\DrawTask.h"
#include "TaskManager\TaskBase\UpdateTask\UpdateTask.h"
#include "TitleBackground\TitleBackground.h"
#include "TitleLogo\TitleLogo.h"
#include "TitleText\TitleText.h"


//----------------------------------------------------------------------
// Constructor	Destructor
//----------------------------------------------------------------------
TitleScene::TitleScene(int _sceneId) :
	SceneBase(_sceneId)
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

	m_State = UPDATE_STATE;

	return true;
}

void TitleScene::Finalize()
{
	m_pTitleText->Finalize();
	delete m_pTitleText;

	m_pTitleLogo->Finalize();
	delete m_pTitleLogo;

	m_pBackground->Finalize();
	delete m_pBackground;

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
}

void TitleScene::Update()
{	
	// デバイスの入力チェック.
	SINGLETON_INSTANCE(Lib::InputDeviceManager)->KeyUpdate();
	SINGLETON_INSTANCE(Lib::InputDeviceManager)->KeyCheck(DIK_SPACE);
	SINGLETON_INSTANCE(Lib::InputDeviceManager)->KeyCheck(DIK_A);
	SINGLETON_INSTANCE(Lib::InputDeviceManager)->MouseUpdate();

	// 更新処理.
	SINGLETON_INSTANCE(Lib::UpdateTaskManager)->Run();

	// 描画処理.
	SINGLETON_INSTANCE(Lib::GraphicsDevice)->BeginScene(Lib::GraphicsDevice::BACKBUFFER_TARGET);
	SINGLETON_INSTANCE(Lib::DrawTaskManager)->Run();
	SINGLETON_INSTANCE(Lib::GraphicsDevice)->EndScene();



	if (SINGLETON_INSTANCE(Lib::InputDeviceManager)->GetKeyState()[DIK_SPACE] == Lib::KeyDevice::KEY_PUSH)
	{
		m_State = FINAL_STATE;
		m_NextSceneID = Application::GAME_SCENE_ID;
	}
	else if (SINGLETON_INSTANCE(Lib::InputDeviceManager)->GetKeyState()[DIK_A] == Lib::KeyDevice::KEY_PUSH)
	{
		///@todo ウィンドウ破棄時のDestroyWindowに問題があると思うので要チェック

		//m_State = FINAL_STATE;
		//m_NextSceneID = Application::END_SCENE_ID;
	}
}
