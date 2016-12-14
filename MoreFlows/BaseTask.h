#pragma once

class BaseTask
{
public:
    virtual void solve() = 0;
private:
    virtual void read() = 0;
    virtual void printAnswer() {}
};
