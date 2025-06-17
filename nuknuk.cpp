#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <ctime>
#include <math.h>
#include <unistd.h>
#include <termio.h>
#include <ncurses.h>
#include <string>
#include <cmath>
#include <locale.h>
#include <cstring>
#include <locale>
#include <set>
#include <random>  
#include <sstream>
#include <fstream>
#include <algorithm>

using namespace std;

// 기물 구조체: 종류(type)와 색(color)을 저장
struct Piece {
    char type;   // p(폰), r(룩), n(나이트), b(비숍), q(퀸), k(킹)
    char color;  // w(백), b(흑)
};

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

    init_color(15, 400, 320, 240);
    init_color(16, 1000, 1000, 1000);
    init_color(17, 500, 500, 500);
    for (int i = 1; i <= 17; i++) {
        init_pair(i, COLOR_BLACK, i);
    }
}
WINDOW* box(int y, int x, int height, int width, const char* value, int color) {
    WINDOW* win = newwin(height-1, width-2, y, x);

    wbkgd(win, COLOR_PAIR(color));

    int val_len = strlen(value);
    int start_y = (height - 1) / 2;               // 세로 중앙
    int start_x = ((width - 2) - val_len) / 2;    // 가로 중앙 (박스 너비에서 value 길이 빼고 절반)

    mvwprintw(win, start_y, start_x, "%s", value);
    
    wrefresh(win);
    return win;
}

int choiceGame() {
    keypad(stdscr, TRUE);

    int choice = 1;
    const char* items[4] = {"2048", "체스", "꼬들", "종료"};

    while (true) { 
        clear();
        refresh();
        for (int i = 0; i < 4; ++i) {
            int color = 13;
            if (i == choice - 1) {
            } else {
                color = 14;
            }
            box(2+i*5, 5, 5, 50, items[i], color);
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
    int sum = 0;
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
        clear(); // 화면 초기화
        refresh();
        int startY = 1;      // 시작 Y 좌표
        int startX = 2;      // 시작 X 좌표
        int boxHeight = 5;   // 각 상자의 세로 길이
        int boxWidth = 10;   // 각 상자의 가로 길이
        sum = 0;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                int y = startY + i * boxHeight;
                int x = startX + j * boxWidth;
                if(arr[i][j])
                    box(y, x, boxHeight, boxWidth, to_string(arr[i][j]).c_str(),log2(arr[i][j]) + 1);
                else
                    box(y, x, boxHeight, boxWidth, to_string(arr[i][j]).c_str(),1);
                sum += arr[i][j];
            }
        }
        box(1,45, 5, 20,to_string(sum).c_str(), 5);
        box(6,45, 5, 20,"q로 종료", 5);
    }
    

    void move(int input) {
        bool sw = false;
        switch (input) {
        case KEY_UP:
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
        case KEY_DOWN:
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
        case KEY_RIGHT:
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
        case KEY_LEFT:
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
    bool checkGameOver() {
        // 빈 칸이 있으면 아직 가능
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                if (arr[i][j] == 0)
                    return false;

        // 상하좌우 인접 숫자가 같으면 가능
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j) {
                if (i < 3 && arr[i][j] == arr[i + 1][j]) return false;
                if (j < 3 && arr[i][j] == arr[i][j + 1]) return false;
            }

        return true; // 더 이상 이동 불가
    }
public:
    void play() override {
        initscr();      // ncurses 초기화
        clear();        // 화면 지우기
        refresh();      // 화면 갱신
        echo();         // 사용자 입력 보이게 함
        spawnTile();
        while (true) {
            displayBoard();
            if (checkGameOver()) {
                mvprintw(22, 0, "💀 더 이상 움직일 수 없습니다. 게임 오버! (q로 종료)");
                refresh();
                char c = getch();
                if (c == 'q') break;
                continue;
            }

            int input = getch();
            if (input == 'q') break;
          
            move(input);
            
            
        }
        endwin();
    }
};

