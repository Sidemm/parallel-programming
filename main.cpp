#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

using namespace std;
#define MAX_LINE       80 /* 80 chars per line, per command, should be enough. */

int parseCommand(char inputBuffer[], char *args[]);
int milisec;
int **seatOwner;
int **seatStatus;
int *reserveCount;
int **waitList;
pthread_mutex_t **seat_mutex;
time_t start_time;
time_t current_time;
FILE *fp;

char inputBuffer[MAX_LINE];
    char *args[MAX_LINE/2 + 1];
    char command[100];
    int shouldrun = 1;
    bool input = true;
    int check;
    int day, passenger, agent, tour, seat;
    int rando;

void ticketLog(int p_id, int a_id, char* operation, int seat_no, int tour_no){
    if(fp != NULL) {
        fprintf(fp, "\t%d\t%d\t%s\t%d\t%d\n", p_id, a_id, operation, seat_no, tour_no);
}
}

void *passengerf(void* passID){
    int i, j;
    long passerID = (long) passID;
    int passengerID = passerID;
    int agentID = 0;
    cout << "pthread: " << passengerID << " lives!" << endl;
    time_t curtime;
    curtime = time(NULL);
    while(curtime < start_time + milisec){
 float ticket_operation = rand() % 10 / 10.0; // generate a random ticket operation
 int ptour;
 if (tour > 1) ptour = rand() % tour; else ptour = 0;
 int pseat;
 //pthread_mutex_lock(&seat_mutex[0][0]);
if(ticket_operation < 0.4){
// reserve a ticket from a tour
if (reserveCount[passengerID-1] < 3){
for(pseat = 0; pseat < seat; pseat++){
int result = pthread_mutex_trylock(&seat_mutex[ptour][pseat]);
if(result == 0){
if(seatOwner[ptour][pseat] == 0){
seatOwner[ptour][pseat] = passengerID;
seatStatus[ptour][pseat] = 1;
reserveCount[passengerID-1]++;
cout << "pthread: " << passengerID << " reserves!" << endl;
ticketLog(passengerID, agentID, "R", pseat, ptour);
pthread_mutex_unlock(&seat_mutex[ptour][pseat]);
break;
}
}
// sleep if there is no available seats
}}}else if(0.4 <=ticket_operation && ticket_operation < 0.6){
// cancel the reserved/bought ticket if any
for(i = 0; i < tour; i++) for(j = 0; j < seat; j++) {if (seatOwner[i][j] == passengerID) {
if(seatStatus[i][j] == 1) reserveCount[passengerID-1]--;
seatStatus[i][j] = 0;
cout << "pthread: " << passengerID << " cancels!" << endl;
ticketLog(passengerID, agentID, "C", j, i);
seatOwner[i][j] = 0;
} }
// notify the passenger waiting on the condition of an empty seat
} else if(0.6 <= ticket_operation && ticket_operation < 0.8){
// view the reserved/bought ticket if any
for(i = 0; i < tour; i++) for(j = 0; j < seat; j++) {if (seatOwner[i][j] == passengerID) cout << "pthread: " << passengerID << " views!" << endl; ticketLog(passengerID, agentID, "V", j, i); }
} else {
// buy the reserved ticket if any
for(pseat = 0; pseat < seat; pseat++){
int result = pthread_mutex_trylock(&seat_mutex[ptour][pseat]);
if(result == 0){
cout << "test" << endl;
if(seatOwner[ptour][pseat] == 0 || (seatOwner[ptour][pseat] == passengerID && seatStatus[ptour][pseat] == 1)) {
seatOwner[ptour][pseat] = passengerID;
if(seatStatus[ptour][pseat] == 1) reserveCount[passengerID-1]--;
seatStatus[ptour][pseat] = 2;
cout << "pthread: " << passengerID << " buys!" << endl;
ticketLog(passengerID, agentID, "B", pseat, ptour);
pthread_mutex_unlock(&seat_mutex[ptour][pseat]);
break;
}}}
}
//pthread_mutex_unlock(&seat_mutex[0][0]);
curtime = time(NULL);
}
cout << "pthread: " << passengerID << " died!" << endl;
pthread_exit(0);
}

