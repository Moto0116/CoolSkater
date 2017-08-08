/**
* @file		ScoreClip.cpp
* @brief	ゲームクリッピングクラス実装
* @author	morimoto
*/

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "ScoreClip.h"

#include "Debugger\Debugger.h"
#include "DirectX11\GraphicsDevice\GraphicsDevice.h"
#include "DirectX11\TextureManager\TextureManager.h"
#include "DirectX11\ShaderManager\ShaderManager.h"
#include "InputDeviceManager\InputDeviceManager.h"


//----------------------------------------------------------------------
// Constructor	Destructor
//----------------------------------------------------------------------
ScoreClip::ScoreClip()
{
	m_Pos = D3DXVECTOR2(640, 360);
	m_Size = D3DXVECTOR2(1280, 720);
}

ScoreClip::~ScoreClip()
{
}


//----------------------------------------------------------------------
// Public Functions
//----------------------------------------------------------------------
bool ScoreClip::Initialize()
{
	SINGLETON_INSTANCE(Lib::DrawTaskManager)->AddTask(m_pDrawTask);
	SINGLETON_INSTANCE(Lib::UpdateTaskManager)->AddTask(m_pUpdateTask);

	if (!CreateVertex2D())
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\TitleScene\\Cliping.png",
		&m_TextureIndex))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\TitleScene\\Cliping2.png",
		&m_TextureIndex2))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::ShaderManager)->LoadVertexShader(
		"Resource\\TitleScene\\ClipEffect.fx",
		"VS",
		&m_VertexShaderIndex))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::ShaderManager)->LoadPixelShader(
		"Resource\\TitleScene\\ClipEffect.fx",
		"PS",
		&m_PixelShaderIndex))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::ShaderManager)->LoadVertexShader(
		"Resource\\TitleScene\\TextureCreateEffect.fx",
		"VS",
		&m_CreateTextureVertexShaderIndex))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::ShaderManager)->LoadPixelShader(
		"Resource\\TitleScene\\TextureCreateEffect.fx",
		"PS",
		&m_CreateTexturePixelShaderIndex))
	{
		return false;
	}

	D3D11_TEXTURE2D_DESC ClipTextureDesc;
	ZeroMemory(&ClipTextureDesc, sizeof(ClipTextureDesc));
	ClipTextureDesc.Width = static_cast<UINT>(1280);
	ClipTextureDesc.Height = static_cast<UINT>(1280);
	ClipTextureDesc.MipLevels = 1;
	ClipTextureDesc.ArraySize = 1;
	ClipTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	ClipTextureDesc.SampleDesc.Count = 1;
	ClipTextureDesc.SampleDesc.Quality = 0;
	ClipTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	ClipTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	ClipTextureDesc.CPUAccessFlags = 0;
	ClipTextureDesc.MiscFlags = 0;

	if (FAILED(SINGLETON_INSTANCE(Lib::GraphicsDevice)->GetDevice()->CreateTexture2D(
		&ClipTextureDesc,
		nullptr,
		&m_pClipTexture)))
	{
		OutputErrorLog("マップ生成に失敗しました");
		return false;
	}

	if (FAILED(SINGLETON_INSTANCE(Lib::GraphicsDevice)->GetDevice()->CreateRenderTargetView(
		m_pClipTexture,
		nullptr,
		&m_pClipRenderTarget)))
	{
		OutputErrorLog("マップのレンダーターゲットビューの設定に失敗しました");
		return false;
	}

	if (FAILED(SINGLETON_INSTANCE(Lib::GraphicsDevice)->GetDevice()->CreateShaderResourceView(
		m_pClipTexture,
		nullptr,
		&m_pClipShaderResourceView)))
	{
		OutputErrorLog("マップのシェーダーリソースビューの生成に失敗しました");
		return false;
	}


	D3D11_TEXTURE2D_DESC DepthStencilDesc;
	DepthStencilDesc.Width = static_cast<UINT>(1280);
	DepthStencilDesc.Height = static_cast<UINT>(1280);
	DepthStencilDesc.MipLevels = 1;
	DepthStencilDesc.ArraySize = 1;
	DepthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
	DepthStencilDesc.SampleDesc.Count = 1;
	DepthStencilDesc.SampleDesc.Quality = 0;
	DepthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	DepthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DepthStencilDesc.CPUAccessFlags = 0;
	DepthStencilDesc.MiscFlags = 0;

	if (FAILED(SINGLETON_INSTANCE(Lib::GraphicsDevice)->GetDevice()->CreateTexture2D(
		&DepthStencilDesc,
		nullptr,
		&m_pClipDepthStencilTexture)))
	{
		OutputErrorLog("深度ステンシルテクスチャ生成に失敗しました");
		return false;
	}

	if (FAILED(SINGLETON_INSTANCE(Lib::GraphicsDevice)->GetDevice()->CreateDepthStencilView(
		m_pClipDepthStencilTexture,
		nullptr,
		&m_pClipDepthStencilView)))
	{
		OutputErrorLog("深度ステンシルテクスチャのデプスステンシルビューの生成に失敗しました");
		return false;
	}

	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;
	m_ViewPort.Width = 1280;
	m_ViewPort.Height = 1280;
	m_ViewPort.MinDepth = 0.0f;
	m_ViewPort.MaxDepth = 1.0f;


	D3D11_SAMPLER_DESC SamplerDesc;
	ZeroMemory(&SamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	if (FAILED(SINGLETON_INSTANCE(Lib::GraphicsDevice)->GetDevice()->CreateSamplerState(
		&SamplerDesc,
		&m_pSamplerState)))
	{
		OutputErrorLog("サンプラステートの生成に失敗しました");
		return false;
	}

	m_ClipSize = D3DXVECTOR2(0, 0);
	m_IsClip = true;
	m_IsFadeIn = true;

	return true;
}

