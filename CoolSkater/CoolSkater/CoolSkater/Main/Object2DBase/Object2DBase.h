﻿/**
 * @file   Object3DBase.cpp
 * @brief  Object3DBaseクラスの実装
 * @author morimoto
 */
#ifndef OBJECT2DBASE_H
#define OBJECT2DBASE_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "ObjectManagerBase\ObjectBase\ObjectBase.h"
#include "DirectX11\Vertex2D\Vertex2D.h"
#include "TaskManager\TaskBase\DrawTask\DrawTask.h"
#include "TaskManager\TaskBase\UpdateTask\UpdateTask.h"


/**
 * 2Dオブジェクトの基底クラス
 */
class Object2DBase : public Lib::ObjectBase
{
public:
	/**
	 * コンストラクタ
	 */
	Object2DBase();

	/**
	 * デストラクタ
	 */
	virtual ~Object2DBase();
	
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

protected:
	/** 
	 * 2D描画オブジェクトの生成
	 * @return 生成に成功したらtrue 失敗したらfalse
	 */
	bool CreateVertex2D();

	/**
	 * 2D描画オブジェクトの破棄
	 */
	void ReleaseVertex2D();


	Lib::DrawTask*		m_pDrawTask;	//!< 描画タスクオブジェクト.
	Lib::UpdateTask*	m_pUpdateTask;	//!< 更新タスクオブジェクト.

	Lib::Vertex2D*	m_pVertex;		//!< 2D描画オブジェクト.
	D3DXVECTOR2		m_Pos;			//!< 描画座標.
	D3DXVECTOR2		m_Size;			//!< 描画サイズ.
	int				m_TextureIndex;	//!< テクスチャのインデックス.
	
};


#endif // !OBJECT2DBASE_H
