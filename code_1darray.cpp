#include<iostream>
using namespace std; 
int N=3;
int getmin(int a[])
{
    int minInd = 0;
    for (int i=1; i<N; i++)
        if (a[i] < a[minInd])
            minInd = i;
    return minInd;
}
int getmax(int a[])
{
    int maxInd = 0;
    for (int i=1; i<N; i++)
        if (a[i] > a[maxInd])
            maxInd = i;
    return maxInd;
}

int minimumoftwo(int x, int y)
{
    return (x<y)? x: y;
}
 
void minCashFlowRec(int amt[])
{
    int maxcredit = getmax(amt), maxdebit = getmin(amt);
    if (amt[maxcredit] == 0 && amt[maxdebit] == 0)
        return;
    int min = minimumoftwo(-amt[maxdebit], amt[maxcredit]);
    amt[maxcredit] -= min;
    amt[maxdebit] += min;
    cout << "Person " << maxdebit << " pays " << min
         << " to " << "Person " << maxcredit
     << endl;
    minCashFlowRec(amt);
}
void mincashflow()
{
    int amt[N] = {0};
     for (int p=0; p<N; p++)
       for (int i=0; i<N; i++)
          amt[p] += (graph[i][p] -  graph[p][i]);
 
    minCashFlowRec(amt);
}
int main()
{
    int graph[N][N] = { {0, 1000, 2000},{0, 0, 5000},
                        {0, 0, 0},};
    mincashflow();
    return 0;
}
