/**
 * @file	StageManager.h
 * @brief	ステージ管理クラス定義
 * @author	morimoto
 */
#ifndef STAGEMANAGER_H
#define STAGEMANAGER_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "ObjectManagerBase\ObjectManagerBase.h"


/**
 * ステージ管理クラス
 */
class StageManager : public Lib::ObjectManagerBase
{
public:
	/**
	 * コンストラクタ
	 */
	StageManager();

	/**
	 * デストラクタ
	 */
	virtual ~StageManager();

	/**
	 * 初期化処理
	 * @return 初期化に成功したらtrue 失敗したらfalse
	 */
	virtual bool Initialize();

	/**
	 * 終了処理
	 */
	virtual void Finalize();

};


#endif // !STAGEMANAGER_H
