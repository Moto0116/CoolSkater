/**
 * @file	MyDefine.h
 * @brief	アプリケーション内定義ヘッダ
 * @author	morimoto
 */
#ifndef MYDEFINE_H
#define MYDEFINE_H

/**
 * 描画オブジェクトの優先順位列挙子
 */
enum DRAW_OBJECT_PRIORITY
{
	NORMAL_OBJECT = 0,		//!< 通常オブジェクト.
	TRANSPARENT_OBJECT = 1	//!< 透過オブジェクト.
};


#endif // !MYDEFINE_H
