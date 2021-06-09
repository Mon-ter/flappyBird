#include <iostream>
#include <ctime>
#include <string>
#include <cstdlib>
#include <windows.h>
#include <fstream>

#define cols 96
#define lines 40

using namespace std;
int height = lines - 2 * 5;
int mode = 0;
int accCounter = 0;
int aoo;
int boo;
int coo;
int doo;
int eoo;
int G = 0;

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
    string Menu = "FlappyBird Game\n\n    New game\n    Load game\n    Leaderboard\n    Quit";
    if (selected == 0) Menu.replace(17, 2, "->");
    else if (selected == 1) Menu.replace(30, 2, "->");
    else if (selected == 2) Menu.replace(44, 2, "->");
    else if (selected == 3) Menu.replace(60, 2, "->");
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

void printLeaderboard() {
    system("CLS");
    cout << "FlappyBird Game\n\n" << "->  Return\n\n" << "Leaderboard:\n";
    ifstream file("leaderboard.json");
    string line;
    if (file.is_open()) {
        getline(file, line); //skips first bracket
        getline(file, line);
        int i;
        for (i = 0; i < line.size(); i++) {
            if (line.at(i) != ' ') continue;
            i++;
            break;
        }
        string temp = "";
        while (line.at(i) != ',') {
            temp += line.at(i);
            i++;
        }
        int rows = stoi(temp);

        getline(file, line); //skips lowest_score
        getline(file, line);
        for (i = 0; i < rows; i++) {
            temp = "";
            getline(file, line);
            getline(file, line);
            cout << i + 1 << ". ";
            int j = 0;
            while (line.at(j) != ' ')
                j++;
            j += 2; //skips ' ' and '"'
            while (line.at(j) != '"') {
                temp += line.at(j);
                j++;
            }
            j = 0;
            cout << temp << ": ";
            getline(file, line);
            temp = "";
            while (line.at(j) != ' ')
                j++;
            j++;
            while (j != line.size()) {
                temp += line.at(j);
                j++;
            }
            cout << temp << " points!" << "\n";
            getline(file, line);
        }

        file.close();
    } else {
        cout << "Empty :(";
    }
    while (true) {
        short key_enter = GetAsyncKeyState(VK_RETURN);
        if (key_enter & (0x8000 != 0)) {
            break;
        }
    }
}

void addToLeaderBoard(const string &name, int score) {
    ifstream read_file("leaderboard.json");
    string row;
    getline(read_file, row);
    if (read_file.is_open()) {
        getline(read_file, row); //gets number of rows
        int i;
        for (i = 0; i < row.size(); i++) {
            if (row.at(i) != ' ') continue;
            i++;
            break;
        }
        string temp = "";
        while (row.at(i) != ',') {
            temp += row.at(i);
            i++;
        }
        int rows = stoi(temp);
        getline(read_file, row); //gets lowest score
        for (i = 0; i < row.size(); i++) {
            if (row.at(i) != ' ') continue;
            i++;
            break;
        }
        temp = "";
        while (row.at(i) != ',') {
            temp += row.at(i);
            i++;
        }
        int lowest = stoi(temp);
        if (rows == 10 && lowest >= score) { //if the score is too low: skip entry to the leaderboard
            read_file.close();
            return;
        }
        getline(read_file, row);
        string scores[rows][2];
        for (i = 0; i < rows; i++) {
            temp = "";
            getline(read_file, row);
            getline(read_file, row);
            int j = 0;
            while (row.at(j) != ' ')
                j++;
            j += 2; //skips ' ' and '"'
            while (row.at(j) != '"') {
                temp += row.at(j);
                j++;
            }
            j = 0;
            scores[i][0] = temp;
            getline(read_file, row);
            temp = "";
            while (row.at(j) != ' ')
                j++;
            j++;
            while (j != row.size()) {
                temp += row.at(j);
                j++;
            }
            scores[i][1] = temp;
            getline(read_file, row);
        }
        ofstream newfile("leaderboard.json");
        if (rows != 10) rows += 1;
        if (lowest > score) lowest = score;
        newfile << "{\n\t\"rows\": " << rows << ",\n\t\"lowest_score\": "
                << lowest << ",\n\t\"leaderboard\": [\n";
        int j, val;
        string nametofile;
        bool isEntried = false;
        for (i = 0, j = 0; j < rows; j++) {
            if ((i == rows - 1 || stoi(scores[i][1]) <= score) && !isEntried) {
                val = score;
                nametofile = name;
                isEntried = true;
            } else {
                val = stoi(scores[i][1]);
                nametofile = scores[i][0];
                i++;
            }
            newfile << "\t\t{\n\t\t\t\"name\": \"" << nametofile << "\",\n\t\t\t\"points\": " << val << "\n\t\t}";
            if (j + 1 == rows) newfile << "\n";
            else newfile << ",\n";
        }
        newfile << "\t]\n}";
        newfile.close();
    } else {
        ofstream first_row("leaderboard.json");
        first_row << "{\n\t\"rows\": " << 1 << ",\n\t\"lowest_score\": "
                  << score << ",\n\t\"leaderboard\": [\n\t\t{\n\t\t\t\"name\": \""
                  << name << "\",\n\t\t\t\"points\": " << score << "\n\t\t}\n\t]\n}";
        first_row.close();
    }
}

