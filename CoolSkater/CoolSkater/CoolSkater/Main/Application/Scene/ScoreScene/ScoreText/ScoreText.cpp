/**
 * @file	ScoreText.cpp
 * @brief	スコアテキストクラス実装
 * @author	morimoto
 */

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "ScoreText.h"

#include "DirectX11\TextureManager\TextureManager.h"
#include "InputDeviceManager\InputDeviceManager.h"
#include <algorithm>
#include <functional>


//----------------------------------------------------------------------
// Constructor	Destructor
//----------------------------------------------------------------------
ScoreText::ScoreText() : 
	m_IsScoreBack(false),
	m_IsScoreFlash(false),
	m_ScoreAlphaColor(1.0f)
{
	m_Pos = D3DXVECTOR2(640, 360);
	m_Size = D3DXVECTOR2(1280, 720);
}

ScoreText::~ScoreText()
{
}


//----------------------------------------------------------------------
// Public Functions
//----------------------------------------------------------------------
bool ScoreText::Initialize()
{
	SINGLETON_INSTANCE(Lib::DrawTaskManager)->AddTask(m_pDrawTask);
	SINGLETON_INSTANCE(Lib::UpdateTaskManager)->AddTask(m_pUpdateTask);

	if (!CreateVertex2D())
	{
		return false;
	}

	m_pFont = new Lib::Font();
	m_pFont->Initialize(SINGLETON_INSTANCE(Lib::GraphicsDevice));
	m_pFont->CreateVertexBuffer(&D3DXVECTOR2(32, 64), &D3DXCOLOR(0xffffffff));


	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\ScoreScene\\ScoreBackground.png",
		&m_TextureIndex))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\ScoreScene\\PushToSpace.png",
		&m_TextureIndex2))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\ScoreScene\\Ranking.png",
		&m_TextureIndex3))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\ScoreScene\\HighScore.png",
		&m_TextureIndex4))
	{
		return false;
	}


	// ファイル読み込み.
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
			break;	// データ全てを取り出すまでループ.
		}
		else
		{
			m_ScoreData.push_back(atoi(pDataStr));
		}

		pDataStr = strtok_s(nullptr, "{},\n", &pContext);
	}

	fclose(pFile);

	delete[] pBuffer;

	std::sort(m_ScoreData.begin(), m_ScoreData.end(), std::greater<int>());


	m_IsScoreBack = false;
	m_IsScoreFlash = false;
	m_ScoreAlphaColor = 1.0f;

	return true;
}

void ScoreText::Finalize()
{
	m_ScoreData.erase(m_ScoreData.begin(), m_ScoreData.end());

	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_TextureIndex4);
	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_TextureIndex3);
	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_TextureIndex2);
	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_TextureIndex);

	m_pFont->ReleaseVertexBuffer();
	m_pFont->Finalize();
	delete m_pFont;

	ReleaseVertex2D();

	SINGLETON_INSTANCE(Lib::UpdateTaskManager)->RemoveTask(m_pUpdateTask);
	SINGLETON_INSTANCE(Lib::DrawTaskManager)->RemoveTask(m_pDrawTask);
}

void ScoreText::Update()
{
	if (SINGLETON_INSTANCE(Lib::InputDeviceManager)->GetKeyState()[DIK_Z] == Lib::KeyDevice::KEY_PUSH)
	{
		m_IsScoreBack = true;
	}

	if (m_IsScoreBack)
	{
		if (m_IsScoreFlash)
		{
			m_ScoreAlphaColor -= 0.1f;
			if (m_ScoreAlphaColor <= 0.0f)
			{
				m_IsScoreFlash = false;
			}
		}
		else
		{
			m_ScoreAlphaColor += 0.1f;
			if (m_ScoreAlphaColor >= 1.0f)
			{
				m_IsScoreFlash = true;
			}
		}
	}
	else
	{
		if (m_IsScoreFlash)
		{
			m_ScoreAlphaColor -= 0.015f;
			if (m_ScoreAlphaColor <= 0.0f)
			{
				m_IsScoreFlash = false;
			}
		}
		else
		{
			m_ScoreAlphaColor += 0.015f;
			if (m_ScoreAlphaColor >= 1.0f)
			{
				m_IsScoreFlash = true;
			}
		}
	}
}

void ScoreText::Draw()
{
	m_Pos = D3DXVECTOR2(640, 360);
	m_Size = D3DXVECTOR2(1280, 720);

	m_pVertex->SetColor(&D3DXCOLOR(1, 1, 1, 1));
	m_pVertex->SetVertex(&m_Size);
	m_pVertex->WriteVertexBuffer();
	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&m_Pos);
	m_pVertex->Draw();

	m_Pos = D3DXVECTOR2(640, 660);
	m_Size = D3DXVECTOR2(700, 150);

	m_pVertex->SetColor(&D3DXCOLOR(1, 1, 1, m_ScoreAlphaColor));
	m_pVertex->SetVertex(&D3DXVECTOR2(m_Size.x - 250, m_Size.y));
	m_pVertex->WriteVertexBuffer();
	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex2));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&m_Pos);
	m_pVertex->Draw();


	m_Pos = D3DXVECTOR2(250, 400);
	m_Size = D3DXVECTOR2(450, 400);

	m_pVertex->SetColor(&D3DXCOLOR(1, 1, 1, 1));
	m_pVertex->SetVertex(&m_Size);
	m_pVertex->WriteVertexBuffer();
	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex3));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&m_Pos);
	m_pVertex->Draw();

	for (unsigned int i = 0; i < m_ScoreData.size(); i++)
	{
		if (i >= 5)
		{
			break;
		}

		char DrawStr[32];
		sprintf_s(DrawStr, 32, "%d", m_ScoreData[i]);
		m_pFont->Draw(&D3DXVECTOR2(200, static_cast<float>(250 + i * 80)), DrawStr);

		sprintf_s(DrawStr, 32, "%d : ", i + 1);
		m_pFont->Draw(&D3DXVECTOR2(50, static_cast<float>(250 + i * 80)), DrawStr);
	}

	m_Pos = D3DXVECTOR2(250, 100);
	m_Size = D3DXVECTOR2(450, 100);

	m_pVertex->SetColor(&D3DXCOLOR(1, 1, 1, 1));
	m_pVertex->SetVertex(&m_Size);
	m_pVertex->WriteVertexBuffer();
	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex4));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&m_Pos);
	m_pVertex->Draw();
}

