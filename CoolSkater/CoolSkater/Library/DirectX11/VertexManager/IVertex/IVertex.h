/**
 * @file	IVertex.h
 * @brief	頂点インターフェース定義
 * @author	morimoto
 */
#ifndef LIB_IVERTEX_H
#define LIB_IVERTEX_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include <Windows.h>

#include "..\..\..\Define\Define.h"


namespace Lib
{
	/**
	 * 頂点インターフェース
	 */
	interface IVertex
	{
	public:
		/**
		 * コンストラクタ
		 */
		IVertex();

		/**
		 * デストラクタ
		 */
		virtual ~IVertex();

	};
}


#endif // !LIB_IVERTEX_H
