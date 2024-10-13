#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <algorithm>
#include <unordered_set>
#include <set>
#include <string>
#include <ctime>
#include <chrono>

using namespace std::chrono;

typedef int fun_type(int);
typedef fun_type* point_fun_type;

struct node {
    int val;
    int cnt;
    int op;
    node* prev;
};

static int add_dir(int num) {
    return num + 3;
}

static int mult_dir(int num) {
    return num * 2;
}

static int sub_dir(int num) {
    return num - 2;
}

static int sub_rev(int num) {
    return num - 3;
}

static int div_rev(int num) {
    return num / 2;
}

static int add_rev(int num) {
    return num + 2;
}

static void print_op_seq(node* n, bool dir_rev) {
    std::vector<std::string> ops;
    if (dir_rev)
        ops = { "+3 ", "*2 ", "-2 " };
    else
        ops = { "-3 ", "/2 ", "+2 " };
    std::vector<int> v;
    v.reserve(n->cnt);
    node* cur = n;
    while (cur->prev) {
        v.push_back(cur->op);
        cur = cur->prev;
    }
    std::cout << "Длина мин. последовательности = " << n->cnt << std::endl;
    for (int i = v.size() - 1; i > -1; --i)
        std::cout << ops[v[i]];
}

static void print_op_seq_bidir(node* n_dir, node* n_rev) {
    std::vector<std::string> ops = { "+3 ", "*2 ", "-2 ", "-3 ", "/2 ", "+2 "};
    std::vector<int> v;
    v.reserve(n_dir->cnt + n_rev->cnt);
    node* cur = n_dir;
    while (cur->prev) {
        v.push_back(cur->op);
        cur = cur->prev;
    }
    std::cout << "Длина мин. последовательности = " << n_dir->cnt + n_rev->cnt << std::endl;
    for (int i = v.size() - 1; i > -1; --i)
        std::cout << ops[v[i]];
    std::cout << "->  <- ";
    v.clear();
    cur = n_rev;
    while (cur->prev) {
        v.push_back(cur->op + 3);
        cur = cur->prev;
    }
    for (int i = 0; i < v.size(); ++i)
        std::cout << ops[v[i]];
}

static void bfs_dir(int start, int finish, std::vector<point_fun_type> fun_vec) {
    std::queue<node*> q;
    q.push(new node{ start, 0, 0, nullptr });
    std::set<int> v;
    v.insert(start);

    while (!q.empty()) {
        node* cur = q.front();
        q.pop();
        for (int i = 0; i < fun_vec.size(); ++i) {           
            int next = fun_vec[i](cur->val);
            if (next == finish) {
                print_op_seq(new node{ next, cur->cnt + 1, i, cur }, 1);
                return;
            }
            if (v.insert(next).second)
                q.push(new node{ next, cur->cnt + 1, i, cur });
        }   
    }
}

static void bfs_rev(int start, int finish, std::vector<point_fun_type> fun_vec) {
    std::queue<node*> q;
    q.push(new node{ finish, 0, 0, nullptr });
    std::set<int> v;
    v.insert(finish);

    while (!q.empty()) {
        node* cur = q.front();
        q.pop();
        for (int i = 0; i < fun_vec.size(); ++i) {
            if (i == 1 && cur->val % 2)
                continue;
            int next = fun_vec[i](cur->val);
            if (next == start) {
                print_op_seq(new node{ next, cur->cnt + 1, i, cur }, 0);
                return;
            }
            if (v.insert(next).second)
                q.push(new node{ next, cur->cnt + 1, i, cur });
        }
    }
}

