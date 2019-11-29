#include <iostream>
#include <sstream>
#include <string>

struct love;
struct fifteen;
struct thirty;
struct fourty;
struct adv;
struct game;

struct love {
    constexpr static std::string_view score = "love";

    using win = fifteen;
    using lose = love;
};

struct fifteen {
    constexpr static std::string_view score = "fifteen";

    using win = thirty;
    using lose = fifteen;
};

struct thirty {
    constexpr static std::string_view score = "thirty";

    using win = fourty;
    using lose = thirty;
};

struct fourty {
    constexpr static std::string_view score = "fourty";

    using win = game;
    using lose = fourty;
    using deuce = adv;
};

struct adv {
    constexpr static std::string_view score = "adv";

    using win = game;
    using lose = fourty;
};

struct game {
    constexpr static std::string_view score = "game";

    using win = game;
    using lose = game;
};


template<typename ScoreP1, typename ScoreP2>
struct match_score {
    match_score() noexcept = default;

    template<typename OtherP1, typename OtherP2>
    match_score(const match_score<OtherP1, OtherP2>& other) noexcept {
        set_p1 = other.set_p1;
        set_p2 = other.set_p2;
        round = other.round + 1;
    }

    int set_p1 = 0;
    int set_p2 = 0;
    ScoreP1 game_score_p1{};
    ScoreP2 game_score_p2{};
    int round = 0;

    [[nodiscard]] std::string to_string() const noexcept {
        std::stringstream printer{};
        printer << "Round " << round << " -> ";
        printer << "Sets: " << set_p1 << " - " << set_p2 << "\n";
        printer << "Score: " << game_score_p1.score.data() << " - " << game_score_p2.score.data();
        return printer.str();
    }
};


template<typename ScoreP1, typename ScoreP2>
auto p1_scores(match_score<ScoreP1, ScoreP2> current_score) -> match_score<typename ScoreP1::win, typename ScoreP2::lose> {
    return match_score<typename ScoreP1::win, typename ScoreP2::lose>{current_score};
}

auto p1_scores(match_score<fourty, fourty> current_score) -> match_score<typename fourty::deuce, typename fourty::lose> {
    return match_score<typename fourty::deuce, typename fourty::lose>{current_score};
}

auto p1_scores(match_score<fourty, adv> current_score) -> match_score<fourty, typename adv::lose> {
    return match_score<fourty, typename fourty::lose>{current_score};
}

template<typename ScoreP1, typename ScoreP2>
auto p2_scores(match_score<ScoreP1, ScoreP2> current_score) -> match_score<typename ScoreP1::lose, typename ScoreP2::win> {
    return match_score<typename ScoreP1::lose, typename ScoreP2::win>{current_score};
}

auto p2_scores(match_score<fourty, fourty> current_score) -> match_score<typename fourty::lose, typename fourty::deuce> {
    return match_score<typename fourty::lose, typename fourty::deuce>{current_score};
}

auto p2_scores(match_score<adv, fourty> current_score) -> match_score<typename adv::lose, fourty> {
    return match_score<typename fourty::lose, fourty>{current_score};
}

template<typename ScoreP1, typename ScoreP2>
void check_game(match_score<ScoreP1, ScoreP2> current_score) {
    std::cout << current_score.to_string() << std::endl;
}

template<typename ScoreP2>
void check_game(match_score<game, ScoreP2> current_score) {
    ++current_score.set_p1;
    std::cout << current_score.to_string() << std::endl;
    std::exit(1);
}

template<typename ScoreP1>
void check_game(match_score<ScoreP1, game> current_score) {
    ++current_score.set_p2;
    std::cout << current_score.to_string() << std::endl;
    std::exit(2);
}

int main()
{
    match_score<love,love> score{};
    check_game(score);

    auto round_1 = p1_scores(score);
    check_game(round_1);

    auto round_2 = p1_scores(round_1);
    check_game(round_2);

    auto round_3 = p1_scores(round_2);
    check_game(round_3);

    auto round_4 = p2_scores(round_3);
    check_game(round_4);

    auto round_5 = p2_scores(round_4);
    check_game(round_5);

    auto round_6 = p2_scores(round_5);
    check_game(round_6);

    auto round_7 = p1_scores(round_6);
    check_game(round_7);

    auto round_8 = p2_scores(round_7);
    check_game(round_8);

    auto round_9 = p1_scores(round_8);
    check_game(round_9);

    auto round_10 = p1_scores(round_9);
    check_game(round_10);
    return 0;
}