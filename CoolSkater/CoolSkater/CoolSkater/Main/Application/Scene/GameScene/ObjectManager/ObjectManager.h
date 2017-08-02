/**
 * @file	ObjectManager.h
 * @brief	オブジェクト管理クラス定義
 * @author	morimoto
 */
#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include <vector>

#include "ObjectManagerBase\ObjectManagerBase.h"


/**
 * オブジェクト管理クラス
 */
class ObjectManager : public Lib::ObjectManagerBase
{
public:
	/**
	 * コンストラクタ
	 */
	ObjectManager();

	/**
	 * デストラクタ
	 */
	virtual ~ObjectManager();

	/**
	 * 初期化処理
	 * @return 初期化に成功したらtrue 失敗したらfalse
	 */
	virtual bool Initialize();

	/**
	 * 終了処理
	 */
	virtual void Finalize();


private:
	std::vector<Lib::ObjectManagerBase*> m_pObjectManagers;	//!< オブジェクト管理クラス.

};


#endif // !OBJECTMANAGER_H
