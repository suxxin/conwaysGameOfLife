#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <random>
#include <chrono>

#define N 100
#define M 100

// forward declaration
static void print_board(int n, int m, int k, std::vector< std::vector<int> > *board,std::vector< std::vector<int> > *neighbours);

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

// neighbours direction
// NW, N, NE, W, E, SW, S, SE
int x[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
int y[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

static void inc_neighbour(int r, int c, std::vector< std::vector<int> > *neighbours) {
    for (int i = 0; i < 8; i++) {
        if (r+y[i] < 0 || r+y[i] >= M) continue;
        if (c+x[i] < 0 || c+x[i] >= N) continue;
        neighbours->at(c+x[i])[r+y[i]] += 1;
    }
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
    std::vector< std::vector<int> > neighbours(n, std::vector<int>(m));
    
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
            board[c][r] = 1;
            // increase neighbour count
            inc_neighbour(r,c,&neighbours);
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
            board[c][r] = 1;
            // increase neighbour count
            inc_neighbour(r,c,&neighbours);
        }
    }
    else {
        invalid_arg();
        return 0;
    }

    // print the init board
    std::cout << "초기 보드의 상태입니다.\n";
    print_board(n, m, 0, &board, &neighbours); 

    std::cout << "\n\n";
        
    int k = std::stoi(second_arg);
    std::cout << k << "-th generation 보드의 상태입니다.\n";
    print_board(n, m, k, &board, &neighbours); 
}

static void next_gen(int n, int m, std::vector< std::vector<int> > *board, std::vector< std::vector<int> > *neighbours) {
    // next generation board
    std::vector< std::vector<int> > new_neighbours(n, std::vector<int>(m));

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            // for efficiency, skip cells that are dead with no live neighbours
            if (board->at(j)[i] == 0 && neighbours->at(j)[i] == 0) 
                continue;
            // else, apply the rules of life
            if (board->at(j)[i] == 1 && neighbours->at(j)[i] < 2) {
                // too lonely, dies
                board->at(j)[i] = 0;
            } else if (board->at(j)[i] == 1 && neighbours->at(j)[i] > 3) {
                // overpopulation, dies
                board->at(j)[i] = 0;
            } else if (board->at(j)[i] == 0 && neighbours->at(j)[i] == 3) {
                // new born cell
                board->at(j)[i] = 1;
                // new neighbour status
                inc_neighbour(i,j,&new_neighbours);
            } else {
                // remains the same
                if (board->at(j)[i] == 1) {
                    // only increase the new neighbour if alive
                    inc_neighbour(i,j,&new_neighbours);
                }
            }  
        }
    }
    *neighbours = new_neighbours;
}

static void print_board(int n, int m, int k, std::vector< std::vector<int> > *board, std::vector< std::vector<int> > *neighbours) {
    // Algorithm
    for (int gen = 0; gen < k; gen++) {
        next_gen(n,m,board,neighbours);
    }

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (board->at(j)[i] == 0) {
                std::cout << "-";
            } else {
                std::cout << "*";
            }
        }
        std::cout << "\n";
    }
}

