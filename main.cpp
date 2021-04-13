#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <random>
#include <chrono>

#define N 5;
#define M 5;

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
        // n개의 랜덤 Cell의 초기 보드 initialize
        int nn = std::stoi(first_arg);
        // generate n distinct random numbers between 1 and NxM
        int range = n * m;
        std::vector<int> gen_rand;
        for (int i = 1; i < range; i++) {
            gen_rand.push_back(i);
        }
        // random shuffle
        unsigned seed = std::chrono::system_clock::now()
                        .time_since_epoch()
                        .count();
        std::shuffle(gen_rand.begin(), gen_rand.end(), std::default_random_engine(seed));
        // extract the first nn element
        gen_rand.resize(nn);

        for (int i = 0; i < nn; i++) {
            // fill in the cells corresponding to the randomly generated number 
            int r,c;
            if (gen_rand[i] % n == 0) {
                r = gen_rand[i] / n - 1;
                c = n - 1;
            } else {
                r = gen_rand[i] / n;
                c = gen_rand[i] % n - 1;
            }
            board[r][c] = 1;
        }
    }
    else {
        invalid_arg();
        return 0;
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

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (board->at(i)[j] == 0) {
                std::cout << "-";
            } else {
                std::cout << "*";
            }
        }
        std::cout << std::endl;
    }
}