void ScoreClip::Finalize()
{
	m_pSamplerState->Release();

	m_pClipDepthStencilView->Release();
	m_pClipDepthStencilTexture->Release();

	m_pClipShaderResourceView->Release();
	m_pClipRenderTarget->Release();
	m_pClipTexture->Release();

	SINGLETON_INSTANCE(Lib::ShaderManager)->ReleasePixelShader(m_CreateTexturePixelShaderIndex);
	SINGLETON_INSTANCE(Lib::ShaderManager)->ReleaseVertexShader(m_CreateTextureVertexShaderIndex);
	SINGLETON_INSTANCE(Lib::ShaderManager)->ReleasePixelShader(m_PixelShaderIndex);
	SINGLETON_INSTANCE(Lib::ShaderManager)->ReleaseVertexShader(m_VertexShaderIndex);

	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_TextureIndex2);
	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_TextureIndex);
	ReleaseVertex2D();

	SINGLETON_INSTANCE(Lib::UpdateTaskManager)->RemoveTask(m_pUpdateTask);
	SINGLETON_INSTANCE(Lib::DrawTaskManager)->RemoveTask(m_pDrawTask);
}

void ScoreClip::Update()
{
	if (m_IsClip)
	{
		if (m_IsFadeIn)
		{
			m_ClipSize = D3DXVECTOR2(m_ClipSize.x + 30, m_ClipSize.y + 30);
			if (m_ClipSize.x > 2000)
			{
				m_ClipSize = D3DXVECTOR2(2000, 2000);
			}
		}
		else
		{
			m_ClipSize = D3DXVECTOR2(m_ClipSize.x - 30, m_ClipSize.y - 30);
			if (m_ClipSize.x < 0)
			{
				m_ClipSize = D3DXVECTOR2(0, 0);
			}
		}
	}
}

void ScoreClip::Draw()
{
	Lib::GraphicsDevice* pGraphicsDevice = SINGLETON_INSTANCE(Lib::GraphicsDevice);
	ID3D11DeviceContext* pContext = SINGLETON_INSTANCE(Lib::GraphicsDevice)->GetDeviceContext();
	Lib::ShaderManager*	pShaderManager = SINGLETON_INSTANCE(Lib::ShaderManager);

	pGraphicsDevice->SetRenderTarget(&m_pClipRenderTarget, 2);
	pGraphicsDevice->SetDepthStencil(&m_pClipDepthStencilView, 2);
	pGraphicsDevice->SetClearColor(D3DXCOLOR(0, 0, 0, 0), 2);
	pGraphicsDevice->SetViewPort(&m_ViewPort, 2);
	pGraphicsDevice->BeginScene(2);


	// ベースの描画.
	m_pVertex->SetVertex(&m_Size);
	m_pVertex->WriteVertexBuffer();
	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex2));

	pContext->VSSetShader(
		SINGLETON_INSTANCE(Lib::ShaderManager)->GetVertexShader(m_CreateTextureVertexShaderIndex),
		nullptr,
		0);
	pContext->PSSetShader(
		SINGLETON_INSTANCE(Lib::ShaderManager)->GetPixelShader(m_CreateTexturePixelShaderIndex),
		nullptr,
		0);
	pContext->GSSetShader(nullptr, nullptr, 0);
	pContext->HSSetShader(nullptr, nullptr, 0);
	pContext->DSSetShader(nullptr, nullptr, 0);
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	m_pVertex->WriteConstantBuffer(&m_Pos);
	m_pVertex->Draw();

	// クリッピング範囲の描画
	m_pVertex->SetVertex(&m_ClipSize);
	m_pVertex->WriteVertexBuffer();
	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex));

	pContext->VSSetShader(
		SINGLETON_INSTANCE(Lib::ShaderManager)->GetVertexShader(m_CreateTextureVertexShaderIndex),
		nullptr,
		0);
	pContext->PSSetShader(
		SINGLETON_INSTANCE(Lib::ShaderManager)->GetPixelShader(m_CreateTexturePixelShaderIndex),
		nullptr,
		0);
	pContext->GSSetShader(nullptr, nullptr, 0);
	pContext->HSSetShader(nullptr, nullptr, 0);
	pContext->DSSetShader(nullptr, nullptr, 0);
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	m_pVertex->WriteConstantBuffer(&m_Pos);
	m_pVertex->Draw();

	// クリッピング.
	pGraphicsDevice->SetScene(Lib::GraphicsDevice::BACKBUFFER_TARGET);

	m_pVertex->SetVertex(&m_Size);
	m_pVertex->WriteVertexBuffer();
	m_pVertex->SetTexture(nullptr);
	pContext->PSSetSamplers(0, 1, &m_pSamplerState);
	pContext->PSSetShaderResources(0, 1, &m_pClipShaderResourceView);

	pContext->VSSetShader(
		SINGLETON_INSTANCE(Lib::ShaderManager)->GetVertexShader(m_VertexShaderIndex),
		nullptr,
		0);
	pContext->PSSetShader(
		SINGLETON_INSTANCE(Lib::ShaderManager)->GetPixelShader(m_PixelShaderIndex),
		nullptr,
		0);
	pContext->GSSetShader(nullptr, nullptr, 0);
	pContext->HSSetShader(nullptr, nullptr, 0);
	pContext->DSSetShader(nullptr, nullptr, 0);
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	m_pVertex->WriteConstantBuffer(&m_Pos);
	m_pVertex->Draw();
}


