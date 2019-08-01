/* ----------------------------------------------------------- */
/* NAME : John Mortimore                     User ID: jgmortim */
/* DUE DATE : 04/05/2019                                       */
/* PROGRAM ASSIGNMENT #4                                       */
/* FILE NAME : thread-support.cpp                              */
/* PROGRAM PURPOSE :                                           */
/*    Contains all of the supporting functions for my threads. */
/* ----------------------------------------------------------- */

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "thread.h"

using namespace std;
void retire(int numBabies, int fcount);

/* avaiPots, eating, reID, and retired are all protected by the mutex Counter. */
int avaiPots=0;
int eating=0;
int repID=0;
bool retired = false;

int maxPots=0;

Semaphore Filled("filled", 0);
Semaphore Sleep("sleep", 0);
Semaphore AllEmpty("allEmpty", 1);
Mutex Counter("counters");

/* ----------------------------------------------------------- */
/* FUNCTION : ready_to_eat                                     */
/*    Blocks the caller until a pot is available to eat from.  */
/* PARAMETER USAGE :                                           */
/*    id {int} - the id of the calling baby eagle.             */
/* FUNCTIONS CALLED :                                          */
/*    Mutex.Lock(), Mutex.Unlock(), Semaphore.Signal(),        */
/*    Semaphore.Wait(), sprintf(), strlen(), write()           */
/* ----------------------------------------------------------- */
int ready_to_eat(int id) {
	char output[1024];
	sprintf(output, "%*cBaby eagle %d is ready to eat.\n", id, ' ', id);
	write(1, output, strlen(output));
	while(1) {
		/* Lock the counters avaiPots and eating; and control variables repID and retired. */
		Counter.Lock();
		if (avaiPots > 0) { /* If a pot is available, */
			avaiPots--;     /* decrease the number of available pots */
			eating++;       /* and increase the number of eating. */
			sprintf(output, "%*cBaby eagle %d is eating using feeding pot %d.\n", id, ' ', id, maxPots-avaiPots);
			write(1, output, strlen(output));
			/* Unlock the counters and return. */
			Counter.Unlock();
			return 0;
		} else {
			/* If there are no available pots, assign one (and only one) baby eagle as  */
			/* the representative for all the waiting babies. The rep will wake up the  */
			/* mother eagle when all pots are empty. The other waiting babies just wait */
			/* until the pots are filled. After the pots are filled, all the waiting    */
			/* babies can compete for the pots.                                         */
			/* After the mother retires, no babies are allowed to become the rep.       */
			if (repID == 0 && !retired) {    /* 0 indicates no rep. */
				repID = id;      /* set the baby as the rep */
				Counter.Unlock();
				AllEmpty.Wait(); /* Wait till all pots are empty. */
				sprintf(output, "%*cBaby eagle %d sees all feeding pots are empty and wakes up the mother.\n", id, ' ', id);
				write(1, output, strlen(output));
				Sleep.Signal(); /* wake up mom */
			} else if (retired)  {  /* If the mother is retired, there will be no more food, so exit. */
				Counter.Unlock();
				return 1;
			} else {
				Counter.Unlock();	
			}
			Filled.Wait();      /* When the pots have been filled then try to eat again. */
		}
	}
}

/* ----------------------------------------------------------- */
/* FUNCTION : finish_eating                                    */
/*    Called by a baby when it finishes eating.                */
/* PARAMETER USAGE :                                           */
/*    id {int} - the id of the calling baby eagle.             */
/* FUNCTIONS CALLED :                                          */
/*    Mutex.Lock(), Mutex.Unlock(), Semaphore.Signal(),        */
/*    sprintf(), strlen(), write()                             */
/* ----------------------------------------------------------- */
void finish_eating(int id) {
	char output[1024];
	sprintf(output, "%*cBaby eagle %d finishes eating.\n", id, ' ', id);
	write(1, output, strlen(output));
	
	/* After a baby finishes eating, it reduces the eating counter by one.   */
	/* If baby was the last one eating and there are no more full pots,      */
	/* signal AllEmpty so that the rep can wake up the mother.               */
	Counter.Lock();
	eating--;
	if (eating == 0 && avaiPots == 0) {
		AllEmpty.Signal();
	}
	Counter.Unlock();
	
}

/* ----------------------------------------------------------- */
/* FUNCTION : goto_sleep                                       */
/*    Called by the mother. Blocks until the mother is woken-  */
/*    up by a baby.                                            */
/* PARAMETER USAGE :                                           */
/*    N/A                                                      */
/* FUNCTIONS CALLED :                                          */
/*    Mutex.Lock(), Mutex.Unlock(), Semaphore.Wait(),          */
/*    sprintf(), strlen(), write()                             */
/* ----------------------------------------------------------- */
void goto_sleep() {
	char output[1024];
	sprintf(output, "Mother eagle takes a nap.\n");
	write(1, output, strlen(output));
	/* Sleep until woken-up by a baby. */
	Sleep.Wait();
	Counter.Lock();
		sprintf(output, "Mother eagle is awoke by baby eagle %d and starts preparing food.\n", repID);
	Counter.Unlock();
	write(1, output, strlen(output));
}

/* ----------------------------------------------------------- */
/* FUNCTION : food_ready                                       */
/*    Fills all of the feeding pots.                           */
/* PARAMETER USAGE :                                           */
/*    m {int} - number of pots.                                */
/*    n {int} - number of baby eagles.                         */
/*    t {int} - the max number of feedings.                    */
/*    count {int*} - the number of completed feedings.         */
/* FUNCTIONS CALLED :                                          */
/*    Mutex.Lock(), Mutex.Unlock(), retire(),                  */
/*    Semaphore.Signal(), sprintf(), strlen(), write()         */
/* ----------------------------------------------------------- */
void food_ready(int m, int n, int t, int* count) {
	int i;
	char output[1024];

	/* Refill the pots (set availPots to #pots) and signal Filled once for   */
	/* each pot. Counter is locked until all pots are filled so that no baby */
	/* can eat until all pots are filled.                                    */
	Counter.Lock();
	avaiPots=m;
	if (maxPots==0) maxPots=m;
	for (i=0; i<m; i++) {
		Filled.Signal();
	}
	/* Output message. */
	(*count)++;
	sprintf(output, "Mother eagle says \"Feeding (%d)\"\n", *count);
	write(1, output, strlen(output));

	/* Retire the mother if necessary. */
	if (*count == t) {
		repID=-1; /* Close position. */
		retired=true; /* Set retired to true. */
		Counter.Unlock();
		retire(n, *count);
	} else {
		repID=0; /* Open the rep position. */
		Counter.Unlock();
	}
}

/* ----------------------------------------------------------- */
/* FUNCTION : retire                                           */
/*    Retires the mother and ends the game.                    */
/* PARAMETER USAGE :                                           */
/*    numBabies {int} - the number of babie eagles.            */
/*    fcount {int} - the number of feedings.                   */
/* FUNCTIONS CALLED :                                          */
/*    Semaphore.Signal(), Semaphore.Wait()                     */
/* ----------------------------------------------------------- */
void retire(int numBabies, int fcount) {
	int i=0;
	/* Wait until the pots are emptied for the last time. */
	AllEmpty.Wait();
	/* Signal Filled to unblock any waiting babies so that they may exit.    */
	/* Notice, however, that avaiPots is not changes from 0. Thus, this call */
	/* to signal will not let the babies attempt eating from empty pots.     */
	while(i<numBabies) {
		Filled.Signal();
		i++;
	}
}
