#include <math.h>

double pow(double base, int e) {
    int result = 1;
    while(1) {
        
		if (e & 1)
            result *= base;
        
		e >>= 1;
        
		if (!e)
            break;
        
		base *= base;
    }

    return result;
}

/*calculating log using taylor series*/
double log(double x) {
	int i = 1;
	int limit = 20;
	double term = (x - 1);
	double result = 0;
	
	while(i<=limit) {
		result += term / i;
		term *= (x - 1) * (-1);
		i++;
	}
	
	return result;	
}

double expdev(double lambda) {
    double dummy;
    do
        dummy= (double) rand() / RAND_MAX;
    while (dummy == 0.0);

    return -log(dummy) / lambda;
}
