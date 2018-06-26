#ifndef UTIL
#define UTIL

#include <utility>
#include <cmath>
#include <vector>
#include <fstream>

/**
 * Estructura para representar los pesos que se le asignan
 * a las características evaluadas para puntuar un tablero
*/
struct genome
{
	genome() {}
	genome(const std::vector<double> &v) : genic_values(v) {}

    /**
     * Caracteres evaluados por cada gen:
     * genes[0]-> Mi equipo está en posesión de la pelota
     * genes[1]-> La pelota está en posesión del equipo contrario
     * genes[2]-> La pelota está libre en el campo (ie, no la tiene ningún jugador)
     * genes[3]-> Si alguno de mis jugadores tiene la pelota, la distancia entre este
     *            y el arco rival
     * genes[4]-> La distancia entre los jugadores de mi equipo y la pelota
     * genes[5]-> La distancia entre mis jugadores y el jugador del equipo contrario
     *            en posesión de la pelota
     * genes[6]-> La distancia entre mis jugadores (dispersión)
     * genes[7]-> La distancia entre la pelota y el arco rival
     * genes[8]-> La distancia entre la pelota y mi arco
     */
    std::vector<double> genic_values = {1, -1, -0.1, -0.4, 0.46, -0.5, 1.0, 1.0, -1.0};
    //Algoritmo genético: std::vector<double> genic_values = {0.151093, -0.953879, 0.00476067, -0.751713, 0.371691, -0.38677, 0.204654, 0.74608, 0.851888};

};

struct genome_fitness {
    int games_played = 0;
    int games_won = 0;
    int games_lost = 0;
    int goals = 0;
    int opponent_goals = 0;

    bool compareFitnessByWonGames(const genome_fitness& other);
    bool compareFitnessByLostGames(const genome_fitness& other);

    bool operator>(const genome_fitness& other){
        //TODO fijarse como compararlos BIEN
        //c++ utiliza la lógica de cortocircuito
        bool ret = games_won > other.games_won;
        ret = ret || goals > other.goals;
        ret = ret || games_lost < other.games_lost;
        ret = ret || opponent_goals < other.opponent_goals;
        return ret;
    }
};

// Parecida al struct de Andy, pero mejor
struct game_series_info {
    int games_played = 0;
    int games_won_by_A = 0;
    int games_won_by_B = 0;
    int tied_games = 0;
    int points_scored_A = 0;
    int points_scored_B = 0;
};

/**
 * Para log
 */
std::ostream& operator<<(std::ostream &os, const genome_fitness& gf);

/**
 * Calcula la distancia entre dos puntos
 */
double distance(const std::pair<int,int>& t1, const std::pair<int,int>& t2);

/**
 * Genera el producto cartesiano entre todos los items de todos
 * un vectores de vectores.
 */
std::vector<std::vector<double>> cart_product(std::vector<std::vector<double>>& v);

/**
 * Dado un genoma, devuelve una matriz cuyas filas corresponden
 * a sus vecinos
 */
std::vector<std::vector<double>> getNeighborhood(genome &g);

/**
 * Dado un vector de pares (genoma, partidos ganados), devuelve alguno de los
 * elementos con más partidos ganados
 */
std::pair<genome, game_series_info> maximum(std::vector<std::pair<genome, game_series_info>> &v);

/**
 * Para imprimir en un log
 */
std::ostream& operator<<(std::ostream &os, const genome &g);

/**
 * Para debugear
 */
std::ostream& operator<<(std::ostream &os, const std::vector<std::vector<double>> &v);

/**
 * Loguea el genoma con una mejor visualización que un arreglo plano.
 * Para debug.
 */
void log(std::ostream& o, genome g);

/**
 * Struct para obtener los parametros de entrada
 * del programa de algoritmos geneticos: funciones de
 * fitness, crossover y seleccion.
 */
struct CliArguments {
    std::string fitnessMethod;
    std::string crossOverMethod;
    std::string selectionMethod;
};


#endif
