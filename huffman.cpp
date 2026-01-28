#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>

using namespace std;

struct Node {
    char ch;
    int freq;
    Node *left, *right;
    Node(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
};

class PriorityQueue {
public:
    vector<Node*> heap;

    void bubbleUp(int i) {
        while (i > 0 && heap[(i - 1) / 2]->freq > heap[i]->freq) {
            swap(heap[i], heap[(i - 1) / 2]);
            i = (i - 1) / 2;
        }
    }

    void bubbleDown(int i) {
        int smallest = i, l = 2 * i + 1, r = 2 * i + 2;
        if (l < heap.size() && heap[l]->freq < heap[smallest]->freq) smallest = l;
        if (r < heap.size() && heap[r]->freq < heap[smallest]->freq) smallest = r;
        if (smallest != i) {
            swap(heap[i], heap[smallest]);
            bubbleDown(smallest);
        }
    }

    void add(Node* n) {
        heap.push_back(n);
        bubbleUp(heap.size() - 1);
    }

    Node* removeMin() {
        if (heap.empty()) return nullptr;
        Node* res = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        if (!heap.empty()) bubbleDown(0);
        return res;
    }

    void build(vector<Node*>& nodes) {
        heap = nodes;
        for (int i = (heap.size() / 2) - 1; i >= 0; i--) bubbleDown(i);
    }

    void decreasePriority(char c, int newFreq) {
        for (int i = 0; i < heap.size(); i++) {
            if (heap[i]->ch == c) {
                heap[i]->freq = newFreq;
                bubbleUp(i);
                break;
            }
        }
    }
    bool isEmpty() { return heap.empty(); }
};


void getCodes(Node* n, string s, map<char, string>& c) {
    if (!n) return;
    if (!n->left && !n->right) c[n->ch] = s;
    getCodes(n->left, s + "0", c);
    getCodes(n->right, s + "1", c);
}

void process() {
    string inF, outF;
    cout << "Plik do kompresji: "; cin >> inF;
    cout << "Plik wynikowy: "; cin >> outF;

    ifstream ifs(inF);
    string txt((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
    ifs.close();

    map<char, int> freqs;
    for (char c : txt) freqs[c]++;

    PriorityQueue pq;
    for (auto p : freqs) pq.add(new Node(p.first, p.second));

    while (pq.heap.size() > 1) {
        Node *l = pq.removeMin(), *r = pq.removeMin();
        Node *par = new Node('\0', l->freq + r->freq);
        par->left = l; par->right = r;
        pq.add(par);
    }

    map<char, string> codes;
    getCodes(pq.removeMin(), "", codes);

    ofstream ofs(outF);
    for (auto const& [ch, code] : codes) ofs << ch << ":" << code << " ";
    ofs << "\n";
    for (char c : txt) ofs << codes[c];
    ofs.close();
    cout << "Gotowe! Sprawdz plik: " << outF << endl;
}

void decompress() {
    string inF, outF;
    cout << "Plik do dekompresji: "; cin >> inF;
    cout << "Plik wyjsciowy: "; cin >> outF;

    ifstream ifs(inF);
    string dictLine, data;
    getline(ifs, dictLine);
    getline(ifs, data);
    ifs.close();

    map<string, char> rev;
    size_t p = 0;
    while ((p = dictLine.find(':')) != string::npos) {
        char c = dictLine[p-1];
        size_t sp = dictLine.find(' ', p);
        rev[dictLine.substr(p+1, sp-p-1)] = c;
        dictLine.erase(0, sp+1);
    }

    ofstream ofs(outF);
    string cur = "";
    for (char b : data) {
        cur += b;
        if (rev.count(cur)) { ofs << rev[cur]; cur = ""; }
    }
}

int main() {
    int m;
    while(true) {
        cout << "\n1.Kompresja\n2.Dekompresja\n3.Test Kolejki\n0.Wyjscie:\n";
        cin >> m;
        if(m==1) process(); else if(m==2) decompress(); 
        else if(m==3) {
            PriorityQueue q; q.add(new Node('x', 10)); q.add(new Node('y', 5));
            cout << "Min: " << q.removeMin()->freq << " (powinno byc 5)\n";
        } else break;
    }
    return 0;
}