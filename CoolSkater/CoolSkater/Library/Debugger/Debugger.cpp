/**
 * @file	Debugger.cpp
 * @brief	デバッグ機能実装
 * @author	morimoto
 */

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "Debugger.h"

#include <stdio.h>
#include <stdarg.h>


namespace Lib
{
	namespace Debugger
	{
#ifdef _DEBUG

		//----------------------------------------------------------------------
		// Static Variables
		//----------------------------------------------------------------------
		static const unsigned int						g_DebugLogMax = 512;	//!< デバッグログの最大文字数.
		static FILE*									g_pLogFile = nullptr;	//!< デバッグ用ファイルポインタ.
		static std::chrono::system_clock::time_point	g_StartTime;			//!< 計測開始時間.
		static std::chrono::system_clock::time_point	g_EndTime;				//!< 計測終了時間.
		static LONGLONG									g_DiffTime;				//!< 計測時間(msec単位).
		static const int								g_StackMax = 100;		//!< 保持するスタックフレームの最大数.
		static const int								g_SymbolNameMax = 256;	//!< シンボル名の最大文字数.


		//----------------------------------------------------------------------
		// Functions
		//----------------------------------------------------------------------
		void CheckMemoryLeak()
		{
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		}

		void OutputDebugLog(TCHAR* _pStr, ...)
		{
			TCHAR OutputStr[g_DebugLogMax];
			va_list ArgsList;

			va_start(ArgsList, _pStr);

			vsprintf_s(OutputStr, sizeof(OutputStr), _pStr, ArgsList);
			OutputDebugString(OutputStr);

			va_end(ArgsList);
		}

		void OpenLogFile(TCHAR* _pFilePath)
		{
			fopen_s(&g_pLogFile, _pFilePath, "w");
		}

		void WriteLogFile(TCHAR* _pStr, ...)
		{
			TCHAR OutputStr[g_DebugLogMax];
			va_list ArgsList;

			va_start(ArgsList, _pStr);

			vsprintf_s(OutputStr, sizeof(OutputStr), _pStr, ArgsList);
			fprintf(g_pLogFile, OutputStr);

			va_end(ArgsList);
		}

		void CloseLogFile()
		{
			fclose(g_pLogFile);
		}

		void StartTimer()
		{
			g_StartTime = std::chrono::system_clock::now();
		}

		void EndTimer()
		{
			g_EndTime = std::chrono::system_clock::now();
			auto Diff = g_EndTime - g_StartTime;
			g_DiffTime = std::chrono::duration_cast<std::chrono::milliseconds>(Diff).count();
		}

		LONGLONG GetTime()
		{
			return g_DiffTime;
		}

		void OutputStackFrame()
		{
			// 現在のプロセスの擬似ハンドルを取得して初期化.
			HANDLE Process = GetCurrentProcess();	
			SymInitialize(Process, nullptr, TRUE);

			// スタック情報をキャプチャ.
			void* pStack[g_StackMax];
			USHORT Frames = CaptureStackBackTrace(0, g_StackMax, pStack, nullptr);

			// シンボル情報構造体を初期化.
			SYMBOL_INFO* pSymbol = reinterpret_cast<SYMBOL_INFO*>(calloc(sizeof(SYMBOL_INFO), Frames));
			pSymbol->MaxNameLen = g_SymbolNameMax;
			pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);

			Lib::Debugger::OutputDebugLog("\n--------------------StackFrame--------------------\n\n");
			for (int i = 0; i < Frames; i++)
			{
				SymFromAddr(Process, reinterpret_cast<DWORD64>(pStack[i]), nullptr, pSymbol);	// シンボル情報取得.

				Lib::Debugger::OutputDebugLog("Frame  : %i\n", Frames - i - 1);			// フレーム番号.
				Lib::Debugger::OutputDebugLog("Name   : %s\n", pSymbol->Name);			// フレーム名.
				Lib::Debugger::OutputDebugLog("Addres : 0x%0X\n", pSymbol->Address);	// フレームアドレス.
				Lib::Debugger::OutputDebugLog("Size   : %lu\n\n", pSymbol->Size);		// フレームサイズ.
			}

			free(pSymbol);

			SymCleanup(Process); // プロセスの解放.
		}

#else

		//----------------------------------------------------------------------
		// Functions
		//----------------------------------------------------------------------
		void CheckMemoryLeak()
		{
		}

		void OutputDebugLog(TCHAR* _pStr, ...)
		{
		}

		void OpenLogFile(TCHAR* _pFilePath)
		{
		}

		void WriteLogFile(TCHAR* _pStr, ...)
		{
		}

		void CloseLogFile()
		{
		}

		void StartTimer()
		{
		}

		void EndTimer()
		{
		}

		LONGLONG GetTime()
		{
			return 0;
		}

		void OutputStackFrame()
		{
		}

#endif // _DEBUG
	}
}

