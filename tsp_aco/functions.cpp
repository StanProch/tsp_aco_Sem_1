#include "structures.h"
#include "functions.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <string>
constexpr double alpha = 1;
constexpr double beta = 5;
/**
 * @brief This Function open the file, reads the data from the file
 * and stores the data in the vector of points
 *
 * @param points special struct that stores the data of the points; id, x, y
 * @param inputFileName self-explanatory
 */
void openFile(std::vector<point>& points, const std::string& inputFileName) {
    std::ifstream inputFile(inputFileName);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        exit(1);
    }
    int ID;
    double x, y;
    while (inputFile >> ID >> x >> y) {
        points.push_back({ID, x, y});
    }
    inputFile.close();
}

/**
 * @brief This function initializes the pheromones with 1.0
 * this is only used at the beginning of the program
 * it is important to fill the matrix with values,
 * otherwise the program will not work properly
 *
 * @param NumberOfPoints this is the number of points in the file
 * @param PHEROMONES matrix of doubles, used for calculating better probabilities later
 */
void initialize_pheromones(int NumberOfPoints, double** PHEROMONES) {
    for (int i = 0; i < NumberOfPoints; i++) {
        for (int j = 0; j < NumberOfPoints; j++) {
            PHEROMONES[i][j] = 1.0;
        }
    }
}
/**
 * @brief This function calculates the distances between the points
 * and stores them in the matrix of distances, the matrix is symmetric
 *
 *
 * @param points vector of points, used for calculating the distances
 * @param DISTANCES matrix of doubles
 * @param NumberOfPoints self-explanatory
 */
void calculate_distances(const std::vector<point>& points, double** DISTANCES, int NumberOfPoints) {
    for (int i = 0; i < NumberOfPoints; i++) {
        for (int j = 0; j < NumberOfPoints; j++) {
            DISTANCES[i][j] = std::sqrt(std::pow(points[i].x - points[j].x, 2) + std::pow(points[i].y - points[j].y, 2));
        }
    }
}
/**
 * @brief Function to initialize the ant population, int this example ants will
 * start from the point 0, but it can be easily changed to start from a random point.
 * It's important to initialize the visited points and the path of the ants, -1 means that
 * point has not been visited yet. 1st point of path is set to the starting point.
 * Length of the path is set to 0.
 *
 * @param ants vector of ants, each ant has its own path, visited points, starting point and length of the path
 * @param NumberOfAnts
 * @param NumberOfPoints
 */
void initialize_ant_population(std::vector<ant>& ants, int NumberOfAnts, int NumberOfPoints) {
    for (int i = 0; i < NumberOfAnts; i++) {
        ants[i].starting_point = 0; // = rand() % NumberOfPoints;
        ants[i].visited_points = std::vector<int>(NumberOfPoints, 0);
        ants[i].path = std::vector<int>(NumberOfPoints + 1, -1);
        ants[i].visited_points[ants[i].starting_point] = 1;
        ants[i].path[0] = ants[i].starting_point;
        ants[i].length_of_path = 0;
    }
}

/**
 * @brief This function calculates the probability of visiting a next point. Formula is:
 * (pheromones on path a to b)^alpha * (1/distance from a to b)^beta / sum of all probabilities
 * alpha and beta are constants, we can change them to adjust the importance of either
 * pheromones or distances. If the sum of all probabilities is 0, then the probability of visiting
 * next point is 1/NumberOfPoints
 *
 * @param ant ant that is currently moving
 * @param a coordinate of the starting point
 * @param b coordinate of the next point
 * @param PHEROMONES
 * @param DISTANCES
 * @param NumberOfPoints
 * @return double The functions returns double value, that will later be normalized
 */
double probability_of_visiting(ant ant, int a, int b, double** PHEROMONES, double** DISTANCES, int NumberOfPoints) {
 double x = pow(PHEROMONES[a][b], alpha) * pow(1.0/DISTANCES[a][b], beta);
 double y = 0.0;
    for(int i = 0; i < NumberOfPoints; i++){
        if(ant.visited_points[i] == 0)
            y += pow(PHEROMONES[a][i], alpha) * pow(1.0/DISTANCES[a][i], beta);
    }
    if(y == 0)
        return 1.0/NumberOfPoints;
    else
        return x/y;
}

