/**
 * @file	OverText.cpp
 * @brief	ゲームオーバーテキスト実装
 * @author	morimoto
 */

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "OverText.h"

#include "DirectX11\TextureManager\TextureManager.h"
#include "InputDeviceManager\InputDeviceManager.h"


//----------------------------------------------------------------------
// Constructor	Destructor
//----------------------------------------------------------------------
OverText::OverText()
{
	m_Pos = D3DXVECTOR2(640, 540);
	m_Size = D3DXVECTOR2(750, 70);
}

OverText::~OverText()
{
}


//----------------------------------------------------------------------
// Public Functions
//----------------------------------------------------------------------
bool OverText::Initialize()
{
	SINGLETON_INSTANCE(Lib::DrawTaskManager)->AddTask(m_pDrawTask);
	SINGLETON_INSTANCE(Lib::UpdateTaskManager)->AddTask(m_pUpdateTask);

	if (!CreateVertex2D())
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\OverScene\\OverText.png",
		&m_TextureIndex))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\OverScene\\OverText2.png",
		&m_TextureIndex2))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\OverScene\\TextBox.png",
		&m_TextureIndex3))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\OverScene\\Score.png",
		&m_TextureIndex4))
	{
		return false;
	}


	m_pFont = new Lib::Font();
	m_pFont->Initialize(SINGLETON_INSTANCE(Lib::GraphicsDevice));
	m_pFont->CreateVertexBuffer(&D3DXVECTOR2(64, 128), &D3DXCOLOR(0xffffffff));

	// ファイル読み込み.
	FILE* pFile = nullptr;
	long FileSize = 0;
	char* pBuffer = nullptr;
	char* pContext = nullptr;
	char* pDataStr = nullptr;

	fopen_s(&pFile, "Resource\\NewScore.csv", "r");

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


	m_IsTitle = false;
	m_IsTitleFlash = true;
	m_TitleAlphaColor = 1.0f;

	m_IsGame = false;
	m_IsGameFlash = true;
	m_GameAlphaColor = 1.0f;

	return true;
}

void OverText::Finalize()
{
	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_TextureIndex4);
	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_TextureIndex3);
	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_TextureIndex2);
	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_TextureIndex);
	ReleaseVertex2D();

	m_pFont->ReleaseVertexBuffer();
	m_pFont->Finalize();
	delete m_pFont;

	SINGLETON_INSTANCE(Lib::UpdateTaskManager)->RemoveTask(m_pUpdateTask);
	SINGLETON_INSTANCE(Lib::DrawTaskManager)->RemoveTask(m_pDrawTask);
}

void OverText::Update()
{
	if (SINGLETON_INSTANCE(Lib::InputDeviceManager)->GetKeyState()[DIK_Z] == Lib::KeyDevice::KEY_PUSH &&
		m_IsGame == false)
	{
		m_IsTitle = true;
	}
	else if (SINGLETON_INSTANCE(Lib::InputDeviceManager)->GetKeyState()[DIK_LSHIFT] == Lib::KeyDevice::KEY_PUSH &&
		m_IsTitle == false)
	{
		m_IsGame = true;
	}


	if (m_IsTitle)
	{
		if (m_IsTitleFlash)
		{
			m_TitleAlphaColor -= 0.1f;
			if (m_TitleAlphaColor <= 0)
			{
				m_TitleAlphaColor = 0;
				m_IsTitleFlash = false;
			}
		}
		else
		{
			m_TitleAlphaColor += 0.1f;
			if (m_TitleAlphaColor >= 1)
			{
				m_TitleAlphaColor = 1;
				m_IsTitleFlash = true;
			}
		}
	}

	if (m_IsGame)
	{
		if (m_IsGameFlash)
		{
			m_GameAlphaColor -= 0.1f;
			if (m_GameAlphaColor <= 0)
			{
				m_GameAlphaColor = 0;
				m_IsGameFlash = false;
			}
		}
		else
		{
			m_GameAlphaColor += 0.1f;
			if (m_GameAlphaColor >= 1)
			{
				m_GameAlphaColor = 1;
				m_IsGameFlash = true;
			}
		}
	}
}

void OverText::Draw()
{
	m_pVertex->SetColor(&D3DXCOLOR(1, 1, 1, m_TitleAlphaColor));
	m_pVertex->SetVertex(&m_Size);
	m_pVertex->WriteVertexBuffer();

	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex3));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&D3DXVECTOR2(m_Pos.x, m_Pos.y));
	m_pVertex->Draw();

	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&m_Pos);
	m_pVertex->Draw();


	m_pVertex->SetColor(&D3DXCOLOR(1, 1, 1, m_GameAlphaColor));
	m_pVertex->SetVertex(&m_Size);
	m_pVertex->WriteVertexBuffer();

	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex3));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&D3DXVECTOR2(m_Pos.x, m_Pos.y + 90));
	m_pVertex->Draw();

	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex2));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&D3DXVECTOR2(m_Pos.x, m_Pos.y + 90));
	m_pVertex->Draw();


	m_pVertex->SetColor(&D3DXCOLOR(1, 1, 1, 1));
	m_pVertex->SetVertex(&D3DXVECTOR2(450, 100));
	m_pVertex->WriteVertexBuffer();

	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex4));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&D3DXVECTOR2(400, 350));
	m_pVertex->Draw();

	char DrawStr[32];
	sprintf_s(DrawStr, 32, "%d", m_ScoreData[0]);
	m_pFont->Draw(&D3DXVECTOR2(690, 360), DrawStr);
}

