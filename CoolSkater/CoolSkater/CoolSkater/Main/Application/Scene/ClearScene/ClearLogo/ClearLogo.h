/**
 * @file	ClearLogo.h
 * @brief	クリアロゴクラス定義
 * @author	morimoto
 */
#ifndef CLEARLOGO_H
#define CLEARLOGO_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "Main\Object2DBase\Object2DBase.h"


class ClearLogo : public Object2DBase
{
public:
	/**
	 * コンストラクタ
	 */
	ClearLogo();

	/**
	 * デストラクタ
	 */
	virtual ~ClearLogo();

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
	D3DXVECTOR2 m_TargetPos;
	D3DXVECTOR2 m_MinSize;
	D3DXVECTOR2 m_MaxSize;
	float m_Speed;
	float m_Time;
	float m_Angle;
	D3DXVECTOR2 m_TmpPos;

	bool m_IsMove;

};


#endif // !CLEARLOGO_H
