#include <iostream>
#include <cstdlib>
#include <ctime>

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
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                cout << arr[i][j] << "\t";
            }
            cout << '\n';
        }
    }

    void move(char input) {
        switch (input) {
        case 'u':
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    for (int k = j + 1; k < 4; k++) {
                        if (arr[j][i] == arr[k][i]) {
                            arr[j][i] *= 2;
                            arr[k][i] = 0;
                            break;
                        }
                    }
                }
                for (int j = 0; j < 4; j++) {
                    for (int k = j + 1; k < 4; k++) {
                        if (!arr[j][i] && arr[k][i]) {
                            arr[j][i] = arr[k][i];
                            arr[k][i] = 0;
                            break;
                        }
                    }
                }
            }
            break;
        case 'd':
            for (int i = 3; i >= 0; i--) {
                for (int j = 3; j >= 0; j--) {
                    for (int k = j - 1; k >= 0; k--) {
                        if (arr[j][i] == arr[k][i]) {
                            arr[j][i] *= 2;
                            arr[k][i] = 0;
                            break;
                        }
                    }
                }
                for (int j = 3; j >= 0; j--) {
                    for (int k = j - 1; k >= 0; k--) {
                        if (!arr[j][i] && arr[k][i]) {
                            arr[j][i] = arr[k][i];
                            arr[k][i] = 0;
                            break;
                        }
                    }
                }
            }
            break;
        case 'r':
            for (int i = 3; i >= 0; i--) {
                for (int j = 3; j >= 0; j--) {
                    for (int k = j - 1; k >= 0; k--) {
                        if (arr[i][j] == arr[i][k]) {
                            arr[i][j] *= 2;
                            arr[i][k] = 0;
                            break;
                        }
                    }
                }
                for (int j = 3; j >= 0; j--) {
                    for (int k = j - 1; k >= 0; k--) {
                        if (!arr[i][j] && arr[i][k]) {
                            arr[i][j] = arr[i][k];
                            arr[i][k] = 0;
                            break;
                        }
                    }
                }
            }
            break;
        case 'l':
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    for (int k = j + 1; k < 4; k++) {
                        if (arr[i][j] == arr[i][k]) {
                            arr[i][j] *= 2;
                            arr[i][k] = 0;
                            break;
                        }
                    }
                }
                for (int j = 0; j < 4; j++) {
                    for (int k = j + 1; k < 4; k++) {
                        if (!arr[i][j] && arr[i][k]) {
                            arr[i][j] = arr[i][k];
                            arr[i][k] = 0;
                            break;
                        }
                    }
                }
            }
            break;
        }
    }

public:
    void play() override {
        while (true) {
            spawnTile();
            displayBoard();
            char input;
            cin >> input;
            if (input == 'q') break;
            move(input);
            system("clear");
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