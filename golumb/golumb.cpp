// golumb.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//global vars
int BestLength[] = {0,1,3,6,11,17,25,34,44,55,72,85,106,127,
							151,177,199,216,246,283,333,358,372,425};
int			SolutionsFound=0;	//Count solutions found
bool		Done=false;			//Stop recursion
double		Count=-t0;
double		SaveCount=t0+SaveCountDown;
bool		TrapF=true;
int			c1=1;
int			ch;
int			Nseg=12, NsegD2, Lmax, LmaxD2;
short		ya=0;
short		ConX=60, ConY=25;//Initial screen size
RulerT		BestLA, Ruler;
bool		MidSegmentA[MaxSegs+1];
int			Difference[MaxSegs+1];
DiffListT	DiffList;
HANDLE		hStdIn, hStdOut; // standard input, output handles
time_t		ltime, ltime1;

int _cdecl main(int argc, char* argv[])
{
	DWORD dwMode;
	char *szLineBuf; // buffer to read the index number into 
	CONSOLE_CURSOR_INFO cci; // used when turning off the cursor 
	CONSOLE_SCREEN_BUFFER_INFO csbi; // used to get cursor position
	OSVERSIONINFO osVer; // for GetVersionEx() 
	
	// check if Win32s, if so, display notice and terminate */
	osVer.dwOSVersionInfoSize = sizeof(osVer);
	if(!GetVersionEx(&osVer)) PERR(false, "GetVersionEx");
	if (osVer.dwPlatformId == VER_PLATFORM_WIN32s)
    {
		MessageBoxA(NULL, 
			"This application cannot run on Windows 3.1.\n"
			"This application will now terminate.",
			"Error: Windows NT or Windows 95 Required to Run",  MB_OK );
		return(1);
    }
	
	if(!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
		PERR(false, "GetConsoleScreenBufferInfo");
	
	// let's put up a meaningful console title
	if(!SetConsoleTitleA("Golomb Ruler Program: N6: 23-Nov-2010"))
		PERR(false, "SetConsoleTitle");
	
	// get the standard handles
	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	PERR(hStdOut != INVALID_HANDLE_VALUE, "GetStdHandle");
	hStdIn = GetStdHandle(STD_INPUT_HANDLE);
	PERR(hStdIn != INVALID_HANDLE_VALUE, "GetStdHandle");
	
	// set up mouse and window input 
	if(!GetConsoleMode(hStdIn, &dwMode))
		PERR(false, "GetConsoleMode");
	// when turning off ENABLE_LINE_INPUT, you MUST also turn off 
	// ENABLE_ECHO_INPUT. 
	if(!SetConsoleMode(hStdIn, (dwMode & ~(ENABLE_LINE_INPUT
		| ENABLE_ECHO_INPUT | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT))))
		PERR(false, "SetConsoleMode");
	// save then hide the cursor
	cci.dwSize = 100;
	cci.bVisible = FALSE;
	if(!SetConsoleCursorInfo(hStdOut, &cci))
		PERR(false, "SetConsoleCursorInfo");
	// resize console to ConX x ConY
	resizeConBufAndWindow(hStdOut, ConX, ConY);
	// set attributes for new writes to the console
	if(!SetConsoleTextAttribute(hStdOut, BACKGROUND_BLUE 
		| FOREGROUND_WHITE))
		PERR(false,"SetConsoleTextAttribute");
	cls(hStdOut); // will clear screen to newly set color attribute 
	// Make a line buffer
	szLineBuf = (char *) malloc(getConX(hStdOut));
	PERR(szLineBuf, "malloc");

	//Initialize vars
	int i, StartDifference=1;

	Lmax=BestLength[Nseg];//Nseg*(Nseg+1)>>1;
	for(i=0;i<=MaxSegs;i++) Difference[i]=0;
	for(i=0;i<=MaxSegs;i++) Ruler[i]=0;
	GoHelp(hStdOut);
	for(i=0;i<=DiffListEnd;i++) DiffList[i]=0;
	time( &ltime );//log current time
	for(;;) {
		NsegD2=Nseg>>1;
		LmaxD2=(Lmax+1)>>1;
		//So that CurrentLength >= LmaxD2 works with odd Lmax
		for(i=1;i<Nseg;i++) {//precompute this now
			BestLA[i]=Lmax-BestLength[Nseg-i-1];
			MidSegmentA[i]=(i==NsegD2)&&(Nseg!=2*(Nseg>>1));
		}
		Ruler[1]=StartDifference;
		DiffList[StartDifference]=1;
		N6(1);
		DiffList[StartDifference]=0;
		if(Done) {
			SaveCount+=t0-1;
			Count=1-t0;c1=1;//Trap to Scanner
			for(i=1;i<=MaxSegs;i++) Ruler[i]=0;
			for(i=1;i<=DiffListEnd;i++) DiffList[i]=0;
			Done=false;
			SolutionsFound=0;
			StartDifference=1;
		}
		else {
			if(StartDifference<Lmax-BestLength[Nseg-1]) 
				StartDifference++;
			else if(SolutionsFound) {
				SaveCount+=c1-1;
				Count+=1-c1;
				c1=1;//Trap to Scanner
				TrapF=true;
			}
			else if(StartDifference>=Lmax-BestLength[Nseg-1]) {
				Lmax++;
				StartDifference=1;
				ClrEOL(hStdOut, 0, ya);
				time( &ltime1 );
				sprintf_s(szLineBuf, 80, "%4u%4u%12u%15.0f%10u", Nseg, Lmax, SolutionsFound, Count+t0-c1, ltime1-ltime );
				myPuts(hStdOut, szLineBuf);
				ClrEOL(hStdOut, 0, ya);
				szLineBuf[0]=0;
				myPuts(hStdOut, szLineBuf);	ClrEOL(hStdOut, 0, ya);
				GoToXY(hStdOut, 0, ya - 2);
			}
		}
	}
	free(szLineBuf); /* free allocated line buffer */
	return 0;
}
