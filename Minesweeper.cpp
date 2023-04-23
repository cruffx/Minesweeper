#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

const int BOARD_SIZE = 10;
const int NUM_MINES = 10;

// Definition of a cell on the board
struct Cell {
    int row;
    int col;
};
// Board Definition
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

// Board constructor
Board::Board() {
    // Create an empty board
    board_ = vector<vector<int>>(BOARD_SIZE, vector<int>(BOARD_SIZE, 0));
    revealed_ = vector<vector<bool>>(BOARD_SIZE, vector<bool>(BOARD_SIZE, false));
    game_over_ = false;
    // Placement of mines on the board
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

// Board display
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

// Cell opening
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

// Checking if a cell is a mine
bool Board::isMine(Cell cell) {
    if (!isValidCell(cell)) {
        return false;
    }
    return board_[cell.row][cell.col] == -1;
}

// Checking if the cage has been opened
bool Board::isRevealed(Cell cell) {
    if (!isValidCell(cell)) {
        return false;
    }
    return revealed_[cell.row][cell.col];
}

// Checking if the game is over
bool Board::isGameOver() {
    return game_over_;
}

// Counting the number of mines around the cell
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

// Checking if a cell is valid on the board
bool Board::isValidCell(Cell cell) {
    return cell.row >= 0 && cell.row < BOARD_SIZE&& cell.col >= 0 && cell.col < BOARD_SIZE;
}

// Checking if two cells are adjacent
bool Board::isAdjacent(Cell c1, Cell c2) {
    return abs(c1.row - c2.row) <= 1 && abs(c1.col - c2.col) <= 1;
}

// The main function for the game
int main() {
    // Initializing the random number generator
    srand(time(nullptr));
    // Creating a board
    Board board;
    // Игровой цикл
    while (!board.isGameOver()) {
        // Board display
        board.display();
        // Cell entry
        int row, col;
        cout << "Enter row and column: ";
        cin >> row >> col;
        Cell cell = { row, col };
        // Cell opening
        board.reveal(cell);
    }
    // Displaying the final board
    board.display();
    return 0;
}
