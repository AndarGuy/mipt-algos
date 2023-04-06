/*

Компьютерная болтовня

В распределённых системах существуют алгоритмы, основанные на пересылке своим соседям только что прибывших сообщений. Такой класс алгоритмов носит название болтовни.

Рассматриваемая нами распределённая система состоит из узлов, связанных по схеме «прямоугольная сетка», где каждый узел внутри сетки имеет ровно четырёх соседей, четыре узла в вершинах прямоугольника — по два соседа, а остальные узлы на границе — по три соседа. Система функционирует в синхронном режима, это означает, что имеются раунды, в пределах каждого из которых происходят одновременно посылка и приём сообщений между соседями.

В какой-то момент времени потребовалось распространить важное сообщение и инициаторами посылки стало несколько узлов. Пришедшее в одном раунде сообщение на следующем отправляется соседу и в этом же раунде достигает его. Если узел, получивший сообщение, уже отправлял его ранее, повторной отправки на происходит. Требуется определить, через сколько раундов все узлы распределённой системы получат сообщения.

*/

#include <algorithm>
#include <iostream>
#include <list>
#include <vector>

template <typename T>
class fast_queue {
   private:
    T *body, *begin, *end, *body_end;

   public:
    explicit fast_queue(size_t size) {
        body = new T[size];
        begin = end = body;
        body_end = body + size;
    }
    ~fast_queue() { delete[] body; }
    void enqueue(T const &el) { *end++ = el; }
    bool empty() const { return begin == end; }
    T dequeue() { return *begin++; }
};

short bfs(int N, int M, std::vector<std::vector<short>> &matrix, std::vector<std::pair<short, short>> &points) {
    fast_queue<std::pair<short, short>> queue(N * M);
    for (auto point : points) {
        matrix[point.first][point.second] = 0;
        queue.enqueue(point);
    }

    std::vector<std::pair<short, short>> variants = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
    short max_distance = -1;
    while (!queue.empty()) {
        auto point = queue.dequeue();
        max_distance = std::max(matrix[point.first][point.second], max_distance);
        for (auto variant : variants) {
            short new_x = point.first + variant.first, new_y = point.second + variant.second;
            if (!(0 <= new_x && new_x < N && 0 <= new_y && new_y < M)) continue;
            if (matrix[new_x][new_y] == -1) {
                matrix[new_x][new_y] = matrix[point.first][point.second] + 1;
                queue.enqueue({new_x, new_y});
            }
        }
    }
    return max_distance;
}

void gossips(int N, int M, int K, std::vector<std::pair<short, short>> &points, std::vector<std::vector<short>> &matrix) {
    std::cout << bfs(N, M, matrix, points) << std::endl;
}

int main() {
    int N, M, K;
    std::cin >> N >> M >> K;
    std::vector<std::pair<short, short>> points;
    for (size_t i = 0; i < K; i++) {
        int x, y;
        std::cin >> x >> y;
        points.push_back({x, y});
    }

    std::vector<std::vector<short>> matrix(N, std::vector<short>(M, -1));

    gossips(N, M, K, points, matrix);

    return 0;
}