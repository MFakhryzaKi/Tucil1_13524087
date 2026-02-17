#include <bits/stdc++.h>
using namespace std;

bool ketemu;
vector <int> jawaban;
int rowCnt, colCnt;
long long iterCount;
vector <string> papan;
vector <int> warna[26];

bool existFile (string fileName, vector<string> &papan) {
    ifstream fInput ("test/" + fileName);

    if (!fInput.is_open()) {
        cout << "File dengan nama: " << fileName << " tidak ditemukan!" << endl;
        return 0;
    }

    string temp;

    while (getline(fInput, temp)) {
        papan.push_back(temp);
    }

    return 1;
}

bool validFile (vector<string> v) {
    // cout << "masudk" << endl;
    // cek warna valid apa engga
    for (string s : v) {
        for (char c : s) {
            if (c < 'A' || c > 'Z') {
                cout << "Isi file tidak valid (terdapat karakter selain A-Z), Silakan masukkan file lain!" << endl;
                return 0;
            }
        }
    }
    // cout << "masudk" << endl;

    int siz = v.size();
    for (string s : v) {
        // cout << s << endl;
        if (s.size() != siz) {
            cout << "Isi file tidak valid (papan permainan tidak berbentuk persegi), Silakan masukkan file lain!" << endl;
            return 0;
        }
    }

    set <char> temp;
    for (string s : v) {
        for (char c : s) {
            temp.insert(c);
        }
    }
    if (temp.size() != siz) {
            cout << "Isi file tidak valid (jumlah warma tidak sama dengan ukuran papan), Silakan masukkan file lain!" << endl;
        return 0;
    }

    return 1;
}

bool cek2Queen (int q1, int q2) {
    int barisQ1 = q1 / colCnt;
    int kolomQ1 = q1 % colCnt;
    int barisQ2 = q2 / colCnt;
    int kolomQ2 = q2 % colCnt;

    if (barisQ1 == barisQ2) {
        return 0;
    }

    if (kolomQ1 == kolomQ2) {
        return 0;
    }

    if (abs(kolomQ1 - kolomQ2) == 1 && abs(barisQ1 - barisQ2) == 1) {
        return 0;
    }

    return 1;
}

bool cekJawaban (vector<int> res) {
    int siz = res.size();

    for (int i = 0; i < siz - 1; i++) {
        for (int j = i + 1; j < siz; j++) {
            if (!cek2Queen(res[i], res[j])) {
                return 0;
            }
        }
    }

    return 1;
}

void initWarna (vector<string> v, vector<int> warna[]) {
    for (int i = 0; i < rowCnt; i++) {
        for (int j = 0; j < colCnt; j++) {
            warna[v[i][j] - 'A'].push_back(rowCnt*i + j);
        }
    }
}

void taroQueen (vector<string> &papan, vector<int> res) {
    for (int i : res) {
        papan[i/colCnt][i%colCnt] = '#';
    }
}

void printPapan (vector<string> papan) {

    for (string s : papan) {
        cout << s << endl;
    }
}

void solve (vector<int> &curJawaban, int curIdx) {
    if (ketemu) {
        return;
    }

    if (curIdx >= 26) {
        iterCount++;
        if (iterCount % max((int)round(pow(10, rowCnt - 4)), 0) == 0) {
            vector<string> temp;
            temp = papan;
            taroQueen(temp, curJawaban);
            cout << "Iterasi ke-" << iterCount << endl;
            printPapan(temp);
        }

        if (cekJawaban(curJawaban)) {
            ketemu = true;
            jawaban = curJawaban;
            return;
        }
        else {
            return;
        }
    }

    if (warna[curIdx].empty()) {
        solve(curJawaban, curIdx + 1);
        return;
    }

    for (int i = 0; i < warna[curIdx].size(); i++) {
        if (ketemu) {
            return;
        }
        curJawaban.push_back(warna[curIdx][i]);
        solve(curJawaban, curIdx + 1);
        curJawaban.pop_back();
    }
}

void printJawaban (vector<string> papan, string inputFile, int ms) {
    cout << "=================================================" << endl;
    printPapan(papan);
    cout << "File masukan       : " << inputFile << endl;
    cout << "Waktu komputasi    : " << ms << " ms" << endl;
    cout << "Jumlah iterasi     : " << iterCount << endl;
    cout << "=================================================" << endl;
    
}

void saveFile (vector<string> papan, string inputFile, int ms) {
    cout << "Masukkan nama file untuk menyimpan hasil (tanpa ekstensi): ";
    string temp;
    cin >> temp;
    temp = "output/" + temp + ".txt";

    ofstream fOut (temp);

    fOut << "=================================================" << endl;
    // printPapan(papan);
    for (string s : papan) {
        fOut << s << endl;
    }
    fOut << "File masukan       : " << inputFile << endl;
    fOut << "Waktu komputasi    : " << ms << " ms" << endl;
    fOut << "Jumlah iterasi     : " << iterCount << endl;
    fOut << "=================================================" << endl;
}

int main () {
    string inputFileName;

    while (true) {
        cout << "Masukkan nama file yang ingin diuji: ";
        getline(cin, inputFileName);
        papan.clear();
        // tambahin prosedur hapus spasi
        if (existFile (inputFileName, papan)) {
            // printPapan(papan);
            if (validFile (papan)) {
                break;
            }
        }
    }

    // printPapan(papan);
    rowCnt = papan.size();
    colCnt = papan[0].size();
    initWarna(papan, warna);

    // for (int i = 0; i < 26; i++) {
    //     cout << i << ' ' << warna[i].size() << endl;
    // }

    auto startTime = chrono::high_resolution_clock::now();
    vector<int> curJawaban;
    // cout << "Sebelum solve" << endl;
    solve(curJawaban, 0);
    auto endTime = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds> (endTime - startTime);
    int ms = duration.count();
    
    if (ketemu) {
        taroQueen(papan, jawaban);
        printJawaban(papan, inputFileName, ms);

        while (1) {
            cout << "Apakah anda ingin menyimpan hasil? (Y/N)" << endl;
            string temp;
            cin >> temp;
            if (temp == "Y") {
                saveFile(papan, inputFileName, ms);
                break;
            }
            else if (temp == "N") {
                break;
            }
            else {
                cout << "Masukan tidak valid!" << endl;
            }
        }
    }
    else {
        printJawaban(papan, inputFileName, ms);
        cout << "Tidak ada penyelesaian yang valid!" << endl;
    }
}