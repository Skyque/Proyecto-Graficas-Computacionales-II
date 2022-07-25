#ifndef BASURA 
#define BASURA

#include"Billboard.h"
#include"Timer.h"
#include<math.h>

class Basura : public BillboardRR {
private:
	bool Activo = false;
	Timer* vida;

	int tipo = 0;

	void Desaparece() {
		this->Activo = false;
	}

public:

	Basura(WCHAR* billb, WCHAR* normal, ID3D11Device* D3DDevice, ID3D11DeviceContext* D3DContext,float X, float Z, float escala, int ptipo)
	{
		this->tipo = ptipo;
		this->posx = X;
		this->posz = Z;
		//copiamos el device y el device context a la clase terreno
		d3dContext = D3DContext;
		d3dDevice = D3DDevice;
		//este es el ancho y el alto del terreno en su escala

		float escal = escala;
		frontal = D3DXVECTOR3(0, 0, 1);
		//aqui cargamos las texturas de alturas y el cesped
		CargaParametros(billb, normal, escal);
		this->vida = new Timer();
		this->vida->Inicializa(39);
	}

	~Basura()
	{
		//libera recursos
		delete vertcol;
		UnloadContent();
	}

	void Spawnea(float X, float Z) {
		this->Activo = true;
		this->vida->Inicializa(39);
		this->posx = X;
		this->posz = Z;
	}

	bool Colision(float Posicion[2]) {
		float distancia = sqrt(pow((this->posx - Posicion[0]), 2) + pow((this->posz - Posicion[1]), 2));
		if (distancia <= 5) {
			//Hace daño al jugador
			Desaparece();
			return true;
		}
		else {
			return false;
		}
	}

	void Actualiza() {
		if (this->Activo) {
			this->vida->Actualiza();
			if (this->vida->Activado()) {
				this->Activo = false;
			}
		}
	}

	bool isActivo() {
		return this->Activo;
	}

	int getTipo() {
		return this->tipo; 
	}
};

#endif
