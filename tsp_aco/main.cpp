#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include "structures.h"
#include "functions.h"

constexpr int NumberOfIterations = 20;

int main(int argc, const char* argv[]) {
    std::string inputFileName;
    std::string outputFileName;
    int NumberOfAnts = 0;
    double EvaporationRate = 0.0;
    std::cout << "For Help type -h!\n";
    for (int i = 0; i < argc; i++) {
        std::string argument = argv[i];
        if (argument == "-i")
            inputFileName = argv[i + 1];
        else if (argument == "-o")
            outputFileName = argv[i + 1];
        else if (argument == "-g")
            NumberOfAnts = std::atoi(argv[i + 1]);
        else if (argument == "-f")
            EvaporationRate = std::atof(argv[i + 1]);
        else if (argument == "-h")
            std::cout << "(-i) input file name ; (-o) output file name ; (-g) number of ants; (-f) evaporation rate";
    }

    std::vector<point> points;
    openFile(points, inputFileName);

    int NumberOfPoints = points.size();

    double** DISTANCES = new double* [NumberOfPoints];
    double** PHEROMONES = new double* [NumberOfPoints];
    for (int i = 0; i < NumberOfPoints; i++) {
        DISTANCES[i] = new double[NumberOfPoints];
        PHEROMONES[i] = new double[NumberOfPoints];
    }
    initialize_pheromones(NumberOfPoints, PHEROMONES);
    calculate_distances(points, DISTANCES, NumberOfPoints);

    std::vector<ant> ants(NumberOfAnts);


    ant best_ant, best_of_the_best;
    /*Initialize best_of_the_best with a very high value, it will be easly beat*/
    best_of_the_best.length_of_path = 1000000000.0;
    for (int i = 0; i < NumberOfIterations; i++) {
        /**
         * @brief Construct a new ant population, it's important that ant are ready to go!
         */
        initialize_ant_population(ants, NumberOfAnts, NumberOfPoints);
        for (int j = 0; j < NumberOfAnts; j++) {
            int starting_point = ants[j].starting_point;

            for (int k = 1; k < NumberOfPoints; k++) {
                /**
                 * @brief We need to select the next point that the ant will visit,
                 * we use our functions to do so.
                 */
                int next_point = select_next_point(ants[j], PHEROMONES, DISTANCES, starting_point, NumberOfPoints);
                if (next_point == -1) break;
                ants[j].path[k] = next_point;
                ants[j].visited_points[next_point] = 1;
                ants[j].length_of_path += DISTANCES[starting_point][next_point];
                starting_point = next_point;
            }
            // Return to starting point
            ants[j].path[NumberOfPoints] = ants[j].starting_point;
            ants[j].length_of_path += DISTANCES[starting_point][ants[j].starting_point];
        }

        /**
         * @brief After each ant has visited all points, we need to update the pheromones.
         *
         */
        update_pheromones(PHEROMONES, ants.data(), NumberOfAnts, NumberOfPoints, EvaporationRate);

        best_ant = selecting_best_ant(ants.data(), NumberOfAnts);
        if(best_ant.length_of_path < best_of_the_best.length_of_path){
            best_of_the_best = best_ant;
        }

        /**
         * @brief At the end of each generation we print the best path length and the best path.
         *
         */
        std::cout << "Generation " << i << " Best Path Length: " << best_ant.length_of_path << std::endl;
        std::cout << "Best Path: ";
        for (int k = 0; k <= NumberOfPoints; k++) {
            std::cout << best_ant.path[k] << " ";
        }
        std::cout << std::endl;
        std::cout << "-------------------------------------------------" << std::endl;
    }

    closeFile(best_of_the_best, outputFileName, NumberOfPoints);

    for (int i = 0; i < NumberOfPoints; i++) {
        delete[] DISTANCES[i];
        delete[] PHEROMONES[i];
    }
    delete[] DISTANCES;
    delete[] PHEROMONES;

    return 0;
}
