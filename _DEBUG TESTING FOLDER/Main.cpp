#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

#include "Noise Algorithms/PRNG.h"
#include "Noise Algorithms/Perlin.h"

int main() {
    // Set the seed for reproducibility
    Noise::Perlin::setSeed(42);

    // Define the range and resolution of the noise values
    double startX = 0.0, endX = 2.5, stepX = 0.001;
    double startY = 0.0, endY = 2.5, stepY = 0.001;
    double z = 1.0; // You can choose any z value you like

    std::ofstream outFile("plot.csv");
    if (!outFile) {
        std::cerr << "Error opening file for writing!" << std::endl;
        return 1;
    }

    for (double x = startX; x <= endX; x += stepX) {
        for (double y = startY; y <= endY; y += stepY) {
            double noiseValue = Noise::Perlin::perlin(x, y, z);
            outFile << x << "," << y << "," << noiseValue << std::endl;
        }
    }

    outFile.close();

    double lenX = (endX-startX)/stepX;
    double lenY = (endY-startY)/stepY;

    std::cout << "Perlin noise values saved to plot.csv with " << lenX*lenY << " (" <<  lenX << "," << lenY << ") parameters" << std::endl;

    return 0;
}