#include "Util.h"

double distance(const std::pair<int,int>& t1, const std::pair<int,int>& t2) {
    double x = pow((t2.second - t1.second),2);
    double y = pow(t2.first - t1.first, 2);
    double dis = std::sqrt(x + y);
    return dis;
}


std::vector<std::vector<double>> cart_product(std::vector<std::vector<double>>& v) {
    std::vector<std::vector<double>> s = {{}};
    for (auto& u : v) {
        std::vector<std::vector<double>> r;
        for (auto& x : s) {
            for (auto y : u) {
                r.push_back(x);
                r.back().push_back(y);
            }
        }
        s.swap(r);
    }
    return s;
}

std::vector<std::vector<double>> getNeighborhood(genome &g) {
    std::vector<std::vector<double>> elInput;
    double granularity = 0.2;
    for (double gene_value : g.genic_values) {
    	/* Chequeo los casos borde para que los valores resultantes
    	   no salgan del rango [0, 1] */
    	if (gene_value-granularity < -1) {
    		elInput.push_back({-1, gene_value, gene_value+granularity});
    	}
    	else if (gene_value+granularity > 1) {
    		elInput.push_back({gene_value-granularity, gene_value, 1});	
    	}
    	else {
		    elInput.push_back({gene_value-granularity, gene_value, gene_value+granularity});
    	}
    }
    // Genero los vecinos sin incluir al genoma que paso como parámetro
    std::vector<std::vector<double>> result = cart_product(elInput);
    for (unsigned int i = 0; i < result.size(); ++i) {
    	if (result[i] == g.genic_values) {
    		result[i] == result[result.size()-1];
    		result.pop_back();
    	}
    }
    // Podo un poco para no recorrer miles de posibles vecinos
    std::vector<std::vector<double>> result2;
    for (unsigned int i = 0; i < result.size(); i+=50) {
        result2.push_back(result[i]);
    }
    return result2;
}

std::pair<genome, game_series_info> maximum(std::vector<std::pair<genome, game_series_info>> &v) {
	unsigned int max_index = 0;
	unsigned int i = 1;
	while (i < v.size()) {
		if (v[i].second.games_won_by_B > v[max_index].second.games_won_by_B) {
			max_index = i;
		}
		++i;
	}
	return v[max_index];
}

std::ostream& operator<<(std::ostream &os, const genome &g) {
	os << '[';
	for (unsigned int i = 0; i < g.genic_values.size(); ++i) {
		os << g.genic_values[i];
		if (i < g.genic_values.size()-1) {
			os << ", ";
		}
	}
	os << ']';
	return os;
}

std::ostream& operator<<(std::ostream &os, const std::vector<std::vector<double>> &v) {
	os << '[';
	for (unsigned int i = 0; i < v.size(); ++i) {
		os << '[';
		for (unsigned int j = 0; j < v[i].size(); ++j) {
			os << v[i][j];
			if (j < v[i].size()-1) {
				os << ", ";
			}
		}
		os << ']' << std::endl;
	}
	os << ']';
	return os;
}

std::ostream& operator<<(std::ostream &os, const genome_fitness& gf) {
    os << "genome_fitness {" << std::endl;
    os << " games_played: " << gf.games_played << std::endl;
    os << " games_won: " << gf.games_won << std::endl;
    os << " games_lost: " << gf.games_lost << std::endl;
    os << " goals: " << gf.goals << std::endl;
    os << " opponent_goals: " << gf.opponent_goals << std::endl;
    os << "}" << std::endl;
    return os;
}