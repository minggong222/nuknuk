# Makefile for game project

# 컴파일러 및 플래그
CXX = g++
CXXFLAGS = -Wall -std=c++17
LDFLAGS = -lncurses

# 소스 및 타겟
SRC = nuknuk.cpp
TARGET = nuknuk

# 기본 타겟
all: $(TARGET)

# 실행 파일 생성
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

# 정리
clean:
	rm -f $(TARGET)

