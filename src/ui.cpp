#include <cursesw.h>
#include "tools.h"
#include <fstream>

int welcomeLoop()
{
    cbreak();
    
    int selected = 1, line_no = 0;
    bool confirmed = false;
    Menu start_menu("start_menu.txt");
    
    while (!confirmed)
    {
        start_menu.showWelcome(selected);
        refresh();
        int ch = getch();
        switch (ch)
        {
            case 'w':
            case KEY_UP:
                selected = max(0, selected - 1);
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
    Menu start_menu("pause.txt");
    
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
    
    ifstream fin(getExecutablePath() + "/../ui/tutorial.txt");
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
    
    ifstream fin(getExecutablePath() + "/../ui/high_scores.txt");
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
    vector<pair<string, int>> histories = getScoreRecords();
    if (histories.size() == 0)
        mvprintw(line_no, 49, "No score record yet.");
    for (int i = 0; i < min((size_t)10, histories.size()); i++)
    {
        mvprintw(line_no, 19, "%d", i + 1);
        mvprintw(line_no, 34, "%s", histories[i].first.c_str());
        mvprintw(line_no, 59, "%5d", histories[i].second);
        mvprintw(line_no, 84, "1");
        line_no++;
    }
    refresh();
    getch();
}
