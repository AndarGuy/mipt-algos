/*

Гиперканалы

В состав Галактической империи входит N планет. Между большинством пар из них существуют гиперканалы. Новый император повелел, чтобы с любой планеты можно было попасть на любую другую, пройдя только через один гиперканал. Каналы устроены так, что позволяют путешествовать только в одну сторону. Единственный оставшийся прокладчик гиперканалов расположен на базе около планеты с номером A. К сожалению, он не может путешествовать по уже существующим каналам, он всегда прокладывает новый. А наличие двух каналов в одном направлении между двумя планетами существенно осложняет навигацию. Ваша задача — найти такой маршрут для прокладчика, чтобы все необходимые каналы были построены, и не было бы построено лишних. В конце своего маршрута прокладчик должен оказаться на своей родной базе, с которой он начал движение.

*/

#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

void euler(int vertex, int N, std::vector<std::vector<int>> &matrix, std::vector<int> &answer) {
    for (size_t i = 0; i < N; i++) {
        if (vertex == i) continue;
        if (matrix[vertex][i] == false) {
            matrix[vertex][i] = true;
            euler(i, N, matrix, answer);
        }
    }
    // std::cout << vertex << std::endl;
    answer.push_back(vertex);
}

void hyperchannels(int N, int A, std::vector<std::vector<int>> matrix) {
    std::vector<int> answer;
    euler(A - 1, N, matrix, answer);
    std::reverse(answer.begin(), answer.end());
    for (size_t i = 0; i < answer.size() - 1; i++) {
        std::cout << answer[i] + 1 << ' ' << answer[i + 1] + 1 << std::endl;
    }
}

int main() {
    int N, A;
    std::cin >> N >> A;

    std::vector<std::vector<int>> matrix(N, std::vector<int>(N));
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            std::cin >> matrix[i][j];
        }
    }

    hyperchannels(N, A, matrix);
}