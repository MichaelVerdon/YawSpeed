#pragma once

#include <string>

class Profile {
public:
    int yawSpeed;
    std::string leftBind;
    std::string rightBind;

    void setYawSpeed(int speed);
    void setLeftBind(std::string bind);
    void setRightBind(std::string bind);

};

void saveFile(Profile profile);

void loadFile(Profile &profile);
