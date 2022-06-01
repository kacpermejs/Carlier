#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#include <algorithm>



struct Task
{
    int index;
    int R;
    int P;
    int Q;
    int C;

    bool finished;

    Task() : R(-1), P(-1), Q(-1), C(-1) {}
    Task(int r, int p, int q) : R(r), P(p), Q(q), C(-1) {}
    Task(int r, int p, int q, int id) : R(r), P(p), Q(q), index(id), C(-1) {}
    Task(const Task& t, int c) : R(t.R), P(t.P), Q(t.Q), index(t.index), C(c) {}

    friend bool operator< (const Task& lhs, const Task& rhs) { return lhs.R < rhs.R; }
    friend bool operator> (const Task& lhs, const Task& rhs) { return lhs.Q > rhs.Q; }


};

int computeTotalLength(const int N, std::vector<int>& R, std::vector<int>& P, std::vector<int>& Q, std::vector<int>& X)
{
    int m = 0, c = 0;
    for (int n = 1; n <= N; n++)
    {
        int x = X[n];
        m = std::max(m, R[x]) + P[x];
        c = std::max(c, m + Q[x]);
    }
    return c;
}


struct compareR
{
    bool operator()(const Task& t1, const Task& t2)
    {
        if (t1.R == t2.R)
            return t1.index > t2.index;
        else
            return t1.R > t2.R;
    }
};

struct compareQ
{
    bool operator()(const Task& t1, const Task& t2)
    {
        if (t1.Q == t2.Q)
            return t1.index > t2.index;
        else
            return t1.Q < t2.Q;
    }
};

int Schrage1(std::priority_queue<Task, std::vector<Task>, compareR >& MinHeapR,
    std::priority_queue<Task, std::vector<Task>, compareQ >& MaxHeapQ,
    std::vector<Task>& C)
{
    int t = 0;
    int Cmax = 0;
    t += MinHeapR.top().R; //czas rozpoczêcia pierwszego zadania

    while (!MinHeapR.empty() || !MaxHeapQ.empty())
    {
        while (!MinHeapR.empty() && MinHeapR.top().R <= t)
        {
            MaxHeapQ.push(MinHeapR.top());
            MinHeapR.pop();
        }

        if (!MaxHeapQ.empty())
        {
            t += MaxHeapQ.top().P; //dodaj czas trwania zadania dostêpnego

            C.push_back(Task(MaxHeapQ.top(), t + MaxHeapQ.top().Q));//doadaj do uszeregowania zadanie i jego czas zakoñczenia
            Cmax = std::max(Cmax, t + MaxHeapQ.top().Q);
            MaxHeapQ.pop();
        }
        else
        {
            t = MinHeapR.top().R;//je¿eli nie ma zadañ dostêpnych, idŸ do momentu w którym pojawi siê zadanie
        }
    }
    return Cmax;
}


int Schrage2(std::priority_queue<Task, std::vector<Task>, compareR >& MinHeapR,
    std::priority_queue<Task, std::vector<Task>, compareQ >& MaxHeapQ,
    std::vector<Task>& C)
{
    int Cmax = 0;
    int t = 0;


    while (!MinHeapR.empty() || !MaxHeapQ.empty())
    {
        while (!MinHeapR.empty() && MinHeapR.top().R <= t)
        {
            MaxHeapQ.push(MinHeapR.top());
            MinHeapR.pop();
            if (!C.empty() && !MaxHeapQ.empty() && MaxHeapQ.top().Q > C.back().Q)
            {
                int pLast = t - MaxHeapQ.top().R;
                t = MaxHeapQ.top().R;
                if (pLast > 0)//dodaj pozosta³¹ czêœæ do zadañ dostêpnych
                {
                    Task temp = Task(C.back());
                    temp.P = pLast;
                    MaxHeapQ.push(temp);
                }
            }
        }


        if (!MaxHeapQ.empty())
        {
            t += MaxHeapQ.top().P; //dodaj czas trwania zadania dostêpnego

            C.push_back(Task(MaxHeapQ.top(), t + MaxHeapQ.top().Q));//doadaj do uszeregowania zadanie i jego czas zakoñczenia
            Cmax = std::max(Cmax, t + MaxHeapQ.top().Q);
            MaxHeapQ.pop();
        }
        else
        {
            t = MinHeapR.top().R;//je¿eli nie ma zadañ dostêpnych, idŸ do momentu w którym pojawi siê zadanie
        }
    }
    return Cmax;
}



int main()
{
    int N;

    //std::vector<Task> tasks;
    std::vector<Task> C;


    int totalC = 0;



    std::ifstream data("C:/Users/kacpe/source/repos/Carlier/carl.data.txt");

    std::string tmp;

    std::string dataSource = "data.000:";

    while (tmp != dataSource)
        data >> tmp;

    data >> N;

    std::priority_queue<Task, std::vector<Task>, compareR > MinHeapR; //niedostêpne
    std::priority_queue<Task, std::vector<Task>, compareQ > MaxHeapQ; //dostêpne

    //! numerowanie od 0
    for (int i = 0; i < N; i++)
    {
        int r, p, q;
        data >> r >> p >> q;
        //data >> R[i] >> P[i] >> Q[i];

        MinHeapR.push(Task(r, p, q, i));
        //std::cout << "id: " << tasks[i-1].index << " R: " << tasks[i-1].R << " P: " << tasks[i - 1].P << " Q: " << tasks[i - 1].Q << std::endl;
    }

    int Cmax = Schrage1(MinHeapR, MaxHeapQ, C);

    std::cout << "Cmax = " << Cmax << std::endl;

    for (int i = 0; i < C.size(); i++)
    {
        std::cout << C[i].index << " ";

    }

    std::cout << std::endl;
    return 0;
}