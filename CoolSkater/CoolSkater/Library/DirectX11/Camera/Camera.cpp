/**
 * @file   Camera.cpp
 * @brief  Cameraクラスの実装
 * @author morimoto
 */

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "Camera.h"


namespace Lib
{
	//----------------------------------------------------------------------------------------------------
	// Constructor	Destructor
	//----------------------------------------------------------------------------------------------------
	Camera::Camera(float _windowWidth, float _windowHeight, float _nearZ, float _farZ) : 
		m_WindowWidth(_windowWidth),
		m_WindowHeight(_windowHeight),
		m_NearZ(_nearZ),
		m_FarZ(_farZ)
	{
		m_Aspect = m_WindowWidth / m_WindowHeight;
	}

	Camera::~Camera()
	{
	}


	//----------------------------------------------------------------------------------------------------
	// Public Functions
	//----------------------------------------------------------------------------------------------------
	void Camera::TransformView(
		const D3DXVECTOR3* _pEyePos,
		const D3DXVECTOR3* _pLookPos,
		const D3DXVECTOR3* _pUpVec,
		float _viewAngle)
	{
		// ビュー座標変換行列の作成.
		D3DXMatrixLookAtLH(&m_MatView, _pEyePos, _pLookPos, _pUpVec);

		// プロジェクション座標変換行列の作成.
		D3DXMatrixPerspectiveFovLH(&m_MatProj, static_cast<float>(D3DXToRadian(_viewAngle)), m_Aspect, m_NearZ, m_FarZ);
	}
}