class Chess : public Game {
private:
    Piece board[8][8];                   // 체스판
    int current_turn;                    // 0: 백의 턴, 1: 흑의 턴
    pair<int, int> en_passant_target;    // 앙파상 대상 위치 (행, 열)
    bool onGame = true;
public:
    // 생성자: 초기 턴은 백, 앙파상 위치 초기화 후 보드 설정
    Chess() : current_turn(0), en_passant_target({ -1, -1 }) 
    {
        init_board();
    }

    // 보드 초기화: 기본 체스 시작 배치
    void init_board() 
    {
        // 빈 칸으로 초기화
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                board[i][j] = { ' ', ' ' };

        // 폰 배치
        for (int i = 0; i < 8; ++i) 
        {
            board[1][i] = { 'p', 'b' }; // 흑
            board[6][i] = { 'p', 'w' }; // 백
        }

        // 주요 기물 배치
        char types[8] = { 'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r' };
        for (int i = 0; i < 8; ++i) 
        {
            board[0][i] = { types[i], 'b' }; // 흑
            board[7][i] = { types[i], 'w' }; // 백
        }
    }

    // 현재 보드 상태 출력
    void print_board() const {
        auto piece_symbol = [](char type, char color) -> string 
        {
            switch (type) 
            {
            case 'p': return color == 'w' ? "♙" : "♟";
            case 'r': return color == 'w' ? "♖" : "♜";
            case 'n': return color == 'w' ? "♘" : "♞";
            case 'b': return color == 'w' ? "♗" : "♝";
            case 'q': return color == 'w' ? "♕" : "♛";
            case 'k': return color == 'w' ? "♔" : "♚";

            default: return " ";
            }
        };
        const char* row[8] = {"a","b","c","d","e","f","g","h"};
        const char* col[8] = {"1","2","3","4","5","6","7","8"};
        box(2, 2, 3, 9, "  ", 17);
        for(int i = 0; i < 8; i++)
        {
            box(2, 11+i*9, 3, 9, row[i], 17);
        }
        box(2, 83, 3, 9, "  ", 17);
        for (int i = 0; i < 8; ++i) 
        {
            box(5+i*3, 2, 3, 9, col[7-i], 17);
            for (int j = 0; j < 8; ++j) 
            {
                Piece p = board[i][j];
                if((i+j)%2 == 0)
                {
                    box(5+i*3, 11+j*9, 3, 9, piece_symbol(p.type, p.color).c_str(), 16);
                }else
                {
                    box(5+i*3, 11+j*9, 3, 9, piece_symbol(p.type, p.color).c_str(), 15);
                }
            }
            box(5+i*3, 83, 3, 9, col[7-i], 17);
        }
        box(29, 2, 3, 9, "  ", 17);
        for(int i = 0; i < 8; i++)
        {
            box(29, 11+i*9, 3, 9, row[i], 17);
        }
        box(29, 83, 3, 9, "  ", 17);
        /*cout << "\n    a   b   c   d   e   f   g   h\n";
        cout << "  +---+---+---+---+---+---+---+---+\n";
        for (int i = 0; i < 8; ++i) 
        {
            cout << 8 - i << " |";
            for (int j = 0; j < 8; ++j) 
            {
                Piece p = board[i][j];
                cout << " " << piece_symbol(p.type, p.color) << " |";
            }
            cout << " " << 8 - i << "\n";
            cout << "  +---+---+---+---+---+---+---+---+\n";
        }
        cout << "    a   b   c   d   e   f   g   h\n\n";*/
    }

    // 이동 경로가 비어있는지 확인 (룩, 비숍, 퀸용)
    bool is_path_clear(int from_row, int from_col, int to_row, int to_col) 
    {
        int d_row = (to_row - from_row != 0) ? (to_row - from_row) / abs(to_row - from_row) : 0;
        int d_col = (to_col - from_col != 0) ? (to_col - from_col) / abs(to_col - from_col) : 0;
        int r = from_row + d_row, c = from_col + d_col;
        while (r != to_row || c != to_col) 
        {
            if (board[r][c].type != ' ')
                return false;
            r += d_row;
            c += d_col;
        }
        return true;
    }

