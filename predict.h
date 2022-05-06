#include <string>

/*
NOTES

Parameters to use in training (to modify ELO score for players):
- score (25%)
- diff in world rank (35%)
- time taken (10%)
############ Above is same, below is separate by player ############
- aces (10%)
- double faults (10%)
- break point conversion % (10%)

Parameters to use for prediction(Take ELO score from training and modify with below):
- H2H: calculate w/o H2H, then multiply by 0.9 and put 0.1 weight on H2H

*/

class Player {
    private:
    const std::string name;
    double elo_hard;
    double elo_clay;
    double elo_grass;
    int ranking;
    
    friend class Prediction;

    public:
    Player(const std::string name_in, const int ranking_in)
    :name(name_in), ranking(ranking_in) {
        setELO(ranking_in);
    }

    void setELO(const int player_ranking) {
        double result = 100000;
        result -= (ranking*100);
        elo_hard = result;
        elo_clay = result;
        elo_grass = result;
    }
};

class Match {
    private:
    std::string surface;
    std::string winner_name;
    std::string loser_name;
    std::string score;
    std::string date;
    int time;
    int w_ace, w_df;
    int l_ace, l_df;
    int w_rank, l_rank;
    double w_bp_conversion;
    double l_bp_conversion;
    char tourney_level;

    friend class Prediction;

    public:
    // surface, w_name, l_name, score, date, time, w_ace, w_df, l_ace, l_df, w_rank, l_rank
    Match(std::string s_in, std::string w_name_in, std::string l_name_in, std::string score_in,
    std::string date_in, int t_in, int w_ace_in, int w_df_in, int l_ace_in, int l_df_in, int w_rank_in, 
    int l_rank_in, int w_bp_saved_in, double w_bp_faced_in, int l_bp_saved_in, double l_bp_faced_in, char t_lvl)
    : surface(s_in), winner_name(w_name_in), loser_name(l_name_in), score(score_in), date(date_in),
    time(t_in), w_rank(w_rank_in), l_rank(l_rank_in), w_ace(w_ace_in), l_ace(l_ace_in), 
    w_df(w_df_in), l_df(l_df_in), tourney_level(t_lvl) {
        setWinnerBPConversion(l_bp_faced_in, l_bp_saved_in);
        setLoserBPConversion(w_bp_faced_in, w_bp_saved_in);
    }

    void setWinnerBPConversion(double l_bp_faced, int l_bp_saved) {
        if (l_bp_faced == 0) {
            w_bp_conversion = 0;
        }
        else {
            w_bp_conversion = (l_bp_faced - l_bp_saved) / l_bp_faced;
        }
    }

    void setLoserBPConversion(double w_bp_faced, int w_bp_saved) {
        if (w_bp_faced == 0) {
            l_bp_conversion = 0;
        }
        else {
            l_bp_conversion = (w_bp_faced - w_bp_saved) / w_bp_faced;
        }
    }
};