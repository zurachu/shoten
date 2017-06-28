/******************************************************************************
//  商店 P/ECE移植版
//  (c)2003 てとら★ぽっと
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <piece.h>
#include "gamelib.h"

extern unsigned char PLAYER[];
extern unsigned char YAMADA[];
extern unsigned char ZAVU[];
PIECE_BMP pl, ym, zv;

int plY, jump, ymX, zvX, zvSpd, zvCnt, zvAll, score;

#define GM_TITLE	1
#define GM_MAIN		2
#define GM_YAMADA1	4
#define GM_YAMADA2	8
#define GM_RESULT	16
char gameMode;

BOOL demo;

//=============================================================================
//  アプリケーションの初期化
//=============================================================================
void pceAppInit( void )
{
	PCETIME pTime;

	pceAppSetProcPeriod( 25 );
	App_Init();	// 起動時処理
	
	Get_PieceBmp( &pl, PLAYER );
	Get_PieceBmp( &ym, YAMADA );
	Get_PieceBmp( &zv, ZAVU );
	
	pceFontSetTxColor( 3 );
	pceFontSetBkColor( 0 );
	
	// 背景
	pceLCDPaint( 0, 0, 0, 128, 88 );
	pceLCDPaint( 3, 0, 83, 128, 5 );
	pceLCDPaint( 3, 0, 78, 128, 4 );
	pceLCDPaint( 2, 0, 74, 128, 3 );
	pceLCDPaint( 2, 0, 71, 128, 2 );
	pceLCDPaint( 1, 0, 69, 128, 1 );
	
	pceTimeGet( &pTime );
	srand( pTime.s100 );	// 乱数を初期化

	plY = 61;
	gameMode = GM_TITLE;
	demo = FALSE;
}


//=============================================================================
//  メインプロセス
//=============================================================================
void pceAppProc( int cnt )
{
	if( pcePadGet() & ( TRG_LF | TRG_RI | TRG_UP | TRG_DN ) ) {
		demo = !demo;
		LCDUpdate = TRUE;
	}
	if( demo ) { pceAppActiveResponse( AAR_ACTIVE ); }
	if( pcePadGet() & TRG_D ) { pceAppReqExit( 0 ); }

	switch( gameMode ) {
		case GM_TITLE:
			if( pcePadGet() & TRG_C ) {
				pceLCDPaint( 0, 0, 0, 128, 69 );
				plY = 61;
				jump = -1;
				zvX = 128;
				zvSpd = 0;
				zvCnt = zvAll = score = 0;
				gameMode = GM_MAIN;
				LCDUpdate = TRUE;
			}
			break;
		case GM_MAIN:
			if( zvAll <= 10 ) {
				pceLCDPaint( 0, zvX, 67 - zvCnt * 2, 12, 2 );
				zvX += zvSpd;
			}
			if( pcePadGet() & TRG_A && jump == -1 ) { jump = 10; }
			if( jump > 0 ) {	// 上昇
				pceLCDPaint( 0, 100, plY, 8, 8 );
				plY -= 2;
				jump--;
			} else {			// 下降
				if( plY < 61 - zvCnt * 2 ) {
					pceLCDPaint( 0, 100, plY, 8, 8 );
					plY += 2;
					if( zvX > 92 && zvX < 104 && plY == 61 - zvCnt * 2 - 2 ) {
						Draw_Object( &zv, zvX, 67 - zvCnt * 2, 0, 0, 12, 2, DRW_NOMAL );
						zvCnt++;
						score += 10 - abs( zvX - 98 );
						jump = -1;
					}
				} else {		// 着地
					jump = -1;
				}
			}
			// 座布団消滅
			if( zvX >= 128 || ( zvX > 92 && zvX < 104 && jump == -1 ) ) {
				zvAll++;
				zvX = -12;
				zvSpd = rand() % 3 + 1;
			}
			if( zvAll > 10 && jump == -1 ) {
				if( zvCnt >= 1 && zvCnt <= 3 ) {
					gameMode = GM_YAMADA1;
					ymX = -8;
				} else {
					gameMode = GM_RESULT;
				}
			}
			LCDUpdate = TRUE;
			break;
		case GM_YAMADA1:
			pceLCDPaint( 0, ymX, 61, 8, 8 );
			ymX++;
			if( ymX >= 84 ) {
				gameMode = GM_YAMADA2;
				pceLCDPaint( 0, 93, 41, 25, 28 );
				zvCnt = score = 0;
				plY = 61;
			}
			LCDUpdate = TRUE;
			break;
		case GM_YAMADA2:
			pceLCDPaint( 0, ymX, 61, 8, 8 );
			ymX--;
			if( ymX <= -8 ) { gameMode = GM_RESULT; }
			LCDUpdate = TRUE;
			break;
		case GM_RESULT:
			if( pcePadGet() & TRG_C ) {
				pceLCDPaint( 0, 33, 15, 62, 23 );
				gameMode = GM_TITLE;
				LCDUpdate = TRUE;
			}
			break;
	}
	
	if( LCDUpdate ) {
		if( demo ) {
			pceFontSetType( 2 );
			wFontPrintf( 1, 0, 0, "demo" );
		} else {
			pceLCDPaint( 0, 0, 0, 16, 6 );
		}
		switch( gameMode ) {
			case GM_TITLE:
				pceFontSetType( 0 );
				wFontPrintf( 0, 29, 10, "商店 for P/ECE" );
				pceFontSetType( 2 );
				wFontPrintf( 0, 40, 28, "2003 ZURACHU" );
				wFontPrintf( 0, 38, 38, "ORIGINAL 1994" );
				wFontPrintf( 0, 34, 44, "URESHISOUNA INU" );
				wFontPrintf( 0, 44, 60, "PUSH START" );
				break;
			case GM_MAIN:
				Draw_Object( &zv, zvX, 67 - zvCnt * 2, 0, 0, 12, 2, DRW_NOMAL );
				break;
			case GM_YAMADA1:
				Draw_Object( &ym, ymX, 61, 0, 0, 8, 8, DRW_NOMAL );
				break;
			case GM_YAMADA2:
				Draw_Object( &ym, ymX, 61, 0, 0, 8, 8, DRW_REVX );
				break;
			case GM_RESULT:
				pceFontSetType( 0 );
				wFontPrintf( 0, 34, 16, "座布団：%2d枚", zvCnt );
				if( score >= 100 ) {
					wFontPrintf( 0, 34, 28, "スコア：満点" );
				} else {
					wFontPrintf( 0, 34, 28, "スコア：%2d点", score );
				}
				pceFontSetType( 2 );
				wFontPrintf( 0, 44, 60, "PUSH START" );
				break;
		}
		Draw_Object( &pl, 100, plY, 0, 0, 8, 8, DRW_NOMAL );
		
		pceLCDTrans();
		LCDUpdate = FALSE;
	}
}

//=============================================================================
//  アプリケーションの終了
//=============================================================================
void pceAppExit( void )
{
	App_Exit();	// 終了時処理
}
