      parameter(maxn=50,maxk=1000,nprint=10,nout=1)
//   maxn is maximun allowed number of marks
//   maxk is maximum allowed length
//   nprint is maximum number of solutions to output
//   nout output progress reports when at level nout, =1 for final only
      integer*4 ip[maxn],ie[maxn],m[maxn],mub[maxn],mubv[maxn]
      integer*4 iflag[maxn],ngt[maxn],kgt[maxn]
      integer*4 id[maxk],l[maxn*maxk]
      real*8 t1,t2,time,tit
L90:
// get number of marks per ruler, length and number of rulers
// input not checked for validity
	  read(5,100)Marks,Length,nt
  100 format(3i5)
// Marks=0 to end program
      if(Marks == 0) goto L300;
// start timing
      call cputime(t1,iret)
// compute total number of marks, must not exceed maxn
      nm=Marks*nt;
// initialize differences used and mark positions available
      do 10 j=1,Length
      id[j]=1;
      l[j]=j;
   10 continue
// initialize mark position bound arrays, iflag and stuff for loop 55
      do 15 j=1,nm
      mub[j]=Length;
      mubv[j]=Length;
      iflag[j]=0;
// number of rulers not yet started at depth j
      ntl=nt-1-(j-1)/Marks;
// number of marks placed in current ruler at depth j
      ja=mod(j-1,Marks)+1;
// number of first differences to come at depth j
      ngt[j]=Marks-ja+ntl*(Marks-1);
// maximum possible sum of first differences in this and remaining rulers
// at depth j
      kgt[j]=Length+ntl*Length-(ntl*(ntl+1))/2;
   15 continue
// use mub to force middle marks to left of middle position
      do 16 ja=1,nt
// flag used for even number of marks
      if(mod(Marks,2) == 0)iflag[(ja-1)*Marks+Marks/2]=1;
      do 16 j=1,(Marks+1)/2;
      jb=(Marks+1)/2 - j;
      mub[(ja-1)*Marks+j]=(Length-1)/2 + mod(Marks,2) - 1 - (jb*(jb+1))/2;
   16 continue
// middle positions
      na=Marks/2;
      nb=(Marks-1)/2;
// initialize backtrack search
      ns=0;
      tit=0.d0;
      m[1]=0;
// first differences decrease so must be at least nt in first ruler
      ip[2]=nt-1;
      ie[2]=Length;
      nr=1;
      nc=2;
      nl=1;
      nh=Marks;
// start backtrack search
       goto L40;
L20:
// backtrack portion of search loop  
// update search depth
	   nc=nc-1;
L25:
// release differences, start loop at nl for current ruler only
	   do 30 j=nl,nc-1;
      id[m[nc]-m[j]]=1;
   30 continue
// check for end of search for current ruler
      if(nc == nl) goto L80;
// if nc = nout go output progress report
      if(nc == nout) goto L185;
L40:
// find next node in search tree, increment node count
	  ip[nc]=ip[nc]+1;
      tit=tit+1.d0;
// tests to prune search
// are there enough valid mark positions remaining to complete ruler?
      if(ie[nc]-ip[nc] < nh-nc) goto L20;
// add mark
      m[nc]=l[ip[nc]];
// check mark position against fixed and variable bounds
      if(m[nc] > mub[nc]) goto L20;
      if(m[nc] > mubv[nc]) goto L20;
// mark new differences used, start loop at nl for current ruler only
      do 50 j=nl,nc-1
      id[m[nc]-m[j]]=0;
   50 continue
// do we have a complete ruler?  note this check now after loop 50
// since we may add more rulers.  this means we  goto 25 instead of
// 40 when we back up since we have to undo loop 50
      if(nc == nh) goto L70;
// check if enough small differences remain to complete the ruler
      j=0;
      jc=0;
      i=m[nc];
      ng=ngt[nc];
L55:
// find next unused difference
	  j=j+1;
      if(id[j] == 0) goto L55;
// add differences to partial ruler length
      i=i+j;
      jc=jc+1;
// ng differences yet?
      if(jc < ng) goto L55;
// can all remaining rulers be completed?
// note failure goes to 25 not 20 as we have not bumped nc yet
// we have to undo 50 loop and we may not be done at this level
      if(i > kgt[nc]) goto L25;
// set mubv bound assuming first difference is largest remaining
      mubv[nc+1]=m[nc]+kgt[nc]-i+j;
