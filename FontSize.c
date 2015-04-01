// Author: Nenad Vasic 01.Apr.2015
// http://www.spoj.com/problems/FONTSIZE/

#include <stdio.h>
#include <stdlib.h>

#define MAX_LENGTH 10000
#define ERROR_MARGIN 0.001
#define FALSE 0
#define TRUE 1


int tryToFit(double W, double H, char* s, double size)
{
	//Initialize helper variables
	int rowLength = -1; // -1 because adding first word adds a space
	int row = 1;
	int wordLength;

	while (*s != '\0'){
		//read next word
		wordLength = 0;
		while (*s != '\0' && *s != ' '){
			wordLength++;
			s++;
		}
		//go past the space
		if (*s == ' ') s++;

		//see if next word can fit in the current row
		if ((rowLength + wordLength + 1) * size * 2 > W * 3){
			//can't fit next word in the current row

			//if word is too long to be alone in a row
			if (wordLength * size * 2 > W * 3)
				return FALSE;
			row++;
			rowLength = wordLength;

			//too much rows
			if (row * size > H)
				return FALSE;
		}
		else
			//add a word in current row, plus one space
			rowLength += wordLength + 1;
	}

	//the word fits
	return TRUE;


}

void readline(char* buffer){
	while(TRUE){
		*buffer = getchar();
		if (*buffer == '\n' || *buffer == '\r' || *buffer == EOF){
			*buffer = '\0';
			break;
		}
		buffer++;
	}
}

int main()
{
	//allocate variables
	double W, H; // Width, Height
	char* s = malloc((MAX_LENGTH+1)*sizeof(char));

	//read from standard input
	readline(s);
	sscanf(s,"%lf %lf", &W, &H);
	readline(s);
	// if newline is \r\n, just in case :D
	if(*s == '\0') readline(s);

	//binary search
	double maxSize = H;
	double minSize = 0;
	while (maxSize - minSize > ERROR_MARGIN/1000){
    	double mean = minSize + (maxSize-minSize)/2;
    	if (tryToFit(W,H,s,mean))
    		minSize = mean;
    	else
    		maxSize = mean;
    }

    //print the resault
    printf("%lf", maxSize);

    return 0;
}