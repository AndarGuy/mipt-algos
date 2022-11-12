/*

Прокладывание дорог

Дураки и дороги — всегда большие проблемы. Дороги в Тьмутараканской губернии всегда были в дурацком состоянии. Более того, существовали целые города, до которых нельзя было добраться по дорогам.

Вновь избранный губернатор, пообещавший жителям, что он обеспечит доступность каждого населённого пункта до другого, нанял вас, чтобы рассчитать, сколько дорог нужно построить. Вполне достаточно, чтобы из каждого населённого пункта можно было добраться до любого другого.

Ещё одна проблема в том, что дурацкая традиция губернии в том, что все дороги — однонаправленные и строить умеют только однонаправленные дороги.

*/

#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

void compression(int N, std::vector<std::vector<int>> forwards, std::vector<std::vector<int>> backwards) {
    std::vector<int> visited(N, false), sorted, palette(N, -1);

    std::function<void(int)> topsort = [&forwards, &visited, &sorted, &topsort](int vertex) -> void {
        visited[vertex] = true;
        for (int y : forwards[vertex])
            if (!visited[y]) topsort(y);
        sorted.push_back(vertex);
    };
    std::function<void(int, int)> paint = [&palette, &backwards, &paint](int vertex, int color) -> void {
        palette[vertex] = color;
        for (int y : backwards[vertex])
            if (palette[y] == -1) paint(y, color);
    };

    for (int x = 0; x < N; x++)
        if (!visited[x]) topsort(x);

    std::reverse(sorted.begin(), sorted.end());
    int colors = 0;
    std::vector<int> color_to_vertex;
    for (auto x : sorted)
        if (palette[x] == -1) {
            paint(x, colors++);
            color_to_vertex.push_back(x);
        }

    // for (size_t i = 0; i < N; i++) {
    //     std::cout << i + 1 << " " << palette[i] << std::endl;
    // }

    std::vector<std::vector<int>> dag_forwards(colors), dag_backwards(colors);
    for (size_t x = 0; x < N; x++) {
        for (auto y : forwards[x]) {
            if (palette[x] != palette[y]) {
                dag_forwards[palette[x]].push_back(palette[y]);
                dag_backwards[palette[y]].push_back(palette[x]);
            }
        }
    }

    std::vector<int> not_in, not_out;
    for (size_t x = 0; x < colors; x++) {
        if (dag_forwards[x].size() == 0) not_out.push_back(x);
        if (dag_backwards[x].size() == 0) not_in.push_back(x);
    }

    std::cout << std::max(not_in.size(), not_out.size()) << std::endl;

    while (!not_in.empty() && !not_out.empty()) {
        std::cout << color_to_vertex[not_out.back()] + 1 << ' ' << color_to_vertex[not_in.back()] + 1 << std::endl;
        not_out.pop_back();
        not_in.pop_back();
    }

    while (!not_in.empty()) {
        std::cout << not_in.back() << ' ' << not_in.back() << std::endl;
        not_in.pop_back();
    }

    while (!not_out.empty()) {
        std::cout << not_out.back() << ' ' << not_out.back() << std::endl;
        not_out.pop_back();
    }
}

int main() {
    int N;
    std::cin >> N;
    std::vector<std::vector<int>> forwards(N), backwards(N);
    int x, y;
    while (std::cin >> x >> y) {
        forwards[x - 1].push_back(y - 1);
        backwards[y - 1].push_back(x - 1);
    }
    compression(N, forwards, backwards);
}