    // 특정 기물이 특정 위치로 이동 가능한지 확인
    bool is_valid_move(const Piece& p, int from_row, int from_col, int to_row, int to_col, bool is_real = true) 
    {
        int d_row = to_row - from_row, d_col = to_col - from_col;
        Piece target = board[to_row][to_col];
        if (target.type != ' ' && target.color == p.color)
            return false;

        switch (p.type) 
        {
        case 'p': 
        { // 폰
            int dir = (p.color == 'w') ? -1 : 1;
            int start_row = (p.color == 'w') ? 6 : 1;
            if (d_col == 0 && target.type == ' ') 
            {
                if (d_row == dir) return true;
                if (from_row == start_row && d_row == 2 * dir &&
                    board[from_row + dir][from_col].type == ' ') return true;
            }
            else if (abs(d_col) == 1 && d_row == dir) 
            {
                if (target.type != ' ' && target.color != p.color) return true;
                if (make_pair(to_row, to_col) == en_passant_target) return true; // 앙파상
            }
            return false;
        }
        case 'r': // 룩
            return (d_row == 0 || d_col == 0) && is_path_clear(from_row, from_col, to_row, to_col);
        case 'b': // 비숍
            return abs(d_row) == abs(d_col) && is_path_clear(from_row, from_col, to_row, to_col);
        case 'q': // 퀸
            return ((d_row == 0 || d_col == 0) || (abs(d_row) == abs(d_col))) &&
                is_path_clear(from_row, from_col, to_row, to_col);
        case 'k': // 킹
            if (abs(d_row) <= 1 && abs(d_col) <= 1) return true;
            // 캐슬링
            if (is_real && d_row == 0 && abs(d_col) == 2) 
            {
                int rook_col = (d_col > 0) ? 7 : 0;
                Piece rook = board[from_row][rook_col];
                if (rook.type == 'r' && rook.color == p.color) 
                {
                    if (is_path_clear(from_row, from_col, from_row, rook_col) &&
                        !is_square_attacked(from_row, from_col, opposite_color(p.color)) &&
                        !is_square_attacked(from_row, from_col + d_col / 2, opposite_color(p.color)) &&
                        !is_square_attacked(from_row, to_col, opposite_color(p.color))) 
                        {
                        return true;
                    }
                }
            }
            return false;
        case 'n': // 나이트
            return (abs(d_row) == 2 && abs(d_col) == 1) || (abs(d_row) == 1 && abs(d_col) == 2);
        default:
            return false;
        }
    }

    // 특정 색의 킹 위치 찾기
    pair<int, int> find_king(char color) 
    {
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                if (board[i][j].type == 'k' && board[i][j].color == color)
                    return { i, j };
        return { -1, -1 };
    }

    // 상대 색 리턴 ('w' → 'b', 'b' → 'w')
    char opposite_color(char color) 
    {
        return color == 'w' ? 'b' : 'w';
    }

