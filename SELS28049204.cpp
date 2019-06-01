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

		friend std::ostream& operator <<  (std::ostream&, Coin&);

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
		this->sup_gauche = Coin (x - longueur / 2, y + hauteur / 2);
		this->sup_droit = Coin (x + longueur / 2, y + hauteur / 2);
		this->inf_gauche = Coin (x - longueur / 2, y - hauteur / 2);
		this->inf_droit = Coin(x + longueur / 2, y - hauteur / 2);

	}

	bool estPositif() {
		return this->type == Type::positif;
	}

	friend std::istream& operator >> (std::ifstream&, Rectangle&);
	friend std::ostream& operator << (std::ostream&, Coin&);
	friend std::ostream& operator << (std::ostream&, Rectangle&);


};

class Grille{

public:

	std::vector<Rectangle> rectangles;

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

	friend std::istream& operator >> (std::istream&, Grille&);

};

/**
* Déclaration 
**/
std::ifstream& validation_fichier(std::ifstream&);
std::ostream& operator << (std::ostream&, Rectangle&);
std::ostream& operator << (std::ostream&, Rectangle::Coin&);
std::istream& operator >> (std::ifstream&, Rectangle&);
std::istream& operator >>  (std::ifstream&, Grille&);


/**
* Main 
**/
int main(){

	std::ifstream fichier;
	validation_fichier(fichier);

	Grille grille = Grille();
	fichier >> grille;
	fichier.close();

	for (int i = 0; i < grille.rectangles.size() ; i++){
		std::cout <<  grille.rectangles[i] <<  std::endl;
	}

	std::cout <<  "aire : " << grille.aire() << std::endl 
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

std::ostream& operator << (std::ostream& os, Rectangle::Coin& coin){

	os << "(" <<  coin.x <<  "," << coin.y << ")";

	return os;
}

std::ostream& operator <<  (std::ostream& os, Rectangle& rectangle){

	if (rectangle.estPositif()) {

		os << "Rectangle positif" << std::endl;

	} else {

		os << "Rectangle negatif" << std::endl;
	}

	os << "Supérieur gauche : " << rectangle.sup_gauche << std::endl;
	os << "Supérieur droite : " << rectangle.sup_droit << std::endl;
	os << "Inférieur gauche : " << rectangle.inf_gauche << std::endl;
	os << "Inférieur droit : " << rectangle.inf_droit << std::endl;
	
	return os;

}

std::istream& operator >> (std::ifstream& is, Rectangle& rectangle){

	if(is){

		char type;
		double x, y, longueur, hauteur;

		is >> type >>  std::ws;
		is >> x >> std::ws;
		is >> y >> std::ws;
		is >> longueur >> std::ws;
		is >> hauteur >> std::ws;
		rectangle = Rectangle(type, x, y, longueur, hauteur);

		std::cout <<  "X : " <<  x <<  " Y : " << y << " L : " << longueur << " H : " << hauteur << std::endl;


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
