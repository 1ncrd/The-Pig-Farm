#include "GameTimer.hpp"
#include <QDebug>
#include "MyRandom.hpp"

GameTimer *game_timer = new GameTimer;

const int GameTimer::DayInterval = 1000;    // millisecond(s).
GameTimer::GameTimer(QObject *parent)
    : QTimer{parent}
{
    total_time = 0;
    this -> setInterval(DayInterval);
    connect(this, QTimer::timeout, this, [&]()
    {
        if ((total_time + 1) % 90 == 0)
        {
            emit Timeout_3Month();
        }

        total_time++;
    });
}

long GameTimer::GetTime()
{
    return this -> total_time;
}

GameTimer::Date GameTimer::ConvertToDate(int day) // 刚开始写的，然而后来并没有用到过，哈哈 ^ ^
{
    int day_temp = day % 30 + 1;
    int month_temp = (day / 30) % 12 + 1;
    int year_temp = day / 360 + 1;
    Date date_temp{day_temp, month_temp, year_temp};
    return date_temp;
}

void GameTimer::SetTime(long time)
{
    this -> total_time = time;
    emit TimeUpdate();
}
