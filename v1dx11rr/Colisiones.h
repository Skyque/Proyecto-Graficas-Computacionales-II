#ifndef COLISIONES
#define COLISIONES

#include<math.h>

class Colisiones {

private:
	double centro[2], foco1[2], foco2[2];
	float radio;

public:
	Colisiones(){}
	Colisiones(float pCentro[2], float pRadio) {
		this->centro[0] = pCentro[0];
		this->centro[1] = pCentro[1];
		this->radio = pRadio;

	}
	Colisiones(float pFoco1[2], float pFoco2[2], float eje) {
		this->foco1[0] = pFoco1[0];
		this->foco1[1] = pFoco1[1];
		this->foco2[0] = pFoco2[0];
		this->foco2[1] = pFoco2[1];
		this->radio = eje;
	}

	bool Chocando(double pCentro[2], float pRadio) {
		double distancia = sqrt(pow((pCentro[0] - this->centro[0]), 2) + pow((pCentro[1] - this->centro[1]), 2));
		if (distancia <= this->radio + pRadio) {
			return true;
		}
		else {
			return false;
		}
	}

	bool Saliendo(double pCentro[2]) {

		double distancia1 = sqrt(pow((pCentro[0] - this->foco1[0]), 2) + pow((pCentro[1] - this->foco1[1]), 2));
		double distancia2 = sqrt(pow((pCentro[0] - this->foco2[0]), 2) + pow((pCentro[1] - this->foco2[1]), 2));
		double distancia = distancia1 + distancia2;
		if (distancia >= this->radio) {
			return true;
		}
		else {
			return false;
		}
	}

};

#endif // !COLISIONES

