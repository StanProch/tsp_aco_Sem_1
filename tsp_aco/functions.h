#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <vector>
#include <string>
#include "structures.h"

void openFile(std::vector<point>& points, const std::string& inputFileName);

void closeFile(ant best_of_the_best, const std::string& outputFileName, int NumberOfPoints);

void initialize_pheromones(int NumberOfPoints, double** PHEROMONES);

void calculate_distances(const std::vector<point>& points, double** DISTANCES, int NumberOfPoints);

void initialize_ant_population(std::vector<ant>& ants, int NumberOfAnts, int NumberOfPoints);

double probability_of_visiting(ant ant1, int a, int b, double** PHEROMONES, double** DISTANCES, int NumberOfPoints);

int select_next_point(ant& a, double** PHEROMONES, double** DISTANCES, int starting_point, int NumberOfPoints);

void update_pheromones(double** PHEROMONES, ant* ants, int NumberOfAnts, int NumberOfPoints, double EvaporationRate);

ant selecting_best_ant(ant* ants, int NumberOfAnts);
#endif //FUNCTIONS_H
