/*
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c        Program to find difference triangle sets by exhaustive search
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c                  IBM SOFTWARE DISCLAIMER
c
c   dts2.f (version 1.0)
c   Copyright (2000,1986)
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
c        date: 2000 (partially derived from code written in 1986)
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c        This program constructs all difference triangle sets with nt
c   triangles, n marks per triangle and length k (or less) by
c   exhaustive backtrack search.
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c        This program is based on the Golomb ruler finding programs.  It
c   is most similar to the lightly tuned grs2.f
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
*/
// DiffSet.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;

// MaxMarks is maximun allowed number of marks
#define MaxMarks 64
// maxk is maximum allowed length
#define MaxLength 1000
#define MaxRulers 10
// nprint is maximum number of solutions to output
#define nprint 10
// nout output progress reports when at level nout, =1 for final only
#define nout 1

//get number of marks per ruler, length and number of rulers
int diffRuler(int Marks, int Length, int nt); //function prototype, was (n, k, nt)

int _tmain(int argc, _TCHAR* argv[])
{
	int marks;
	int length;
	int nrulers;

	for (;;) {
		for (;;) {
			cout << "Enter number of rulers <zero to quit> ";
			cin >> nrulers;
			if (nrulers <= 0)
				return 0;// = 0 to end program
			else if (nrulers>MaxRulers)
				cout<<"Too many rulers, must be less than "<<MaxRulers<<endl;
			else
				break;
		}

		for (;;) {
			cout << "Enter number of marks/ruler <zero to quit> ";
			cin >> marks;
			if(marks <= 0) return 0;// = 0 to end program
			if (marks*nrulers > MaxMarks)
				cout << "Too many marks, the max is " << MaxMarks << endl;
			else if (marks<=2)
				cout << "Too few marks, the min is 3\n" ;
			else
				break;
		}

		for (;;) {
			cout << "Enter length <zero to quit> ";
			cin >> length;
			if (length <= 0)
				return 0;// = 0 to end program
			else if (length>MaxLength)
				cout<<"Length too long, must be less than "<<MaxLength<<endl;
			else
				break;
		}

		cout << "\n\n";
		//call diffRuler
		diffRuler(marks, length, nrulers);
	}
}
//
//get number of marks per ruler, length and number of rulers
//input not checked for validity
int diffRuler(int Marks, int Length, int nt)// (n, k, nt)
{
	int ip[MaxMarks+1],ie[MaxMarks+1],m[MaxMarks+1],mub[MaxMarks+1],mubv[MaxMarks+1];
	int iflag[MaxMarks+1],ngt[MaxMarks+1],kgt[MaxMarks+1];
	int id[MaxLength+1],l[MaxMarks*MaxLength+1];
	int nm = Marks*Length;
	double tit;
	time_t t1, t2;
	//various indexes
	int i, j, ja, jb, jc;
	int ntl;
	int na, nb, nc;
	int ng, nh, nl;
	int nr, ns;
	int jsum;
	int lt;

L100:
	//61 start timing
	time( &t1 );//   call cputime(t1,iret)
	//63 compute total number of marks, must not exceed maxn
	nm=Marks*nt;
	//65 initialize differences used and mark positions available
	for (int j=1; j<=Length; j++) {
		id[j]=1;
		l[j]=j;
	}
	//70 initialize mark position bound arrays, iflag and stuff for loop 55
	for (int j=1; j<=nm; j++) {
		mub[j]=Length;
		mubv[j]=Length;
		iflag[j]=0;
		//75 number of rulers not yet started at depth j
		ntl = nt-1-(j-1)/Marks;
		//77 number of marks placed in current ruler at depth j
		ja = ((j-1) % Marks) + 1;
		//79 number of first differences to come at depth j
		ngt[j]=Marks-ja+ntl*(Marks-1);
		//81 maximum possible sum of first differences in this and remaining rulers
		//82 at depth j
		kgt[j]=Length+ntl*Length-(ntl*(ntl+1))/2;
	}
	//85 use mub to force middle marks to left of middle position
	for (int ja=1; ja<=nt; ja++) {
		//87 flag used for even number of marks
		if((Marks%2)==0) iflag[(ja-1)*Marks+Marks/2]=1;
		for (int j=1; j<=(Marks+1)/2; j++) {
			jb=(Marks+1)/2 - j;
			mub[(ja-1)*Marks+j]=(Length-1)/2 + (Marks%2) - 1 - (jb*(jb+1))/2;
		}
	}
	//93 middle positions
	na=Marks/2;
	nb=(Marks-1)/2;
	//96 initialize backtrack search
	ns=0;
	tit=0;
	m[1]=0;
	//100 first differences decrease so must be at least nt in first ruler
	ip[2]=nt-1;
	ie[2]=Length;
	nr=1;
	nc=2;
	nl=1;
	nh=Marks;
	//107 start backtrack search
	goto L40;
L20:
	//109 backtrack portion of search loop  
	//110 update search depth
	nc--;//=nc-1;
L25:
	//112 release differences, start loop at nl for current ruler only
	for (int j=nl; j<=nc-1; j++)
		id[m[nc]-m[j]]=1;
	//116 check for end of search for current ruler
	if(nc==nl) goto L80;// search complete at current ruler level
	//118 if nc = nout go output progress report
	if(nc==nout) goto L185;// progress report wanted
L40:
	//120 find next node in search tree, increment node count
	ip[nc]++;//=ip(nc)+1
	tit++;//=tit+1.d0
	//123 tests to prune search
	//124 are there enough valid mark positions remaining to complete ruler?
	if(ie[nc]-ip[nc] < nh-nc) goto L20;
	//126 add mark
	m[nc]=l[ip[nc]];
	//128 check mark position against fixed and variable bounds
	if(m[nc] > mub[nc]) goto L20;
	if(m[nc] > mubv[nc]) goto L20;
	//131 mark new differences used, start loop at nl for current ruler only
	for (int j=nl; j<=nc-1; j++)
		id[m[nc]-m[j]]=0;
	//135 do we have a complete ruler?  note this check now after loop 50
	//136 since we may add more rulers.  this means we goto 25 instead of
	//137 40 when we back up since we have to undo loop 50
	if(nc == nh) goto L70;
	//139 check if enough small differences remain to complete the ruler
	j=0;
	jc=0;
	i=m[nc];
	ng=ngt[nc];
L55:
	//144 find next unused difference
	j++;//=j+1
	if (id[j] == 0) goto L55;
	//147 add differences to partial ruler length
	i=i+j;
	jc++;//=jc+1
	//150 ng differences yet?
	if(jc < ng) goto L55;
	//152 can all remaining rulers be completed?
	//153 note failure goes to 25 not 20 as we have not bumped nc yet
	//154 we have to undo 50 loop and we may not be done at this level
	if(i>kgt[nc]) goto L25;
	//156 set mubv bound assuming first difference is largest remaining
	mubv[nc+1]=m[nc]+kgt[nc]-i+j;
	//158 sum of middle marks must be less than length
	//159 note use l(ie(nc)) which may be less than Length
	if (iflag[nc] == 1)
		mub[nc+1] = l[ie[nc]]-m[nc]-1;
	//161 update list of eligible mark positions
	ip[nc+1]=ie[nc];
	i=ie[nc];
	for (int j=ip[nc]+1; j<=ie[nc]; j++)  {
		lt=l[j];
		l[i+1]=lt;
		i=i+id[lt-m[nc]];
	}
	ie[nc+1]=i;
	//175 increase depth and continue search
	nc++;//=nc+1
	goto L40;
L70:
	//178 current ruler complete, check if it satisfies middle mark property
	if (m[nc-na]+m[nc-nb] > m[nc]) goto L25;
	//180 have all rulers been completed?
	if(nc==nm) goto L170;
	//182 start next ruler
	//183 ntl rulers remain
	ntl=nt-nr;
	//185 find ntl smallest unused differences
	j=0;
	jc=0;
L72:
	j++;//=j+1
	if(id[j]==0) goto L72;
	jc++;//=jc+1
	if(jc<ntl) goto L72;
	//192 since we are assuming first differences decrease
	//193 j is the mimimum possible for first difference in current ruler
	//194 this must be less than first difference in previous ruler
	if (j > m[nc-Marks+2]) goto L25;
	//196 initialize eligible mark position list for current ruler
	ja=j;
	l[ie[nc]+1]=0;
	for (int j=ja; j<=Length; j++)
		l[ie[nc]+2+j-ja]=j;
	ip[nc+1]=ie[nc];
	ie[nc+1]=ie[nc]+2+Length-ja;
	//203 upper bound for first difference in current ruler
	mub[nc+2]=m[nc-Marks+2];
	//205 find sum ntl largest unused differences
	j=Length+1;
	jc=0;
	jsum=0;
L76:
	j--;//=j-1
	if (id[j]==0) goto L76;
	jsum += j;
	jc++;//=jc+1
	if(jc < ntl) goto L76;
	//214 update kgt array
	for (int j=1; j<=Marks; j++) {
		kgt[nr*Marks+j]=jsum;
	}
	//218 find largest unused differences
	j=Length+1;
L78:
	j--;//=j-1
	if(id[j]==0) goto L78;
	//222 update mub array
	//223ja=(j-1)/2+iand[Marks,1]-1;
	ja=(j-1)/2+( Marks & 1)-1;
	for (int j=3; j<=(Marks+1)/2; j++) {
		jb=(Marks+1)/2 - j;
		mub[nr*Marks+j]=ja - (jb*(jb+1))/2;
	}
	//228 update search status information
	nr++;//=nr+1
	nc++;//=nc+1
	nl+=Marks;//nl+Marks
	nh+=Marks;//nh+Marks
	//233 go resume search
	goto L40;
L170:
	//235 increment number of solutions found
	ns++;//=ns+1
	//237 check if we should output solution
	if(ns>nprint) goto L25;
	//unit 1 is a terminal, unit 6 is a disk
	//239    write(1,200)Marks,m[Marks],nt
	//242  200 format(3i10)
	time( &t2 );//log current time
	for (int j=Marks, MaxMark=0; j<=nm; j+=Marks) {
		if (MaxMark < m[j]) MaxMark=m[j];
		if (j==nm)
			cout<<nt<<" "<<Marks<<" "<<MaxMark<<" time="<<t2-t1<<"sec"<<endl;
	}
	//240    write(1,210)(m[j],j=1,nm)
	//243  210 format(10i6)
	for (int j=1; j<=nm; j+=Marks) {//for each ruler
		for (int k=1; k<Marks; k++) {//for each mark
			cout << m[j+k-1] << ", ";
		}
		cout << m[j+Marks-1] << endl;
	}
	cout << endl;
	//241      write(6,220)(m[j],j=1,nm)
	//244  220 format(1x,10i6)
	goto L25;//245

L80:
	//246 search complete at current ruler level
	//248 is this first ruler?  if so we are done
	if(nl==1) goto L180;
	//250 else backup to previous ruler and resume search
	nr--;//=nr-1
	nc--;//=nc-1
	nl-=Marks;//nl-Marks
	nh-=Marks;//nh-Marks
	goto L25;
L180:
	//256 stop timing
	//257 call cputime(t2,irc)
	time( &t2 );//log current time
	//258 output search statistics to disk
	//259      write(6,230)Marks,Length,ns,t2-t1,tit
	//260  230 format(1x,'Marks=',i3,' Length=',i4,' solutions=',i8,' time=',-6pf16.2,
	//261     x ' nodes=',0pf15.0)
	cout<<"Marks="<<Marks<<" Length="<<Length<<" solutions="<<ns<<" time="<<t2-t1<<"sec nodes="<<tit<<"\n\n";
	//262 go get next input
	goto L90;
L185:
	//264 progress report wanted
	//265  call cputime(t2,irc)
	time( &t2 );
	//266 output current search statistics
	//267      write(6,235)Marks,Length,ns,t2-t1,tit,m[nc]
	//268  235 format(1x,'Marks=',i3,' Length=',i4,' solutions=',i4,' time= ',-6pf16.2,
	//269     x ' nodes=',0pf15.0,i5)
	//270 go resume search
	goto L40;
	//272 program done, mark end of output file and stop
	//273  300 continue
	//274      write(1,200)0,0
	//275      stop
	//276      end
	// timing subroutine for aix
	// cputime is vs fortran timing routine
	// t is microseconds
	//      subroutine cputime(t,irc)
	//      real*8 t
	//      t=mclock()*10000.d0
	// for g77 on pcs replace above statement with
	//     t=second()*1000000.d0
	//      return
L90:
	return 0;
}
