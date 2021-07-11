#include <string>
#include <iostream>
#include <queue>
#include "DrawBoard.h"

const int MAX_START_POS = 8;

struct XYJ {
    int x, y, j;
    XYJ(int _x, int _y, int _j) : x(_x), y(_y), j(_j) {}
};

class Board {
  public:
    int boardWidth = 8;
    int boardHeight = 8;
    int ** tiles;

    Board(int width = 8, int height = 8) {
        boardWidth = width;
        boardHeight = height;
        tiles = new int * [boardWidth];
        for (int i = 0; i < boardWidth; ++i) {
            tiles[i] = new int[boardHeight];
            for (int j = 0; j < boardHeight; ++j) {
                tiles[i][j]=0;
            }
        }
    }

    void printBoard() {
        for (int i = 0; i < boardWidth; i++) {
            for (int j = 0; j < boardHeight; j++) {
                std::cout << tiles[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
};

class Knight {
  public:
    Board board;
    std::queue<XYJ> moveQueue;

    Knight(int width = 8, int height = 8, int ** startingPositions = NULL) {
        board = Board(width, height);
        if (startingPositions == NULL || startingPositions[0][0] == -1) {
            moveQueue.push(XYJ(7, 1, 1));
            moveQueue.push(XYJ(7, 6, 1));
        } else {
            for (int i = 0; i < MAX_START_POS; i++) {
                if (startingPositions[i][0] < 0 || startingPositions[i][1] < 0) {
                    break;
                }
                moveQueue.push(XYJ(startingPositions[i][0], startingPositions[i][1], 1));
            }
        }
    }

    void knightMoves(int Kx, int Ky, int jumpNum = 1) {
        if (jumpNum == 1) {
            //std::cout << "knightMoves- Kx: " << Kx << ", Ky: " << Ky << ", J: " << jumpNum << std::endl;
            board.tiles[Kx][Ky] = -1;
        }
        for (int i = -2; i < 3; i++) {
            for (int j = -2; j < 3; j++) {
                if (i == 0 || j == 0 || (std::abs(i) == std::abs(j))) {
                    continue;
                }
                if ((0 <= Kx + i && Kx + i < board.boardWidth) 
                    && (0 <= Ky + j && Ky + j < board.boardHeight)) {
                    if (board.tiles[Kx+i][Ky+j] == 0) {
                        board.tiles[Kx+i][Ky+j] = jumpNum;
                        moveQueue.push(XYJ(Kx+i, Ky+j, jumpNum+1));
                    }
                }
            }
        }
    }

    void knightBreadthSearch() {
        while (!moveQueue.empty()) {
            XYJ nextMove = moveQueue.front();
            moveQueue.pop();
            knightMoves(nextMove.x, nextMove.y, nextMove.j);
        }

        board.printBoard();
    }
};

int main(int argc, char **argv) {
    int board_rows = 8;
    int board_cols = 8;
    int ** starting_pos;
    starting_pos = new int *[MAX_START_POS];
    for (int i = 0; i < MAX_START_POS; i++) {
        starting_pos[i] = new int[2];
        for (int j = 0; j < 2; j++) {
            starting_pos[i][j] = -1;
        }
    }

    if (argc >= 3) {
        board_rows = std::atoi(argv[1]);
        board_cols = std::atoi(argv[2]);
        if ((argc >= 5) && (argc % 2 == 1)) {
            for (int i = 3; i < argc; i++) {
                starting_pos[(int)((i-3)/2)][(i-1)%2] = std::atoi(argv[i]);
            }
        }
    } else if (argc != 1) {
        fprintf(stderr, "Usage: Knights [rows cols] [pos1x pos1y] [pos2x pos2y]...up to %d starting positions\n", MAX_START_POS);
        exit(EXIT_FAILURE);
    }

    Knight standard = Knight(board_cols, board_rows, starting_pos);
    standard.knightBreadthSearch();

    draw("K", 10, board_cols, board_rows, standard.board.tiles);
}