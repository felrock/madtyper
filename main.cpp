 #include <array>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <ncurses.h>
#include <thread>
#include <chrono>

using namespace std;
using namespace std::chrono;

const int num_words { 100 };
const int game_words { 20 };
std::array<std::string, num_words> words {
    "apple", "banana", "orange", "grape", "peach", "strawberry", "watermelon", "pineapple", "kiwi", "blueberry",
    "lemon", "lime", "cherry", "avocado", "raspberry", "blackberry", "mango", "papaya", "apricot", "fig",
    "grapefruit", "pomegranate", "cranberry", "pear", "plum", "cantaloupe", "honeydew", "coconut", "lychee", "guava",
    "tangerine", "nectarine", "persimmon", "kiwifruit", "passionfruit", "date", "elderberry", "gooseberry", "boysenberry",
    "starfruit", "durian", "jackfruit", "kumquat", "quince", "tamarind", "currant", "mandarin", "pomelo", "rambutan",
    "ugli fruit", "ackee", "breadfruit", "feijoa", "plantain", "soursop", "acerola", "jabuticaba", "maracuja", "maypop",
    "saskatoon", "bilberry", "loganberry", "loquat", "sapodilla", "surinam cherry", "carambola", "pitahaya", "pawpaw",
    "kaki", "medlar", "salal berry", "yumberry", "pepino", "cloudberry", "physalis", "camu camu", "kiwano", "tamarillo",
    "noni", "santol", "cupuaçu", "jaboticaba", "canistel", "longan", "santol", "feijoa", "açaí", "cupuaçu", "ambarella",
    "boysenberry", "cachew fruit", "cempedak", "durian", "feijoa", "gooseberry", "longan", "mangosteen", "muscadine",
    "pepino", "quince"
};

string generate_random_word() {
    return words[rand() % num_words];
}

bool check_word(const string& typed_word, const string& actual_word) {
    return typed_word == actual_word;
}

int main() {
    srand(time(0));
    int num_completed { 0 };
    high_resolution_clock::time_point start_time, end_time;

    start_color();
    init_pair(2, COLOR_RED, COLOR_BLACK);


    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    int row, col;
    getmaxyx(stdscr, row, col);


    for (int i{0}; i < 3; i++)
    {

        int x = (col - 38) / 2;
        move(row / 2 + 1, x);
        printw("Are you mad bro?! Game starts in %i", 3 - i);
        refresh();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        clear();
    }
    start_time = high_resolution_clock::now();

    for (int i{0}; i < game_words; ++i) {
        string word_to_type = generate_random_word();
        int x = (col - word_to_type.length()) / 2;
        move(row / 2 + 1, x);
        attron(A_DIM);
        printw("%s", word_to_type.c_str());
        attroff(A_DIM);
        refresh();
        string typed_word;
        int ch;
        while ((ch = getch()) != '\n') {
            if (ch == 127 || ch == 8 || ch == 263) {
                if (!typed_word.empty()) {
                    typed_word.pop_back();


                    move(row / 2 + 1, x);
                    attron(A_DIM);
                    printw("%s", word_to_type.c_str());
                    attroff(A_DIM);

                    move(row / 2 + 1, x);
                    attron(A_BOLD);
                    mvprintw(row / 2 + 1, x, "%s", typed_word.c_str());
                    attroff(A_BOLD);
                    refresh();
                }
            } else if (typed_word.size() < word_to_type.size()) {
                typed_word.push_back(ch);
                for (int j{0}; j < typed_word.length(); ++j) {
                    if (typed_word[j] != word_to_type[j]) {
                        attron(A_BOLD);
                        mvprintw(row / 2 + 1, x + j, "%c", typed_word[j]);
                        attroff(A_BOLD);
                    } else {
                        attron(COLOR_PAIR(2));
                        mvprintw(row / 2 + 1, x + j, "%c", typed_word[j]);
                        attroff(COLOR_PAIR(2));
                    }
                }
                refresh();
            }
        }
        clear();
        if (check_word(typed_word, word_to_type)) {
            num_completed++;
        }
    }
    end_time = high_resolution_clock::now();

    auto elapsed_time = duration_cast<milliseconds>(end_time - start_time).count();
    double wpm = (static_cast<double>(num_completed) / game_words) * 60 / (elapsed_time / 1000.0 / 60.0);

    int x = (col - 46) / 2;
    move(row / 2 + 1, x);
    printw("You completed %d out of %d curse words.\n", num_completed, game_words);
    move(row / 2 + 2, x);
    //printw("Your typing speed: %.2f words per minute (WPM)\n", wpm);
    //move(row / 2 + 3, x);

    refresh();
    getch();

    endwin();
    return 0;
}
