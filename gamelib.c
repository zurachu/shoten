/*******************************************************************************
//  �ėp�Q�[�����C�u����
//  (c)2003 �ĂƂ灚�ۂ���
*******************************************************************************/

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <piece.h>
#ifdef USE_PVNSUNPK
#include "pvnsunpk.h"
#endif
#ifdef USE_HTOMEI
#include "htomei2.h"
#endif
#ifdef USE_MUSLIB
#include <muslib.h>
#endif
#include <smcvals.h>

unsigned char *vbuff;
BOOL LCDUpdate;

//=============================================================================
//  �N��������
//=============================================================================
void App_Init()
{
	pceLCDDispStop();		// �t���\���𖳌���
	vbuff = pceHeapAlloc( 128*88 );
	memset( vbuff, 0, 128*88 );
	pceLCDSetBuffer( vbuff );

#ifdef USE_MUSLIB
	loadInst();				// �h�������������L�b�g�̏�����
	InitMusic();			// ���y���C�u�����̏�����
#endif

	pceLCDDispStart();		//�t���\����L����
	LCDUpdate = TRUE;
}


//=============================================================================
//  �I��������
//=============================================================================
void App_Exit()
{
#ifdef USE_MUSLIB
	StopMusic();			// BGM�̒�~
#endif
	pceWaveStop( 0 );		// ���̒�~
}


//=============================================================================
//  P/ECE�p�r�b�g�}�b�v����
//=============================================================================
void Get_PieceBmp( PIECE_BMP* pBmp, unsigned char* data )
{
	memcpy( &(pBmp->header), data, sizeof(PBMP_FILEHEADER) );
	pBmp->buf = data + sizeof(PBMP_FILEHEADER);
	pBmp->mask = pBmp->buf + ( pBmp->header.w * pBmp->header.h ) / 4;
}


//=============================================================================
//  P/ECE�p�r�b�g�}�b�v�`��
//=============================================================================
void Draw_Object( PIECE_BMP *pBmp, int dx, int dy, int sx, int sy, int w, int h, int param )
{
	DRAW_OBJECT dObj;

	pceLCDSetObject( &dObj, pBmp, dx, dy, sx, sy, w, h, param );
	pceLCDDrawObject( dObj );
}

//=============================================================================
//  P/ECE�pPCM��������
//=============================================================================
void Get_PieceWave( PCEWAVEINFO* pWav, unsigned char* data )
{
	*pWav = *((PCEWAVEINFO*)(data+8));
	pWav->pData = data + 8 + sizeof(PCEWAVEINFO);
}


//=============================================================================
//  ����蕶���`��
//=============================================================================
int wFontPrintf( short c, short x, short y, const char* fmt, ... )
{
	extern unsigned char _def_vbuff[]; /* ������W�J�ɗ��p */
	int ret;
	va_list ap;
	short i, j;

	va_start( ap, fmt );
	ret = vsprintf( _def_vbuff, fmt, ap );
	va_end( ap );

	pceFontSetBkColor( FC_SPRITE );
	if( c == 0 ) { pceFontSetTxColor( 3 ); }
	else		 { pceFontSetTxColor( 0 ); }
	for( i = y - 1; i <= y + 1; i++ ) {
		for( j = x - 1; j <= x + 1; j++ ) {
			if( j != x || i != y ) {
				pceFontSetPos( j, i );
				pceFontPutStr( _def_vbuff );
			}
		}
	}
	if( c == 0 ) { pceFontSetTxColor( 0 ); }
	else		 { pceFontSetTxColor( 3 ); }
	pceFontSetPos( x, y );
	pceFontPutStr( _def_vbuff );

	return( ret );
}


