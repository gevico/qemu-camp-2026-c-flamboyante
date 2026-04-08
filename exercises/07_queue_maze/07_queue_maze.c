#include <stdio.h>

#define MAX_ROW 5
#define MAX_COL 5

#define true 1
#define false 0

int maze[MAX_ROW][MAX_COL] = {
	0, 1, 0, 0, 0,
	0, 1, 0, 1, 0,
	0, 0, 0, 0, 0,
	0, 1, 1, 1, 0,
	0, 0, 0, 1, 0,
};

typedef struct 
{
	int x;
	int y;
} Node;

int visited[MAX_ROW][MAX_COL] = {0};

int is_valid(Node p, int dx, int dy)
{
	if(p.x + dx < 0 || p.x + dx >= MAX_ROW || p.y + dy < 0 || p.y + dy >= MAX_COL)
		return false;
	if(visited[p.x + dx][p.y + dy])
		return false;
	if(maze[p.x + dx][p.y + dy] == 1)
		return false;
	return true;
}

int main(void)
{
	// TODO: 在这里添加你的代码
	Node p = {0, 0};
	int head = 0;
	int tail = 0;
	Node queue[MAX_ROW*MAX_COL] = {{0}};
	Node path[MAX_ROW][MAX_COL] = {{0}};
    
	queue[tail++] = p;
	visited[p.x][p.y] = 1;
	path[0][0] = p;

	int dx[4] = {-1, 0, 1, 0};
	int dy[4] = {0, 1, 0, -1};
	int found = false;


	while (head < tail)
	{
		p = queue[head++];
		if(p.x == MAX_ROW-1 && p.y == MAX_COL-1)
		{
			found = true;
			break;
		}
		for(int i = 0; i < 4; i++)
		{
			Node next = p;
			if(is_valid(next, dx[i], dy[i]))
			{
				next.x += dx[i];
				next.y += dy[i];
				queue[tail++] = next;
				visited[next.x][next.y] = 1;
				path[next.x][next.y] = p;
			}
		}

	}

	if(!found)
	{
		printf("No path!\n");
		return 0;
	}

	while(!(p.x == 0 && p.y == 0))
	{
		printf("(%d, %d)\n", p.x, p.y);
		p = path[p.x][p.y];
	}
	printf("(0, 0)\n");

		
	
	
	
    

	return 0;
}
