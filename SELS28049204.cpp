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

public:

	std::vector<Rectangle> rectangles;
	friend std::istream& operator >> (std::istream& is, Grille& grille);

	void ajouter(Rectangle& rectangle){

		//ajouter un rectangle
	}

};

std::ifstream& lecture_fichier_texte(std::ifstream& fichier);
std::istream& operator >> (std::istream& is, Rectangle& rectangle);

int main(){

	std::ifstream fichier;
	lecture_fichier_texte(fichier);


}

std::ifstream& lecture_fichier_texte(std::ifstream& fichier){

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

		is >> type_tmp;

		assert(type_tmp == 'p' || type_tmp == 'n');

		rectangle.type = type_tmp == 'p' ? Rectangle::Type::positif : Rectangle::Type::negatif;

		is >> rectangle.x >> rectangle.y >> rectangle.longueur >> rectangle.hauteur;

	}

	return is;
}

std::istream& operator >> (std::istream& is, Grille& grille){

	while (is){

		Rectangle rectangle;
		is >> rectangle;
		//ajouter à la grille
	}

	return is;
}
