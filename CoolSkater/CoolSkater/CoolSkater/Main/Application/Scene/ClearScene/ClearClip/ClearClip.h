#ifndef CLEARCLIP_H
#define CLEARCLIP_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "Main\Object2DBase\Object2DBase.h"

#include <D3D11.h>
#include <random>


class ClearClip : public Object2DBase
{
public:
	/**
	* コンストラクタ
	*/
	ClearClip();

	/**
	* デストラクタ
	*/
	virtual ~ClearClip();

	/**
	* 初期化処理
	* @return 初期化に成功したらtrue 失敗したらfalse
	*/
	virtual bool Initialize();

	/**
	* 終了処理
	*/
	virtual void Finalize();

	/**
	* オブジェクトの更新
	*/
	virtual void Update();

	/**
	* オブジェクトの描画
	*/
	virtual void Draw();

private:
	ID3D11Texture2D*			m_pClipTexture;
	ID3D11RenderTargetView*		m_pClipRenderTarget;
	ID3D11ShaderResourceView*	m_pClipShaderResourceView;

	ID3D11Texture2D*			m_pClipDepthStencilTexture;
	ID3D11DepthStencilView*		m_pClipDepthStencilView;

	D3D11_VIEWPORT				m_ViewPort;


	int		m_VertexShaderIndex;	//!< 頂点シェーダーインデックス.
	int		m_PixelShaderIndex;		//!< ピクセルシェーダーインデックス.

	int		m_CreateTextureVertexShaderIndex;	//!< 頂点シェーダーインデックス.
	int		m_CreateTexturePixelShaderIndex;	//!< ピクセルシェーダーインデックス.

	ID3D11SamplerState* m_pSamplerState;

	int m_TextureIndex2;

	D3DXVECTOR2 m_ClipSize[10];
	D3DXVECTOR2 m_ClipPos[10];
	int			m_ClipTime[10];
	bool m_IsClip;

	std::random_device			m_RandDevice;				//!< 乱数生成デバイス.
	std::mt19937				m_MersenneTwister;			//!< 乱数生成オブジェクト.

};

#endif // CLEARCLIP_H
