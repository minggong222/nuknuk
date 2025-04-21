#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <ctime>
#include <math.h>
#include <unistd.h>
#include <termio.h>
#include <ncurses.h>
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
    WINDOW* createBox(int y, int x, int height, int width, int value) {
        WINDOW* win = newwin(height-1, width-2, y, x);

        if (value != 0) {
            wbkgd(win, COLOR_PAIR(log2(value) + 1)); 
            mvwprintw(win, height / 2, (width - 4) / 2, "%4d", value);
        }else{
            wbkgd(win, COLOR_PAIR(1)); 
        }
        wrefresh(win);
        return win;
    }
    
    void displayBoard() {
        clear(); // 화면 초기화
        int startY = 1;      // 시작 Y 좌표
        int startX = 2;      // 시작 X 좌표
        int boxHeight = 5;   // 각 상자의 세로 길이
        int boxWidth = 10;   // 각 상자의 가로 길이
    
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                int y = startY + i * boxHeight;
                int x = startX + j * boxWidth;
                createBox(y, x, boxHeight, boxWidth, arr[i][j]);
            }
        }
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
    int getc(void){
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
    void setColor(){
        init_color(12, 250, 250, 250);    // 어두운 회색
        init_color(11, 300, 300, 300);    // 어두운 회색
        init_color(10, 350, 350, 350);    // 어두운 회색
        init_color(9, 400, 400, 400);    // 어두운 회색
        init_color(8, 450, 450, 450);    // 어두운 회색
        init_color(7, 500, 500, 500);    // 어두운 회색
        init_color(6, 550, 550, 550);    // 어두운 회색
        init_color(5, 600, 600, 600);    // 어두운 회색
        init_color(4, 650, 650, 650);    // 어두운 회색
        init_color(3, 700, 700, 700);    // 어두운 회색
        init_color(2, 750, 750, 750);    // 어두운 회색
        init_color(1, 800, 800, 800);    // 어두운 회색

        for (int i = 1; i <= 12; i++) {
            init_pair(i, COLOR_BLACK, i);
        }
    }
public:
    void play() override {
        initscr();
        start_color();
        curs_set(0);
        setColor();
        spawnTile();
        while (true) {
            displayBoard();
            char input;
            
            input = getc();
            if (input == 'q') break;
            if (input == 27) { // ESC
                input = getc();
                if (input == 91) {
                    input= getc();
                    move(input);
                }
            }
        }
        endwin();
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