/**
 * @brief This Function selects the next point that the ant will visit.
 * It uses the probability_of_visiting function. Firstly it checks if the point has been visited
 * if not, it calculates the probability of visiting the point. Then it normalizes the probability
 * by dividing it by the sum of all probabilities. If sum of probabilities is 0, then function returns -1.
 * Then it generates a random number and checks if the cumulative probability is greater than the random number.
 *
 *
 *
 * @param a ant that is currently moving
 * @param PHEROMONES
 * @param DISTANCES
 * @param starting_point
 * @param NumberOfPoints
 * @return int function returns the index of the next point that the ant will visit
 */
int select_next_point(ant& a, double** PHEROMONES, double** DISTANCES, int starting_point, int NumberOfPoints) {
    double probability[NumberOfPoints] = {};
    double sum_of_probability = 0.0;
    double sum_of_standardized_probability = 0.0;

    for (int i = 0; i < NumberOfPoints; i++) {
        if (a.visited_points[i] == 0) {
            probability[i] = probability_of_visiting(a, starting_point, i, PHEROMONES, DISTANCES, NumberOfPoints);
            sum_of_probability += probability[i];
        }
    }
    for(int i = 0; i < NumberOfPoints; i++){
        probability[i] /= sum_of_probability;
        sum_of_standardized_probability += probability[i];
    }

    if (sum_of_standardized_probability == 0) {
        std::cout << "All points have been visited" << std::endl;
        return -1;
    }

    double random_number = (double)rand() / RAND_MAX;
    double cumulative_probability = 0.0;
    for (int i = 0; i < NumberOfPoints; i++) {
        cumulative_probability += probability[i];
        if(random_number < cumulative_probability){
            return i;
        }
    }

    return -1;
}
/**
 * @brief After all ants of given generation have visited all points, we need to update the pheromones.
 * This is important because now we can either reward or penalize the paths that the ants have taken.
 * The shorter the path, the more pheromones will be added to the path. Pheromones also evaporate over generation
 * so that the ants can explore new paths. Formula is : Q/L where Q is a constant and L is the length of the path.
 *
 * @param PHEROMONES
 * @param ants
 * @param NumberOfAnts
 * @param NumberOfPoints
 * @param EvaporationRate double value set by the user, when higher ants will explore more diversified paths, the lower the value, the ants will exploit the best path
 */
void update_pheromones(double** PHEROMONES, ant* ants, int NumberOfAnts, int NumberOfPoints, double EvaporationRate) {
    for (int i = 0; i < NumberOfPoints; i++) {
        for (int j = 0; j < NumberOfPoints; j++) {
            PHEROMONES[i][j] *= (1.0 - EvaporationRate);
        }
    }

    for (int i = 0; i < NumberOfAnts; i++) {
        if (ants[i].length_of_path > 0) {
            for (int j = 0; j < NumberOfPoints; j++) {
                int from = ants[i].path[j];
                int to = ants[i].path[j + 1];
                PHEROMONES[from][to] += 1.0 / ants[i].length_of_path;
                PHEROMONES[to][from] += 1.0 / ants[i].length_of_path;
            }
        }
    }
}
/**
 * @brief After all ants have traveled to every point, we need to decide which ant did the best job.
 * This function returns the best ant of the generation.
 * It simply compares the length of the path of each ant, shortest win!
 *
 * @param ants
 * @param NumberOfAnts
 * @return ant We return the ant type, it's later a lot easier to show ants path
 */
ant selecting_best_ant(ant* ants, int NumberOfAnts) {
    ant best_ant = ants[0];
    for (int i = 1; i < NumberOfAnts; i++) {
        if (ants[i].length_of_path < best_ant.length_of_path) {
            best_ant = ants[i];
        }
    }
    return best_ant;
}
/**
 * @brief This function creates output file and writes the best path of the best ant to the file.
 *
 * @param best_of_the_best as the name sugest, this one is realy the best !
 * @param outputFileName
 * @param NumberOfPoints
 */
void closeFile(ant best_of_the_best, const std::string& outputFileName, int NumberOfPoints) {
    std::ofstream outputFile(outputFileName);
    if (!outputFile.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return;
    }
    for (int i = 0; i <= NumberOfPoints; i++) {
        outputFile << best_of_the_best.path[i] << " ";
    }
    outputFile << std::endl << "Length of Path: " << best_of_the_best.length_of_path;
    outputFile.close();
}
