#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <ctime>
#include <math.h>
#include <unistd.h>
#include <termio.h>
#include <ncurses.h>
using namespace std;

    void setColor(){
        init_color(1, 1000, 1000, 1000);   // 하양
        init_color(2, 900, 950, 1000);     // 하양 + 살짝 파랑
        init_color(3, 800, 900, 1000);     // 연한 하늘색
        init_color(4, 700, 850, 1000);     // 하늘색

        init_color(5, 600, 800, 1000);     // 밝은 하늘 → 푸른빛
        init_color(6, 500, 700, 1000);     // 연한 파랑
        init_color(7, 400, 600, 950);      // 파랑 증가
        init_color(8, 300, 500, 900);      // 파랑

        init_color(9, 200, 400, 850);      // 중간 파랑
        init_color(10, 100, 300, 800);     // 진해지는 파랑
        init_color(11, 50, 150, 700);      // 어두운 파랑
        init_color(12, 0, 0, 600);         // 어두운 짙은 파랑
        init_color(13, 320, 760, 880);    // 어두운 회색
        init_color(14, 1000, 1000, 1000);

        for (int i = 1; i <= 14; i++) {
            init_pair(i, COLOR_BLACK, i);
        }
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
WINDOW* box(int y, int x, int height, int width, const char* value, bool sw) {
    WINDOW* win = newwin(height-1, width-2, y, x);

    if (sw) {
        wbkgd(win, COLOR_PAIR(13));
        mvwprintw(win, height / 2, (width - 4) / 2, "%4s", value);
    }else{
        wbkgd(win, COLOR_PAIR(14));
        mvwprintw(win, height / 2, (width - 4) / 2, "%4s", value);
    }
    wrefresh(win);
    return win;
}
int choiceGame() {
    keypad(stdscr, TRUE);

    int choice = 1;
    const char* items[4] = {"2048", "Chess", "Kordle", "Exit"};

    while (true) {
        clear();
        refresh();
        for (int i = 0; i < 4; ++i) {
            bool sw = true;
            if (i == choice - 1) {
            } else {
                sw = false;
            }
            box(2+i*5, 5, 5, 50, items[i], sw);
        }

        int input = getch();
        if (input == KEY_UP && choice > 1) {
            choice--;
        } else if (input == KEY_DOWN && choice < 4) {
            choice++;
        } else if (input == '\n') {
            endwin();
            return choice;
        }
    }
}
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
        refresh();
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

public:
    void play() override {
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

    initscr();                // 한 번만 초기화
    start_color();            // 컬러 초기화도 한 번만
    setColor();               // 색상 설정도 한 번만
    curs_set(0);              // 커서 숨김
    keypad(stdscr, TRUE);
    noecho();

    while (true) {
        int choice = choiceGame();  // 선택 UI 실행
        Game* game = nullptr;

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
        case 4:
            endwin(); // 프로그램 종료 시 종료
            return 0;
        }

        game->play();         // 게임 실행 (endwin 제거)
        delete game;
    }

    endwin();  // 안전하게 추가
    return 0;
}

