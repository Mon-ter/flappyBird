#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <string>
#include <windows.h>
#include <thread>

#define cols 96
#define lines 40
#define reps 200
#define interval 100ms

using namespace std;
int height = lines - 2 * 5;

void printPauseMenu(int selected) {
    string game = "FlappyBird Game\n\n";
    string options = "    Continue game\n    Save game\n    Main menu";
    if (selected == 0) options.replace(0, 2, "->");
    else if (selected == 1) options.replace(18, 2, "->");
    else if (selected == 2) options.replace(32, 2, "->");
    system("CLS");
    cout << game << options;
}

void printMainMenu(int selected) {
    string Menu = "FlappyBird Game\n\n    New game\n    Load game\n    Continue game\n    Leaderboard\n    Quit";
    if (selected == 0) Menu.replace(17, 2, "->");
    else if (selected == 1) Menu.replace(30, 2, "->");
    else if (selected == 2) Menu.replace(44, 2, "->");
    else if (selected == 3) Menu.replace(62, 2, "->");
    else if (selected == 4) Menu.replace(78, 2, "->");
    system("CLS");
    cout << Menu;
}

void printAfterGameMenu(int selected, int val) {
    string game = "FlappyBird Game\n";
    string score = "Well done! You scored " + to_string(val) + " points!\n\n";
    string options = "    Save result\n    New game\n    Main menu";
    if (selected == 0) options.replace(0, 2, "->");
    else if (selected == 1) options.replace(16, 2, "->");
    else if (selected == 2) options.replace(29, 2, "->");
    system("CLS");
    cout << game << score << options;
}

void addToLeaderBoard(const string& name, int score) {

}

void bHC(int *birdsHeight) {
    short movementControl = 1;

    short key_esc = GetAsyncKeyState(VK_ESCAPE);
    if (key_esc & (0x8000 != 0)) {
        int selected = 0;
        int confirmed = -1;
        while (confirmed == -1) {
            printPauseMenu(selected);
            short key_up = GetAsyncKeyState(VK_UP);
            short key_down = GetAsyncKeyState(VK_DOWN);
            short key_enter = GetAsyncKeyState(VK_RETURN);
            if (key_up & (0x8000 != 0)) {
                if (selected == 0) selected = 3;
                selected--;
            } else if (key_down & (0x8000 != 0)) {
                if (selected == 2) selected = -1;
                selected++;
            } else if (key_enter & (0x8000 != 0)) {
                confirmed = selected;
            }
        }
        if(confirmed == 0) {
            //nothing
        } else if(confirmed == 1) {
            //save game, go to main menu
        } else {
            //abort game, go to main menu
        }
    }

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



int game();

int main() {
    int selected;
    int confirmed;

    selected = 0;
    confirmed = -1;
    while (true) {
        while (confirmed == -1) {
            printMainMenu(selected);
            short key_up = GetAsyncKeyState(VK_UP);
            short key_down = GetAsyncKeyState(VK_DOWN);
            short key_enter = GetAsyncKeyState(VK_RETURN);
            if (key_up & (0x8000 != 0)) {
                if (selected == 0) selected = 5;
                selected--;
            } else if (key_down & (0x8000 != 0)) {
                if (selected == 4) selected = -1;
                selected++;
            } else if (key_enter & (0x8000 != 0)) {
                confirmed = selected;
            }
        }
        if (confirmed == 0) {
            int score = game();

            if(score == -1) { //player discontinued prev game
                selected = 0;
                confirmed = -1;
                continue;
            }

            selected = 0;
            confirmed = -1;
            while (confirmed == -1) {
                printAfterGameMenu(selected, score);
                short key_up = GetAsyncKeyState(VK_UP);
                short key_down = GetAsyncKeyState(VK_DOWN);
                short key_enter = GetAsyncKeyState(VK_RETURN);
                if (key_up & (0x8000 != 0)) {
                    if (selected == 0) selected = 3;
                    selected--;
                } else if (key_down & (0x8000 != 0)) {
                    if (selected == 2) selected = -1;
                    selected++;
                } else if (key_enter & (0x8000 != 0)) {
                    confirmed = selected;
                }
            }
            if(confirmed == 0) {
                system("cls");
                string name;
                cout << "FlappyBird Game\n\nWpisz swoje imie: ";
                cin >> name;
                addToLeaderBoard(name, score);
            } else if (confirmed == 1) {
                confirmed = 0;
                continue;
            } else { //confirmed == 2
                //nothing
            }
        }
        else if (confirmed == 1) {}
        else if (confirmed == 2) {}
        else if (confirmed == 3) {}
        else break; //confirmed == 4



        selected = 0;
        confirmed = -1;
    }


    return 0;
}

int game() {
    string s[cols];
    s[0] = "";
    for (int i = 1; i < cols; i++) {
        s[i] = s[i - 1] + " ";
    }
    string c[cols + 1];
    c[0] = "";
    for (int i = 1; i <= cols; i++) {
        c[i] = c[i - 1] + "-";
    }
    int counter = 0;
    int length = 4;
    int lengthMinusOne = length - 1;
    int blank = 8; // blank space betweeen obstacles
    int boundOfLowerBounds = height - blank;
    int xd = cols - length;
    string d = "Score: ";
    system("mode con:cols=96 lines=40");
    srand(time(NULL));
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
            cout << d << counter << " " << "|| press \"ESC\" to pause the game." << "\n";
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
                for (k = 0; k < 4; k++) {
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
                for (k = 0; k < 4; k++) {
                    cout << c[cols] << "\n";
                }
            }
            j++;
            system("cls");
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
                cout << d << counter << " " << "|| press \"ESC\" to pause the game." << "\n";
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
                cout << d << counter << " " << "|| press \"ESC\" to pause the game." << "\n";
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
                cout << d << counter << " " << "|| press \"ESC\" to pause the game." << "\n";
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
        }
        j = length - 1;
        while (j >= 0) {
            cout << d << counter << " || press \"ESC\" to pause the game.\n";
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
            for (k = 0; k < 5; k++) {
                cout << c[cols] << "\n";
            }
            system("cls");
        }
        counter++;
        if (counter == currentBound) {
            lengthOfScore--;
            currentBound *= 10;
        }
    }
}
