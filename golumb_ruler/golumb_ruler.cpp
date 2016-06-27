//The original grs2.f program by James B. Shearer adapted to C++ below

/*
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c        Program to find Golomb rulers by exhaustive backtrack search
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c                  IBM SOFTWARE DISCLAIMER
c
c   grs2.f (version 1.0)
c   Copyright (1999,1986)
c   International Business Machines Corporation
c
c   Permission to use, copy, modify and distribute this software for
c any purpose and without fee is hereby granted, provided that this
c copyright and permission notice appear on all copies of the software.
c The name of the IBM corporation may not be used in any advertising or
c publicity pertaining to the use of the software.  IBM makes no
c warranty or representations about the suitability of the software
c for any purpose.  It is provided "AS IS" without any express or
c implied warranty, including the implied warranties of merchantability,
c fitness for a particular purpose and non-infringement.  IBM shall not
c be liable for any direct, indirect, special or consequential damages
c resulting from the loss of use, data or projects, whether in action
c of contract or tort, arising out or in the connection with the use or
c performance of this software.
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c        Author: James B. Shearer
c        email:  jbs@watson.ibm.com
c        website: http://www.research.ibm.com/people/s/shearer/
c        date: 1999 (partially derived from code written in 1986)
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c        This program constructs all Golomb rulers with n marks and
c   length k (or less) by exhaustive backtrack search.
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c   This is a lightly tuned version of grs1.f.
c
c   Loop 60 was rewritten to eliminate the if test.  This included
c reversing the meaning of 0 and 1 entries in the id array as this
c makes the new form of loop 60 more efficient.
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
*/
// golumb_ruler.cpp : Defines the entry point for the console application.
//
//  Some FORTRAN variable names are replaced for greater code clarity.



#include "stdafx.h"
#include <iostream>
#include <cstdlib>

using namespace std;

int ruler(int Marks, int Length); //function prototype

#define MaxMarks	26
#define MaxLength	528
#define nprint		25

int _tmain(int argc, _TCHAR* argv[])
{

	int BestLength[] = {0,1,3,6,11,17,25,34,44,55,72,85,106,127,
		151,177,199,216,246,283,333,358,372,425,480};
	int marks;
	int length;

	for(;;) {
		for(;;) {
			cout << "Enter Number of marks, zero to quit: ";
			cin >> marks;
			if(marks <= 0) return 0;// n=0 to end program
			if (marks>MaxMarks)
				cout << "Too many marks, the max is " << MaxMarks << endl;
			else if (marks<=2)
				cout << "Too few marks, the min is 3\n" ;
			else
				break;
		}
		cout << "The shortest ruler for " << marks << " marks is " << BestLength[marks-1] << endl;

		for(;;) {
		cout << "Enter length, zero to quit: ";
		cin >> length;
		if (length <= 0)
			return 0;// n=0 to end program
		else if (length>MaxLength)
			cout << "Length too long, must be less than " << MaxLength << endl;
		else
			break;
		}
		cout << "\n\n";
		//call ruler
		ruler(marks, length);
	}
}

int ruler(int Marks, int Length)// was (n, k)
{
	//Fortran Arrays Start at 1
	int ip[MaxMarks+1]; 
	int ie[MaxMarks+1];
	int m[MaxMarks+1];
	int mub[MaxMarks+1];
	int mubv[MaxMarks+1];
	int id[MaxLength+1];
	int l[MaxMarks*MaxLength+1];
	// other identifiers
	int i, jb, jc;
	int ns;
	int iflag;
	int nc;

	// initialize differences used and mark positions available
	for (int j=1; j<=Length; j++) {
		id[j] = 1;
		l[j]=j;
	}
	// initialize mark position bound arrays
	for(int j=1; j<=Marks; j++) {
		mub[j]=Length;
		mubv[j]=Length;
	}
	// use mub to force middle mark to left of middle position
	for(int j=1; j<=(Marks+1)/2; j++) {
		jb = (Marks+1)/2 - j;
		mub[j] = (Length-1)/2 + (Marks % 2) - 1 - jb*(jb+1)/2;
	}
	// flag used for even number of marks
	iflag = ((Marks+1) % 2) * Marks/2;
	// initialize backtrack search
	ns = 0;
	m[1] = 0;
	ip[2] = 0;
	ie[2] = Length;
	nc = 2;
	// start backtrack search
	goto L40;

L20:
	// backtrack portion of search loop  
	// update search depth
	nc--;

L25:
	// release differences
	for (int j=1; j <= nc-1; j++)
		id[m[nc]-m[j]] = 1;
	// check for end of search
	if(nc == 1){ // search complete
		//cout <<"answers "<<Marks<<", "<<Length<<", "<< ns<<"\n\n";
		cout << "\n\n";
		return 0;
	}
L40:
	// find next node in search tree, increment node count
	ip[nc]++;// = ip[nc]+1;
	// tests to prune search
	// are there enough valid mark positions remaining to complete ruler?
	if(ie[nc]-ip[nc] < Marks-nc) goto L20;
	// add mark
	m[nc] = l[ip[nc]];
	// check mark position against fixed and variable bounds
	if(m[nc] > mub[nc]) goto L20;
	if(m[nc] > mubv[nc]) goto L20;
	// do we have a complete ruler
	if(Marks == nc) goto L70;
	// mark new differences used
	for (int j=1; j <= nc-1; j++) {
		id[m[nc]-m[j]] = 0;
	}
	// check if enough small differences remain to complete the ruler
	int j55 = 0;
	jc = nc;
	i = m[nc];

L55:
	// find next unused difference
	j55++;
	if(id[j55] == 0) goto L55;
	// add differences to partial ruler length
	i = i+j55;
	jc++;
	// n marks yet?
	if(jc < Marks) goto L55;
	// can ruler be completed?
	// note failure goes to L25 not L20 as we have not bumped nc yet
	// we have to undo L50 loop and we may not be done at this level
	if(i > Length) goto L25;
	// set mubv bound assuming first difference is largest remaining
	mubv[nc+1] = m[nc]+Length-i+j55;
	// sum of middle marks must be less than length
	if(iflag == nc)
		mub[nc+1] = Length-m[nc]-1;
	// update list of eligible mark positions
	ip[nc+1] = ie[nc];
	i = ie[nc];
	for(int j=ip[nc]+1; j<=ie[nc]; j++)
	{
		int lt=l[j];
		l[i+1]=lt;
		i=i+id[lt-m[nc]];
	}
	ie[nc+1] = i;
	// increase depth and continue search
	nc++;
	goto L40;

L70:
	// solution found, check if it satisfies middle mark property
	if(m[(Marks+1)/2]+m[(Marks+2)/2] > m[Marks]) goto L40;
	// increment number of solutions found
	ns++;
	cout << "List = " << m[1];
	for(int j=2; j<=Marks; j++)
		cout << ",  " << m[j];
	cout << "\n";
	//return when the max lines are printed
	if (ns>=nprint){
		cout << "Printing stopped, " << nprint << " line limit reached\n\n";
		return 0;
	}
	goto L40;

}
