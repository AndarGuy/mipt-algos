/*

Центральная симметрия

Имеется множество из 4⩽N⩽100000 точек в декартовом трёхмерном пространстве с
целочисленными координатами в диапазоне от -10^18 до 10^18. Интересно, ли у
этого множества центр симметрии? Это вам и предстоит выяснить.

*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int N;
    scanf("%d", &N);

    struct Point {
        int x;
        int y;
        int z;
    };

    struct Point *points = calloc(N, sizeof(struct Point));

    for (size_t i = 0; i < N; i++)
        scanf("%d%d%d", &points[i].x, &points[i].y, &points[i].z);

    int max_distance = 0;
    struct Point center;
    for (size_t i = 0; i < N; i++) {
        for (size_t j = i + 1; j < N; j++) {
            int distance =
                (points[i].x - points[j].x) * (points[i].x - points[j].x) +
                (points[i].y - points[j].y) * (points[i].y - points[j].y) +
                (points[i].z - points[j].z) * (points[i].z - points[j].z);
            if (distance > max_distance) {
                max_distance = distance;
                center.x = points[i].x + points[j].x;
                center.y = points[i].y + points[j].y;
                center.z = points[i].z + points[j].z;
            }
        }
    }

    if (center.x % 2)
        printf("NO\n");
    else if (center.y % 2)
        printf("NO\n");
    else if (center.z % 2)
        printf("NO\n");
    else {
        center.x /= 2;
        center.y /= 2;
        center.z /= 2;
    }

    return 0;
}