/**
 * @file	GimmickManager.h
 * @brief	ギミック管理クラス定義
 * @author	morimoto
 */
#ifndef GIMMICKMANAGER_H
#define GIMMICKMANAGER_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "ObjectManagerBase\ObjectBase\ObjectBase.h"

#include <vector>


class GimmickManager : public Lib::ObjectBase
{
public:
	enum MAPGIMMICK
	{
		NONE_GIMMICK,
		OBSTACLE_GIMMICK,
		JUMPRAMP_GIMMICK,
		HANRAIL_GIMMICK
	};

	/**
	 * コンストラクタ
	 */
	GimmickManager();

	/**
	 * デストラクタ
	 */
	virtual ~GimmickManager();

	/**
	 * 初期化処理
	 * @return 初期化に成功したか
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
	std::vector<Lib::ObjectBase*> m_pObjects;
	std::vector<int> m_StageData;

};


#endif // !GIMMICKMANAGER_H
