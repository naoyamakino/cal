/*
 *cmpt-300
 *Assignment 5
 *Naoya Makino
 *SFU email ID: nmakino@sfu.ca
 *SFU #: 301117541
 *Dec 3rd, 2009
 *File: main.c
 *Description: display_3months(part a): display 3 months sequentially
 *	       display_3months_side(part b): output the 3 months side-by-side
 * 	       in order to run part a, please comment out display_3months_side(month, year) 
 *	       and take out comments on display_3months(month, year) in main function
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string>
#include <sys/types.h>
#include <assert.h>

#ifndef TRUE
#  define TRUE 1
#endif

#ifndef FALSE
#  define FALSE -1
#endif
#define NUM_OF_LINES_OF_MONTH 8
#define MAXSIZE 20
#define JAN 1
#define DEC 12

using namespace std;

void display_3months(int month, int year);
void display_year(int year);
void display_3months_side(int month, int year);
void addSpaces(int count);

main (int argc, char **argv) {
	time_t rawtime;
  	struct tm *timeinfo;
	int month,year;
	char buf[10], buf2[10];
	
  	time ( &rawtime );
  	timeinfo = localtime( &rawtime );
	strftime(buf, sizeof(buf), "%m", timeinfo);
	strftime(buf2, sizeof(buf2), "%Y", timeinfo);

	month = atoi(buf);
	year = atoi(buf2);
	if(argc == 1)
		//display_3months(month, year);
		display_3months_side(month, year);
	else if(argc == 2)
	{
		year = atoi(argv[1]);
		display_year(year);
	}
	else if(argc == 3)
	{
		month = atoi(argv[1]);
		year = atoi(argv[2]);
		//display_3months(month, year);
		display_3months_side(month, year);
	}
			
}
/*part a*/
void display_3months(int month, int year)
{
	assert(year > 0 && 9999 > year);
	assert(month > 0 && month < 13);
	char cal[MAXSIZE];
	if(month == JAN)
	{
		sprintf(cal, "%s %d %d","/usr/bin/cal", DEC, year-1);
		system(cal);
		sprintf(cal, "%s %d %d","/usr/bin/cal", month, year);
		system(cal);
		sprintf(cal, "%s %d %d","/usr/bin/cal", month+1, year);
		system(cal);
	}
	else if(month == DEC)
	{
		sprintf(cal, "%s %d %d","/usr/bin/cal", month-1, year);
		system(cal);
		sprintf(cal, "%s %d %d","/usr/bin/cal", month, year);
		system(cal);
		sprintf(cal, "%s %d %d","/usr/bin/cal", JAN, year+1);
		system(cal);
	}
	else
	{	
		sprintf(cal, "%s %d %d","/usr/bin/cal", month-1, year);
		system(cal);
		sprintf(cal, "%s %d %d","/usr/bin/cal", month, year);
		system(cal);
		sprintf(cal, "%s %d %d","/usr/bin/cal", month+1, year);
		system(cal);
	}
}
void display_year(int year)
{
	assert(year > 0 && 9999 > year);
	char cal[MAXSIZE];
	sprintf(cal, "%s %d","/usr/bin/cal", year);
	system(cal);
}
/*part b*/
void display_3months_side(int month, int year)
{
	assert(year > 0 && 9999 > year);
	assert(month > 0 && month < 13);

	char cal[MAXSIZE],preCal[MAXSIZE],nextCal[MAXSIZE],
		buf[MAXSIZE],buf2[MAXSIZE],buf3[MAXSIZE];
	int fd[2],fd2[2],fd3[2];//filedes[0] is for reading, filedes[1] is for writing. 
	int defout,i,count = 0;
	
	if (month == JAN) {
		sprintf(cal, "%s %d %d","/usr/bin/cal", month, year);
		sprintf(preCal, "%s %d %d","/usr/bin/cal", DEC, year-1);
		sprintf(nextCal, "%s %d %d","/usr/bin/cal", month+1, year);
	}
	else if(month == DEC){
		sprintf(cal, "%s %d %d","/usr/bin/cal", month, year);
		sprintf(preCal, "%s %d %d","/usr/bin/cal", month-1, year);
		sprintf(nextCal, "%s %d %d","/usr/bin/cal", JAN, year+1);	
	}
	else {
		sprintf(cal, "%s %d %d","/usr/bin/cal", month, year);
		sprintf(preCal, "%s %d %d","/usr/bin/cal", month-1, year);
		sprintf(nextCal, "%s %d %d","/usr/bin/cal", month+1, year);		
	}
	
	if (pipe(fd) == FALSE) { perror("pipe"); exit(EXIT_FAILURE); }
	if (pipe(fd2) == FALSE) { perror("pipe"); exit(EXIT_FAILURE); }
	if (pipe(fd3) == FALSE) { perror("pipe"); exit(EXIT_FAILURE); }
	
	defout  = dup(STDOUT_FILENO);//make copy of stdout
	dup2(fd[1],  STDOUT_FILENO);//makes fd[1] be the copy of stdout, closing stdout first if necessary.
	if (system(preCal) == FALSE) { perror("system"); exit(EXIT_FAILURE); }
	
	dup2(fd2[1],  STDOUT_FILENO);
	if (system(cal) == FALSE) { perror("system"); exit(EXIT_FAILURE); }
	
	dup2(fd3[1],  STDOUT_FILENO);
	if (system(nextCal) == FALSE) { perror("system"); exit(EXIT_FAILURE); }
	

	for(i = 0;i < NUM_OF_LINES_OF_MONTH;i++)
	{
		dup2(defout, STDOUT_FILENO);//bring back defout to STDOUT_FILENO
		/*i have gained this while loop idea from Juhan Houang*/	
		while (read(fd[0], buf, 1) > 0 && strcmp(buf, "\n") != 0)
		{
			write(STDOUT_FILENO, buf, 1);
			count++;
		}
		addSpaces(count);//if there needs to space, ie) last lines of months, add spaces accordingly 
		write(STDOUT_FILENO, "\t", 1);		 
		count = 0;
		while (read(fd2[0], buf, 1) > 0 && strcmp(buf, "\n") != 0)
		{
			write(STDOUT_FILENO, buf, 1);
			count++;
		}
		addSpaces(count);  
		write(STDOUT_FILENO, "\t", 1);			
		count = 0;
		while (read(fd3[0], buf, 1) > 0 && strcmp(buf, "\n") != 0)
		{
			write(STDOUT_FILENO, buf, 1);
			count++;
		} 
		count = 0;
		write(STDOUT_FILENO, "\n", 1);		
	}	
	close(fd[0]);close(fd[1]);
	close(fd2[0]);close(fd2[1]);
	close(fd3[0]);close(fd3[1]);
}
void addSpaces(int count)
{
	int i;
	if(count < MAXSIZE)
	{
		for(i=0;i<MAXSIZE - count;i++)
			write(STDOUT_FILENO," ", 1);
	}
}

