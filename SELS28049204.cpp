#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cassert>

class Rectangle {

	enum class Type { 

		positif, negatif 

	};

	class Coin {

		double x, y;

	public:

		Coin() : x(0), y(0) {}

		Coin(double x, double y){

			this->x = x;
			this->y = y;

		}

	};

public:

	Type type;
	Coin sup_gauche, sup_droit, inf_gauche, inf_droit;

	Rectangle() : 
		sup_gauche(Coin()),
		sup_droit(Coin()),
		inf_gauche(Coin()),
		inf_droit(Coin())
	{}

	Rectangle(char type, double x, double y, double longueur, double hauteur){

		assert(type == 'p' || type == 'n');
		assert(longueur > 0 && hauteur > 0);

		this->type = type == 'p' ? Type::positif : Type::negatif;

		double sup_gauche_x = x - (longueur / 2) + (hauteur / 2);
		double sup_gauche_y = y + (hauteur / 2) - (longueur / 2);

		this->sup_gauche = Coin (sup_gauche_x, sup_gauche_y);

		double sup_droit_x = x + (longueur / 2) + (hauteur / 2);
		double sup_droit_y = y + (hauteur / 2) + (longueur + 2);

		this->sup_droit = Coin (sup_droit_x, sup_droit_y);

		double inf_gauche_x = x - (longueur / 2) - (hauteur / 2);
		double inf_gauche_y = y - (hauteur / 2) - (longueur / 2);

		this->inf_gauche = Coin (inf_gauche_x, inf_gauche_y);

		double inf_droit_x = x + (longueur / 2) - (hauteur / 2);
		double inf_droit_y = y - (hauteur / 2) + (longueur /2);

		this->inf_droit = Coin(inf_droit_x, inf_droit_y);

	}

	friend std::istream& operator >> (std::istream& is, Rectangle& rectangle);

};

class Grille{

private:

	std::vector<int> x;
	std::vector<int> y;
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

	std::cout << "aire : " << grille.aire() << std::endl 
	<< "perimetre : " << grille.perimetre() << std::endl;


}

/**
* Implémentation
**/
std::ifstream& validation_fichier(std::ifstream& fichier){

	bool valide = false;

	while(!valide){

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

			valide = true;

		}

	}

	return fichier;

}

std::istream& operator >> (std::istream& is, Rectangle& rectangle){

	if(is){

		char type;
		double x, y, longueur, hauteur;

		is >> type >> std::ws;
		is >> x >> std::ws;
		is >> y >> std::ws;
		is >> longueur >> std::ws;
		is >> hauteur >> std::ws;

		rectangle = Rectangle(type, x, y, longueur, hauteur);

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
