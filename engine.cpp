#include <bits/stdc++.h>
using namespace std;

int num_frames;
string input_file;
string algorithm;
vector<int> page_references;
vector<int> memory_frames;
int page_faults = 0;
int page_reference_count = 0;
bool miss = false;
unordered_map<int, int> page_frequency;
int first_empty_space = 0;

void count_page_frequency(){
    for (int page : page_references)
        page_frequency[page]++;
}

void read_page_references(){
    ifstream file(input_file);
    if (!file.is_open()){
        cerr << "Error: Input file not found." << endl;
        exit(1);
    }

    int page;
    while (file >> page){
        if (page < 0 || page > 99){
            cerr << "Error: Invalid page reference. Must be between 0 and 99." << endl;
            exit(1);
        }
        page_references.push_back(page);
    }

    // cout << "Page references:";
    // for (int page : page_references)
    //     cout << " " << page;
    // cout << endl;
}

void FIFO(int page){
    if (find(memory_frames.begin(), memory_frames.end(), page) == memory_frames.end()){
        miss = true;
        if (memory_frames.size() >= num_frames)
            memory_frames.erase(memory_frames.begin());
        memory_frames.push_back(page);
    }
    else
        miss = false;
}

void LRU(int page){
    if (find(memory_frames.begin(), memory_frames.end(), page) == memory_frames.end()){
        FIFO(page);
        return;
    }

    miss = false;
    int index = find(memory_frames.begin(), memory_frames.end(), page) - memory_frames.begin();
    memory_frames.erase(memory_frames.begin() + index);
    memory_frames.push_back(-1);
    first_empty_space--;
    memory_frames[first_empty_space] = page;
    first_empty_space++;
}

void OPT(int page){
    if (find(memory_frames.begin(), memory_frames.end(), page) != memory_frames.end()){
        miss = false;
        page_frequency[page]--;
        return;
    }

    miss = true;
    int mini = page_frequency[memory_frames[0]];
    int index = 0;
    for (int i = 0; i < memory_frames.size(); i++){
        if (page_frequency[memory_frames[i]] < mini){
            mini = page_frequency[memory_frames[i]];
            index = i;
        }
    }

    memory_frames[index] = page;
}
