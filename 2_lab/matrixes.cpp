#include <vector>
#include <thread>
#include<iostream>
#include <fstream>
#include <ctime>
std::vector<std::vector<int>> A, B;
//обычное перемножение
void ord_mult(std::vector<std::vector<int>>& C) {
    for (int i = 0; i < A.size(); i++) {
        for (int j = 0; j < B[i].size(); j++) {
            for (int k = 0; k < A[i].size(); k++)
                C[i][j] += A[i][k] * B[k][j];
        }
    }
}
//А на к потоков по строкам
void f1(std::vector<std::vector<int>>&, int, int);
void flow_mult1(std::vector<std::vector<int>>& C, int k) {
    std::vector<std::thread> threads;
    int n = A.size() / k;
    for (int i = 0; i < k; i++)
        threads.push_back(std::thread(f1, std::ref(C), i * n, i == k - 1 ? A.size() - 1 : (i + 1) * n - 1));
    for (auto& t : threads)
        t.join();
}
void f1(std::vector<std::vector<int>>& C, int first, int last) {
    for (int i = first; i <= last; i++) {
        for (int j = 0; j < B[i].size(); j++) {
            for (int k = 0; k < A[i].size(); k++)
                C[i][j] += A[i][k] * B[k][j];
        }
    }
}
//А на к потоков по столбцам
void f2(std::vector<std::vector<int>>&, int,int);
void flow_mult2(std::vector<std::vector<int>>& C, int k) {
    std::vector<std::thread> threads;
    std::vector<std::vector<std::vector<int>>>matrs(k, std::vector<std::vector<int>>(A.size(), std::vector<int>(B[0].size())));
    int n = B.size() / k;
    for (int i = 0; i < k; i++)
        threads.push_back(std::thread(f2, std::ref(matrs[i]), i * n, i == k - 1 ? A[0].size() - 1 : (i + 1) * n - 1));
    for (auto& t : threads)
        t.join();
    for (int i = 0; i < C.size(); i++) {
        for (int j = 0; j < C[i].size(); j++)
            for (int z = 0; z < k; z++)
                C[i][j] += matrs[z][i][j];
   }
}
void f2(std::vector<std::vector<int>>& C, int first, int last) {
    for (int i = 0; i < A.size(); i++) {
        for (int j = 0; j < B[0].size(); j++) {
            for (int k = first; k <=last; k++)
                C[i][j] += A[i][k] * B[k][j];
        }
    }
}
int main() {
    std::ifstream fin("input.txt");
    int n1, m1, n2, m2, k;
    fin >> n1 >> m1 >> n2 >> m2 >> k;
    if (m1 != n2) return 0;
    A = std::vector<std::vector<int>>(n1, std::vector<int>(m1));
    B = std::vector<std::vector<int>>(n2, std::vector<int>(m2));
    for(int i = 0;i<n1;i++)
        for (int j = 0; j < m1; j++) {
            int x;
            fin >> x;
            A[i][j] = x;
        }

    for (int i = 0; i < n2; i++)
        for (int j = 0; j < m2; j++) {
            int x;
            fin >> x;
            B[i][j] = x;
        }
    std::vector<std::vector<int>> C1(n1, std::vector<int>(m2));
    std::vector<std::vector<int>> C2(n1, std::vector<int>(m2));
    std::vector<std::vector<int>> C3(n1, std::vector<int>(m2));
    clock_t time1 = clock();
    ord_mult(C1);
    clock_t time2 = clock();
    std::cout << "time 1: " << time2 - time1 << "\n";
    flow_mult1(C2,k);
    time1 = clock();
    std::cout << "time 2: " << time1 - time2 << "\n";
    flow_mult2(C3, k);
    time2 = clock();
    std::cout << "time 3: " << time2 - time1<<"\n";
    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < m2; j++)
            std::cout << C1[i][j] << " ";
        std::cout << "\n";
    }
    std::cout << "\n";
    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < m2; j++)
            std::cout << C2[i][j] << " ";
        std::cout << "\n";
    }
    std::cout << "\n";
    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < m2; j++)
            std::cout << C3[i][j] << " ";
        std::cout << "\n";
    }
}