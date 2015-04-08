// Author: Nenad Vasic 01.Apr.2015
// http://www.spoj.com/problems/EST/

#include <stdio.h>
#include <math.h>

#define MAX_LENGTH 2011
#define MAX_K 205
#define DVAES_ILJADA 20110

//gobal arrays

//array from input
int a[MAX_LENGTH];
//s[i] = sum(from a[0] to a[i])
int s[MAX_LENGTH];
//count[i] = number of numbers in a that equals i
int count[DVAES_ILJADA];
//partialSolution[i][j] = solution from 0 to i with j sections 
double partialSolution[MAX_LENGTH][2];
//minimumError[i][j] = minimum error for section from i to j
double minimumError[MAX_LENGTH][MAX_LENGTH];


//calculate the error in section from min to max (excluding max)
double calculateError(int min, int max)
{
	double avg = 0;
	for (int i = min; i < max; i++){
		avg += a[i];
	}
	avg /= max - min;

	double error = 0;
	for (int i = min; i < max; i++){
		error += abs(a[i] - avg);
	}

	return error;
}


//calculate average between i(included) and j(excluded) in array a
double avg(int i, int j)
{
	if (i == 0) return ((double) s[j - 1]) / j;
	return ((double) (s[j - 1] - s[i - 1])) / (j - i);
}


//adds an element to the interval and calculates error for new interval
double dodajSmrada(int* kolkoIhIma,
			       int* iznad,
			       int* ispod,
			       int* tacnoStariAvg,
			       double stariAvg,
			       double noviAvg,
			       int krajIntervala,
			       double Greska)
{
	if (stariAvg != noviAvg){
		double manjiAvg = stariAvg > noviAvg ? noviAvg : stariAvg;
		double veciAvg = stariAvg < noviAvg ? noviAvg : stariAvg;

		int kolkoIhJeIzmedju = 0;
		for (int i = (int) ceil(manjiAvg); i < veciAvg; i++){
			if (i <= manjiAvg) continue;
			kolkoIhJeIzmedju += kolkoIhIma[i];
			Greska -= abs(stariAvg - i) * kolkoIhIma[i];
			Greska += abs(noviAvg - i) * kolkoIhIma[i];
		}

		if (stariAvg < noviAvg){
			Greska += *ispod * (noviAvg - stariAvg);
			Greska -= (*iznad - kolkoIhJeIzmedju) * (noviAvg - stariAvg);
			Greska += *tacnoStariAvg * (noviAvg - stariAvg);

			*iznad = *iznad - kolkoIhJeIzmedju;
			*ispod = *ispod + kolkoIhJeIzmedju;
			*ispod += *tacnoStariAvg;

			if (ceil(noviAvg) == noviAvg)
				*tacnoStariAvg = kolkoIhIma[(int) noviAvg];
			else
				*tacnoStariAvg = 0;

			*iznad -= *tacnoStariAvg;
			
		}
		else{
			Greska += *iznad * (stariAvg - noviAvg);
			Greska -= (*ispod - kolkoIhJeIzmedju) * (stariAvg - noviAvg);
			Greska += *tacnoStariAvg * (stariAvg- noviAvg);

			*iznad = *iznad + kolkoIhJeIzmedju;
			*ispod = *ispod - kolkoIhJeIzmedju;
			*iznad += *tacnoStariAvg;

			if (ceil(noviAvg) == noviAvg)
				*tacnoStariAvg = kolkoIhIma[(int) noviAvg];
			else
				*tacnoStariAvg = 0;

			*ispod -= *tacnoStariAvg;
		}
		

	}
	Greska += abs(noviAvg - a[krajIntervala]);
	kolkoIhIma[a[krajIntervala]]++;
	if (a[krajIntervala] == noviAvg)
		*tacnoStariAvg++;
	else if (a[krajIntervala] < noviAvg)
		(*ispod)++;
	else
		(*iznad)++;

	return Greska;
}



int solve(int n, int k)
{
	int* kolkoIhIma = count + (DVAES_ILJADA / 2);
	for (int i = 0; i < n - 1; i++){
		//base case, calculate first interval of length 1
		double Greska = 0;
		int iznad = 0;
		int ispod = 0;
		int tacnoStariAvg = 1;
		minimumError[i][i + 2] = Greska;
		kolkoIhIma[a[i]]++;

		//extend interval
		for (int j = i + 1; j < n; j++){
			Greska = dodajSmrada(kolkoIhIma,
								 &iznad,
								 &ispod,
								 &tacnoStariAvg,
								 avg(i, j),
								 avg(i, j + 1),
								 j,
								 Greska);
			minimumError[i][j+1] = Greska;
		}

		//return values of kolkoIhIma to 0
		for (int j = i; j < n; j++){
			kolkoIhIma[a[j]] = 0;
		}

	}
	
	//compute partialSolution[i][0]
	for (int i = 0; i < n; i++){
		partialSolution[i][0] = minimumError[0][i + 1];
	}

	//compute other values of array partialSolution
	for (int j = 1; j < k; j++){
		for (int i = j; i < n; i++){
			for (int l = j; l <= i; l++){
				double tmp = partialSolution[l - 1][(j - 1) % 2] + minimumError[l][i + 1];
				if (l == j || tmp < partialSolution[i][j])
					partialSolution[i][j % 2] = tmp;
			}

		}
	}

	return (int) partialSolution[n - 1][(k - 1) % 2];
}

int main(int argc, char** argv)
{
	int n, k;
	while (1){
		//read dimensions
		scanf("%d %d", &n, &k);

		//exit contition
		if (n == 0 && k == 0)
			break;

		//read array
		int i;
		for (i = 0; i<n; i++){
			scanf("%d", &a[i]);
			s[i] = i>0 ? s[i - 1] + a[i] : a[i];
		}

		//printf("pera %d %d\n", n, k);
		//solve the problem
		int asd = solve(n, k);
		printf("%d\n", asd);
	}

	return 0;
}