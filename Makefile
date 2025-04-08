# Author: AidonMaster
#
# Version: v-a-0.1-4-25
#
# The purpose of this file is not to be complex
# 	but just a helper for commands in our project.

CXX = g++

SRC_DIR = src
DIST_DIR = dist

MAIN = $(SRC_DIR)/game.cpp

TARGET = $(DIST_DIR)/app.exe

all:
	$(CXX) -o $(TARGET) $(MAIN)


run:
	$(TARGET)