void *agentf(void* agtID){
    int i, j;
    long agentID = (long) agtID;
    cout << "athread: " << agentID << " lives!" << endl;
    time_t curtime;
    curtime = time(NULL);
    while(curtime < start_time + milisec){
 double ticket_operation = rand() % 10 / 10.0;// generate a random ticket operation
if(ticket_operation < 0.4){
// reserve a ticket from a tour for any passenger
// do nothing if there is no available seats
} else if(0.4 <=ticket_operation && ticket_operation < 0.6){
// cancel the reserved/bought ticket of any passenger
// notify the passenger waiting on the condition of an empty seat
} else if(0.6 <= ticket_operation && ticket_operation < 0.8){
// view all the seats

} else {
// buy the reserved ticket of any passenger

}
curtime = time(NULL);
}
cout << "athread: " << agentID << " died!" << endl;
pthread_exit(0);
}

int main()
{
    day = 1;
    passenger = 4;
    agent = 2;
    tour = 2;
    seat = 20;
    rando = 5;
    int i, j;
    cout << "input \"exe\" to execute" << endl;
    while(input){
    shouldrun = parseCommand(inputBuffer,args);
    if (strncmp(inputBuffer, "-d", 2) == 0) day = atoi(args[1]);
    else if (strncmp(inputBuffer, "-p", 2) == 0) passenger = atoi(args[1]);
    else if (strncmp(inputBuffer, "-a", 2) == 0) agent = atoi(args[1]);
    else if (strncmp(inputBuffer, "-t", 2) == 0) tour = atoi(args[1]);
    else if (strncmp(inputBuffer, "-s", 2) == 0) seat = atoi(args[1]);
    else if (strncmp(inputBuffer, "-r", 2) == 0) rando = atoi(args[1]);
    else if (strncmp(inputBuffer, "exe", 3) == 0) input = false;
    else cout << "invalid command" << endl;
    }
    /////////LOG///////////
    fp=fopen("tickets.log", "w");
    if(fp!= NULL)
    fprintf(fp, "Time \t\t P_ID \t\t A_ID \t\t Operation \t\t Seat No \t\t Tour No \n");

    int sec=5;
    milisec = 5 * day;
    srand(rando);
    seatOwner = new int *[tour];
    for(i=0; i < tour; i++) seatOwner[i] = new int[seat];
    seatStatus = new int *[tour];
    for(i=0; i < tour; i++) seatStatus[i] = new int[seat];
    waitList = new int *[tour];
    for(i=0; i < tour; i++) waitList[i] = new int[passenger];
    seat_mutex = new pthread_mutex_t *[tour];
    for(i=0; i < tour; i++) {seat_mutex[i] = new pthread_mutex_t[seat];
    for(j=0; j < tour; j++) {seat_mutex[i][j] = PTHREAD_MUTEX_INITIALIZER; pthread_mutex_init(&seat_mutex[i][j], NULL); }
    }
    reserveCount = new int[passenger];
    pthread_t pthreads[passenger];
    pthread_t athreads[agent];
    start_time = time(NULL);
    long a;
    for(a=0; a < passenger; a++) check = pthread_create(&pthreads[i], NULL, passengerf, (void *) a+1);
    for(a=0; a < agent; a++) check = pthread_create(&athreads[i], NULL, agentf, (void *) a+1);

    ////////////Summary of the Day//////////////
    int exit = 0;
    while(exit < day){

        current_time = time(NULL);
        if((current_time-start_time)>sec){

           printf("==============\nDay %d \n==============\n", sec/5);
           printf("Reserved Seats:\n ");
           for(int i=0; i<tour; i++)
           {
             printf("Tour %d: ", i+1);
             for(int j=0;j<seat; j++){
                if(seatStatus[i][j]==1)
                printf("%d, ",j+1);
             }
             printf("\n");
           }

           printf("Bought Seats:\n ");
           for(int i=0; i<tour; i++)
           {
             printf("Tour %d: ", i+1);
             for(int j=0;j<seat; j++){
                if(seatStatus[i][j]==2)
                printf("%d, ",j+1);
             }
             printf("\n");
           }

           printf("Waitlist: \n");
           for(int i=0; i<tour; i++)
           {
             printf("Tour %d: ", i+1);
             for(int j=0;j<passenger; j++){
                if(waitList[i][j]==1)
                printf("%d, ",j);
             }
             printf("\n");
           }
           exit++;
           sec+=5;
        }

    }



    //wait threads
    for(a=0; a < passenger; a++) pthread_join(pthreads[i], NULL);
    for(a=0; a < agent; a++) pthread_join(athreads[i], NULL);
    //free arrays
    for(i=0; i < tour; i++) delete[] seatOwner[i];
    delete[] seatOwner;
    for(i=0; i < tour; i++) delete[] seatStatus[i];
    delete[] seatStatus;
    delete[] reserveCount;
    //print stuff, log, whatever

    // while(curtime < starttime + milisec){ cout << "tatli" << endl; curtime = time(NULL);}
    fclose(fp);
    return 0;
}

