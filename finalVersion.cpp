#include <iostream>
#include <ctime>
#include <string>
#include <cstdlib>
#include <windows.h>
#include <fstream>
// parameters defining resolution of game's console
#define cols 96
#define lines 40

using namespace std;
int height = lines - 2 * 5; // height of game's board (without surrounding lines)
int mode = 0; // variable determines whether we are in first cycle after resuming game or not
int accCounter = 0; // variable to calculate acceleration
int aoo; // variables used for game saving
int boo;
int coo;
int doo;
int eoo;
int G = 0; // gravity (=0 - normal one)

void printPauseMenu(int selected) { //prints Pause Menu
    string game = "FlappyBird Game\n\n";
    string options = "    Continue game\n    Save game\n    Main menu";
    if (selected == 0) options.replace(0, 2, "->");  //switches the selections arrow depending on current option
    else if (selected == 1) options.replace(18, 2, "->");
    else if (selected == 2) options.replace(32, 2, "->");
    system("CLS");
    cout << game << options;
}

void printMainMenu(int selected) { //prints Main Menu
    string Menu = "FlappyBird Game\n\n    New game\n    Load game\n    Leaderboard\n    Quit";
    if (selected == 0) Menu.replace(17, 2, "->"); //switches the selections arrow depending on current option
    else if (selected == 1) Menu.replace(30, 2, "->");
    else if (selected == 2) Menu.replace(44, 2, "->");
    else if (selected == 3) Menu.replace(60, 2, "->");
    system("CLS");
    cout << Menu;
}

void printAfterGameMenu(int selected, int val) { //prints Menu that appears after finished game
    string game = "FlappyBird Game\n";
    string score = "Well done! You scored " + to_string(val) + " points!\n\n";
    string options = "    Save result\n    New game\n    Main menu";
    if (selected == 0) options.replace(0, 2, "->"); //switches the selections arrow depending on current option
    else if (selected == 1) options.replace(16, 2, "->");
    else if (selected == 2) options.replace(29, 2, "->");
    system("CLS");
    cout << game << score << options;
}

