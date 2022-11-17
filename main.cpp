#include "simulation.hpp"

#include <iostream>
#include <fstream>
#include <regex>
#include <random>

using namespace std;

// ./ims {input file}

tuple<string, int> ParseRow(string row)
{
    std::smatch m;
    std::regex e("(\".*\")"); // matches words beginning by "sub"

    string name = "";
    int flights = 0;

    if (std::regex_search(row, m, e))
    {
        name = m.str(1);
        name.erase(0, 1);
        name.erase(name.length() - 1, 1);
    }

    auto index = row.find("\" : ") + 4;
    string flight_str(row);
    flight_str.erase(0, index);

    flights = stoi(flight_str);

    return make_tuple(name, flights);
}

void ParseInput(string filepath, Simulation &s)
{
    ifstream input_file(filepath);

    if (input_file.fail())
    {
        cerr << "Error: file " << filepath << " doesn't exist" << endl;
        exit(1);
    }

    string line;

    char state = '\0';

    while (getline(input_file, line))
    {
        // skiping blank rows
        if (line == "")
        {
            state = '\0';
            continue;
        }

        // Change of state
        if (state == '\0')
        {
            if (line == "A:")
            {
                state = 'A';
                continue;
            }
            if (line == "D:")
            {
                state = 'D';
                continue;
            }
            if (line == "B:")
            {
                state = 'B';
                continue;
            }
        }

        // reading of row
        auto res = ParseRow(line);
        switch (state)
        {
        case 'B':
            s.AddBomber(Bomber(get<0>(res), get<1>(res), &s));
            break;
        case 'D':
            s.AddDefender(Fighter(get<0>(res), get<1>(res), Team::Defenders, &s));
            break;
        case 'A':;
            s.AddAttacker(Fighter(get<0>(res), get<1>(res), Team::Attackers, &s));
        default:
            break;
        }
    }
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cerr << "Error: Missing argument specifying the input file" << endl;
        exit(1);
    }

    Simulation simulation(50);
    ParseInput(string(argv[1]), simulation);

    simulation.LogStatus();
    simulation.Run(300000);

    return 0;
}