    // 주어진 위치가 상대 기물에 의해 공격받고 있는지 확인
    bool is_square_attacked(int row, int col, char by_color) 
    {
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j) 
            {
                Piece p = board[i][j];
                if (p.type != ' ' && p.color == by_color)
                    if (is_valid_move(p, i, j, row, col, false))
                        return true;
            }
        return false;
    }

    // 특정 색의 킹이 체크 상태인지 확인
    bool is_in_check(char color) 
    {
        pair<int, int> king = find_king(color);
        if (king.first == -1) return false;
        return is_square_attacked(king.first, king.second, opposite_color(color));
    }

    // 해당 색이 둘 수 있는 합법적인 수가 하나라도 있는지 확인
    bool has_any_valid_move(char color) 
    {
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j) {
                Piece p = board[i][j];
                if (p.type != ' ' && p.color == color) 
                {
                    for (int r = 0; r < 8; ++r)
                        for (int c = 0; c < 8; ++c)
                            if (is_valid_move(p, i, j, r, c)) 
                            {
                                // 시뮬레이션 실행
                                Piece from = board[i][j], to = board[r][c];
                                board[r][c] = from;
                                board[i][j] = { ' ', ' ' };
                                bool still_in_check = is_in_check(color);
                                board[i][j] = from;
                                board[r][c] = to;
                                if (!still_in_check)
                                    return true;
                            }
                }
            }
        return false;
    }

    // 실제 기물 이동 수행
    bool move_piece(const string& move) 
    {
        initscr();
        if (move.length() != 4) 
        {
            cout << "잘못된 입력입니다! (예: e2e4)                          \n";
            return false;
        }

        // 좌표 변환
        int from_col = move[0] - 'a', from_row = 8 - (move[1] - '0');
        int to_col = move[2] - 'a', to_row = 8 - (move[3] - '0');

        if (from_row < 0 || from_row > 7 || to_row < 0 || to_row > 7 ||
            from_col < 0 || from_col > 7 || to_col < 0 || to_col > 7) 
            {
            cout << "잘못된 위치입니다!                                     \n";
            return false;
        }

        Piece p = board[from_row][from_col];
        if (p.type == ' ') 
        {
            cout << "빈 칸입니다!                                           \n";
            return false;
        }

        if ((current_turn == 0 && p.color != 'w') ||
            (current_turn == 1 && p.color != 'b')) 
            {
            cout << "잘못된 턴입니다!                                        \n";
            return false;
        }

        if (!is_valid_move(p, from_row, from_col, to_row, to_col)) 
        {
            cout << "허용되지 않은 이동입니다!                                  \n";
            return false;
        }

        Piece captured = board[to_row][to_col];
        board[to_row][to_col] = p;
        board[from_row][from_col] = { ' ', ' ' };

        // 앙파상 처리
        if (p.type == 'p' && to_col != from_col && captured.type == ' ') 
        {
            board[from_row][to_col] = { ' ', ' ' };
        }

        // 캐슬링 시 룩 이동
        if (p.type == 'k' && abs(to_col - from_col) == 2) 
        {
            int rook_from = (to_col > from_col) ? 7 : 0;
            int rook_to = (to_col + from_col) / 2;
            board[to_row][rook_to] = board[to_row][rook_from];
            board[to_row][rook_from] = { ' ', ' ' };
        }

        // 체크 상태 여부 확인
        if (is_in_check(p.color)) 
        {
            cout << "자기 킹이 체크에 빠집니다!                             \n";
            board[from_row][from_col] = p;
            board[to_row][to_col] = captured;
            return false;
        }

        // 폰 승격
        if (p.type == 'p' && (to_row == 0 || to_row == 7)) 
        {
            char choice;
            do 
            {
                cout << "승격할 기물을 선택하세요 (q, r, b, n):                         ";
                cin >> choice;
            } while (choice != 'q' && choice != 'r' && choice != 'b' && choice != 'n');
            board[to_row][to_col].type = choice;
        }

        // 앙파상 대상 위치 업데이트
        en_passant_target = { -1, -1 };
        if (p.type == 'p' && abs(to_row - from_row) == 2)
            en_passant_target = { (to_row + from_row) / 2, to_col };

        // 턴 전환
        current_turn = 1 - current_turn;
        char next_color = current_turn == 0 ? 'w' : 'b';

        // 체크/체크메이트/스테일메이트 확인
        if (is_in_check(next_color)) 
        {
            if (!has_any_valid_move(next_color)) 
            {
                //print_board();
                cout << (next_color == 'w' ? "백" : "흑") << "이 체크메이트로 패배했습니다!                         \n";
                onGame = false;
            }
            else 
            {
                cout << "체크!                                                      \n";
            }
        }
        else if (!has_any_valid_move(next_color)) 
        {
            //print_board();
            cout << "스테일메이트! 무승부입니다.                                        \n";
            onGame = false;
        }

        return true;
    }

    // 게임 루프 실행
    void play() override 
    {
    initscr();      // ncurses 초기화
    clear();        // 화면 지우기
    refresh();      // 화면 갱신
    echo();         // 사용자 입력 보이게 함

    char buffer[100];
    string input;

    while (true) 
    {
        print_board();  // 보드 그리기 함수 (ncurses 기반이어야 함)
        move(35, 0);     // 커서 이동
        if(!onGame){
            getch();
            break;
        }
        clrtoeol();      // 기존 줄 내용 지우기
        if (current_turn == 0) 
        {
            mvprintw(35, 0, "백의 턴입니다. (예: e2e4): ");
        } else 
        {
            mvprintw(35, 0, "흑의 턴입니다. (예: e2e4): ");
        }

        move(35, 27);     // 입력 위치로 이동
        getstr(buffer);   // scanw보다 안정적인 방법
        input = buffer;

        if (input == "exit") break;
        if (!move_piece(input)) continue;
    }

    endwin(); // ncurses 종료
}


};

