#include <cursesw.h>
#include "tools.h"
#include <fstream>
#include <filesystem>

int config()
{
    ifstream fin(getExecutablePath() + "/../data/config.txt");
    bool ascii_saved;
    if (!fin.fail())
    {
        if (fin >> ascii_saved)
            return ascii_saved ? 1 : 0;
    }
    fin.close();

    cbreak();
    
    int selected = 3, line_no = 0;
    bool confirmed = false;
    Menu config_menu("config.txt");

    while (!confirmed)
    {
        config_menu.showWelcome(selected);
        refresh();
        int ch = getch();
        switch (ch)
        {
            case 'w':
            case KEY_UP:
                selected = max(3, selected - 1);
                break;
            case 's':
            case KEY_DOWN:
                selected = min(4, selected + 1);
                break;
            case ' ':
            case '\n':
                confirmed = true;
                break;
        }
    }
    ofstream fout(getExecutablePath() + "/../data/config.txt");
    fout << selected - 3 << endl;
    fout.close();
    return selected - 3;
}

int welcomeLoop()
{
    cbreak();
    string filename = ascii ? "start_menu_ascii.txt" : "start_menu.txt";
    int selected = 1, line_no = 0;
    bool confirmed = false, saved_data_exists = false;
    Menu start_menu(filename);

    if (filesystem::exists(getExecutablePath() + "/../data/temp.txt"))
        saved_data_exists = true;
    
    while (!confirmed)
    {
        start_menu.showWelcome(selected);
        if (!saved_data_exists)
        {
            for (int i = 14; i <= 16; i++)
                mvprintw(i, 45, "                                        ");
            mvprintw(32, 0, " ");
        }

        refresh();
        int ch = getch();
        switch (ch)
        {
            case 'w':
            case KEY_UP:
                selected = max(saved_data_exists ? 0 : 1, selected - 1);
                break;
            case 's':
            case KEY_DOWN:
                selected = min(4, selected + 1);
                break;
            case ' ':
            case '\n':
                confirmed = true;
                break;
        }
    }
    return selected;
}

int pauseLoop()
{
    int selected = 1, line_no = 0;
    bool confirmed = false;
    string filename1 = ascii ? "pause_ascii.txt" : "pause.txt";
    Menu start_menu(filename1);
    
    while (!confirmed)
    {
        start_menu.showWelcome(selected);
        refresh();
        int ch = getch();
        switch (ch)
        {
            case 'w':
            case KEY_UP:
                selected = max(1, selected - 1);
                break;
            case 's':
            case KEY_DOWN:
                selected = min(4, selected + 1);
                break;
            case ' ':
            case '\n':
                confirmed = true;
                break;
            case 'p':
                selected = 1;
                confirmed = true;
                break;
        }
    }
    return selected;
}

void showTutorial()
{
    cbreak();
    nodelay(stdscr, false);
    string filename2 = ascii ? "tutorial_ascii.txt" : "tutorial.txt";
    filename2 = "/../ui/" + filename2;
    ifstream fin(getExecutablePath() + filename2);
    if (fin.fail())
        cout << "Error opening file." << endl;
    
    string line;
    int line_no = 0;
    while (getline(fin, line))
    {
        mvprintw(line_no, 0, "%s", line.c_str());
        line_no++;
    }
    refresh();
    getch();
}

void showHighScore()
{
    cbreak();
    nodelay(stdscr, false);
    string filename3 = ascii ? "high_scores_ascii.txt" : "high_scores.txt";
    filename3 = "/../ui/" + filename3;
    ifstream fin(getExecutablePath() + filename3);
    if (fin.fail())
        cout << "Error opening file." << endl;
    
    string line;
    int line_no = 0;
    while (getline(fin, line))
    {
        mvprintw(line_no, 0, "%s", line.c_str());
        line_no++;
    }
    line_no = 15;
    vector<ScoreRecord> histories = getScoreRecords();
    if (histories.size() == 0)
        mvprintw(line_no, 49, "No score record yet.");
    for (int i = 0; i < min((size_t)10, histories.size()); i++)
    {
        mvprintw(line_no, 22, "%d", i + 1);
        mvprintw(line_no, 37, "%s", histories[i].username.c_str());
        mvprintw(line_no, 62, "%5d", histories[i].score);
        mvprintw(line_no, 87, "%d", histories[i].level);
        line_no++;
    }
    refresh();
    getch();
}
