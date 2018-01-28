#include "StepRobot.h"

int main() {
    RobotWithCondvars rc;
    rc.start();
    std::cout << "RC finished" << std::endl;

    RobotWithSemaphores rs;
    rs.start();
    std::cout << "RS finished" << std::endl;

    return 0;
}