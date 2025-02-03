Traveling Salesman Problem with Ant Colony Optimisation by Stanisław Prochowski

To run this code please insert in terminal:
g++ main.cpp functions.cpp -o tsp_aco
./tsp_aco -i (input_file) -o (output_file) -g (number_of_ants) -f (evaporation_rate)

Example of commends
g++ main.cpp functions.cpp -o tsp_aco
./tsp_aco -i points1.txt -o results.txt -g 10 -f 0.1
./tsp_aco -i points2.txt -o results.txt -g 20 -f 0.5
./tsp_aco -i points3.txt -o results.txt -g 30 -f 0.3

Each exemplary file contains different amount of points.
If you wish to adjust ant colony optimisation path-finding, change alpha or beta const variable.
    Alpha   - Importance of pheromones
    Beta    - Importance of distance
The amount of generations is easily changeable at the star of main.cpp
