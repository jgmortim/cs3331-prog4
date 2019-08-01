/* ----------------------------------------------------------- */
/* NAME : John Mortimore                     User ID: jgmortim */
/* DUE DATE : 04/05/2019                                       */
/* PROGRAM ASSIGNMENT #4                                       */
/* FILE NAME : thread.h                                        */
/* PROGRAM PURPOSE :                                           */
/*    Contains all class definitions for my threads.           */
/* ----------------------------------------------------------- */

#include "ThreadClass.h"

class BabyThread : public Thread {
	public:
		BabyThread(int id);
	private:
		int ID;
		void ThreadFunc();
};

class MotherThread : public Thread {
	public:
		MotherThread(int m, int n, int t);
	private:
		int numPots;
		int feedingCount;
		int maxFeed;
		int numBabies;
		void ThreadFunc();
};
