#include <iostream>
#include <stdlib.h>
#include <string>
#include <windows.h>
#define cols 240
#define lines 40
#define reps 200
using namespace std;
void ClearScreen()
{
    COORD cursorPosition;   cursorPosition.X = 0;   cursorPosition.Y = 0;   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
}
int main() {
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
	int height = lines - 2 * 5;
	int length = cols / 8;
	int doubleLength = length << 1;
	int tripleLength = 3 * length;
	int cor = cols / 40;
	string d;
	system("mode con:cols=240 lines=40");
	system("cls");
	while (1) {		
		int j = tripleLength;
		while (j >= doubleLength) {
			for (int k = 0; k < 5; k++) {
				cout << c[cols] << "\n"; 
			}
			for (int k = 0; k < height; k++) {
					cout << s[j - length] << c[length] << s[doubleLength] << c[length] << s[doubleLength] << c[tripleLength - j] << "\n";
			}
			if (GetAsyncKeyState(VK_SPACE)) {
				cout << "i will be jumping sometime\n";
			}
			j--;
			for (int k = 0; k < 5; k++) {
				cout << c[cols] << "\n"; 
			}
			ClearScreen();
		}
		while (j >= length) {
			for (int k = 0; k < 5; k++) {
				cout << c[cols] << "\n"; 
			}
			for (int k = 0; k < height; k++) {
					cout << s[j - length] << c[length] << s[doubleLength] << c[length] << s[doubleLength] << c[length] << "\n";
			}
			if (GetAsyncKeyState(VK_SPACE)) {
				cout << "i will be jumping sometime\n";
			}
			j--;
			for (int k = 0; k < 5; k++) {
				cout << c[cols] << "\n"; 
			}
			ClearScreen();
		}
		while (j >= 0) {
			for (int k = 0; k < 5; k++) {
				cout << c[cols] << "\n"; 
			}
			for (int k = 0; k < height; k++) {
					cout << c[j] << s[doubleLength] << c[length] << s[doubleLength] << c[length] << "\n";
			}
			if (GetAsyncKeyState(VK_SPACE)) {
				cout << "i will be jumping sometime\n";
			}
			j--;
			for (int k = 0; k < 5; k++) {
				cout << c[cols] << "\n"; 
			}
			ClearScreen();
		}
	}
}
