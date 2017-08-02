/**
 * @file	VertexManager.cpp
 * @brief	頂点管理クラス実装
 * @author	morimoto
 */

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "VertexManager.h"

#include "..\GraphicsDevice\GraphicsDevice.h"
#include "Vertex\Vertex.h"


namespace Lib
{
	//----------------------------------------------------------------------
	// Constructor	Destructor
	//----------------------------------------------------------------------
	VertexManager::VertexManager()
	{
	}

	VertexManager::~VertexManager()
	{
	}


	//----------------------------------------------------------------------
	// Public Functions
	//----------------------------------------------------------------------
	bool VertexManager::Initialize(GraphicsDevice* _pGraphicsDevice)
	{
		m_pGraphicDevice = _pGraphicsDevice;
		return true;
	}

	void VertexManager::Finalize()
	{
		for (auto itr = m_pVertexs.begin(); itr != m_pVertexs.end(); itr++)
		{
			SafeDelete(*itr);
		}
	}
}
