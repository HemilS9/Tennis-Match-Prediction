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

class PlayerError {};

class Prediction {
    private:
    string surface;
    const int max_update = 500;

    int find_num_sets(const string &score) {
        int num = 0;
        for (auto c : score) {
            if (c == '-') {
                ++num;
            }
        }
        return num;
    }

    // Output score_update given the score, num sets, and max update
    double find_score_update(const string &score, double max_score_update, int num_sets) {
        vector<int> sets;
        sets.reserve(static_cast<size_t>(num_sets));
        int counter = 0;
        sets.push_back(0);
        for (char c : score) {
            if (c == ' ')
                sets.push_back(counter +1);
            ++counter;
        }
        
        double result = 0;
        double max_per_set = max_score_update / num_sets;
        for (int i = 0; i < num_sets; ++i) {
            int set_ix = sets[i];
            int first = (score[set_ix]) - '0';
            int second = (score[set_ix +2]) - '0';

            // If winner won set
            if (first > second) {
                // If tiebreaker, assign max/10 points
                if (score[set_ix + 2] == '6') {
                    result += max_per_set / 10;
                }
                else {
                    result += ((6 - second) / 6) * max_per_set;
                }
            }
            else {
                // If tiebreaker, assign max/10 points
                if (score[set_ix] == '6') {
                    result += max_per_set / 10;
                }
                else {
                    result += (first / 6) * max_per_set;
                }
            }
        }
        return result;
    }

    double equal_updates(const Match &m, const Player &winner, const Player &loser) {
        // Score, Rank_Diff, Time
        double rank_diff_update = 0.20*max_update, time_update = 0.2*max_update;
        int num_sets = find_num_sets(m.score);
        int minutes = m.time;
        
        double score_update = find_score_update(m.score, 0.30*max_update, num_sets);

        // update time according to Bo5
        if (m.best_of == 5) {
            // Lower cutoff: 90 mins and Upper cutoff: 270 mins
            double change = (0.1*max_update) / 180;
            if (minutes > 270) 
                time_update = 0;
            else {
                while (minutes > 90) {
                    --minutes;
                    time_update -= change;
                }
            }
        }
        // update time according to Bo3
        else {
            // Lower cutoff: 50 mins and Upper cutoff: 170 mins
            double change = (0.1*max_update) / 120;
            if (minutes > 170) 
                time_update = 0;
            else {
                while (minutes > 50) {
                    --minutes;
                    time_update -= change;
                }
            }
        }
        // Update rank_diff. The bigger the upset, the larger the value
        // 30 ranking points is the cutoff for biggest upset
        int upset = winner.ranking - loser.ranking;
        if (upset > 0 && upset < 30)
            rank_diff_update = (upset * 0.2 * max_update) / 30;
        else if (upset < 0 && upset >= -8)  
            rank_diff_update *= 0.25;
        else if (upset < -5)
            rank_diff_update = 0.1;

        return score_update + time_update + rank_diff_update;
    }

    // Ace: Upper cutoff is 15 in Bo3, 20 in Bo5
    // Upper onwards = full for winner, and 0 for loser
    double ace_update(const Match &m, const Player &p, bool winner) {
        double max = 0.1*max_update;
        if (m.best_of == 5) {
            if (winner) {
                if (m.w_ace < 20) 
                    return (m.w_ace * max) / 20;
                else {
                    return max;
                }
            }
            else {
                if (m.l_ace >= 20) 
                    return 0;
                else {
                    return max - ((m.l_ace * max) / 20);
                }
            }
        }
        else {
            if (winner) {
                if (m.w_ace < 15) 
                    return (m.w_ace * max) / 15;
                else {
                    return max;
                }
            }
            else {
                if (m.l_ace >= 15) 
                    return 0;
                else {
                    return max - ((m.l_ace * max) / 15);
                }
            }
        }
    }

