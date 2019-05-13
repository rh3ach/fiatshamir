#include <iostream>
#include <vector>
#include <ctime> // time() for srand()
#include <cstdlib> // rand()
#include <cmath> // pow()
#include <cstdint> // int32_t
#include <ctime>
int start_s=clock();
	// the code you wish to time goes here


#define p(X) (std::cout << X << std::endl)
// Uncomment to have the user calculate value of y
#define USER_INPUT

int32_t GetRandomNumber(const int32_t min, const int32_t max)
{
	return (rand() % (max - min)) + min;
}

int32_t GetRandomSign()
{
	return (rand() % 2) ? -1 : 1;
}

class NumberGenerator
{
	int32_t min = 1000;
	int32_t max = 10000;
	std::vector<int> primeNumbers;

	void GeneratePrimeNumbers(int32_t min, int32_t max)
	{
		primeNumbers.clear();

		for (int32_t i = min; i != max + 1; i++) {
			int32_t j;
			for (j = 2; j < i; j++) {
				if (i % j == 0) {
					break;
				}
			}
			if (i == j) {
				primeNumbers.push_back(i);
			}
		}
	}
public:
	NumberGenerator(int32_t min_ = 1000, int32_t max_ = 10000)
	{
		min = min_;
		max = max_;
	}
	int32_t GetLargePrimeNumber()
	{
		GeneratePrimeNumbers(min, max);

		int32_t index = GetRandomNumber(0, primeNumbers.size());
		return primeNumbers[ index ];
	}
};


int32_t getGCD(int32_t a, int32_t b)
{
	int32_t c;

	while (a != 0) {
		c = a;
		a = b % a;
		b = c;
	}

	return b;
}

int32_t getCoprime(int32_t n)
{
	int32_t coprime;
	
	do {
		coprime = GetRandomNumber(1, n);
	}
	while (getGCD(n, coprime) != 1);
	
	return coprime;
}

int main()
{
	srand(time(NULL));

	int32_t k = 5;

	auto numGen = NumberGenerator(18, 29);

	int32_t p = numGen.GetLargePrimeNumber();
	int32_t q = numGen.GetLargePrimeNumber();

	// n can be Blum integer for easier calculations,
	// but that is not required
	int32_t n = p * q;

	p("public              N = " << p << "*" << q << " = " << n );

	int32_t s = getCoprime(n);
	p("A's secret          S = Co-prime with N = " << s );

  int32_t s2 = getCoprime(n);
	p("A's 2nd secret     S2 = Co-prime with N = " << s2 );

	int32_t v = (s * s) % n;
	p("Public              V = (s^2)mod N = " << v );

  int32_t v2 = (s2 * s2) % n;
	p("Public2            V2 = (S2^2)mod N = " << v2 );

	// Protocol
	p("---------------------------------------");
	
	int32_t r = GetRandomNumber(1, n);
	p("A's random private  R = " << r );
	
	int32_t x = (r * r) % n;
	p("Discrete log        X = (R^2)mod N = " << x );

	int32_t e = GetRandomNumber(0, 2);
	p("Bool Challenge      E = " << e );

  int32_t e2 = GetRandomNumber(0, 2);
	p("2nd Bool Challenge E2 = " << e2 );

	int32_t y = 0;
p ("");
#ifdef USER_INPUTtt
	std::cout << "Calculate Y = (R*(S^E)*(S2^E2))mod N: ";
	std::cin >> y;
#else   
	y = r * pow(s, e) * pow(s2, e2);
	y = y % n;
	p("Y1 = " << y);
#endif
	p("---------------------------------------");
	int32_t y_sq_mod_n = (y * y) % n;
	p("Calculating using challenge answer:");
  p("Y1 = (Y^2)mod N = " << "(" << y << "^2)" << "mod " << n << "= " << y_sq_mod_n);
  p("");

	int32_t test = (x * (int)pow(v, e) * (int)pow(v2,e2)) % n;
	p("Value to be proven:");
  p("(X*(V^E)*(V2^E2)mod N = ("<< x << "*("<< v << "^" << e << "*"<< v2 << "^"<< e2 <<")mod "<< n << " = " << test);


  p("");
	if (y_sq_mod_n == test) {
		p("Congrats! You used the right secret.");
	} else {
		p("Your answer was wrong, please try again.");
	}



int stop_s=clock();
p ("time: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000);
	return 0;
}