// sum of middle marks must be less than length
// note use l(ie(nc)) which may be less than Length
      if(iflag[nc] == 1)
		mub[nc+1]=l[ie[nc]]-m[nc]-1;
// update list of eligible mark positions
      ip[nc+1]=ie[nc];
      i=ie[nc];
c1    do 60 j=ip[nc]+1,ie[nc]
c1    if(id[l[j]-m[nc]]) == 0) goto L60;
c1    i=i+1;
c1    l[i]=l[j];
c1 60 continue
      do 60 j=ip[nc]+1,ie[nc]
      lt=l[j];
      l[i+1]=lt;
      i=i+id[lt-m[nc]];
	60 continue
      ie[nc+1]=i;
// increase depth and continue search
      nc=nc+1;
       goto L40;
L70:
// current ruler complete, check if it satisfies middle mark property
	   if(m[nc-na]+m[nc-nb] > m[nc]) goto L25;
// have all rulers been completed
      if(nc == nm) goto L170;
// start next ruler
// ntl rulers remain
      ntl=nt-nr;
// find ntl smallest unused differences
      j=0;
      jc=0;
L72:
	  j=j+1;
      if(id[j] == 0) goto L72;
      jc=jc+1;
      if(jc < ntl) goto L72;
// since we are assuming first differences decrease
// j is the mimimum possible for first difference in current ruler
// this must be less than first difference in previous ruler
      if(j > m[nc-Marks+2]) goto L25;
// initialize eligible mark position list for current ruler
      ja=j;
      l[ie[nc]+1]=0;
      do 75 j=ja,Length;
   75 l[ie[nc]+2+j-ja]=j;
      ip[nc+1]=ie[nc];
      ie[nc+1]=ie[nc]+2+Length-ja;
// upper bound for first difference in current ruler
      mub[nc+2]=m[nc-Marks+2];
// find sum ntl largest unused differences
      j=Length+1;
      jc=0;
      jsum=0;
L76:
	  j=j-1;
      if(id[j] == 0) goto L76;
      jsum=jsum+j;
      jc=jc+1;
      if(jc < ntl) goto L76;
// update kgt array
      do 77 j=1,Marks
      kgt[nr*Marks+j]=jsum;
   77 continue
// find largest unused differences
      j=Length+1;
L78:
   j=j-1;
      if(id[j] == 0) goto L78;
// update mub array
      ja=(j-1)/2+iand(Marks,1)-1;
      do 79 j=3,(Marks+1)/2
      jb=(Marks+1)/2 - j;
      mub[nr*Marks+j]=ja - (jb*(jb+1))/2;
   79 continue
// update search status information
      nr=nr+1;
      nc=nc+1;
      nl=nl+Marks;
      nh=nh+Marks;
// go resume search
       goto L40;
L170:
// increment number of solutions found
	   ns=ns+1;
// check if we should output solution
//unit 1 is a terminal, unit 6 is a disk
      if(ns > nprint) goto L25;
      write(1,200)Marks,m[Marks),nt
      write(1,210)(m[j),j=1,nm)
      write(6,220)(m[j),j=1,nm)
  200 format(3i10)
  210 format(10i6)
  220 format(1x,10i6)
       goto L25;
L80:
// search complete at current ruler level
	  continue
// is this first ruler?  if so we are done
      if(nl == 1) goto L180;
// else backup to previous ruler and resume search
      nr=nr-1;
      nc=nc-1;
      nl=nl-Marks;
      nh=nh-Marks;
       goto L25;
L180:
// stop timing
	   call cputime(t2,irc)
// output search statistics
      write(6,230)Marks,Length,ns,t2-t1,tit
  230 format(1x,'Marks=',i3,' Length=',i4,' solutions=',i8,' time=',-6pf16.2,
     x ' nodes=',0pf15.0)
// go get next input
       goto L90;
L185:
// progress report wanted
	   call cputime(t2,irc)
// output current search statistics
      write(6,235)Marks,Length,ns,t2-t1,tit,m[nc)
  235 format(1x,'Marks=',i3,' Length=',i4,' solutions=',i4,' time= ',-6pf16.2,
     x ' nodes=',0pf15.0,i5)
// go resume search
       goto L40;
L300:
// program done, mark end of output file and stop
	   continue
      write(1,200)0,0
      stop
      end
// timing subroutine for aix
// cputime is vs fortran timing routine
// t is microseconds
      subroutine cputime(t,irc)
      real*8 t
      t=mclock()*10000.d0
// for g77 on pcs replace above statement with
//     t=second()*1000000.d0
      return
      end
