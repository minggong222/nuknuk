#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <termio.h>
using namespace std;

class Game {
public:
    virtual void play() = 0;
};

class Game2048 : public Game {
private:
    int arr[4][4] = {};

    void spawnTile() {
        while (true) {
            int x = rand() % 4;
            int y = rand() % 4;
            if (!arr[x][y]) {
                arr[x][y] = 2;
                break;
            }
        }
    }

    void displayBoard() {
        system("clear");
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                cout << arr[i][j] << "\t";
            }
            cout << '\n';
        }
        cout << "q : 종료\n";
    }

    void move(char input) {
        bool sw = false;
        switch (input) {
        case 'A':
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    if(!arr[j][i])  continue;
                    
                    for (int k = j + 1; k < 4; k++) {
                        if (arr[j][i] == arr[k][i]) {
                            arr[j][i] *= 2;
                            arr[k][i] = 0;
                            sw = true;
                            break;
                        }
                    }
                }
                for (int j = 0; j < 4; j++) {
                    
                    for (int k = j + 1; k < 4; k++) {
                        if (!arr[j][i] && arr[k][i]) {
                            arr[j][i] = arr[k][i];
                            arr[k][i] = 0;
                            sw = true;
                            break;
                        }
                    }
                }
            }
            break;
        case 'B':
            for (int i = 3; i >= 0; i--) {
                for (int j = 3; j >= 0; j--) {
                    if(!arr[j][i])  continue;
                    
                    for (int k = j - 1; k >= 0; k--) {
                        if (arr[j][i] == arr[k][i]) {
                            arr[j][i] *= 2;
                            arr[k][i] = 0;
                            sw = true;
                            break;
                        }
                    }
                }
                for (int j = 3; j >= 0; j--) {
                    
                    for (int k = j - 1; k >= 0; k--) {
                        if (!arr[j][i] && arr[k][i]) {
                            arr[j][i] = arr[k][i];
                            arr[k][i] = 0;
                            sw = true;
                            break;
                        }
                    }
                }
            }
            break;
        case 'C':
            for (int i = 3; i >= 0; i--) {
                for (int j = 3; j >= 0; j--) {
                    if(!arr[i][j])  continue;
                    
                    for (int k = j - 1; k >= 0; k--) {
                        if (arr[i][j] == arr[i][k]) {
                            arr[i][j] *= 2;
                            arr[i][k] = 0;
                            sw = true;
                            break;
                        }
                    }
                }
                for (int j = 3; j >= 0; j--) {
                    
                    for (int k = j - 1; k >= 0; k--) {
                        if (!arr[i][j] && arr[i][k]) {
                            arr[i][j] = arr[i][k];
                            arr[i][k] = 0;
                            sw = true;
                            break;
                        }
                    }
                }
            }
            break;
        case 'D':
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    if(!arr[i][j])  continue;
                    
                    for (int k = j + 1; k < 4; k++) {
                        if (arr[i][j] == arr[i][k]) {
                            arr[i][j] *= 2;
                            arr[i][k] = 0;
                            sw = true;
                            break;
                        }
                    }
                }
                for (int j = 0; j < 4; j++) {
                    
                    for (int k = j + 1; k < 4; k++) {
                        if (!arr[i][j] && arr[i][k]) {
                            arr[i][j] = arr[i][k];
                            arr[i][k] = 0;
                            sw = true;
                            break;
                        }
                    }
                }
            }
            break;
        }
        if(sw)
            spawnTile();
        return;
    }
    int getch(void){
        int ch;

        struct termios buf;
        struct termios current;

        tcgetattr(0, &buf);

        current = buf;
        buf.c_lflag &= ~ICANON;    // non-canonical input 설정
	    buf.c_lflag &= ~ECHO; 

        tcsetattr(0, TCSANOW, &current);
        ch = getchar();
        tcsetattr(0, TCSANOW, &buf);

        return ch;
    }

public:
    void play() override {
        spawnTile();
        while (true) {
            displayBoard();
            char input;
            
            input = getch();
            if (input == 'q') break;
            if (input == 27) { // ESC
                input = getch();
                if (input == 91) {
                    input= getch();
                    move(input);
                }
            }
        }
    }
};

class Chess : public Game {
public:
    void play() override {
        // 재욱이 이 함수가 메인이야
    }
};

class Wordle : public Game {
public:
    void play() override {
        //스더 이 함수가 메인이야

        cout <<"hello";
    }
};

int main() {
    srand(static_cast<unsigned>(time(0)));
    Game* game = nullptr;
    int choice;
    while (1) {
        cout << "게임 선택\n1. 2048\n2. 체스\n3. 꼬들\n";
        cin >> choice;
        switch (choice) {
        case 1:
            game = new Game2048();
            break;
        case 2:
            game = new Chess();
            break;
        case 3:
            game = new Wordle();
            break;
        default:
            cout << "다시 선택해주세요." << endl;
            break;
        }

        game->play();
        delete game;
    }
    return 0;
}