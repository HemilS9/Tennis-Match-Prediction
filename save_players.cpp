#include <fstream>
#include <string>
#include <unordered_set>
#include "csv.h"

using std::string;

std::unordered_set<string> players;
void store_players_from_input(string input_file_name);
void save_players_to_file(string output_file_name);

int main() {
    string input_file_name = "2022_data.csv";
    string output_file_name = "all_players.txt";

    store_players_from_input(input_file_name);
    save_players_to_file(output_file_name);

    return 0;
}

void store_players_from_input(string input_file_name) {
    string winner_name, loser_name;
    io::CSVReader<2> csvinput(input_file_name);
    csvinput.read_header(io::ignore_extra_column, "winner_name", "loser_name");

    while (csvinput.read_row(winner_name, loser_name)) {
        players.insert(winner_name);
        players.insert(loser_name);
    }
}

void save_players_to_file(string output_file_name) {
    std::ofstream output_file(output_file_name);

    for (string name : players) {
        output_file << name << std::endl;
    }

    output_file.close();
}