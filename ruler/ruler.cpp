// ruler.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cstdlib>

const int Max_N = 26;
const int Max_L = 800;

int M_tbl[Max_N];//an array to hold ruler's marks
int L;//length of ruler
int N;//number of marks
int MM;//middle mark
int R[Max_N];

//function prototypes
int ruler(bool *p, bool *d, int s);

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	bool Prohibited[Max_L];//prohibits for rulers of length to Max_L-1
	bool Diff_tbl[Max_L];//store differences
	int i;//loop index

	for (;;) {
		cout << "Enter Number of marks, 0 halts: ";
		cin >> N;
		if(N == 0) return 0;// n=0 ends program

		cout << "Enter length: ";
		cin >> L;
		cout << "\n\n";

		for(;;)
		{
			//
			//initialize the table of marks, M_tbl[]
			//initialize the recursions table, R[]
			//
			for(i=0; i<Max_N; i++) {
				R[i] = 0;
				M_tbl[i] = 0;
			}
			M_tbl[N-1] = L;
			MM = (L-1)/2;//mark spot at or below where half the marks should be.
			//
			//initialize the table of prohibited ruler locations
			//
			for (i=1; i < Max_L; i++)
				Prohibited[i] = 0;//clear all prohibited locations
			Prohibited[0] = 1;//mark start of ruler
			Prohibited[L] = 1;//mark the end of the ruler
			//
			//initialize the difference table
			//
			for (i=0; i < Max_L; i++)
				Diff_tbl[i] = 0;
			if (!(L % 2)) Prohibited[L/2] = 1;//flag the split if even L

			//call ruler with the 2 marks already made i.e. 0 and N.
			//if (ruler(Prohibited, Diff_tbl, 1)) {
			//no ruler with length L found.
			//lengthen the ruler and try again
			//	L++;
			//	cout << "Trying longer ruler, L = " << L << "\n";
			//}
			//else {
			ruler(Prohibited, Diff_tbl, 1);
			cout << "\n";
			for (i=0; i < N; i++)
				cout << R[i] << "\n";
			cout << "\n";
			break;//leave for another ruler problem
			//}
		}
	}
}

int ruler(bool *p, bool *d, int s)
{
	bool Prohibited[Max_L];
	bool Diff_tbl[Max_L];//store differences
	int i, k;
	int nm;//the next mark
	int lm;//marks the last mark at this recursion

	lm = M_tbl[s-1];//save the last good mark
	do
	{
L4:
		k = 0;
		for (i=L-1; i>lm; i--) {
			Prohibited[i] = p[i];//copy previous Prohibited array
			//scan for next available mark
			if (!Prohibited[i]) {
				nm = i;//save lowest free slot
				k++;//accumulate number of open slots
			}
		}
		//only find rulers with midpoints to
		//the left of the ruler's center.
		if (2*s <= N) {//check for midpoint
			if (N % 2) {
				//odd number of marks
				if (2*nm > L) {
					R[s]++;
					return 1; 
				}
			}
			else {
				//even number of marks
				if (nm + M_tbl[s-1] > L) {
					R[s]++;
					return 1;
				}
			}
		}
		//are there enough empty slots to finish the ruler?
		if (k+s+1 < N) {
			R[s]++;
			//not enough spots to finish ruler
			return 1;
		}
		M_tbl[s] = nm;//keep this mark's value
		lm=nm;//point to next possible slot if needed
		//
		if (s == N-2) {
			//the last value has been found.
			for (i=0; i < N-1; i++) cout << M_tbl[i] << ", ";
			cout << M_tbl[N-1] << "\n";
			R[s]++;
			goto L4;
			//return 1;
		}
		// *******************************
		// update the difference table
		// *******************************
		//copy difference table
		for (i=1; i<L; i++)
			Diff_tbl[i] = d[i];
		//add new differences
		for (i=0; i<s; i++) {
			k = nm - M_tbl[i];//subtract old marks from the new mark
			Diff_tbl[k] = 1;
		}
		// *******************************
		// mark all new Prohibits
		// *******************************
		// mark the midpoint, if integal, between the new mark and the
		// end of the ruler.
		if (((L - M_tbl[s]) % 2) == 0) {
			//if even difference, mark the split
			Prohibited[ (L + M_tbl[s])/2 ] = 1;
		}
		// Prohibit duplicate differences
		Prohibited[nm] = 1;//x the new mark
		for (i=1; i<=nm; i++)
			if (Diff_tbl[i])//if a new difference
			{
				if (i + nm >= L) break;// x will be out of bounds
				// x all slots where the difference between this mark and
				// further additional marks would duplicate existing differences.
				Prohibited[nm + i] = 1;
				// x all slots where the difference between the ruler end and
				// further additional marks would duplicate existing differences.
				Prohibited[L - i] = 1;
			}
	}
	while (ruler(Prohibited, Diff_tbl, s+1) == 1);

	return 0;//answer found, we are done!

}
