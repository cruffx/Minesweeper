#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

const int BOARD_SIZE = 10;
const int NUM_MINES = 10;

// Определение клетки на доске
struct Cell {
    int row;
    int col;
};
// Определение доски
class Board {
public:
    Board();
    void display();
    void reveal(Cell cell);
    bool isMine(Cell cell);
    bool isRevealed(Cell cell);
    bool isGameOver();
private:
    int numMinesAround(Cell cell);
    bool isValidCell(Cell cell);
    bool isAdjacent(Cell c1, Cell c2);
    vector<vector<int>> board_;
    vector<vector<bool>> revealed_;
    bool game_over_;
};

// Конструктор доски
Board::Board() {
    // Создание пустой доски
    board_ = vector<vector<int>>(BOARD_SIZE, vector<int>(BOARD_SIZE, 0));
    revealed_ = vector<vector<bool>>(BOARD_SIZE, vector<bool>(BOARD_SIZE, false));
    game_over_ = false;
    // Размещение мин на доске
    int num_mines = 0;
    while (num_mines < NUM_MINES) {
        int row = rand() % BOARD_SIZE;
        int col = rand() % BOARD_SIZE;
        if (board_[row][col] != -1) {
            board_[row][col] = -1;
            num_mines++;
        }
    }
}

// Отображение доски
void Board::display() {
    cout << " ";
    for (int col = 0; col < BOARD_SIZE; col++) {
        cout << " " << col;
    }
    cout << endl;
    for (int row = 0; row < BOARD_SIZE; row++) {
        cout << row << " ";
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (!revealed_[row][col]) {
                cout << ".";
            }
            else if (board_[row][col] == -1) {
                cout << "*";
            }
            else {
                cout << numMinesAround(Cell{ row, col });
            }
            cout << " ";
        }
        cout << endl;
    }
}

// Открытие клетки
void Board::reveal(Cell cell) {
    if (game_over_) {
        return;
    }
    if (!isValidCell(cell)) {
        return;
    }
    if (revealed_[cell.row][cell.col]) {
        return;
    }
    revealed_[cell.row][cell.col] = true;
    if (isMine(cell)) {
        game_over_ = true;
        cout << "You lose!" << endl;
    }
    else if (numMinesAround(cell) == 0) {
        vector<Cell> neighbors;
        for (int row_offset = -1; row_offset <= 1; row_offset++) {
            for (int col_offset = -1; col_offset <= 1; col_offset++) {
                Cell neighbor = { cell.row + row_offset, cell.col + col_offset };
                if (isValidCell(neighbor) && !isMine(neighbor)) {
                    neighbors.push_back(neighbor);
                }
            }
        }
        for (Cell neighbor : neighbors) {
            reveal(neighbor);
        }
    }
}

// Проверка, является ли клетка миной
bool Board::isMine(Cell cell) {
    if (!isValidCell(cell)) {
        return false;
    }
    return board_[cell.row][cell.col] == -1;
}

// Проверка, была ли клетка открыта
bool Board::isRevealed(Cell cell) {
    if (!isValidCell(cell)) {
        return false;
    }
    return revealed_[cell.row][cell.col];
}

// Проверка, окончена ли игра
bool Board::isGameOver() {
    return game_over_;
}

// Подсчет количества мин вокруг клетки
int Board::numMinesAround(Cell cell) {
    if (!isValidCell(cell)) {
        return 0;
    }
    int num_mines = 0;
    for (int row_offset = -1; row_offset <= 1; row_offset++) {
        for (int col_offset = -1; col_offset <= 1; col_offset++) {
            Cell neighbor = { cell.row + row_offset, cell.col + col_offset };
            if (isMine(neighbor)) {
                num_mines++;
            }
        }
    }
    return num_mines;
}

// Проверка, является ли клетка допустимой на доске
bool Board::isValidCell(Cell cell) {
    return cell.row >= 0 && cell.row < BOARD_SIZE&& cell.col >= 0 && cell.col < BOARD_SIZE;
}

// Проверка, являются ли две клетки соседними
bool Board::isAdjacent(Cell c1, Cell c2) {
    return abs(c1.row - c2.row) <= 1 && abs(c1.col - c2.col) <= 1;
}

// Функция main для игры
int main() {
    // Инициализация генератора случайных чисел
    srand(time(nullptr));
    // Создание доски
    Board board;
    // Игровой цикл
    while (!board.isGameOver()) {
        // Отображение доски
        board.display();
        // Ввод клетки
        int row, col;
        cout << "Enter row and column: ";
        cin >> row >> col;
        Cell cell = { row, col };
        // Открытие клетки
        board.reveal(cell);
    }
    // Отображение окончательной доски
    board.display();
    return 0;
}
