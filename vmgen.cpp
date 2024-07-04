#include <bits/stdc++.h>
using namespace std;

vector<int> generate_page_references(int page_range, int sequence_length)
{
    vector<int> page_references;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, page_range - 1);

    page_references.push_back(dis(gen));
    for (int i = 1; i < sequence_length; ++i)
    {
        int next_page = dis(gen);
        while (next_page == page_references.back())
        {
            next_page = dis(gen);
        }
        page_references.push_back(next_page);
    }
    return page_references;
}

void write_to_file(vector<int> &page_references, string &filename)
{
    ofstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error opening file " << filename << endl;
        exit(1);
    }

    for (int i = 0; i < page_references.size(); ++i)
    {
        file << page_references[i];
        if (i != page_references.size() - 1)
        {
            file << " ";
        }
    }
    file.close();
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        cerr << "Error: Invalid number of arguments." << endl;
        cout << "Usage: ./vmgen <page_range> <sequence_length> <output_filename>" << endl;
        return 1;
    }

    int page_range = stoi(argv[1]);
    int sequence_length = stoi(argv[2]);
    string output_filename = argv[3];

    if (page_range <= 0 || page_range >= 100)
    {
        cerr << "Error: Page range must be between 1 and 99." << endl;
        return 1;
    }

    if (sequence_length < 0)
    {
        cerr << "Error: Sequence length must be a positive integer." << endl;
        return 1;
    }

    vector<int> page_references = generate_page_references(page_range, sequence_length);
    write_to_file(page_references, output_filename);

    return 0;
}