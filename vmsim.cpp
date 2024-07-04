#include <bits/stdc++.h>
#include "engine.cpp"
using namespace std;

//global declarations
// int num_frames;
// string input_file;
// string algorithm;
// vector<int> page_references;
// vector<int> memory_frames;
// int page_faults = 0;
// int page_reference_count = 0;
// bool miss = false;
// unordered_map<int, int> page_frequency;
// int first_empty_space = 0;


// void read_page_references(){
//     ifstream file(input_file);
//     if (!file.is_open()){
//         cerr << "Error: Input file not found." << endl;
//         exit(1);
//     }

//     int page;
//     while (file >> page){
//         if (page < 0 || page > 99){
//             cerr << "Error: Invalid page reference. Must be between 0 and 99." << endl;
//             exit(1);
//         }
//         page_references.push_back(page);
//     }

//     // cout << "Page references:";
//     // for (int page : page_references)
//     //     cout << " " << page;
//     // cout << endl;
// }

// void count_page_frequency(){
//     for (int page : page_references)
//         page_frequency[page]++;
// }

void display_memory_state(int page){
    cout << (page <= 9 ? " " : "") << page << ": [";
    for (int i = 0; i < memory_frames.size(); ++i){
        if (memory_frames[i] == -1)
            cout << (i < num_frames - 1 ? "  |" : "  ");
        else if (memory_frames[i] <= 9)
            cout << " " << memory_frames[i] << (i < num_frames - 1 ? "|" : "");
        else
            cout << memory_frames[i] << (i < num_frames - 1 ? "|" : "");
    }
    cout << "]";

    if ((find(memory_frames.begin(), memory_frames.end(), page) == memory_frames.end() &&
         find(memory_frames.begin(), memory_frames.end(), -1) == memory_frames.end()) ||
        miss){
        cout << " F";
    }
    cout << endl;
}

// void FIFO(int page){
//     if (find(memory_frames.begin(), memory_frames.end(), page) == memory_frames.end()){
//         miss = true;
//         if (memory_frames.size() >= num_frames)
//             memory_frames.erase(memory_frames.begin());
//         memory_frames.push_back(page);
//     }
//     else
//         miss = false;
// }

// void LRU(int page){
//     if (find(memory_frames.begin(), memory_frames.end(), page) == memory_frames.end()){
//         FIFO(page);
//         return;
//     }

//     miss = false;
//     int index = find(memory_frames.begin(), memory_frames.end(), page) - memory_frames.begin();
//     memory_frames.erase(memory_frames.begin() + index);
//     memory_frames.push_back(-1);
//     first_empty_space--;
//     memory_frames[first_empty_space] = page;
//     first_empty_space++;
// }

// void OPT(int page){
//     if (find(memory_frames.begin(), memory_frames.end(), page) != memory_frames.end()){
//         miss = false;
//         page_frequency[page]--;
//         return;
//     }

//     miss = true;
//     int mini = page_frequency[memory_frames[0]];
//     int index = 0;
//     for (int i = 0; i < memory_frames.size(); i++){
//         if (page_frequency[memory_frames[i]] < mini){
//             mini = page_frequency[memory_frames[i]];
//             index = i;
//         }
//     }

//     memory_frames[index] = page;
// }


void simulate(){
    read_page_references();
    if (algorithm == "opt")
        count_page_frequency();

    string disp_algo = algorithm;
    transform(disp_algo.begin(), disp_algo.end(), disp_algo.begin(), ::toupper);
    cout << "Simulating " << disp_algo << " with " << num_frames << " frames:" << endl;

    for (int i = 0; i < page_references.size(); i++){
        if ((find(memory_frames.begin(), memory_frames.end(), page_references[i]) == memory_frames.end()) && (find(memory_frames.begin(), memory_frames.end(), -1) == memory_frames.end()))
            page_faults++;

        if ((find(memory_frames.begin(), memory_frames.end(), -1) != memory_frames.end()) && (find(memory_frames.begin(), memory_frames.end(), page_references[i]) == memory_frames.end())){
            memory_frames[first_empty_space] = page_references[i];
            first_empty_space++;

            if (algorithm == "opt")
                page_frequency[page_references[i]]--;
        }
        else{
            // cout<<algorithm;
          
            if (algorithm == "fifo")
                FIFO(page_references[i]);
            else if (algorithm == "lru")
                LRU(page_references[i]);
            else if (algorithm == "opt")
                OPT(page_references[i]);
        }

        display_memory_state(page_references[i]);
        if (i >= num_frames)
            page_reference_count++;
    }

    cout << "\nTotal page faults: " << page_faults << endl;
    float miss_rate = (float)page_faults / page_reference_count * 100;
    cout << "Miss rate = " << page_faults << " / " << page_reference_count << " = " << fixed << setprecision(2) << miss_rate << "%" << endl;
}

int main(int argc, char *argv[]){
  
    if (argc != 4){
        cerr << "Error: Invalid number of arguments." << endl;
        cerr << "Usage: " << argv[0] << " <num_frames> <input_filename> <algorithm>" << endl;
        cerr << "Algorithms: opt, lru, fifo" << endl;
        return 1;
    }

    num_frames = stoi(argv[1]);
    input_file = argv[2];
    algorithm = argv[3];
    memory_frames.resize(num_frames, -1);

    if (num_frames <= 0 || num_frames > 100){
        cerr << "Error: Invalid number of frames. Must be between 1 and 100." << endl;
        cout << "Usage: vmsim <numframes> <tracefile> <algorithm>" << endl;
        cout << "Algorithms: opt, lru, fifo" << endl;
        return 1;
    }

    if (algorithm != "opt" && algorithm != "lru" && algorithm != "fifo"){
        cerr << "Error: Invalid algorithm. Choose from opt, lru, fifo." << endl;
        cout << "Usage: vmsim <numframes> <tracefile> <algorithm>" << endl;
        cout << "Algorithms: opt, lru, fifo" << endl;
        return 1;
    }

    simulate();

    return 0;
}