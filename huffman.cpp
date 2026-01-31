#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <bitset>

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
        if (l < (int)heap.size() && heap[l]->freq < heap[smallest]->freq) smallest = l;
        if (r < (int)heap.size() && heap[r]->freq < heap[smallest]->freq) smallest = r;
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
    int size() { return heap.size(); }
};

void getCodes(Node* n, string s, map<char, string>& c) {
    if (!n) return;
    if (!n->left && !n->right) c[n->ch] = s;
    getCodes(n->left, s + "0", c);
    getCodes(n->right, s + "1", c);
}

void compress() {
    string inF, outF;
    cout << "Plik do kompresji: "; cin >> inF;
    cout << "Plik wynikowy (.huf): "; cin >> outF;

    ifstream ifs(inF, ios::binary);
    if (!ifs) { cout << "Blad pliku!\n"; return; }
    string txt((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
    ifs.close();

    if (txt.empty()) return;

    map<char, int> freqs;
    for (char c : txt) freqs[c]++;

    PriorityQueue pq;
    for (auto p : freqs) pq.add(new Node(p.first, p.second));

    while (pq.size() > 1) {
        Node *l = pq.removeMin(), *r = pq.removeMin();
        Node *par = new Node('\0', l->freq + r->freq);
        par->left = l; par->right = r;
        pq.add(par);
    }

    map<char, string> codes;
    getCodes(pq.removeMin(), "", codes);

    ofstream ofs(outF, ios::binary);
    
    int dictSize = codes.size();
    ofs.write((char*)&dictSize, sizeof(dictSize));
    for (auto const& [ch, code] : codes) {
        ofs.put(ch);
        int codeLen = code.length();
        ofs.write((char*)&codeLen, sizeof(codeLen));
        ofs.write(code.c_str(), codeLen);
    }

    long long totalChars = txt.length();
    ofs.write((char*)&totalChars, sizeof(totalChars));


    unsigned char buffer = 0;
    int bitCount = 0;

    for (char c : txt) {
        for (char bit : codes[c]) {
            buffer <<= 1;
            if (bit == '1') buffer |= 1;
            bitCount++;
            if (bitCount == 8) {
                ofs.put(buffer);
                buffer = 0;
                bitCount = 0;
            }
        }
    }
    if (bitCount > 0) {
        buffer <<= (8 - bitCount);
        ofs.put(buffer);
    }
    ofs.close();
    cout << "Skompresowano pomyślnie!\n";
}

void decompress() {
    string inF, outF;
    cout << "Plik do dekompresji: "; cin >> inF;
    cout << "Plik wynikowy: "; cin >> outF;

    ifstream ifs(inF, ios::binary);
    if (!ifs) return;

    int dictSize;
    ifs.read((char*)&dictSize, sizeof(dictSize));
    map<string, char> rev;
    for (int i = 0; i < dictSize; i++) {
        char ch; int len;
        ifs.get(ch);
        ifs.read((char*)&len, sizeof(len));
        char* buf = new char[len + 1];
        ifs.read(buf, len);
        buf[len] = '\0';
        rev[string(buf)] = ch;
        delete[] buf;
    }

    long long totalChars;
    ifs.read((char*)&totalChars, sizeof(totalChars));

    ofstream ofs(outF, ios::binary);
    string curCode = "";
    long long decodedCount = 0;
    unsigned char byte;

    while (ifs.get((char&)byte) && decodedCount < totalChars) {
        for (int i = 7; i >= 0 && decodedCount < totalChars; i--) {
            int bit = (byte >> i) & 1;
            curCode += (bit ? '1' : '0');
            if (rev.count(curCode)) {
                ofs.put(rev[curCode]);
                curCode = "";
                decodedCount++;
            }
        }
    }
    ofs.close();
    ifs.close();
    cout << "Zdekompresowano pomyślnie!\n";
}

int main() {
    int m;
    while(true) {
        cout << "\n1. Kompresja\n2. Dekompresja\n0. Wyjscie\nWybor: ";
        cin >> m;
        if(m==1) compress(); 
        else if(m==2) decompress(); 
        else break;
    }
    return 0;
}