#include <iostream>
#include <vector>
#include <chrono>

/*IN BELOW CODE WE ARE GOING TO IMPLEMENT, TEST, AND USE BIG FIVE
MAKING OUR OWN PUSH BACK FUNCTION
AND PERFORMING A TIMING TEST FOR CAPMPARING VECTORS*/

/*We dont need to use BIG FIVE in complex class but vector class because in
vector class we deal with pointer & dynamically allocateed memory which takes place on heaps
and therefore since big five is doing resource management at its finest
in order to not having dangling pointers, Memory leak, and not explode the memeory heap and destruct/move, etc->
(manage resources) all automatically and efficient
we use big five.simply done by encapsulating the properties of big 5 as memeber functions --> code below: */

class Vector {
private:
	int* elems;
	int sz, capacity;

public:
	Vector() : elems{ nullptr }, sz{ 0 }, capacity{ 0 } {}		   //Default constructor
	explicit Vector(int s) :elems{ new int[s] }, sz{ s }, capacity{ sz } //This is our normal constructor 
	{
		std::cout << "Normal Constructor Called! " << std::endl;
	}
	Vector(Vector& rhs) : elems{ new int[rhs.sz] }, sz{ rhs.sz }  //This is our Copy Constructor     
	{
		std::cout << "Copy Constructor Called!" << std::endl;
		for (int i = 0; i != sz; i++) {
			elems[i] = rhs.elems[i];
		}
	}
	Vector& operator=(Vector& rhs) {                              //This is Copy Assignment Operator-> a rhs to a vector
		std::cout << "Copy Assignment Operator Called!" << std::endl;
		elems = new int[rhs.sz];
		sz = rhs.sz;

		for (int i = 0; i != sz; i++) {
			elems[i] = rhs.elems[i];
			return *this;
		}
	}
	Vector(Vector&& rhs) :elems{ rhs.elems }, sz{ rhs.sz } {    //This is our Move Constructor 
		std::cout << "Move Constructor Called!" << std::endl;
		rhs.elems = nullptr;
		rhs.sz = 0;
	}
	Vector& operator=(Vector&& rhs) {                           //Move Assignment Operator
		std::cout << "Move Assignment Operator Called!" << std::endl;

		elems = rhs.elems;
		sz = rhs.sz;

		rhs.elems = nullptr;
		rhs.sz = 0;
		return*this;
	}

	~Vector() { delete[] elems; }                               //Destructor

	int size() const { return sz; }
	int& operator[](int i) {
		return elems[i];
	}
	int getCapacity() const {
		return capacity;
	}

	Vector& my_push_back(const int d)							//Making our own push back
	{
		if (capacity == sz)
		{
			capacity = capacity * 2;
			int* doubSize = new int[capacity];
			for (int i = 0; i != sz; i++)
			{
				doubSize[i] = elems[i];
			}
			delete[] elems;
			elems = doubSize;
		}
		elems[sz] = d;
		sz++;
		return *this;
	}
};


int main()
{
	{   /*BIG FIVE TEST!*/
		Vector v{ 13 };                                             //Normal constructor         
		Vector v2 = v;                                              //Copy Constructor
		v = v2;                                                     //Copy Assignment Operator 
		Vector V3 = std::move(v);                                   //Move Constructor
		V3 = std::move(v);                                          //Move Assignment Operator 
		/*BIG FIVE WORKED PERFECTLY!*/
		/***********************************************************************************************/
		/*PUSH BACK FUNTION TEST!*/
		Vector G1(6);                                               //vector of size 6 (initiliazed with i)
		for (int i = 0; i < G1.size(); i++) {
			G1[i] = i;
		}
		std::cout << "\nThe size of Vector initially is: " << G1.size() << std::endl;

		G1.my_push_back(20);                                         //using our push back to append 2 and double the size
		G1.my_push_back(30);

		std::cout << "The size of vector after our function called: " << G1.size() << std::endl;
		std::cout << "The Capacity of the vector: " << G1.getCapacity() << std::endl;
		std::cout << "The elements in the vector: " << std::endl;
		for (int i = 0; i != G1.size(); i++) {
			std::cout << G1[i] << "\n";
		}
		/*PUSH BACK FUNTION ALSO WORKED PERFECTLY!*/
		/***********************************************************************************************/
		/*TIMING TEST  TEST:1  -> our self made class vector worked better since it took about 100 seconds less to run      */

		// !!!! ***** BEFORE RUNNING THIS PART: COMMENT THE PRINTING SECTION OF 
		// "cout: COPY CONSTRUCTOR CALLED" & "cout: NORMAL CONSTRUCTOR CALLED         !!!!!!!"

		int numofvecs = 10000;
		std::cout << "Time Test 1: " << std::endl << std::endl;
		clock_t startTime = clock();
		for (int i = 0; i < numofvecs; i++)
		{
			std::vector  <int> test;
			for (int j = 0; j < 100; j++) {
				test.push_back(j);
			}
		}
		std::cout << "std vector : " << double(clock() - startTime) / (double)CLOCKS_PER_SEC << " seconds." << std::endl;


		clock_t startTime2 = clock();
		for (int i = 0; i < numofvecs; i++)
		{
			Vector testm(100);
			for (int k = 0; k < 100; k++) {
				testm.my_push_back(k);
			}
		}
		std::cout << "Time took for Our class vector : " << double(clock() - startTime2) / (double)CLOCKS_PER_SEC << " seconds." << std::endl << std::endl;
	}

	/*TEST 1: completed perfectly ! our self made class vector worked better since it took about 100 seconds less to run*/
	/****************************************************************************************/
	/*TEST 2: Timing Test : AGAIN OUR CLASS VECTOR WORKED MUCH BETTER SINCE IT TOOK SIGNIFICANT LESS TIME*/
	//
	// !!!! ***** BEFORE RUNNING THIS PART: COMMENT THE PRINTING SECTION OF 
		// "cout: COPY CONSTRUCTOR CALLED" & "cout: NORMAL CONSTRUCTOR CALLED    !!!!!!!"

	std::cout << "Time Test 2: " << std::endl << std::endl;
	auto start = std::chrono::high_resolution_clock::now();
	std::vector  <int> test2(1);
	for (int j = 0; j < 100000; j++) {
		test2.push_back(j);
	}
	auto finish = std::chrono::high_resolution_clock::now();
	std::cout << "Time took for std vector : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() << " nano seconds\n";
	start = std::chrono::high_resolution_clock::now();
	Vector testm(1);
	for (int k = 0; k < 100; k++) {
		testm.my_push_back(k);
	}
	finish = std::chrono::high_resolution_clock::now();
	std::cout << "Time took for Our class vector : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() << " nano seconds\n";
	/*TIME TEST 2 WORKED PERFECTLY TOO , SEE THE COMMENTS ABOVE, THANK YOU*/

	/*SO, ALTHOUGH WE DID NOT DO MANY OTHER CHECKINGS TO REALLY CONFIRM THAT OUR CLASS IS BETTER OR NOT BUT AT LEAST BY THIS TIME TESTINGS WE CAN BE SURE THAT OUR OWN-IMPLEMENTED CLASS TAKES SIGNIFICANT LESS TIME TO RUN MEANING THAT IT'S MORE EFFICIENT IN TERMS OF TIMES OF ITERATIONS IN CPU (PROCCESSOR)*/
}

