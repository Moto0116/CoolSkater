﻿/**
 * @file   Object2DBase.cpp
 * @brief  Object2DBaseクラスの実装
 * @author morimoto
 */

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "Object2DBase.h"

#include "Debugger\Debugger.h"
#include "DirectX11\GraphicsDevice\GraphicsDevice.h"
#include "DirectX11\TextureManager\TextureManager.h"


//----------------------------------------------------------------------
// Constructor	Destructor
//----------------------------------------------------------------------
Object2DBase::Object2DBase() : 
	m_pVertex(nullptr),
	m_Pos(D3DXVECTOR2(0, 0)),
	m_Size(D3DXVECTOR2(0, 0))
{
	m_pDrawTask = new Lib::DrawTask();
	m_pUpdateTask = new Lib::UpdateTask();

	m_pDrawTask->SetDrawObject(this);
	m_pUpdateTask->SetUpdateObject(this);
}

Object2DBase::~Object2DBase()
{
	delete m_pUpdateTask;
	delete m_pDrawTask;
}


//----------------------------------------------------------------------
// Public Functions
//----------------------------------------------------------------------
bool Object2DBase::Initialize()
{
	return true;
}

void Object2DBase::Finalize()
{
}

void Object2DBase::Update()
{
}

void Object2DBase::Draw()
{
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&m_Pos);
	m_pVertex->Draw();
}


//----------------------------------------------------------------------
// Protected Functions
//----------------------------------------------------------------------
bool Object2DBase::CreateVertex2D()
{
	m_pVertex = new Lib::Vertex2D();
	if(!m_pVertex->Initialize(SINGLETON_INSTANCE(Lib::GraphicsDevice)))
	{
		OutputErrorLog("2D描画オブジェクトの初期化に失敗しました");
		SafeDelete(m_pVertex);
		return false;
	}

	if (!m_pVertex->CreateVertexBuffer(&m_Size))
	{
		OutputErrorLog("頂点バッファの生成に失敗しました");
		m_pVertex->Finalize();
		SafeDelete(m_pVertex);
		return false;
	}

	if (!m_pVertex->WriteConstantBuffer(&m_Pos))
	{
		OutputErrorLog("定数バッファへの書き込みに失敗しました");
		m_pVertex->ReleaseVertexBuffer();
		m_pVertex->Finalize();
		SafeDelete(m_pVertex);
		return false;
	}

	return true;
}

void Object2DBase::ReleaseVertex2D()
{ 
	if (m_pVertex != nullptr)
	{
		m_pVertex->ReleaseVertexBuffer();
		m_pVertex->Finalize();
		SafeDelete(m_pVertex);
	}
}
