N, M, L, K = map(int, input().split())
matrix = [[0 for _ in range(M)] for _ in range(N)]
for _ in range(L):
    x, y = map(int, input().split())
    matrix[x][y] = 1

for _ in range(K):
    new_matrix = [[0 for _ in range(M)] for _ in range(N)]
    for i in range(N):
        for j in range(M):
            if 
