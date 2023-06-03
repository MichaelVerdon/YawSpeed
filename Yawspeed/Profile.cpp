#include <string>
#include "Profile.h"
#include <fstream>
#include <iostream>

void Profile::setYawSpeed(int speed) {

	yawSpeed = speed;
}

void Profile::setLeftBind(std::string bind) {

	leftBind = bind;
}

void Profile::setRightBind(std::string bind) {

	rightBind = bind;
}

void saveFile(Profile profile) {

	std::ofstream outFile("CFG.txt");
	
	outFile << profile.yawSpeed << std::endl;
	outFile << profile.leftBind << std::endl;
	outFile << profile.rightBind << std::endl;

	outFile.close();
}

void loadFile(Profile &profile) {

	std::ifstream inFile("CFG.txt");

	int speed;
	std::string left;
	std::string right;

	inFile >> speed >> left >> right;
	profile.setYawSpeed(speed);
	profile.setLeftBind(left);
	profile.setRightBind(right);
}