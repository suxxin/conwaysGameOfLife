#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

#define N 100;
#define M 100;

// forward declaration
static void print_board(int n, int m, int k, std::vector< std::vector<int> > *board);

static void invalid_arg() {
    std::cout << "Invalid Argument" << std::endl;
    std::cout << "Usage:" << std::endl;
    std::cout << "  (1) ./game InputFileName.txt k" << std::endl;
    std::cout << "   or" << std::endl;
    std::cout << "  (2) ./game N k" << std::endl;
}

static bool is_number(std::string s) {
    for (int i = 0; i < s.length(); i++)
        if (!std::isdigit(s[i]))
            return false;
    return true;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        invalid_arg();
        return 0;
    }

    int n = N;
    int m = M;

    std::string first_arg(argv[1]);
    std::string second_arg(argv[2]);

    std::vector< std::vector<int> > board(n, std::vector<int>(m));
    
    // 1. Input 파일 제공: filename.txt, generation k 입력
    if (!is_number(first_arg) && is_number(second_arg)) {
        // handles input file
        std::ifstream input_file(first_arg);
        if (!input_file.is_open()) {
            // file does not exist or some error happened
            invalid_arg();
            return 0;
        }
        
        std::string input;
        while (getline(input_file, input)) {
            // initialize the board while reading the input file
            std::string row, col;
            std::istringstream ss(input);
            ss >> row;
            ss >> col;
            int r = stoi(row);
            int c = stoi(col);

            board[r][c] = 1;

        }
        input_file.close();
    }
    // 2. Input 파일 미제공: number of living cell N, generation k 입력
    else if (is_number(first_arg) && is_number(second_arg)) {
        // n개의 랜덥 Cell의 초기 보드 initialize
        int n = std::stoi(first_arg);
    }

    // print the init board
    std::cout << "초기 보드의 상태입니다." << std::endl;
    print_board(n, m, 0, &board); 
        
    int k = std::stoi(second_arg);
    std::cout << k << "-th generation 보드의 상태입니다." << std::endl;
    print_board(n, m, k, &board); 
}

static void print_board(int n, int m, int k, std::vector< std::vector<int> > *board) {
    // Algorithm

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (board->at(i)[j] == 0) {
                std::cout << "-";
            } else {
                std::cout << "*";
            }
        }
        std::cout << std::endl;
    }
}