static void bfs_bidir(int start, int finish, std::vector<point_fun_type> fun_vec_dir, std::vector<point_fun_type> fun_vec_rev) {
    std::queue<node*> q1, q2;
    q1.push(new node{ start, 0, 0, nullptr });
    q2.push(new node{ finish, 0, 0, nullptr });
    
    std::set<int> v1, v2; 
    v1.insert(start);
    v2.insert(finish);
   
    node* cur1;
    node* cur2;

    while (!(q1.empty() || q2.empty())) {
        cur1 = q1.front();
        q1.pop();
        for (int i = 0; i < fun_vec_dir.size(); ++i) {
            int next1 = fun_vec_dir[i](cur1->val);
            if (v2.find(next1) != v2.end()) {
                node* search = q2.front();
                q2.pop();
                while (search->val != next1) {
                    search = q2.front();
                    q2.pop();
                }
                print_op_seq_bidir(new node{ next1, cur1->cnt + 1, i, cur1 }, search);
                return;
            }
            if (v1.insert(next1).second)
                q1.push(new node{ next1, cur1->cnt + 1, i, cur1 });
        }
        cur2 = q2.front();
        q2.pop();
        for (int i = 0; i < fun_vec_rev.size(); ++i) {
            if (i == 1 && cur2->val % 2)
                continue;
            int next2 = fun_vec_rev[i](cur2->val);
            if (v1.find(next2) != v1.end()) {
                node* search = q1.front();
                q1.pop();
                while (search->val != next2) {
                    search = q1.front();
                    q1.pop();
                }
                print_op_seq_bidir(search, new node{ next2, cur2->cnt + 1, i, cur2 });
                return;
            }
            if (v2.insert(next2).second)
                q2.push(new node{ next2, cur2->cnt + 1, i, cur2 });
        }
    }
}

int main()
{
    setlocale(LC_ALL, "russian");

    int start, finish;
    std::cout << "Введите первое число: ";
    std::cin >> start;
    std::cout << "Введите второе число: ";
    std::cin >> finish;

    std::vector<point_fun_type> fun_vec_dir;
    fun_vec_dir.push_back(add_dir);
    fun_vec_dir.push_back(mult_dir);

    std::vector<point_fun_type> fun_vec_rev;
    fun_vec_rev.push_back(sub_rev);
    fun_vec_rev.push_back(div_rev);
    
    std::cout << "\nПрямой обход с операциями +3 и *2" << std::endl;
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    bfs_dir(start, finish, fun_vec_dir);
    high_resolution_clock::time_point t2 = high_resolution_clock::now();  
    std::cout << "\nВремя работы: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms;\n";

    std::cout << "\nОбратный обход с операциями +3 и *2" << std::endl;
    t1 = high_resolution_clock::now();
    bfs_rev(start, finish, fun_vec_rev);
    t2 = high_resolution_clock::now();
    std::cout << "\nВремя работы: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms;\n";

    std::cout << "\nДвунаправленный обход с операциями +3 и *2" << std::endl;
    t1 = high_resolution_clock::now();
    bfs_bidir(start, finish, fun_vec_dir, fun_vec_rev);
    t2 = high_resolution_clock::now();
    std::cout << "\nВремя работы: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms;\n";

    fun_vec_dir.push_back(sub_dir);
    fun_vec_rev.push_back(add_rev);

    std::cout << "\n\nПрямой обход с операциями +3, *2, -2" << std::endl;
    t1 = high_resolution_clock::now();
    bfs_dir(start, finish, fun_vec_dir);
    t2 = high_resolution_clock::now();
    std::cout << "\nВремя работы: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms;\n";

    std::cout << "\nОбратный обход с операциями +3, *2, -2" << std::endl;
    t1 = high_resolution_clock::now();
    bfs_rev(start, finish, fun_vec_rev);
    t2 = high_resolution_clock::now();
    std::cout << "\nВремя работы: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms;\n";

    std::cout << "\nДвунаправленный обход с операциями +3, *2, -2" << std::endl;
    t1 = high_resolution_clock::now();
    bfs_bidir(start, finish, fun_vec_dir, fun_vec_rev);
    t2 = high_resolution_clock::now();
    std::cout << "\nВремя работы: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms;\n";
}