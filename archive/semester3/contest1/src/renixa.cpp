/*

Чепуха

Многие в детстве использовали различные перестановочные шифры для того, чтобы тайно общаться со сверстниками. В перестановочных шифрах каждая буква или символ однозначно заменяется на заранее выбранный. Такие шифры удобно записывать строками и это называется шифрующей строкой. Например, для алфавита из четырёх символов ABCD шифр BDCA означает, что каждый символ исходного сообщения A заменяется на символ B, каждый символ B — на символ D и так далее.

Для получения шифрующей строки обычно достаточно провести частотный анализ текста.

У нас задача намного проще. Удалось получить несколько зашифрованных слов, которые в незашифрованном виде были лексикографически отсортированы. Эти строки состоят из прописных букв латинского алфавита и, на первый взгдяд, этот текст — какая-то чепуха.

Ваша задача состоит в том, чтобы, используя факт отсортированности, вы восстановили шифрующую строку.

Известно, что входной словарь состоит из подряд идущих букв латинского алфавита и известна его мощность.

*/

#include <iostream>
#include <string>
#include <vector>

void dfs(int vertex, std::vector<std::vector<int>> &connections, std::vector<int> &visited, std::vector<int> &backlog) {
    visited[vertex] = true;
    for (int y : connections[vertex]) {
        if (!visited[y]) dfs(y, connections, visited, backlog);
    }
    backlog.push_back(vertex);
}

void renixa(int N, std::vector<std::vector<int>> &connections) {
    std::vector<int> visited(N, 0), backlog;
    for (size_t i = 0; i < N; i++)
        if (!visited[i])
            dfs(i, connections, visited, backlog);
    reverse(backlog.begin(), backlog.end());
    for (int x : backlog) std::cout << ((char)('A' + x));
    std::cout << std::endl;
}

int main() {
    int N, M;
    std::cin >> N >> M;
    std::vector<std::vector<int>> connections(N);
    std::vector<std::string> words(M);
    for (size_t i = 0; i < M; i++) std::cin >> words[i];
    for (size_t i = 1; i < M - 1; i++) {
        std::string word_a = words[i - 1], word_b = words[i];
        for (size_t j = 0; j < std::min(word_a.length(), word_b.length()); j++) {
            if (word_a[j] != word_b[j]) {
                connections[word_a[j] - 'A'].push_back(word_b[j] - 'A');
                break;
            }
        }
    }

    renixa(N, connections);

    return 0;
}