    // DF: Upper cutoff is 12 in Bo3, 16 in Bo5
    // Upper onwards = 0 for winner, and full value for loser 
    double df_update(const Match &m, const Player &p, bool winner) {
        double max = 0.1*max_update;
        if (m.best_of == 5) {
            if (winner) {
                if (m.w_df < 16) 
                    return max - ((m.w_df * max) / 16);
                else {
                    return 0;
                }
            }
            else {
                if (m.l_df >= 16) 
                    return max;
                else {
                    return (m.l_df * max) / 16;
                }
            }
        }
        else {
            if (winner) {
                if (m.w_df < 12) 
                    return max - ((m.w_df * max) / 12);
                else {
                    return 0;
                }
            }
            else {
                if (m.l_df >= 12) 
                    return max;
                else {
                    return (m.l_df * max) / 12;
                }
            }
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
        cout << " " << l_name << " (" << (l_elo / total) * 100 << "%)" << endl;
    }

    // Remove spaces from the input string
    // string no_spaces(const std::string &name) {
    //     string result = "";
    //     for (char c : name) {
    //         if (c != ' ') {
    //             result.push_back(c);
    //         }
    //     }
    //     return result;
    // }


    public:

    unordered_map<string, Player> players; // Key: player name | Value: Player info

    // static void parse_time(const Match &m, int &year, int &month, int &day) {
    //     year = stoi(m.date.substr(0, 4));
    //     month = stoi(m.date.substr(4, 2));
    //     day = stoi(m.date.substr(6, 2));
    // }

    void update_player_ELO(const Match &m, Player &winner, Player &loser) {
        winner.ranking = m.w_rank;
        loser.ranking = m.l_rank;
        double winner_update = 0, loser_update = 0;
        double first_update = equal_updates(m, winner, loser);

        winner_update = first_update;
        loser_update = first_update;

        winner_update = first_update + ace_update(m, winner, true) + df_update(m, winner, true) 
                        + (m.w_bp_conversion * 0.1 * max_update);
        loser_update = first_update + ace_update(m, loser, false) + df_update(m, loser, false) 
                        + (max_update - (m.l_bp_conversion * 0.1 * max_update));

        switch(m.surface[0]) {
            case 'H':
                winner.elo_hard += winner_update;
                loser.elo_hard -= loser_update;
                break; 
            case 'C':
                winner.elo_clay += winner_update;
                loser.elo_clay -= loser_update;
                break;
            case 'G':
                winner.elo_grass += winner_update;
                loser.elo_grass -= loser_update;
                break;
        }
    }

    // https://github.com/ben-strasser/fast-cpp-csv-parser
    void train(string &train_file) {
        string surface, score, date;
        string winner_name, loser_name;  
        int time;
        int w_rank, l_rank;
        int w_ace, l_ace;
        int w_df, l_df;
        double l_bp_faced, w_bp_faced;
        int l_bp_saved, w_bp_saved;
        uint8_t best_of;

        io::CSVReader<17> csvin(train_file);
        csvin.read_header(io::ignore_extra_column, "surface", "tourney_date", "winner_name",
        "loser_name", "score", "best_of", "minutes", "w_ace", "w_df", "w_bpSaved", 
        "w_bpFaced", "l_ace", "l_df", "l_bpSaved", "l_bpFaced", "winner_rank", "loser_rank");

        while (csvin.read_row(surface, date, winner_name, loser_name, score, best_of, time, 
        w_ace, w_df, w_bp_saved, w_bp_faced, l_ace, l_df, l_bp_saved, l_bp_faced, w_rank, l_rank)) {
            // If somebody retired mid-match, then skip this match data
            if (score.find("RET") != std::string::npos) {
                continue;
            }
            
            // instantiate match data from csv data
            Match *m = new Match(surface, winner_name, loser_name, score, date, time, w_ace, w_df,
            l_ace, l_df, w_rank, l_rank, best_of, w_bp_saved, w_bp_faced, l_bp_saved, l_bp_faced);

            // find players, if not found then create new Players
            auto winner = players.find(winner_name);
            auto loser = players.find(loser_name);
            if (winner == players.end()) {
                Player p1(winner_name, w_rank);
                players.insert({winner_name, p1});
                winner = players.find(winner_name);
            }
            if (loser == players.end()) {
                Player p2(loser_name, l_rank);
                players.insert({loser_name, p2});
                loser = players.find(loser_name);
            }
            
            update_player_ELO(*m, winner->second, loser->second);
            
            delete m;
        }
    }

    // Player predict(const string &player1, const string &player2, char surface) {
    //     // Check if player names are valid
    //     auto p1 = players.find(player1);
    //     auto p2 = players.find(player2);
    //     if (p1 == players.end() || p2 == players.end()) {
    //         throw PlayerError();
    //     }

    //     switch (surface) {
    //         case 'H':

    //         case 'C':

    //         case 'G':
    //     }
    // }
    

    void print_prediction(const string player1, const string player2, const string surface) {
        // Check if player names are valid
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
        }
    }

    bool validate_surface(const string &surface) {
        if (surface != "Hard" && surface != "Clay" && surface != "Grass") {
            return false;
        }
        return true;
    }
};

/*
argc = 5
argv[0] = ./predict.exe
argv[1] = player1
argv[2] = player2
argv[3] = surface ("Hard", "Clay", "Grass")
argv[4] = training_data.csv
*/
int main(int argc, char *argv[]) {
    std::cout << std::setprecision(3);
    if (argc != 5) {
        cout << "Please enter the following information in the order below:\n" <<
        "\"Player1\" \"Player2\" Surface training_file.csv\n"
        << "Note:\n- Please refer to the input dataset for full player names and spelling\n"
        << "- The surface must be one of the following: Hard, Clay, Grass" << endl;
        return 1;
    }
    string player1 = argv[1];
    string player2 = argv[2];
    string surface = argv[3];
    string training_file = argv[4];

    Prediction *p = new Prediction;
    if (!p->validate_surface(surface)) {
        cout << "Invalide surface\n"
        << "The surface must be one of the following: Hard, Clay, Grass" << endl;
        return 1;
    }
    p->train(training_file);

    try {
        p->print_prediction(player1, player2, surface);
    }
    catch (PlayerError &pe) {
        cout << "Please enter a valid player name.\n" <<
        "When entering player names in the command line, ensure they are inside quotes." << endl;
    }
    
    delete p;
    return 0;
}