#ifndef TIMER
#define TIMER

#include<Windows.h>

//Clase que sir ve para crear timers
class Timer {

public:
	float tiempoTranscurrido;
	float tiempoTotal;

	bool Inicializa(float pDuracion) {
		LARGE_INTEGER i;

		if (!QueryPerformanceFrequency(&i))
			return false;

		//obtenemos frecuencia en segundos
		this->Frecuencia = (float)i.QuadPart;

		//Obtenemos el valor actual del ciclo
		QueryPerformanceCounter(&i);
		this->Inicio = i.QuadPart;
		this->tiempoTotal = 0;
		this->tiempoTranscurrido = 0;
		this->UltimaMarca = 0;
		this->Duracion = pDuracion;
		return true;
	}

	void Actualiza() {
		LARGE_INTEGER i;

		QueryPerformanceCounter(&i);
		this->tiempoTranscurrido = (float)(i.QuadPart - Inicio) / Frecuencia;
		this->Inicio = i.QuadPart;
		this->tiempoTotal += this->tiempoTranscurrido;
	}

	bool Activado() {
		if (this->tiempoTotal - this->UltimaMarca >= this->Duracion) {
			UltimaMarca = tiempoTotal;
			return true;
		}
		else {
			return false;
		}
	}

	float getTiempo() {
		return this->tiempoTotal;
	}

private:
	LONGLONG Inicio;
	float Frecuencia;
	float Duracion;
	float UltimaMarca;
};

#endif