bool loadGame() {

    ifstream file("save.json");
    int temp[5];
    if (!file.is_open()) return false;
    string line;
    getline(file, line); //skips first bracket
    for(int j = 0; j < 5; j++) {
        getline(file, line);
        int i;
        for (i = 0; i < line.size(); i++) {
            if (line.at(i) != ' ') continue;
            i++;
            break;
        }
        string temp_string;
        while (i != line.size() && line.at(i) != ',') {
            temp_string += line.at(i);
            i++;
        }
        temp[j] = stoi(temp_string);
    }
    aoo = temp[0];
    boo = temp[1];
    coo = temp[2];
    doo = temp[3];
    eoo = temp[4];
    file.close();
    return true;
}

void saveGame() {
    ofstream newfile("save.json");
    newfile << "{\n\t\"height\": " << aoo << ",\n\t\"upperBound\": " << boo << ",\n\t\"phase\": "
    << coo << ",\n\t\"index\": " << doo << ",\n\t\"points\": " << eoo << "\n}";
    newfile.close();
}

void bHC(int *birdsHeight, int upperBound, int phase, int index, int counter) {
    short movementControl = 1;

    short key_esc = GetAsyncKeyState(VK_ESCAPE);
    if (key_esc & (0x8000 != 0)) {
        int selected = 0;
        int confirmed = -1;
        printPauseMenu(selected);
        while (confirmed == -1) {
            short key_up = GetAsyncKeyState(VK_UP);
            short key_down = GetAsyncKeyState(VK_DOWN);
            short key_enter = GetAsyncKeyState(VK_RETURN);
            if (key_up & (0x8000 != 0)) {
                if (selected == 0) selected = 3;
                selected--;
                printPauseMenu(selected);
            } else if (key_down & (0x8000 != 0)) {
                if (selected == 2) selected = -1;
                selected++;
                printPauseMenu(selected);
            } else if (key_enter & (0x8000 != 0)) {
                confirmed = selected;
            }
        }
        if (confirmed == 0) {
            //goes back to the game
        } else if (confirmed == 1) {
        	aoo = *birdsHeight;
        	boo = upperBound;
        	coo = phase;
        	doo = index;
        	eoo = counter;
            saveGame();
            (*birdsHeight) = -1;
            return;
        } else {
            (*birdsHeight) = -1;
            G = 0;
            return;
        }
    }
	short gravity = GetAsyncKeyState(0x47);
    if (gravity & movementControl) {
    	G = (G == 0) ? 1 : 0;
	}
    short mov = GetAsyncKeyState(VK_SPACE);
    if (G == 0) {
	    if (mov & movementControl) {
	    	accCounter = 0;
	        (*birdsHeight) -= 6;
	        if (*birdsHeight < 0) {
	            *birdsHeight = 0;
	        }
	    } else {
	    	accCounter++;
	    	if (accCounter > 15) {
	    		(*birdsHeight) += 2;
			} else {
				(*birdsHeight)++;
			}
	        if (*birdsHeight > height) {
	        	*birdsHeight = height;
			}
	    }
	} else {
		if (mov & movementControl) {
	    	accCounter = 0;
	        (*birdsHeight) += 6;
	        if (*birdsHeight > height) {
	            *birdsHeight = height;
	        }
	    } else {
	    	accCounter++;
	    	if (accCounter > 15) {
	    		(*birdsHeight) -= 2;
			} else {
				(*birdsHeight)--;
			}
	        if (*birdsHeight < 0) {
	        	*birdsHeight = 0;
			}
	    }
	}
    short hack = GetAsyncKeyState(0x48);
    if (hack & movementControl) {
    	*birdsHeight = rand() % height;
    	accCounter = 0;
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

int whereIsBirdBonus(int upperBound, int lowerBound, int hei, int blank) {
    if (hei == 0 || hei == height || hei <= upperBound || hei >= lowerBound) {
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
        printMainMenu(selected);
        while (confirmed == -1) {
            short key_up = GetAsyncKeyState(VK_UP);
            short key_down = GetAsyncKeyState(VK_DOWN);
            short key_enter = GetAsyncKeyState(VK_RETURN);
            if (key_up & (0x8000 != 0)) {
                if (selected == 0) selected = 4;
                selected--;
                printMainMenu(selected);
            } else if (key_down & (0x8000 != 0)) {
                if (selected == 3) selected = -1;
                selected++;
                printMainMenu(selected);
            } else if (key_enter & (0x8000 != 0)) {
                confirmed = selected;
            }
        }
        if (confirmed == 0) {
        	G = 0;
            int score = game();
            mode = 0;
            if (score == -1) { //player discontinued prev game
                selected = 0;
                confirmed = -1;
                continue;
            }

            selected = 0;
            confirmed = -1;
            printAfterGameMenu(selected, score);
            while (confirmed == -1) {
                short key_up = GetAsyncKeyState(VK_UP);
                short key_down = GetAsyncKeyState(VK_DOWN);
                short key_enter = GetAsyncKeyState(VK_RETURN);
                if (key_up & (0x8000 != 0)) {
                    if (selected == 0) selected = 3;
                    selected--;
                    printAfterGameMenu(selected, score);
                } else if (key_down & (0x8000 != 0)) {
                    if (selected == 2) selected = -1;
                    selected++;
                    printAfterGameMenu(selected, score);
                } else if (key_enter & (0x8000 != 0)) {
                    confirmed = selected;
                }
            }
            if (confirmed == 0) {
                system("cls");
                string name;
                cout << "FlappyBird Game\n\nEnter your nickname: ";
                cin >> name;
                addToLeaderBoard(name, score);
                short key_enter = GetAsyncKeyState(VK_RETURN);
            } else if (confirmed == 1) {
                confirmed = 0;
                continue;
            } else { //confirmed == 2
                //nothing
            }
        } else if (confirmed == 1) { //Main menu -> load game
        	G = 0; 
            if (loadGame()) {
                mode = 1;
                confirmed = 0;
                continue;
            }

        } else if (confirmed == 2) { //Main menu -> leaderboard
            printLeaderboard();
        } else break; //exit



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
    int blank = 10; // blank space betweeen obstacles
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
        int j;
        if (mode == 0) {
            j = 0;
            upperBound = rand() % boundOfLowerBounds;
        } else {
            if (coo == 1)
                j = doo;
            else
                j = length;
            birdsHeight = aoo;
            upperBound = boo;
            counter = eoo;
        }
        lowerBound = upperBound + blank;
        while (j < length) {
            int sw = whereIsBird(upperBound, lowerBound, birdsHeight);
            if (j % 2 == 0)
                bHC(&birdsHeight, upperBound, 1, j, counter);
            if (sw < 0) {
                return counter;
            }
            if (birdsHeight == -1) {
            	G = 0;
                return -1;
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
        if (mode == 0)
            j = xd;
        else {
            if (coo == 3)
                j = length - 1;
            else
                j = xd;
        }

        while (j >= length) {
            if (j % 2 == 0)
                bHC(&birdsHeight, upperBound, 2, j, counter);
            int sw = whereIsBird(upperBound, lowerBound, birdsHeight);
            if (sw < 0) {
                return counter;
            }
            if (birdsHeight == -1) {
            	G = 0;
                return -1;
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

        while (j >= 0) {
            if (j % 2 == 0)
                bHC(&birdsHeight, upperBound, 2, j, counter);
            int sw = whereIsBirdBonus(upperBound, lowerBound, birdsHeight, blank);
            if (sw < 0) {
                return counter;
            }
            if (birdsHeight == -1) {
            	G = 0;
                return -1;
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
        if (mode != 0) {
            if (coo == 3) {
                j = doo;
            } else {
                j = length - 1;
            }
        } else {
            j = length - 1;
        }

        while (j >= 0) {
            cout << d << counter << " || press \"ESC\" to pause the game.\n";
            bHC(&birdsHeight, upperBound, 3, j, counter);
            for (int k = 1; k < 5; k++) {
                cout << c[cols] << "\n";
            }
            if (birdsHeight == -1) {
            	G = 0;
                return -1;
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
        mode = 0;
        if (counter == currentBound) {
            lengthOfScore--;
            currentBound *= 10;
        }
    }
}
