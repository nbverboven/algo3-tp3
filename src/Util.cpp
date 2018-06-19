#include "Util.h"

/**
 * Calcula la distancia entre dos puntos
 */
double distance(const std::pair<int,int>& t1, const std::pair<int,int>& t2){
    double x = pow((std::get<0>(t2) - std::get<0>(t1)),2);
    double y = pow((std::get<1>(t2) - std::get<1>(t1)),2);
    double dis = std::sqrt(x + y);
    return dis;
}

