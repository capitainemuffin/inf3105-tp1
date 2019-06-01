#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cassert>

class Rectangle {

public:

	enum class Type { positif, negatif };

	Type type;
	double x;
	double y;
	double longueur;
	double hauteur;

	friend std::istream& operator >> (std::istream& is, Rectangle& rectangle);

};

class Grille{

private:

	std::vector<Rectangle> rectangles;

public:

	friend std::istream& operator >> (std::istream& is, Grille& grille);

	void ajouter(Rectangle& rectangle){

		this->rectangles.push_back(rectangle);
	}

	long double aire(){

		long double aire = 0;

		return aire;
	}

	double perimetre(){

		double perimetre = 0;

		return perimetre;
	}

	void afficher(){

		for(int i = 0 ; i < this->rectangles.size() ; i++ ) {

			if (rectangles[i].type == Rectangle::Type::positif) {

				std::cout << "Positif " << std::endl;

			} else if (rectangles[i].type == Rectangle::Type::negatif){

				std::cout << "Négatif " << std::endl;

			}

			std::cout << "X : " << rectangles[i].x << std::endl;
			std::cout << "Y : " << rectangles[i].y << std::endl;
			std::cout << "Longueur : " <<rectangles[i].longueur << std::endl; 
			std::cout << "Hauteur : " <<rectangles[i].hauteur << std::endl;

		}
	}	

};

/**
* Déclaration 
**/
std::ifstream& validation_fichier(std::ifstream&);
std::istream& operator >> (std::istream&, Rectangle&);
std::istream& operator >> (std::ifstream&, Grille&);

/**
* Main 
**/
int main(){

	std::ifstream fichier;
	validation_fichier(fichier);

	Grille grille = Grille();
	fichier >> grille;
	fichier.close();

	grille.afficher();

}

/**
* Implémentation
**/
std::ifstream& validation_fichier(std::ifstream& fichier){

	while(true){

		std::string repertoire;

		std::cout << "Veuillez entrer le nom du fichier." << std::endl;
		std::cin >> repertoire;

		fichier.open(repertoire.c_str());

		if (repertoire.length() < 5 || !fichier){

			std::cout << "Fichier spécifié invalide." << std::endl;
			fichier.close();

		} else if(repertoire.substr(repertoire.length() - 4, 4) != ".txt"){

			std::cout << "Le fichier doit terminer par \".txt\"" << std::endl;
			fichier.close();

		} else {

			return fichier;

		}

	}

}

std::istream& operator >> (std::istream& is, Rectangle& rectangle){

	if(is){

		char type_tmp;

		is >> type_tmp >> std::ws;
		assert(type_tmp == 'p' || type_tmp == 'n');

		rectangle.type = type_tmp == 'p' ? Rectangle::Type::positif : Rectangle::Type::negatif;

		is >> rectangle.x >> std::ws;
		is >> rectangle.y >> std::ws;
		is >> rectangle.longueur >> std::ws;
		is >> rectangle.hauteur >> std::ws;

	}

	return is;
}

std::istream& operator >> (std::ifstream& is, Grille& grille){

	while (is){

		Rectangle rectangle;
		is >> rectangle;
		grille.ajouter(rectangle);
	}

	return is;
}
