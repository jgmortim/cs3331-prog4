/* ----------------------------------------------------------- */
/* NAME : John Mortimore                     User ID: jgmortim */
/* DUE DATE : 04/05/2019                                       */
/* PROGRAM ASSIGNMENT #4                                       */
/* FILE NAME : thread.cpp                                      */
/* PROGRAM PURPOSE :                                           */
/*    Contains all class implementations for my threads.       */
/* ----------------------------------------------------------- */

#include <iostream>
#include <string.h>
#include <stdio.h>
#include "thread.h"

using namespace std;
int ready_to_eat(int id);
void finish_eating(int id);
void goto_sleep();
void food_ready(int m, int n, int t, int* count);

/* ----------------------------------------------------------- */
/* FUNCTION : constructor                                      */
/*    Creates a new BabyThread.                                */
/* PARAMETER USAGE :                                           */
/*    id {int} - the id of the thread.                         */
/* FUNCTIONS CALLED :                                          */
/*    sprintf(), strlen(), ThreadName.seekp(), write()         */
/* ----------------------------------------------------------- */
BabyThread::BabyThread(int id) {
	char output[1024];
	sprintf(output, "%*cBaby eagle %d started.\n", id, ' ', id);
	write(1, output, strlen(output));
	ID=id;
	ThreadName.seekp(0, ios::beg);
	ThreadName << "Baby_" << ID << '\0';
}

/* ----------------------------------------------------------- */
/* FUNCTION : ThreadFunc                                       */
/*    Process for the baby threads to perform when begin() is  */
/*    called.                                                  */
/* PARAMETER USAGE :                                           */
/*    N/A                                                      */
/* FUNCTIONS CALLED :                                          */
/*    Delay(), Exit(), finish_eating(), ready_to_eat(),        */
/*    Thread::ThreadFunc()                                     */
/* ----------------------------------------------------------- */
void BabyThread::ThreadFunc() {
	Thread::ThreadFunc();
	while (1) {
		Delay();           			/* play for a while */
		if (ready_to_eat(ID) == 1) Exit(); 	/* get hungry */
		Delay();				/* eat for a while */
		finish_eating(ID); 			/* finish eating */
	}
}

/* ----------------------------------------------------------- */
/* FUNCTION : constructor                                      */
/*    Creates a new MotherThread.                              */
/* PARAMETER USAGE :                                           */
/*    m {int} - number of feeding pots.                        */
/*    n {int} - number of baby eagles.                         */
/*    t {int} - number of feedings.                            */
/* FUNCTIONS CALLED :                                          */
/*    strlen(), ThreadName.seekp(), write()                    */
/* ----------------------------------------------------------- */
MotherThread::MotherThread(int m, int n, int t) {
	feedingCount=0;
	maxFeed = t;
	numPots = m;
	numBabies = n;
	char* output = "Mother eagle started.\n";
	write(1, output, strlen(output));
	ThreadName.seekp(0, ios::beg);
	ThreadName << "Mother" << '\0';
}

/* ----------------------------------------------------------- */
/* FUNCTION : ThreadFunc                                       */
/*    Process for the mother thread to perform when begin() is */
/*    called.                                                  */
/* PARAMETER USAGE :                                           */
/*    N/A                                                      */
/* FUNCTIONS CALLED :                                          */
/*    Delay(), Exit(), food_ready(), goto_sleep(),             */
/*    Thread::ThreadFunc()                                     */
/* ----------------------------------------------------------- */
void MotherThread::ThreadFunc() {
	Thread::ThreadFunc();
	while (feedingCount < maxFeed) {            
		goto_sleep(); /* take a nap */
		Delay();
		food_ready(numPots, numBabies, maxFeed, &feedingCount);	/* make food available */
		Delay();
	}
	Exit();
}
