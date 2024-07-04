#include <bits/stdc++.h>
#include "engine.cpp"
using namespace std;

double simulate(int num_of_frames, string &current_algorithm)
{
    if (current_algorithm == "opt")
      count_page_frequency();

    string disp_algo = current_algorithm;
    transform(disp_algo.begin(), disp_algo.end(), disp_algo.begin(), ::toupper);

    for (int i = 0; i < page_references.size(); i++){
      if ((find(memory_frames.begin(), memory_frames.end(), page_references[i]) == memory_frames.end()) && (find(memory_frames.begin(), memory_frames.end(), -1) == memory_frames.end()))
          page_faults++;

      if ((find(memory_frames.begin(), memory_frames.end(), -1) != memory_frames.end()) && (find(memory_frames.begin(), memory_frames.end(), page_references[i]) == memory_frames.end())){
          memory_frames[first_empty_space] = page_references[i];
          first_empty_space++;

          if (current_algorithm == "opt")
              page_frequency[page_references[i]]--;
      }
      else{

          if (current_algorithm == "fifo")
              FIFO(page_references[i]);
          else if (current_algorithm == "lru")
              LRU(page_references[i]);
          else if (current_algorithm == "opt")
              OPT(page_references[i]);
      }

      if (i >= num_of_frames)
          page_reference_count++;
    }

    cout << disp_algo << ", " << num_of_frames << " frames: ";
    double miss_rate = (float)page_faults / page_reference_count * 100;
    cout << "Miss rate = " << page_faults << " / " << page_reference_count << " = " << fixed << setprecision(2) << miss_rate << "%" << endl;
  return miss_rate;
}

void generate_results_file(int min_frames, int max_frames, int frame_increment)
{
    ofstream results_file("vmrates.dat");
    if (!results_file.is_open())
    {
        cerr << "Error: Unable to open vmrates.dat for writing." << endl;
        exit(1);
    }

    vector<int> frames_sequence;
    for (int i = min_frames; i <= max_frames; i += frame_increment)
    {
        frames_sequence.push_back(i);
    }
    for (int frame : frames_sequence)
    {
        results_file << frame << " ";
    }
    results_file << endl;
    string disp_algo;
    string algorithms[] = {"opt", "lru", "fifo"};
    for (string &current_algorithm : algorithms)
    {
        vector<double> algorithm_miss_rates;
        disp_algo = current_algorithm;
        transform(disp_algo.begin(), disp_algo.end(), disp_algo.begin(),::toupper);
        cout << "Running simulation for " << disp_algo << " algorithm..." << endl;
        for (int num_of_frames : frames_sequence)
        {
            page_faults = 0;
            page_reference_count = 0;
            first_empty_space = 0;
            miss = false;
            memory_frames.resize(num_of_frames, -1);
            double miss_rate = simulate(num_of_frames, current_algorithm);
            memory_frames.clear();
            page_frequency.clear();
            algorithm_miss_rates.push_back(miss_rate);
        }
        for (double miss_rate : algorithm_miss_rates)
        {
            results_file << fixed << setprecision(2) << miss_rate << " ";
        }
        results_file << endl;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        cerr << "Error: Invalid number of arguments." << endl;
        cerr << "Usage: ./vmstats <min_frames> <max_frames> <frame_increment> <input_filename>" << endl;
        return 1;
    }

    int min_frames = stoi(argv[1]);
    int max_frames = stoi(argv[2]);
    int frame_increment = stoi(argv[3]);
    input_file = argv[4];

    if (min_frames < 2 || max_frames > 100 || frame_increment <= 0)
    {
        cerr << "Error: Input must have min_frames>=2, max_frames<=99 and increment value must be a positive integer." << endl;
        return 1;
    }
    read_page_references();
    generate_results_file(min_frames, max_frames, frame_increment);

    return 0;
}
