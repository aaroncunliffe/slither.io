#pragma once


// the application time based timer
class Timer
{
public:
    // initializes variables
    Timer();

    // the various clock actions
    void start();
    void stop();
    void pause();
    void unpause();

    // gets the timer's time
    Uint32 getTicks();

    // checks the status of the timer
    bool isStarted();
    bool isPaused();

private:
    // the clock time when the timer started
    Uint32 startTicks;

    // the ticks stored when the timer was paused
    Uint32 pausedTicks;

    // the timer status
    bool paused;
    bool started;
};