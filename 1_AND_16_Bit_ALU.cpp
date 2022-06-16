#include <iostream>
#include <string>
using namespace std;

//ALU Single Bit Function
int* ALU1(int A, int B, int bInv, int cIn, int cOut, int op, int* result);

//ALU 16 Bit Function
int* ALU16(int A[], int B[], int bInv, int op);

//Booth Multiplier Algorithm Function
int* Booth_Mult(int MD[], int MQ[]);


int main()
{
	int op = 10;
	static int* result;
	string MQ;
	string MD;
	int MQArray[16];
	int MDArray[16];

	cout << "Enter your 16 digit multiplicand: " << endl;
	getline(cin, MD);

	cout << "Enter your 16 digit multiplier: " << endl;
	getline(cin, MQ);

	//Converts the strings into an int array
	for (int i = 0; i < 16; i++)
	{
		MDArray[i] = MD[i] - '0';
		MQArray[i] = MQ[i] - '0';
	}

	result = Booth_Mult(MDArray, MQArray);

	//Displays the results
	cout << "\n\nResult: ";
	for (int i = 0; i < 32; i++)
	{
		cout << *(result + i);
	}
	cout << endl;

	system("pause");
	return 0;
}

int* ALU1(int A, int B, int bInv, int cIn, int cOut, int op, int* result)
{
	//Checks if bInv is active
	if (bInv == 1)
	{
		if (B == 0)
			B = 1;
		else if (B == 1)
			B = 0;
	}

	//Big list of if conditions simulating the A, B, and C IN values of an ALU.
	if (A == 0 && B == 0 && cIn == 0)
	{
		cOut = 0;
		*result = 0;
	}
	if (A == 1 && B == 0 && cIn == 0)
	{
		cOut = 0;
		*result = 1;
	}
	if (A == 0 && B == 1 && cIn == 0)
	{
		cOut = 0;
		*result = 1;
	}
	if (A == 1 && B == 1 && cIn == 0)
	{
		cOut = 1;
		*result = 0;
	}
	if (A == 0 && B == 0 && cIn == 1)
	{
		cOut = 0;
		*result = 1;
	}
	if (A == 1 && B == 0 && cIn == 1)
	{
		cOut = 1;
		*result = 0;
	}
	if (A == 0 && B == 1 && cIn == 1)
	{
		cOut = 1;
		*result = 0;
	}
	if (A == 1 && B == 1 && cIn == 1)
	{
		cOut = 1;
		*result = 1;
	}
	return result;
}

int* ALU16(int* A, int* B, int bInv, int op)
{
	int aElem;
	int bElem;
	static int result[16];
	int bElemInvArr[16];
	int cIn = 0;
	int cOut = 0;

	//checks if b invert is active
	if (bInv == 1)
		cIn = 1;

	//Actually changes the values of the b inverted array.
	for (int i = 15; i >= 0; i--)
	{
		bElemInvArr[i] = B[i];

		if (bInv == 1)
		{
			if (bElemInvArr[i] == 0)
				bElemInvArr[i] = 1;
			else if (bElemInvArr[i] == 1)
				bElemInvArr[i] = 0;
		}
	}

	//The main portion of the ALU 16 function.
	for (int i = 15; i >= 0; i--)
	{
		aElem = A[i];
		bElem = B[i];

		result[i] = *ALU1(aElem, bElem, bInv, cIn, cOut, op, result);
		//Used bElemInvArr so that I wouldnt have to make another if conditition inside this for loop. I
		//instead just had the ALU check for b invert in both functions.
		if (aElem == 1 && bElemInvArr[i] == 1)
		{
			cOut = 1;
			cIn = cOut;
		}
		else if (aElem == 1 && cIn == 1 && bElemInvArr[i] == 0)
		{
			cOut = 1;
			cIn = cOut;
		}
		else if (bElemInvArr[i] == 1 && cIn == 1 && aElem == 0)
		{
			cOut = 1;
			cIn = cOut;
		}
		else
			cIn = 0;
	}
	return result;
}


int* Booth_Mult(int* MD, int* MQ)
{
	int cycleCounter = 15;
	static int* ACptr = 0;
	int AC[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	int zero[] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	int MQRight = 0;
	static int result[32];
	int temp[32], tempMD[16], tempMQ[16];

	//Sets the MD and MQ values
	for (int i = 15; i >= 0; i--)
	{
		tempMD[i] = MD[i];
		tempMQ[i] = MQ[i];
	}

	//cycle counter countdown loop
	while (cycleCounter >= 0)
	{
		for (int i = 0; i < 16; i++)
		{
			//Checks conditions for the algorithm
			if (tempMQ[15] == 0 && MQRight == 0)
				ACptr = ALU16(AC, zero, 0, 10);
			else if (tempMQ[15] == 0 && MQRight == 1)
				ACptr = ALU16(AC, tempMD, 0, 10);
			else if (tempMQ[15] == 1 && MQRight == 0)
				ACptr = ALU16(AC, tempMD, 1, 10);
			else if (tempMQ[15] == 1 && MQRight == 1)
				ACptr = ALU16(AC, zero, 1, 10);

			//sets the ALU16 values to the AC array
			for (int i = 0; i < 16; i++)
			{
				AC[i] = *(ACptr + i);
			}

			//First Iterations Display, shows the mathematical procedure.
			cout << "  Cycle Counter	" << "	MD	" << "		 AC	  " << "		MQ	  " << "	MQ-1	 " << endl;
			cout << "	" << cycleCounter << "	";
			for (int i = 0; i < 16; i++)
			{
				cout << tempMD[i];
			}
			cout << "	";
			for (int i = 0; i < 16; i++)
			{
				cout << AC[i];
			}
			cout << "	";
			for (int i = 0; i < 16; i++)
			{
				cout << tempMQ[i];
			}
			cout << "	" << MQRight << "	" << "Mathematical Operation" << endl;

			//First part of the shift right. Sets the MQ-1 equal to the last digit of MQ
			MQRight = tempMQ[15];

			//Sign extends first value of AC
			if (AC[0] == 0)
				temp[0] = 0;
			else if (AC[0] == 1)
				temp[0] = 1;

			//Made a temp array that combines both the AC and MQ into one array to make the right shift 
			//easier to manage
			for (int i = 0; i < 16; i++)
			{
				temp[i + 1] = AC[i];
			}

			for (int i = 0; i < 15; i++)
			{
				temp[i + 17] = tempMQ[i];
			}

			//Places the values from the temp array back into AC and MQ
			for (int i = 0; i < 32; i++)
			{
				if (i < 16)
					AC[i] = temp[i];
				else if (i >= 16)
					tempMQ[i - 16] = temp[i];
			}

			//Second iteration display (shows the right shift)
			cout << "  Cycle Counter	" << "	MD	" << "		 AC	  " << "		MQ	  " << "	MQ-1	 " << endl;
			cout << "	" << cycleCounter << "	";
			for (int i = 0; i < 16; i++)
			{
				cout << tempMD[i];
			}
			cout << "	";
			for (int i = 0; i < 16; i++)
			{
				cout << AC[i];
			}
			cout << "	";
			for (int i = 0; i < 16; i++)
			{
				cout << tempMQ[i];
			}
			cout << "	" << MQRight << "	" << "SHift Right" << endl;
			cycleCounter--;
		}
	}

	//uploads temp to the result
	for (int i = 0; i < 32; i++)
	{
		result[i] = temp[i];
	}

	return result;
}