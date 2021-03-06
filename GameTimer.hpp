/* A encapsulated class to record the time and emit signal once a second. */

#ifndef GAMETIMER_H
#define GAMETIMER_H

#include <QTimer>

class GameTimer : public QTimer
{
    Q_OBJECT
private:
    QTimer * timer;
    long total_time;    // Record the total time of the game.
    static const int DayInterval;
public:
    explicit GameTimer(QObject *parent = nullptr);

    long GetTime();     // Return the total time now.
    // The `Date` do not take the difference in days of the month into account.
    struct Date
    {
        int day = 0;    // 1 ~ 30
        int month = 0;  // 1 ~ 12
        int year = 0;
    };
    static Date ConvertToDate(int day);
    void SetTime(long time);
signals:
    Timeout_3Month();
    InfectionOccur();
    TimeUpdate();
};
extern GameTimer *game_timer;

#endif // GAMETIMER_H