class Wordle : public Game {
private:
    vector<vector<wstring>> word_list;
    vector<wstring> today_word;
    set<vector<wstring>> word_set;

    void load_word_list(const string& filename) {
        wifstream file(filename);
        file.imbue(locale(""));  
        if (!file) {
            wcout << L"파일을 열 수 없습니다: " << filename.c_str() << L'\n';
            return;
        }

        wstring line;
        while (getline(file, line)) {
            if (!line.empty()) {
                vector<wstring> word;
                wstringstream wss(line);
                wstring jamo;
                while (wss >> jamo) {
                    word.push_back(jamo);
                }
                if (word.size() == 6) {
                    word_list.push_back(word);
                    word_set.insert(word);
                }
            }
        }
        file.close();
    }

    void choose_random_word() {
        if (word_list.empty()) {
            today_word.clear();
            return;
        }
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, word_list.size() - 1);
        today_word = word_list[dis(gen)];
    }

    bool is_valid_word(const vector<wstring>& guess) {
        return word_set.find(guess) != word_set.end();
    }

    void print_feedback(const vector<wstring>& guess) {
        for (int i = 0; i < 6; ++i) {
            if (guess[i] == today_word[i]) {
                wcout << L"\033[32mO(" << guess[i] << L") \033[0m";  // 초록색
            }
            else if (find(today_word.begin(), today_word.end(), guess[i]) != today_word.end()) {
                wcout << L"\033[33m△(" << guess[i] << L") \033[0m";  // 노란색
            }
            else {
                wcout << L"\033[90mX(" << guess[i] << L") \033[0m";  // 회색
            }
        }
        wcout << L'\n';
    }

public:
    void play() override {
        locale::global(locale(""));
        wcin.imbue(locale());
        wcout.imbue(locale());
        load_word_list("wordlist_jamo.txt");
        choose_random_word();
        write (1, "\033[1;1H\033[2J", 10); 
        vector<wstring> guess(6);
        int max_attempts = 5;

        for (int attempt = 1; attempt <= max_attempts; ++attempt) {
            while (true) {
                wcout << L"\n[" << attempt << L"/" << max_attempts << L"] 자모 6개를 공백으로 구분해 입력하세요 (종료하려면 'exit'): ";
                bool user_wants_exit = false;

                for (int i = 0; i < 6; ++i) {
                    wstring input;
                    wcin >> input;

                    if (input == L"exit") {
                        user_wants_exit = true;
                        break;
                    }

                    guess[i] = input;
                }

                if (user_wants_exit) {
                    wcout << L"게임을 종료합니다.\n";
                    return; // 또는 break;를 사용해 바깥 for문만 종료하게 할 수도 있음
                }

                if (!is_valid_word(guess)) {
                    wcout << L"단어 목록에 없는 단어입니다. 다시 입력하세요.\n";
                    continue;
                }
                break;
            }

            if (guess == today_word) {
                wcout << L"\n정답입니다!\n";
                getch();
                return;
            }

            print_feedback(guess);
            wcout << L"남은 기회: " << (max_attempts - attempt) << L"\n";
        }


        wcout << L"\n실패! 정답은: ";
        for (const wstring& j : today_word) wcout << j;
        wcout << L"\n";
        getch();
    }
};


int main() {
    srand(static_cast<unsigned>(time(0)));
    setlocale(LC_ALL, "");
    initscr();                // 한 번만 초기화
    start_color();            // 컬러 초기화도 한 번만
    setColor();               // 색상 설정도 한 번만
    curs_set(0);              // 커서 숨김
    keypad(stdscr, TRUE);
    noecho();
    locale::global(std::locale(""));
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

