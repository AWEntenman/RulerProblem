// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#if !defined(AFX_STDAFX_H__93683568_C4F2_11D2_8538_004005A1DA6B__INCLUDED_)
#define AFX_STDAFX_H__93683568_C4F2_11D2_8538_004005A1DA6B__INCLUDED_
#endif // !defined(AFX_STDAFX_H__93683568_C4F2_11D2_8538_004005A1DA6B__INCLUDED_)

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers


#include <stdio.h>
#include <tchar.h>
#include <STDLIB.H>
#include <windows.h>
#include <string.h>
#include <time.h>
#include "console.h"

#define MaxSegs			23
#define DiffListEnd		550
#define t0				0x10000
#define SaveCountDown	2e7

// console attributes
#define DefaultConsoleAttrib BACKGROUND_BLUE | FOREGROUND_WHITE | FOREGROUND_INTENSITY
#define AltConsoleAttrib BACKGROUND_BLUE | FOREGROUND_YELLOW | FOREGROUND_INTENSITY

typedef BYTE	DiffListT[DiffListEnd+1];

typedef int RulerT[MaxSegs+1];
typedef struct  {
	int nsg, //number of segments of current work
		lev, //current recursion level
		lmx; //current Lmax
	double cntr; //the count
	RulerT r1; //the work so far
} InitRecordT;

#define PERR(bSuccess, api) {if (!(bSuccess)) perr(__FILE__, __LINE__, \
    api, GetLastError());}

// function prototypes
extern void _fastcall	N6(int Level);
extern void		Scanner(int Level);
extern void		Finally(void);
extern void		cls(HANDLE hConsole);
extern void		GoHelp(HANDLE hStdOut);
extern void		ClrEOL(HANDLE hStdOut, short x, short y);
extern void		WriteIt(HANDLE hConsole, int Level);
extern bool		KeyPressed(void);
extern int		ReadKey();
extern void		perr(PCHAR szFileName, int line, 
					 PCHAR szApiName, DWORD dwError);
extern void		GoToXY(HANDLE hOut, short x, short y);
extern void		Write(HANDLE hOut, PCHAR s);


//Variable prototype
extern int			BestLength[];
extern int			SolutionsFound;	//Count solutions
extern bool			Done;			//Stop recursion
extern double		Count;
extern double		SaveCount;
extern bool			TrapF;
extern int			c1;
extern int			ch;
extern InitRecordT	Irec;
extern int			Nseg, NsegD2, Lmax, LmaxD2;
extern short		ya;
extern short		ConX, ConY; //Initial screen size
extern RulerT		BestLA, Ruler;
extern bool			MidSegmentA[MaxSegs+1];
extern int			Difference[MaxSegs+1];
extern DiffListT	DiffList;
extern HANDLE		hStdIn, hStdOut; // standard input, output handles
extern time_t		ltime;//starting time




// TODO: reference additional headers your program requires here
