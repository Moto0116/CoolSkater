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
#include "TaskManager\TaskBase\UpdateTask\UpdateTask.h"
#include <vector>
#include <D3DX11.h>
#include <D3DX10.h>


class GimmickBase;


class GimmickManager : public Lib::ObjectBase
{
public:
	enum GIMMICK_TYPE
	{
		NONE_GIMMICK,
		OBSTACLE_GIMMICK,
		OBSTACLECAR_GIMMICK,
		JUMPRAMP_GIMMICK,
		HANRAIL_GIMMICK,
		POINT_GIMMICK,
		SCAFFOLD_GIMMICK,
		SYRINGE_GIMMICK,
		GOLDEN_GIMMICK,
		GIMMICK_MAX
	};

	static const float m_DrawRangeMin;
	static const float m_DrawRangeMax;

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
	enum MAP_TYPE
	{
		MAP1_TYPE,
		MAP2_TYPE,
		MAP3_TYPE,
		MAP4_TYPE,
		MAP5_TYPE,
		MAP6_TYPE,
		MAP7_TYPE,
		MAP8_TYPE,
		MAP9_TYPE,
		MAP10_TYPE,
		MAP11_TYPE,
		MAP12_TYPE,
		MAP13_TYPE,
		MAP14_TYPE,
		MAP15_TYPE,
		MAP_TYPE_MAX
	};

	struct MAP_DATA
	{
		int GimmickType;
		int GimmickStatus;
		D3DXVECTOR2 GimmickPos;
	};

	static LPCTSTR		m_FileNames[MAP_TYPE_MAX];

	Lib::UpdateTask*	m_pUpdateTask;	//!< 更新タスクオブジェクト.

	GimmickBase*		m_pGimmick[GIMMICK_MAX];

	std::vector<float>	m_StageData;
	std::vector<MAP_DATA> m_MapData[MAP_TYPE_MAX];	//!< ランダムに配置するマップのギミックデータ.
	bool m_IsStageLoad;
	float m_StageProgress;


};


#endif // !GIMMICKMANAGER_H
