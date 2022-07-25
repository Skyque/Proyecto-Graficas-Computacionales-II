#ifndef _camara
#define _camara

//Clase camara hecha por Rafael Rosas para los UltraLMADs
//Videojuegos

#include <d3d11.h>
#include <D3DX11.h>
#include <D3DX10math.h>
#include"Colisiones.h"

class Camara{
public:
	D3DXVECTOR3 posCam, posCam3P;
	D3DXVECTOR3 hdveo;
	D3DXVECTOR3 hdvoy;
	D3DXVECTOR3 refUp;
	D3DXVECTOR3 refRight;
	D3DXVECTOR3 refFront;
	D3DXMATRIX vista;
	D3DXMATRIX proyeccion;
	int ancho;
	int alto;

	double posSig[2];
	float radio = 0;

	bool Colision = false;

	Camara(D3DXVECTOR3 eye, D3DXVECTOR3 target, D3DXVECTOR3 up, int Ancho, int Alto)
	{
		//posicion de la camara
		posCam = eye;

		posCam3P = eye;

		//a donde ve
		hdveo = target;
		refUp = up;
		ancho = Ancho;
		alto = Alto;

		//crea la matriz de vista
		D3DXMatrixLookAtLH(&vista, &posCam, &hdveo, &refUp);
		//la de proyeccion
		D3DXMatrixPerspectiveFovLH( &proyeccion, D3DX_PI/4.0, ancho / alto, 0.01f, 1000.0f );
		//las transpone para acelerar la multiplicacion
		D3DXMatrixTranspose( &vista, &vista );
		D3DXMatrixTranspose( &proyeccion, &proyeccion );

		D3DXVec3Normalize(&refUp, &refUp);

		refFront = D3DXVECTOR3(target.x - eye.x, target.y - eye.y, target.z - eye.z);
		D3DXVec3Normalize(&refFront, &refFront);

		D3DXVec3Cross(&refRight, &refFront, &refUp);
		D3DXVec3Normalize(&refRight, &refRight);
		
	}

	D3DXMATRIX UpdateCam(float vel, float arriaba, float izqder, int tipo, Colisiones** colisiones)
	{
		D3DXVECTOR4 tempo;
		D3DXQUATERNION quatern; //quaternion temporal para la camara
		D3DXMATRIX giraUp, giraRight; //matrices temporales para los giros

		//creamos al quaternion segun el vector up
		D3DXQuaternionRotationAxis(&quatern, &refUp, izqder); 
		//lo normalizamos para que no acumule error
		D3DXQuaternionNormalize(&quatern, &quatern);
		//creamos la matriz de rotacion basados en el quaternion
		D3DXMatrixRotationQuaternion(&giraUp, &quatern);

		//transformamos a los vectores ded la camara
		D3DXVec3Transform(&tempo, &refFront, &giraUp);
		//como el resultado de la operacion anterior es Vec4 lo casteamos para hacerlo vec3
		refFront = (D3DXVECTOR3)tempo;
		//normalizamos para no acumular error
		D3DXVec3Normalize(&refFront, &refFront);
		//Con el vector de referencia y el nuevo front calculamos right de nuevo
		D3DXVec3Cross(&refRight, &refFront, &refUp);

		//una vez calculado right a partir de front y up ahora rotamos sobre right
		//repetimos el procedimiento anterior
		D3DXQuaternionRotationAxis(&quatern, &refRight, arriaba);
		D3DXQuaternionNormalize(&quatern, &quatern);
		D3DXMatrixRotationQuaternion(&giraRight, &quatern);

		D3DXVec3Transform(&tempo, &refFront, &giraRight);
		refFront = (D3DXVECTOR3)tempo;
		D3DXVec3Normalize(&refFront, &refFront);
		

		//ajustamos la matriz de vista con lo obtenido

		//Verifico la colisión antes de moverme

	

		if (tipo == 1) {
			posSig[0] = posCam.x + refFront.x * vel / 10.0;
			posSig[1] = posCam.z + refFront.z * vel / 10.0;
			radio = 12;
		}
		else {
			posSig[0] = posCam3P.x + refFront.x * vel / 10.0;
			posSig[1] = posCam3P.z + refFront.z * vel / 10.0;
			radio = 17;
		}
		for (int i = 0; i <= 8; i++) {
			if (i == 8) {
				if (colisiones[i]->Saliendo(posSig)) {
					Colision = true;
					i = 100;
				}
			}
			else {
				if (colisiones[i]->Chocando(posSig, radio)) {
					Colision = true;
					i = 100;
				}
			}
		}

		if (!Colision) {
			posCam += refFront * vel / 10.0;

			posCam3P += refFront * vel / 10.0;
		}
		else {
			Colision = false;
		}
		
		if (tipo == 1) {
			hdveo = posCam + refFront;
			D3DXMatrixLookAtLH(&vista, &posCam, &hdveo, &refUp);
		}
		else {
			hdveo = posCam3P + refFront;
			D3DXMatrixLookAtLH(&vista, &posCam3P, &hdveo, &refUp);
		}
		
		D3DXMatrixTranspose( &vista, &vista );
		return vista;
	}
	~Camara()
	{
	}
};
#endif