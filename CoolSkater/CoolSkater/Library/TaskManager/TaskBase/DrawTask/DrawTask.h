/**
 * @file	DrawTask.h
 * @brief	描画のタスククラス定義
 * @author	morimoto
 */
#ifndef LIB_DRAWTASK_H
#define LIB_DRAWTASK_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "..\TaskBase.h"
#include "..\..\TaskManager.h"


namespace Lib
{
	class ObjectBase;

	/**
	 * 描画のタスククラス
	 */
	class DrawTask : public TaskBase<float>
	{
	public:
		/**
		 * コンストラクタ
		 */
		DrawTask();
	
		/**
		 * デストラクタ
		 */
		virtual ~DrawTask();
	
		/**
		 * 描画タスク実行
		 */
		virtual void Run();
	
		/**
		 * 描画オブジェクトをセット
		 * @param[in] _pObject 描画オブジェクト
		 */
		void SetDrawObject(ObjectBase* _pObject);

	private:
		ObjectBase* m_pObject;	//!< 描画を行うオブジェクト.
	
	};


	typedef TaskManager<DrawTask> DrawTaskManager;
}


#endif // !LIB_DRAWTASK_H
