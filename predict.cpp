#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map> 
#include <unordered_map>
#include <ctime>
#include <iomanip>
#include "csv.h"
#include "predict.h"

using namespace std;

class SurfaceError {};
class PlayerError {};

class Prediction {
    private:
    string surface;

    public:

    unordered_map<string, Player> players; // Key: player name | Value: Player info

    // static void parse_time(const Match &m, int &year, int &month, int &day) {
    //     year = stoi(m.date.substr(0, 4));
    //     month = stoi(m.date.substr(4, 2));
    //     day = stoi(m.date.substr(6, 2));
    // }

    void update_player_ELO(const Match &m, Player &winner, Player &loser);

    // https://github.com/ben-strasser/fast-cpp-csv-parser
    void train(string &train_file) {
        string surface, score, date;
        string winner_name, loser_name;  
        int time;
        int w_rank, l_rank;
        int w_ace, l_ace;
        int w_df, l_df;
        double l_bp_faced; int l_bp_saved;
        double w_bp_faced; int w_bp_saved;
        io::CSVReader<16> csvin(train_file);
        csvin.read_header(io::ignore_extra_column, "surface", "tourney_date", "winner_name", 
        "loser_name", "score", "best_of", "minutes", "w_ace", "w_df", "w_bpSaved", "w_bpFaced", 
        "l_ace", "l_df", "l_bpSaved", "l_bpFaced", "winner_rank", "loser_rank");

        while (csvin.read_row(surface, winner_name, loser_name, score, date, time, w_ace, w_df,
        w_bp_saved, w_bp_faced, l_ace, l_df, l_bp_saved, l_bp_faced, w_rank, l_rank)) {
            // If somebody retired mid-match, then skip this match data
            if (score.find("RET") != std::string::npos) {
                continue;
            }
            
            // instantiate match data using Match class
            Match *m = new Match(surface, winner_name, loser_name, score, date, time, w_ace, 
            w_df, l_ace, l_df, w_rank, l_rank, w_bp_saved, w_bp_faced, l_bp_saved, l_bp_faced);
            // find players, if not found then create new Players
            auto winner = players.find(winner_name);
            auto loser = players.find(loser_name);
            if (winner == players.end()) {
                // create new Player
                Player p1(winner_name, w_rank);
                players.insert({winner_name, p1});
                winner = players.find(winner_name);
            }
            if (loser == players.end()) {
                // create new Player
                Player p2(loser_name, l_rank);
                players.insert({loser_name, p2});
                loser = players.find(loser_name);
            }
            
            // update Player metrics from Match data
            update_player_ELO(*m, winner->second, loser->second);
            
            delete m;
        }
    }

    // Winner_name (%) ---------|------ Loser_name (%)
    // 20 dashes in total, so round to nearest 5%
    void print_visual(const string &w_name, const string &l_name, double w_elo, double l_elo) {
        double total = w_elo + l_elo;
        double w_perc = (w_elo / total) * 100;
        double l_perc = (l_elo / total) * 100;
        cout << w_name << " (" << w_perc << "%) ";
        while (w_perc > 0) {
            cout << "-";
            w_perc -= 5;
        }
        cout << "|";
        while (l_perc > 0) {
            cout << "-";
            l_perc -= 5;
        }
        cout << l_name << " (" << (l_elo / total) * 100 << "%)" << endl;
    }
    

    void print_prediction(const string player1, const string player2, const string surface) {
        if (surface != "Hard" && surface != "Clay" && surface != "Grass") {
            throw SurfaceError();
        }

        auto p1 = players.find(player1);
        auto p2 = players.find(player2);
        if (p1 == players.end() || p2 == players.end()) {
            throw PlayerError();
        }

        // predict winner and print visual
        switch (surface[0]) {
            case 'H':
            {
                Player winner = (p1->second.elo_hard > p2->second.elo_hard) ? p1->second : p2->second;
                Player loser = (winner.name == player1) ? p2->second : p1->second;
                cout << "Winner : " << winner.name << endl;
                print_visual(winner.name, loser.name, winner.elo_hard, loser.elo_hard);
            }
            break;
            case 'C':
            {
                Player winner = (p1->second.elo_clay > p2->second.elo_clay) ? p1->second : p2->second;
                Player loser = (winner.name == player1) ? p2->second : p1->second;
                cout << "Winner : " << winner.name << endl;
                print_visual(winner.name, loser.name, winner.elo_clay, loser.elo_clay);
            }
            break;
            case 'G':
            {
                Player winner = (p1->second.elo_grass > p2->second.elo_grass) ? p1->second : p2->second;
                Player loser = (winner.name == player1) ? p2->second : p1->second;
                cout << "Winner : " << winner.name << endl;
                print_visual(winner.name, loser.name, winner.elo_grass, loser.elo_grass);
            }
            break;
        }//switch
    }
};

/*
argc = 5
argv[0] = ./main.exe
argv[1] = player1
argv[2] = player2
argv[3] = surface ("Hard", "Clay", "Grass")
argv[4] = training_data.csv
*/
int main(int argc, char *argv[]) {
    std::cout << std::setprecision(2);
    if (argc != 4) {
        cout << "Please enter the following information in the order below:\n" <<
        "Player1 Player2 Surface training_file.csv" << endl;
        return 1;
    }
    string training_file = argv[4];

    Prediction *p = new Prediction;
    p->train(training_file);

    string player1 = argv[1];
    string player2 = argv[2];
    string surface = argv[3];
    try {
        p->print_prediction(player1, player2, surface);
    }
    catch (SurfaceError &s) {
        cout << "Invalid surface" << endl;
    }
    catch (PlayerError &pe) {
        cout << "Please enter a valid player name" << endl;
    }
    

    delete p;
    return 0;
}