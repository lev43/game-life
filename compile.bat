g++ -o game -Wall -I ./module quad.cpp ./module/*.cpp -lX11 -lGL -lGLU -pthread && ./game || echo 'Error'