/* ----------------------------------------------------------- */
/* NAME : John Mortimore                     User ID: jgmortim */
/* DUE DATE : 04/05/2019                                       */
/* PROGRAM ASSIGNMENT #4                                       */
/* FILE NAME : thread-main.cpp                                 */
/* PROGRAM PURPOSE :                                           */
/*    Multi-theaded hungry eagles problem. Automated testing   */
/*    performed with over 40,000 runs with no deadlock.        */
/* ----------------------------------------------------------- */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "thread.h"

using namespace std;

/* ----------------------------------------------------------- */
/* FUNCTION : main                                             */
/*    The main method for the program.                         */
/* PARAMETER USAGE :                                           */
/*    argc {int} - number of argument.                         */
/*    argv {char**} - list of arguments.                       */
/*       argv[1] - number of feeding pots.                     */
/*       argv[2] - number of baby eagles.                      */
/*       argv[3] - number of feedings.                         */
/* FUNCTIONS CALLED :                                          */
/*    atoi(), BabyThread::Begin(), BabyThread::Join(),         */
/*    MotherThread::Begin(), MotherThread::Join(), sprintf(),  */
/*    strlen(), write()                                        */
/* ----------------------------------------------------------- */
int main(int argc, char *argv[]) {
	int m, n, t;
	int i;
	char output[1024];
	
	if (argc != 4) return 1;
	if ((m = atoi(argv[1])) == 0) m=10;
	if ((n = atoi(argv[2])) == 0) n=10;
	if ((t = atoi(argv[3])) == 0) t=10;
	sprintf(output, "MAIN: There are %d baby eagles, %d feeding pots, and %d feedings.\n", n, m, t);
	write(1, output, strlen(output));
	sprintf(output, "MAIN: Game starts!!!!!\n          ......\n");
	write(1, output, strlen(output));

	/* Create threads. */
	MotherThread* mother = new MotherThread(m, n, t);
	BabyThread* babies[n];
	for (i=0; i < n; i++) babies[i] = new BabyThread(i+1);
	/* Start threads. */
	mother->Begin();
	for (i=0; i < n; i++) babies[i]->Begin();
	/* Wait for threads to finish. */
	mother->Join();
	for (i=0; i < n; i++) babies[i]->Join();
	
	sprintf(output, "Mother eagle retires after serving %d feedings. Game ends!!!\n", t);
	write(1, output, strlen(output));
	return 0;
}

