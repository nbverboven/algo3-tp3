#include "Util.h"

/**
 * Calcula la distancia entre dos puntos
 */
int distance(const std::pair<int,int>& t1, const std::pair<int,int>& t2){
    int x = (std::get<0>(t2) - std::get<0>(t1))^2;
    int y = (std::get<1>(t2) - std::get<1>(t1))^2;
    int dis = std::sqrt(x - y);
    return dis;
}