int parseCommand(char inputBuffer[], char *args[])
{
    int length,		/* # of characters in the command line */
      i,		/* loop index for accessing inputBuffer array */
      start,		/* index where beginning of next command parameter is */
      ct,	        /* index of where to place the next parameter into args[] */
      command_number;	/* index of requested command number */

    ct = 0;

    /* read what the user enters on the command line */
    do {
	  printf("input>");
	  fflush(stdout);
	  length = read(STDIN_FILENO,inputBuffer,MAX_LINE);



    }
    while (inputBuffer[0] == '\n'); /* swallow newline characters */

    /**
     *  0 is the system predefined file descriptor for stdin (standard input),
     *  which is the user's screen in this case. inputBuffer by itself is the
     *  same as &inputBuffer[0], i.e. the starting address of where to store
     *  the command that is read, and length holds the number of characters
     *  read in. inputBuffer is not a null terminated C-string.
     */
    start = -1;
    if (length == 0)
      exit(0);            /* ^d was entered, end of user command stream */

    /**
     * the <control><d> signal interrupted the read system call
     * if the process is in the read() system call, read returns -1
     * However, if this occurs, errno is set to EINTR. We can check this  value
     * and disregard the -1 value
     */

    if ( (length < 0) && (errno != EINTR) ) {
      perror("error reading the command");
      exit(-1);           /* terminate with error code of -1 */
    }

    /**
     * Parse the contents of inputBuffer
     */

    for (i=0;i<length;i++) {
      /* examine every character in the inputBuffer */

      switch (inputBuffer[i]){
      case ' ':
      case '\t' :               /* argument separators */
	if(start != -1){
	  args[ct] = &inputBuffer[start];    /* set up pointer */
	  ct++;
	}
	inputBuffer[i] = '\0'; /* add a null char; make a C string */
	start = -1;
	break;

      case '\n':                 /* should be the final char examined */
	if (start != -1){
	  args[ct] = &inputBuffer[start];
	  ct++;
	}
	inputBuffer[i] = '\0';
	args[ct] = NULL; /* no more arguments to this command */
	break;

      default :             /* some other character */
	if (start == -1)
	  start = i;
	if (inputBuffer[i] == '&') {
	  inputBuffer[i-1] = '\0';
	}
      } /* end of switch */
    }    /* end of for */

    /**
     * If we get &, don't enter it in the args array
     */


    args[ct] = NULL; /* just in case the input line was > 80 */

    return 1;

}