void printLeaderboard() {//prints Leaderboard that is downloaded from json file
    system("CLS");
    cout << "FlappyBird Game\n\n" << "->  Return\n\n" << "Leaderboard:\n";
    ifstream file("leaderboard.json");
    string line;
    if (file.is_open()) {
        getline(file, line); //skips first bracket
        getline(file, line);
        int i;
        for (i = 0; i < line.size(); i++) { //gets the number of rows from the
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
        for (i = 0; i < rows; i++) { //gets each score and prints every score that is saved to the file
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
    } else { //if file doesn't exist print an information about it's emptiness
        cout << "Empty :(";
    }
    while (true) {
        short key_enter = GetAsyncKeyState(VK_RETURN);
        if (key_enter & (0x8000 != 0)) {
            break;
        }
    }
}

void addToLeaderBoard(const string &name, int score) { //adds a score to the leaderboard
    //it saves only the best ones (only top 10 scores), so if the scores isn't great enough, it
    //does not get saved in the json file.
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
        string scores[rows][2]; //downloads the whole leaderboard to a 2d string array
        for (i = 0; i < rows; i++) { //this whole loop is to avoid spaces and tabs from each row in the json file
                                     // to get certain values directly to the variables
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
                temp += row.at(j); //finally the values we wanted!
                j++;
            }
            scores[i][1] = temp;
            getline(read_file, row);
        }
        ofstream newfile("leaderboard.json"); //creates a new file, and uploads the updated version of
        //the leaderboard.
        if (rows != 10) rows += 1;
        if (lowest > score) lowest = score;
        newfile << "{\n\t\"rows\": " << rows << ",\n\t\"lowest_score\": "
                << lowest << ",\n\t\"leaderboard\": [\n"; //writes lowest score and number of rows
        int j, val;
        string nametofile;
        bool isEntried = false;
        for (i = 0, j = 0; j < rows; j++) { //checks when it should paste the new entry
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
    } else { //if this is the first entry to the leaderboard file just simply write the whole string below
        ofstream first_row("leaderboard.json");
        first_row << "{\n\t\"rows\": " << 1 << ",\n\t\"lowest_score\": "
                  << score << ",\n\t\"leaderboard\": [\n\t\t{\n\t\t\t\"name\": \""
                  << name << "\",\n\t\t\t\"points\": " << score << "\n\t\t}\n\t]\n}";
        first_row.close();
    }
}

bool loadGame() {
    //this algorithm reads saved file and gets values needed to recreate the situation in which
    //a player was placed.
    ifstream file("save.json");
    int temp[5];
    if (!file.is_open()) return false; //if the save file doesn't exists, return false value
    //which will inform that loading the save ended up with an error
    string line;
    getline(file, line); //skips first bracket
    for (int j = 0; j < 5; j++) { //it gets all values necessary to recreate the game
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
    aoo = temp[0]; //it uploads the values to global variables
    boo = temp[1];
    coo = temp[2];
    doo = temp[3];
    eoo = temp[4];
    file.close();
    return true; //informs that the game was loaded successfully
}

void saveGame() { //uploads the global variables to a file
    ofstream newfile("save.json");
    newfile << "{\n\t\"height\": " << aoo << ",\n\t\"upperBound\": " << boo << ",\n\t\"phase\": "
            << coo << ",\n\t\"index\": " << doo << ",\n\t\"points\": " << eoo << "\n}";
    newfile.close();
}

void bHC(int *birdsHeight, int upperBound, int phase, int index, int counter) { // bird's Height Correction
    // upperBound - of a current obstacle
    // counter - score
    // index - of the mode
    short movementControl = 1; // helpful by logical operations on GetAsyncKeyState - like functions

    short key_esc = GetAsyncKeyState(VK_ESCAPE); //if the game is paused by the user
    if (key_esc & (0x8000 != 0)) {               //print a pause menu with it's possibility
        //to end game, resume or save.
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
            aoo = *birdsHeight; // saving game
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
    short gravity = GetAsyncKeyState(0x47); // possible gravity flip
    if (gravity & movementControl) {
        G = (G == 0) ? 1 : 0;
    }
    short mov = GetAsyncKeyState(VK_SPACE); // possible jump
    if (G == 0) { // change depends on current gravity mode
        if (mov & movementControl) {
            accCounter = 0; // acceleration counter reset to zero
            (*birdsHeight) -= 6; // sole jump
            if (*birdsHeight < 0) { // bird is out of bound
                *birdsHeight = 0;
            }
        } else {
            accCounter++; // if accCounter exceedes 15, than speed, by which bird's falling, increases
            if (accCounter > 15) {
                (*birdsHeight) += 2;
            } else {
                (*birdsHeight)++;
            }
            if (*birdsHeight > height) {
                *birdsHeight = height;
            }
        }
    } else { // fully analogical to the if statement
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
    short hack = GetAsyncKeyState(0x48); // random position set
    if (hack & movementControl) {
        *birdsHeight = rand() % height;
        accCounter = 0;
    }
}

int whereIsBird(int upperBound, int lowerBound, int hei) { // function checks, whether bird has exceeded bounds, used when not in the range of an obstacle
    if (hei == 0 || hei == height) {
        return -1; // bird's exceeded limits
    }
    if (hei <= upperBound) { // bird is level with upper obstacle horizontally
        return 1;
    } else if (hei <= lowerBound) { // bird is level with lower obstacle horizontally
        return 2;
    } else {
        return 3; // bird fits in the blank
    }
}

int whereIsBirdBonus(int upperBound, int lowerBound, int hei, int blank) { // like previous one, here one bonus condition to check, whether bird's fitting into the blank space
    if (hei == 0 || hei == height || hei <= upperBound || hei >= lowerBound) {
        return -1; // bird's exceeded limits (or hit the obstacle)
    }
    if (hei <= upperBound) {
        return 1;
    } else if (hei <= lowerBound) {
        return 2;
    } else {
        return 3;
    }
}


int game(); // declaration of the game function


int main() {
    int selected;
    int confirmed;

    selected = 0;
    confirmed = -1;
    while (true) {
        printMainMenu(selected); //funtionality to switching different options in menu through arrows
        while (confirmed == -1) {
            short key_up = GetAsyncKeyState(VK_UP);
            short key_down = GetAsyncKeyState(VK_DOWN);
            short key_enter = GetAsyncKeyState(VK_RETURN);
            if (key_up & (0x8000 != 0)) { //if the key pressed is up, the arrow in the list goes... up!
                if (selected == 0) selected = 4;
                selected--;
                printMainMenu(selected);
            } else if (key_down & (0x8000 != 0)) { //the arrow goes down...
                if (selected == 3) selected = -1;
                selected++;
                printMainMenu(selected);
            } else if (key_enter & (0x8000 != 0)) { //the user used Enter to confirm an option
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
            while (confirmed == -1) { //functionality to menu appearing after finishing the game
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
            if (confirmed == 0) {  //enter your nickname to get a place in the leaderboard
                system("cls");
                string name;
                cout << "FlappyBird Game\n\nEnter your nickname: ";
                cin >> name;
                addToLeaderBoard(name, score); //save it
                short key_enter = GetAsyncKeyState(VK_RETURN);
            } else if (confirmed == 1) { //start a new game
                confirmed = 0;
                continue;
            } else { //confirmed == 2, go back to the main menu
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
    string s[cols]; // array of spaces of each length, up to width of the screen
    s[0] = "";
    for (int i = 1; i < cols; i++) {
        s[i] = s[i - 1] + " ";
    }
    string c[cols + 1]; // like previous, but with hyphenes
    c[0] = "";
    for (int i = 1; i <= cols; i++) {
        c[i] = c[i - 1] + "-";
    }
    /* whole idea of printing the game is displaying lines (one after another) - then clearing the screen
    	there are five types of lines:
    	-> upper line with some informational stuff
    	-> lines consisting solely of "-" signs
    	-> lines in which the bird currently residents
    	-> lines between obstacles
    	-> lines level with obstacles
    	naturally, some lines belong to several types at time, for example 3. and 5. (or 3. and 4.)
    	displaying line of the first kind - each time same text - no problem
    	same with "-" signs
    	displaying lines level with obstacles:
    	(empty space - not each time occurs) (possibly a bird - if so, another empty space afterwards) (obstacle) (empty space up to the screen's boundary)
		displaying lines level with the obstacle's blank
		same as level with obstacles, but without obstacles
    */
    int counter = 0; // score
    int length = 4; // of an obstacle
    int lengthMinusOne = length - 1;
    int blank = 10; // blank space betweeen obstacles
    int boundOfLowerBounds = height - blank; // lowest possible blank index
    int xd = cols - length; // limit used while displaying
    string d = "Score: ";
    system("mode con:cols=96 lines=40"); // resize
    srand(time(NULL));
    int upperBound; // upper limit of blank space
    int lowerBound; // lower limit of blank space
    int lengthOfScore = 2; // maximum space score is allowed to take
    int currentBound = 10;
    int carry = cols - 10;
    int birdsHeight = height / 2; // initial bird's coordinate
    // a game is being repeated after fully executing each of three stages, after which a new obstacle occurs
    while (1) {
        int j;
        if (mode == 0) { // game is not just reloaded
            j = 0;
            upperBound = rand() % boundOfLowerBounds; // creating an obstacle
        } else { // is realoaded indeed, so we would just copy values from before the "save" usage
            if (coo == 1)
                j = doo;
            else
                j = length;
            birdsHeight = aoo;
            upperBound = boo;
            counter = eoo;
        }
        lowerBound = upperBound + blank;
        while (j < length) { // first part of each cycle, obstacle comes from behind the right boundary of the screen
            int sw = whereIsBird(upperBound, lowerBound, birdsHeight);
            if (j % 2 == 0)
                bHC(&birdsHeight, upperBound, 1, j, counter); // described earlier
            if (sw < 0) {
                return counter; // game ends
            }
            if (birdsHeight == -1) {
                G = 0;
                return -1;
            }
            // here begins game displaying
            // for detailed information see line 471
            cout << d << counter << " " << "|| press \"ESC\" to pause the game." << "\n"; // first line
            if (sw == 2) { // if the bird is between obstacle blocks (horizontally)
                for (int k = 1; k < 4; k++) { // some four lines consisting of hyphenes
                    cout << c[cols] << "\n";
                }
                int k = 0;
                for (; k < upperBound; k++) { // displaying lines up to upperBound
                    cout << s[cols - j] << c[j] << "\n";
                }
                for (; k < birdsHeight; k++) { // empty lines...
                    cout << "\n";
                }
                cout << s[lengthMinusOne] << "@\n"; // up to the bird
                for (; k < lowerBound; k++) {
                    cout << "\n";
                }
                for (; k < height; k++) { // like earlier
                    cout << s[cols - j] << c[j] << "\n";
                }
                for (k = 0; k < 4; k++) { // ending lines
                    cout << c[cols] << "\n";
                }
            } else if (sw == 1) { // like earlier, here the bird is level with upper obstacle (horizontally)...
                for (int k = 1; k < 4; k++) {
                    cout << c[cols] << "\n";
                }
                int k = 0;
                for (; k < birdsHeight; k++) {
                    cout << s[cols - j] << c[j] << "\n";
                }
                cout << s[lengthMinusOne] << "@" << s[xd - j] << c[j] << "\n"; // ... so it will be displayed along with it
                for (; k < upperBound; k++) { // the rest without any changes
                    cout << s[cols - j] << c[j] << "\n";
                }
                for (; k < lowerBound; k++) {
                    cout << "\n";
                }
                for (; k < height; k++) {
                    cout << s[cols - j] << c[j] << "\n";
                }
                for (k = 0; k < 4; k++) {
                    cout << c[cols] << "\n";
                }
            } else { // like earlier, but the bird is level with lower obstacle (horizontally)
                // for detailed information see line 471
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
                cout << s[lengthMinusOne] << "@\n" << s[xd - j] << c[j]; // here it comes
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
        if (mode == 0) // are we in the first cycle after resuming game?
            j = xd;
        else {
            if (coo == 3)
                j = length - 1;
            else
                j = xd;
        }

        while (j >= length) { // second part of each cycle, here an obstacle is already fully visible
            // but the bird is not already able to hit it
            // only difference (comparing to previos loop) occours by displaying full, not partitioned obstacle...
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
            if (sw == 2) { // between the obstacles
                // for detailed information see line 471
                cout << d << counter << " " << "|| press \"ESC\" to pause the game." << "\n";
                for (int k = 1; k < 4; k++) {
                    cout << c[cols] << "\n";
                }
                int k = 0;
                for (; k < upperBound; k++) {
                    cout << s[j] << c[length] << "\n"; // ... here
                }
                for (; k < birdsHeight; k++) {
                    cout << "\n";
                }
                cout << s[lengthMinusOne] << "@\n";
                for (; k < lowerBound; k++) {
                    cout << "\n";
                }
                for (; k < height; k++) {
                    cout << s[j] << c[length] << "\n"; // ... and here
                }
            } else if (sw == 1) { // like earlier
                // for detailed information see line 471
                // this time - bird's level with the upper one
                cout << d << counter << " " << "|| press \"ESC\" to pause the game." << "\n";
                for (int k = 1; k < 4; k++) {
                    cout << c[cols] << "\n";
                }
                int k = 0;
                for (; k < birdsHeight; k++) {
                    cout << s[j] << c[length] << "\n"; // once again - full obstacle's width (length) displayed
                }
                if (j >= length) {
                    cout << s[lengthMinusOne] << "@" << s[j - length] << c[length] << "\n"; // and here becomes displayed
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
            } else { // exactly like earlier, though ->
                // for detailed information see line 471
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
                for (; k < birdsHeight; k++) { // this time bird's level with the lower one
                    cout << s[j] << c[length] << "\n";
                }
                if (j >= length) {
                    cout << s[lengthMinusOne] << "@" << s[j - length] << c[length] << "\n"; // here comes the bird
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

        while (j >= 0) { // first part of the last part of the cycle
            // main difference occurs
            if (j % 2 == 0)
                bHC(&birdsHeight, upperBound, 2, j, counter);
            int sw = whereIsBirdBonus(upperBound, lowerBound, birdsHeight, blank); // <- here, as now we must check, whether bird's not fitted into the blank space
            if (sw < 0) { // actually it didn't
                return counter;
            }
            if (birdsHeight == -1) {
                G = 0;
                return -1;
            }
            if (sw == 2) { // explanation like in the previous occurences
                // for detailed information see line 471
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
                // for detailed information see line 471
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
                // for detailed information see line 471
                cout << d << counter << " " << "|| press \"ESC\" to pause the game." << "\n";
                for (int k = 1; k < 4; k++) {
                    cout << c[cols] << "\n";
                }
                int k = 0;
                for (; k < upperBound; k++) {
                    cout << s[j] << c[length] << "\n"; // once again - full obstacle display
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

        while (j >= 0) { // second part of the last part, obstacle comes before the bird
            // finally it is safe from being hitted
            // for detailed information see line 471
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
        mode = 0; // even if we have just resumed the game, it does not matter anymore
        if (counter == currentBound) {
            lengthOfScore--;
            currentBound *= 10;
        }
    }
}
