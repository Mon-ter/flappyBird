#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string>
#include <windows.h>
#define cols 96
#define lines 40
#define reps 200
using namespace std;
int height = lines - 2 * 5;
void bHC(int* birdsHeight) {
	short movementControl = 1;
	short mov = GetAsyncKeyState(VK_SPACE);
		if (mov & movementControl) {
			(*birdsHeight) -= 10;
			if (*birdsHeight < 0) {
				*birdsHeight = 0;
			}
		} else {
			if (*birdsHeight < height)
				(*birdsHeight)++;
		}
}
int whereIsBird(int upperBound, int lowerBound, int hei) {
	if (hei == 0 || hei == height) {
		return -1;
	}
	if (hei <= upperBound) {
		return 1;
	} else if (hei <= lowerBound) {
		return 2;
	} else {
		return 3;
	}
}
void ClearScreen()
{
    COORD cursorPosition;   cursorPosition.X = 0;   cursorPosition.Y = 0;   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
}
int game();
int main() {
	// menu wejsciowe
	int a = game(); // a:= score reached by Player
	// menu wyjsciowe, na razie:
	cout << "You scored " << a;
	return 0;
}
int game() {
	string s [ cols ];
	s [0] = "";
	for (int i = 1; i < cols; i++) {
		s [i] = s[i - 1] + " ";
	}
	string c [ cols + 1];
	c [0] = "";
	for (int i = 1; i <= cols; i++) {
		c [i] = c [i - 1] + "-";
	}
	int counter = 0;
	int length = 4;
	int lengthMinusOne = length - 1;
	int blank = 8; // blank space betweeen obstacles
	int boundOfLowerBounds = height - blank;
	int xd = cols - length;
	string d = "Score: ";
	system("mode con:cols=96 lines=40");
	system("cls");
	srand (time(NULL));
	int upperBound; // upper limit of blank space
	int lowerBound; // lower limit of blank space
	int lengthOfScore = 2;
	int currentBound = 10;
	int carry = cols - 10;
	int birdsHeight = height / 2;
	while (1) {		
		int j = 0;
		upperBound = rand() % boundOfLowerBounds;
		lowerBound = upperBound + blank;
		while (j < length) {
			int sw = whereIsBird(upperBound, lowerBound, birdsHeight);
			if (j % 2 == 0)
				bHC(&birdsHeight);
			if (sw < 0) {
				return counter;
			}
			cout << d << counter << " " << "||" << "\n";
			if (sw == 2) {
				for (int k = 1; k < 4; k++) {
					cout << c[cols] << "\n"; 
				}
				int k = 0;
				for (; k < upperBound; k++) {
					cout << s[cols - j] << c[j] << "\n"; 
				}
				for (; k < birdsHeight; k++) {
					cout << "\n";
				}
				cout << s[lengthMinusOne] << "@\n";
				for (; k < lowerBound; k++) {
					cout << "\n";
				}
				for (; k < height; k++) {
					cout << s[cols - j] << c[j] << "\n"; 
				}
				for (int k = 0; k < 4; k++) {
					cout << c[cols] << "\n"; 
				}
			} else if (sw == 1) {
				for (int k = 1; k < 4; k++) {
					cout << c[cols] << "\n"; 
				}
				int k = 0;
				for (; k < birdsHeight; k++) {
					cout << s[cols - j] << c[j] << "\n"; 
				}
				cout << s[lengthMinusOne] << "@" << s[xd - j] << c[j] << "\n";
				for (; k < upperBound; k++) {
					cout << s[cols - j] << c[j] << "\n"; 
				}
				for (; k < lowerBound; k++) {
					cout << "\n";
				}
				for (; k < height; k++) {
					cout << s[cols - j] << c[j] << "\n"; 
				}
				for (int k = 0; k < 4; k++) {
					cout << c[cols] << "\n"; 
				}
			} else {
				for (int k = 1; k < 4; k++) {
					cout << c[cols] << "\n"; 
				}
				int k = 0;
				
				for (; k < upperBound; k++) {
					cout << s[cols - j] << c[j] << "\n"; 
				}
				for (; k < lowerBound; k++) {
					cout << "\n";
				}
				for (; k < birdsHeight; k++) {
					cout << s[cols - j] << c[j] << "\n"; 
				}
				cout << s[lengthMinusOne] << "@\n" << s[xd - j] << c[j];
				for (; k < height; k++) {
					cout << s[cols - j] << c[j] << "\n"; 
				}
				for (int k = 0; k < 4; k++) {
					cout << c[cols] << "\n"; 
				}
			}
			j++;
			system("cls");
		ClearScreen();
		}
		j = xd;
		while (j >= 0) {
			int sw = whereIsBird(upperBound, lowerBound, birdsHeight);
			if (j % 2 == 0)
				bHC(&birdsHeight);
			if (sw < 0) {
				return counter;
			}
			if (sw == 2) {
				cout << d << counter << " " << "||" << "\n";
				for (int k = 1; k < 4; k++) {
					cout << c[cols] << "\n"; 
				}
				int k = 0;
				for (; k < upperBound; k++) {
					cout << s[j] << c[length] << "\n"; 
				}
				for (; k < birdsHeight; k++) {
					cout << "\n";
				}
				cout << s[lengthMinusOne] << "@\n";
				for (; k < lowerBound; k++) {
					cout << "\n";
				}
				for (; k < height; k++) {
					cout << s[j] << c[length] << "\n"; 
				}
			} else if (sw == 1) {
				cout << d << counter << " " << "||" << "\n";
				for (int k = 1; k < 4; k++) {
					cout << c[cols] << "\n"; 
				}
				int k = 0;
				for (; k < birdsHeight; k++) {
					cout << s[j] << c[length] << "\n"; 
				}
				if (j >= length) {
					cout << s[lengthMinusOne] << "@" << s[j - length] << c[length] << "\n";
				}
				for (; k < upperBound; k++) {
					cout << s[j] << c[length] << "\n"; 
				}
				for (; k < lowerBound; k++) {
					cout << "\n";
				}
				for (; k < height; k++) {
					cout << s[j] << c[length] << "\n"; 
				}
			} else {
				cout << d << counter << " " << "||" << "\n";
				for (int k = 1; k < 4; k++) {
					cout << c[cols] << "\n"; 
				}
				int k = 0;
				for (; k < upperBound; k++) {
					cout << s[j] << c[length] << "\n"; 
				}
				for (; k < lowerBound; k++) {
					cout << "\n";
				}
				for (; k < birdsHeight; k++) {
					cout << s[j] << c[length] << "\n"; 
				}
				if (j >= length) {
					cout << s[lengthMinusOne] << "@" << s[j - length] << c[length] << "\n";
				}
				for (; k < height; k++) {
					cout << s[j] << c[length] << "\n"; 
				}
			}
			j--;
			for (int k = 0; k < 4; k++) {
				cout << c[cols] << "\n"; 
			}
			system("cls");
			//ClearScreen();
		}
		j = length - 1;
		while (j >= 0) {
			cout << d << counter << " ||\n"; 
			bHC(&birdsHeight);
			for (int k = 1; k < 5; k++) {
				cout << c[cols] << "\n"; 
			}
			int k = 0;
			for (; k < upperBound; k++) {
					cout << c[j] << "\n"; 
			}
			for (; k < lowerBound; k++) {
				cout << "\n";
			}
			for (; k < height; k++) {
				cout << c[j] << "\n";
			}
			j--;
			for (int k = 0; k < 5; k++) {
				cout << c[cols] << "\n"; 
			}
			system("cls");
			//ClearScreen();
		}
		counter++;
		if (counter == currentBound) {
			lengthOfScore--;
			currentBound *= 10;
		}
	}
}
