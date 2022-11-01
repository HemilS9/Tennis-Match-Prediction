#include <string>
using std::string;

const double MAX_INITIAL_RANKING = 100000;

class Player {
    private:
    const string name;
    string last_match_date;
    double elo_hard;
    double elo_clay;
    double elo_grass;
    int ranking;
    int recent_wins;
    
    friend class Prediction;

    public:
    Player(const string name_in, const int ranking_in)
    : name(name_in), ranking(ranking_in), recent_wins(10) {
        setELO(ranking_in);
    }

    void setELO(const int player_ranking) {
        double result = MAX_INITIAL_RANKING;
        result -= (ranking*100);
        elo_hard = result;
        elo_clay = result;
        elo_grass = result;
    }
};

class Match {
    private:
    string surface;
    string winner_name;
    string loser_name;
    string score;
    string date;
    double w_bp_conversion = 0;
    double l_bp_conversion = 0;
    int time;
    int w_ace, w_df;
    int l_ace, l_df;
    int w_rank, l_rank;
    uint8_t best_of;

    friend class Prediction;

    public:
    // surface, w_name, l_name, score, date, time, w_ace, w_df, l_ace, l_df, w_rank, l_rank, best_of
    Match(string s_in, string w_name_in, string l_name_in, string score_in,
    string date_in, int t_in, int w_ace_in, int w_df_in, int l_ace_in, int l_df_in,
    int w_rank_in, int l_rank_in, uint8_t bo, int w_bp_saved_in, double w_bp_faced_in, 
    int l_bp_saved_in, double l_bp_faced_in)
    : surface(s_in), winner_name(w_name_in), loser_name(l_name_in), score(score_in), date(date_in),
    time(t_in), w_ace(w_ace_in), w_df(w_df_in), l_ace(l_ace_in), l_df(l_df_in),
    w_rank(w_rank_in), l_rank(l_rank_in), best_of(bo) {
        setWinnerBPConversion(l_bp_faced_in, l_bp_saved_in);
        setLoserBPConversion(w_bp_faced_in, w_bp_saved_in);
    }

    void setWinnerBPConversion(double l_bp_faced, int l_bp_saved) {
        if (l_bp_faced > 0) {
            w_bp_conversion = (l_bp_faced - l_bp_saved) / l_bp_faced;
        }
    }

    void setLoserBPConversion(double w_bp_faced, int w_bp_saved) {
        if (w_bp_faced > 0) {
            l_bp_conversion = (w_bp_faced - w_bp_saved) / w_bp_faced;
        }
    }
};