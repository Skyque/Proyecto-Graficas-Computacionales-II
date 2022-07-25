#ifndef _dxrr
#define _dxrr
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <d3dx10math.h>
#include "TerrenoRR.h"
#include "Camara.h"
#include "SkyDome.h"
#include "Billboard.h"
#include "ModeloRR.h"
#include "XACT3Util.h"
#include "Colisiones.h"
#include "Basura.h"
#include "Timer.h"
#include "GUI.h"
#include "Text.h"
#include"Agua.h" //Copia
#include <string>

class DXRR {

private:
	int ancho;
	int alto;
	int VidaActual = 6;
	bool Cambiado = false, Perdiste = false;
public:
	HINSTANCE hInstance;
	HWND hWnd;

	D3D_DRIVER_TYPE driverType;
	D3D_FEATURE_LEVEL featureLevel;

	ID3D11Device* d3dDevice;
	ID3D11DeviceContext* d3dContext;
	IDXGISwapChain* swapChain;
	ID3D11RenderTargetView* backBufferTarget;

	ID3D11Texture2D* depthTexture;
	ID3D11DepthStencilView* depthStencilView;

	ID3D11DepthStencilState* depthStencilState;
	ID3D11DepthStencilState* depthStencilDisabledState;

	ID3D11BlendState* alphaBlendState, * commonBlendState;

	int frameBillboard;

	TerrenoRR* terreno;
	SkyDome* skydome;
	Basura* basura[12];
	Camara* camara;
	ModeloRR* model;
	//MIS MODELOS
	ModeloRR* carro1;
	ModeloRR* carroAmarillo;
	ModeloRR* Gradas;
	ModeloRR* Muro;
	ModeloRR* Semaforo_Grande;
	ModeloRR* Semaforo_En_L;
	ModeloRR* Semaforo_En_T;
	ModeloRR* Cono;
	ModeloRR* Barrera;
	ModeloRR* Reja;
	ModeloRR* SeñalNaranja;
	ModeloRR* Vivienda1;
	ModeloRR* Vivienda2;

	float izqder;
	float arriaba;
	float vel;
	float CarroAngulo;
	bool breakpoint;
	int tipoCam = 1;
	vector2 uv1[32];
	vector2 uv2[32];
	vector2 uv3[32];
	vector2 uv4[32];

	XACTINDEX cueIndex;
	XACTINDEX cueIndex2;
	XACTINDEX cueIndex3;
	XACTINDEX cueIndex4;
	CXACT3Util m_XACT3;

	float CamRot;

	//puntos de colisión
	Colisiones* colisiones[9];

	Timer *timerSpawn = new Timer();
	Timer* timerAbucheo = new Timer();
	Timer* timerPolicia = new Timer();
	Timer* timerMusica = new Timer();

	Timer* TiempoJuego = new Timer();
	
	bool Parado = false;

	Text* Derrota, *Victoria, *TiempoRestante;

	GUI** Vida = new GUI*[6];

	Agua* agua; //Copia

    DXRR(HWND hWnd, int Ancho, int Alto)
	{
		breakpoint = false;
		frameBillboard = 0;
		ancho = Ancho;
		alto = Alto;
		driverType = D3D_DRIVER_TYPE_NULL;
		featureLevel = D3D_FEATURE_LEVEL_11_0;
		d3dDevice = 0;
		d3dContext = 0;
		swapChain = 0;
		backBufferTarget = 0;
		IniciaD3D(hWnd);
		izqder = 0;
		arriaba = 0;
		billCargaFuego();
		camara = new Camara(D3DXVECTOR3(0,0,0), D3DXVECTOR3(0,0,1), D3DXVECTOR3(0,1,0), Ancho, Alto);
		terreno = new TerrenoRR(900, 900, d3dDevice, d3dContext);
		skydome = new SkyDome(40, 40, 200.0f, &d3dDevice, &d3dContext, L"Imágenes//Sky-090.jpg", L"Imágenes//Noche.jpg");
		basura[0] = new Basura(L"Assets/Billboards/Basura1.png",L"Assets/Billboards/Basura1_Normal.png", d3dDevice, d3dContext, 150, 5, 4, 1);
		basura[1] = new Basura(L"Assets/Billboards/Basura2.png", L"Assets/Billboards/Basura2_Normal.png", d3dDevice, d3dContext, 150, 5, 4, 2);
		basura[2] = new Basura(L"Assets/Billboards/Basura3.png", L"Assets/Billboards/Basura3_Normal.png", d3dDevice, d3dContext, 150, 5, 4, 3);
		basura[3] = new Basura(L"Assets/Billboards/Basura1.png", L"Assets/Billboards/Basura1_Normal.png", d3dDevice, d3dContext, 150, 5, 4, 1);
		basura[4] = new Basura(L"Assets/Billboards/Basura2.png", L"Assets/Billboards/Basura2_Normal.png", d3dDevice, d3dContext, 150, 5, 4, 2);
		basura[5] = new Basura(L"Assets/Billboards/Basura3.png", L"Assets/Billboards/Basura3_Normal.png", d3dDevice, d3dContext, 150, 5, 4, 3);
		basura[6] = new Basura(L"Assets/Billboards/Basura1.png", L"Assets/Billboards/Basura1_Normal.png", d3dDevice, d3dContext, 150, 5, 4, 1);
		basura[7] = new Basura(L"Assets/Billboards/Basura2.png", L"Assets/Billboards/Basura2_Normal.png", d3dDevice, d3dContext, 150, 5, 4, 2);
		basura[8] = new Basura(L"Assets/Billboards/Basura3.png", L"Assets/Billboards/Basura3_Normal.png", d3dDevice, d3dContext, 150, 5, 4, 3);
		basura[9] = new Basura(L"Assets/Billboards/Basura1.png", L"Assets/Billboards/Basura1_Normal.png", d3dDevice, d3dContext, 150, 5, 4, 1);
		basura[10] = new Basura(L"Assets/Billboards/Basura2.png", L"Assets/Billboards/Basura2_Normal.png", d3dDevice, d3dContext, 150, 5, 4, 2);
		basura[11] = new Basura(L"Assets/Billboards/Basura3.png", L"Assets/Billboards/Basura3_Normal.png", d3dDevice, d3dContext, 150, 5, 4, 3);
		//model = new ModeloRR(d3dDevice, d3dContext, "Assets/Cofre/Cofre.obj", L"Assets/Cofre/Cofre-color.png", L"Assets/Cofre/Cofre-spec.png", L"Assets/Cofre/Cofre-spec.png", 0, 0);
		//MIS MODELOS
		carro1 = new ModeloRR(d3dDevice, d3dContext, "Assets/Modelos/Auto/Auto_Final.obj", L"Assets/Modelos/Auto/Auto_BaseColor.png", L"Assets/Cofre/Cofre-spec.png", L"Assets/Modelos/Auto/Auto_Normal.jpeg", -50, 20);
		carroAmarillo = new ModeloRR(d3dDevice, d3dContext, "Assets/Modelos/AutoAmarillo/AutoAmarillo.obj", L"Assets/Modelos/AutoAmarillo/sportcar.017_Body_BaseColor.png", L"Assets/Modelos/AutoAmarillo/AutoAmarilloSpec.png", L"Assets/Modelos/AutoAmarillo/AutoAmarilloNormal.png", -8, 150);
		Gradas = new ModeloRR(d3dDevice, d3dContext, "Assets/Modelos/Gradas/Gradas.obj", L"Assets/Modelos/Gradas/Gradas_BaseColor.png", L"Assets/Modelos/Gradas/Gradas_Specular.png", L"Assets/Modelos/Gradas/Gradas_Normal.png", -25, 5);
		Muro = new ModeloRR(d3dDevice, d3dContext, "Assets/Modelos/Muros/Muro/Muro.obj", L"Assets/Modelos/Muros/Muro/CementBarrier_Base_Color.png", L"Assets/Cofre/Cofre-spec.png", L"Assets/Modelos/Muros/Muro/CementBarrier_Normal_DirectX.png", -15, 4);
		Semaforo_Grande = new ModeloRR(d3dDevice, d3dContext, "Assets/Modelos/Semaforo/Semaforo_Grande/Semaforo_Grande.obj", L"Assets/Modelos/Semaforo/Semaforo_Grande/Señales_BaseColor.png", L"Assets/Cofre/Cofre-spec.png", L"Assets/Modelos/Semaforo/Semaforo_Grande/Señales_Normal.jpg", 127, 40);
		Semaforo_En_L = new ModeloRR(d3dDevice, d3dContext, "Assets/Modelos/Semaforo/Semaforo_En_L/Semaforo_En_L.obj", L"Assets/Modelos/Semaforo/Semaforo_En_L/Señales_BaseColor.png", L"Assets/Cofre/Cofre-spec.png", L"Assets/Modelos/Semaforo/Semaforo_En_L/Señales_Normal.jpg", 40, 275);
		Semaforo_En_T = new ModeloRR(d3dDevice, d3dContext, "Assets/Modelos/Semaforo/Semaforo_En_T/Semaforo_En_T.obj", L"Assets/Modelos/Semaforo/Semaforo_En_T/Señales_BaseColor.png", L"Assets/Cofre/Cofre-spec.png", L"Assets/Modelos/Semaforo/Semaforo_En_L/Señales_Normal.jpg", -80, 28);
		Cono = new ModeloRR(d3dDevice, d3dContext, "Assets/Modelos/Señales/Cono/Cono.obj", L"Assets/Modelos/Señales/Cono/Señales_BaseColor.png", L"Assets/Cofre/Cofre-spec.png", L"Assets/Modelos/Señales/Cono/Señales_Normal.png", -87.5f, -170);
		Barrera = new ModeloRR(d3dDevice, d3dContext, "Assets/Modelos/Señales/Barrea/Barrera.obj", L"Assets/Modelos/Señales/Barrea/Barrera_BaseColor.png", L"Assets/Modelos/Señales/Barrea/Barrera_Specular.png", L"Assets/Modelos/Señales/Barrea/Barrera_Normal.png", -8, 150);
		Reja = new ModeloRR(d3dDevice, d3dContext, "Assets/Modelos/Señales/Reja/Reja.obj", L"Assets/Modelos/Señales/Reja/Reja_BaseColor.png", L"Assets/Cofre/Cofre-spec.png", L"Assets/Modelos/Señales/Cono/Señales_Normal.png", 72, -40);
		SeñalNaranja = new ModeloRR(d3dDevice, d3dContext, "Assets/Modelos/Señales/SeñalNaranja/SeñalNaranja.obj", L"Assets/Modelos/Señales/SeñalNaranja/SeñalNaranja_BaseColor.png", L"Assets/Cofre/Cofre-spec.png", L"Assets/Modelos/Señales/Cono/Señales_Normal.png", -73, 191);
		Vivienda1 = new ModeloRR(d3dDevice, d3dContext, "Assets/Modelos/Vivienda_1/Vivienda_1.obj", L"Assets/Modelos/Vivienda_1/Car_lambert1_BaseColor.png", L"Assets/Cofre/Cofre-spec.png", L"Assets/Modelos/Vivienda_1/Car_lambert1_Normal.png", -8, -130);
		Vivienda2 = new ModeloRR(d3dDevice, d3dContext, "Assets/Modelos/Vivienda_2/Vivienda_2.obj", L"Assets/Modelos/Vivienda_2/fbx_plane_material_lambert1_BaseColor.png", L"Assets/Cofre/Cofre-spec.png", L"Assets/Modelos/Vivienda_2/fbx_plane_material_lambert1_Normal_2.png", -400, -100);
		
		

		agua = new Agua(d3dDevice, d3dContext, "Assets/Modelos/Agua/Agua.obj", L"Assets/Modelos/Agua/Agua_Base.png", L"Assets/Modelos/Agua/Agua_Base2.png", L"Assets/Cofre/Cofre-spec.png", 0, 0);
		

		//Texto
		TiempoRestante = new Text(d3dDevice, d3dContext, 2.4, 0.8, L"Assets/GUI/font.jpg", XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		Victoria = new Text(d3dDevice, d3dContext, 4.5, 1.5, L"Assets/GUI/font.jpg", XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		Derrota = new Text(d3dDevice, d3dContext, 4.5, 1.5, L"Assets/GUI/font.jpg", XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		//UI
		Vida[0] = new GUI(d3dDevice, d3dContext, .6, .1, L"Assets/GUI/Vida1.png");
		Vida[1] = new GUI(d3dDevice, d3dContext, .6, .1, L"Assets/GUI/Vida2.png");
		Vida[2] = new GUI(d3dDevice, d3dContext, .6, .1, L"Assets/GUI/Vida3.png");
		Vida[3] = new GUI(d3dDevice, d3dContext, .6, .1, L"Assets/GUI/Vida4.png");
		Vida[4] = new GUI(d3dDevice, d3dContext, .6, .1, L"Assets/GUI/Vida5.png");
		Vida[5] = new GUI(d3dDevice, d3dContext, .6, .1, L"Assets/GUI/Vida6.png");

		CamRot = 0;

		//posición iicial de la cámara/jugador
		camara->posCam.x = 150;
		camara->posCam.z = 5;
		camara->posCam3P.x = 150;
		camara->posCam3P.z = camara->posCam.z - 20;

		float centro[2] = { -13, 122 };

		colisiones[0] = new Colisiones(centro, 87);
		centro[1] = -114;
		colisiones[1] = new Colisiones(centro, 87);
		centro[1] = 60;
		colisiones[2] = new Colisiones(centro, 88);
		centro[1] = -68;
		colisiones[3] = new Colisiones(centro, 88);
		centro[0] = 82;
		centro[1] = -9;
		colisiones[4] = new Colisiones(centro, 1.5f);
		centro[1] = -36;
		colisiones[5] = new Colisiones(centro, 1.5f);
		centro[0] = 34;
		centro[1] = 296;
		colisiones[6] = new Colisiones(centro, .25f);
		centro[0] = 170;
		centro[1] = -9;
		colisiones[7] = new Colisiones(centro, 1.5f);

		float Foco1[2] = { -10, -225 };
		float Foco2[2] = { -10, 228 };

		colisiones[8] = new Colisiones(Foco1, Foco2, 625);

		//Timers
		timerSpawn->Inicializa(3.5);
		//timerAbucheo->Inicializa(30);
		//timerPolicia->Inicializa(40);
		//timerMusica->Inicializa(120);

		TiempoJuego->Inicializa(120);
	}

	~DXRR()
	{
		LiberaD3D();
		m_XACT3.Terminate();
	}
	
	bool IniciaD3D(HWND hWnd)
	{
		this->hInstance = hInstance;
		this->hWnd = hWnd;

		//obtiene el ancho y alto de la ventana donde se dibuja
		RECT dimensions;
		GetClientRect(hWnd, &dimensions);
		unsigned int width = dimensions.right - dimensions.left;
		unsigned int heigth = dimensions.bottom - dimensions.top;

		//Las formas en como la pc puede ejecutar el DX11, la mas rapida es D3D_DRIVER_TYPE_HARDWARE pero solo se puede usar cuando lo soporte el hardware
		//otra opcion es D3D_DRIVER_TYPE_WARP que emula el DX11 en los equipos que no lo soportan
		//la opcion menos recomendada es D3D_DRIVER_TYPE_SOFTWARE, es la mas lenta y solo es util cuando se libera una version de DX que no sea soportada por hardware
		D3D_DRIVER_TYPE driverTypes[]=
		{
			D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP, D3D_DRIVER_TYPE_SOFTWARE
		};
		unsigned int totalDriverTypes = ARRAYSIZE(driverTypes);

		//La version de DX que utilizara, en este caso el DX11
		D3D_FEATURE_LEVEL featureLevels[]=
		{
			D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0
		};
		unsigned int totalFeaturesLevels = ARRAYSIZE(featureLevels);

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Width = width;
		swapChainDesc.BufferDesc.Height = heigth;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = hWnd;
		swapChainDesc.Windowed = true;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;

		HRESULT result;
		unsigned int driver = 0, creationFlags = 0;
		for(driver = 0; driver<totalDriverTypes; driver++)
		{
			result = D3D11CreateDeviceAndSwapChain(0, driverTypes[driver], 0,
				creationFlags, featureLevels, totalFeaturesLevels, 
				D3D11_SDK_VERSION, &swapChainDesc, &swapChain,
				&d3dDevice, &featureLevel, &d3dContext);

			if(SUCCEEDED(result))
			{
				driverType = driverTypes[driver];
				break;
			}
		}

		if(FAILED(result))
		{

			//Error al crear el Direct3D device
			return false;
		}
		
		ID3D11Texture2D* backBufferTexture;
		result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferTexture);
		if(FAILED(result))
		{
			//"Error al crear el swapChainBuffer
			return false;
		}

		result = d3dDevice->CreateRenderTargetView(backBufferTexture, 0, &backBufferTarget);
		if(backBufferTexture)
			backBufferTexture->Release();

		if(FAILED(result))
		{
			//Error al crear el renderTargetView
			return false;
		}


		D3D11_VIEWPORT viewport;
		viewport.Width = (FLOAT)width;
		viewport.Height = (FLOAT)heigth;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;

		d3dContext->RSSetViewports(1, &viewport);

		D3D11_TEXTURE2D_DESC depthTexDesc;
		ZeroMemory(&depthTexDesc, sizeof(depthTexDesc));
		depthTexDesc.Width = width;
		depthTexDesc.Height = heigth;
		depthTexDesc.MipLevels = 1;
		depthTexDesc.ArraySize = 1;
		depthTexDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthTexDesc.SampleDesc.Count = 1;
		depthTexDesc.SampleDesc.Quality = 0;
		depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
		depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthTexDesc.CPUAccessFlags = 0;
		depthTexDesc.MiscFlags = 0;
		
		result = d3dDevice->CreateTexture2D(&depthTexDesc, NULL, &depthTexture);
		if(FAILED(result))
		{
			MessageBox(0, L"Error", L"Error al crear la DepthTexture", MB_OK);
			return false;
		}

		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = depthTexDesc.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;

		result = d3dDevice->CreateDepthStencilView(depthTexture, &descDSV, &depthStencilView);
		if(FAILED(result))
		{
			MessageBox(0, L"Error", L"Error al crear el depth stencil target view", MB_OK);
			return false;
		}

		d3dContext->OMSetRenderTargets(1, &backBufferTarget, depthStencilView);

		//Inicializamos XACT para música ambiental
		bool res = m_XACT3.Initialize();
		if (!res) return false;
		res = m_XACT3.LoadWaveBank(L"Win\\WaveBank.xwb");
		if (!res) return false;
		res = m_XACT3.LoadSoundBank(L"Win\\SoundBank.xsb");
		if (!res) return false;
        
		cueIndex = m_XACT3.m_pSoundBank->GetCueIndex("Auto");
		//m_XACT3.m_pSoundBank->Play(cueIndex, 0, 0, 0);

		cueIndex2 = m_XACT3.m_pSoundBank->GetCueIndex("Rock");
		//m_XACT3.m_pSoundBank->Play(cueIndex2, 0, 0, 0);

		cueIndex3 = m_XACT3.m_pSoundBank->GetCueIndex("Abucheo");
		//Abucheo
		//m_XACT3.m_pSoundBank->Play(cueIndex3, 0, 1000, 0);

		cueIndex4 = m_XACT3.m_pSoundBank->GetCueIndex("Policia");
		//Policia
		//m_XACT3.m_pSoundBank->Play(cueIndex4, 0, 1000, 0);

		return true;			
		
	}

	void LiberaD3D(void)
	{
		if(depthTexture)
			depthTexture->Release();
		if(depthStencilView)
			depthStencilView->Release();
		if(backBufferTarget)
			backBufferTarget->Release();
		if(swapChain)
			swapChain->Release();
		if(d3dContext)
			d3dContext->Release();
		if(d3dDevice)
			d3dDevice->Release();

		depthTexture = 0;
		depthStencilView = 0;
		d3dDevice = 0;
		d3dContext = 0;
		swapChain = 0;
		backBufferTarget = 0;
	}
	
	void Render(void)
	{
		//XACT Funcione
		m_XACT3.DoWork();

		if (breakpoint) {
			int x = 0;
		}

		float sphere[3] = { 0,0,0 };
		float prevPos[3] = { camara->posCam.x, camara->posCam.z, camara->posCam.z };
		static float angle = 0.0f;
		angle += 0.005;
		if (angle >= 360) angle = 0.0f;
		bool collide = false;
		if( d3dContext == 0 )
			return;

		CamRot += izqder;

		float clearColor[4] = { 0, 0, 0, 1.0f };
		d3dContext->ClearRenderTargetView( backBufferTarget, clearColor );
		d3dContext->ClearDepthStencilView( depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );

		//Movimiento cámara
		camara->posCam.y = terreno->Superficie(camara->posCam.x, camara->posCam.z) + 17;

		camara->posCam3P.y = terreno->Superficie(camara->posCam.x, camara->posCam.z) + 23;

		if (!Cambiado) {
			Cambiado = true;
			camara->UpdateCam(vel, - 17 * 3.1416 / 180, izqder, tipoCam, colisiones);
		}
		else {
			camara->UpdateCam(vel, 0, izqder, tipoCam, colisiones);
		}
		skydome->Update(camara->vista, camara->proyeccion);

		float camPosXZ[2] = { camara->posCam.x, camara->posCam.z };

		TurnOffDepth();
		skydome->Render(camara->posCam);
		TurnOnDepth();
		terreno->Draw(camara->vista, camara->proyeccion);
		//TurnOnAlphaBlending();

		if (!Parado) {
			TiempoJuego->Actualiza();
			if (TiempoJuego->Activado()) {
				Parado = true;
			}
		}

		//Cada cierto tiempo spawneamos un nuevo obstáculo
		timerSpawn->Actualiza();
		static int indice = 0;
		if (timerSpawn->Activado()) {
			basura[indice]->Spawnea(-carro1->getPosX(), -carro1->getPosZ());
			if (indice == 11) {
				indice = 0;
			}
			else {
				indice++;
			}
		}

		timerAbucheo->Actualiza();
		if (timerAbucheo->Activado()) {
			//Abucheo
			m_XACT3.m_pSoundBank->Play(cueIndex3, 0, 1000, 0);
		}

		timerPolicia->Actualiza();
		if (timerPolicia->Activado()) {
			//Policia
			m_XACT3.m_pSoundBank->Play(cueIndex4, 0, 1000, 0);
		}

		timerMusica->Actualiza();
		if (timerMusica->Activado()) {
			//Múicia
			m_XACT3.m_pSoundBank->Play(cueIndex2, 0, 1000, 0);
		}

		//Comprobamos colisiones con obstáculos
		float pos[2];
		if (tipoCam == 1) {
			pos[0] = -1;
			pos[1] = 22;
		}
		else {
			pos[0] = -1;
			pos[1] = 35;
		}

		//Aplicamos la rotación y traslación que le plcamos al modelo
		float NuevaPos[2];

		//Rotación
		NuevaPos[0] = pos[0] * cos(-CamRot) - pos[1] * sin(-CamRot);
		NuevaPos[1] = pos[0] * sin(-CamRot) + pos[1] * cos(-CamRot);
		//Traslación
		NuevaPos[0] += carro1->getPosX();
		NuevaPos[1] += carro1->getPosZ();
		int hola = 0;
		for (int i = 0; i <= 11; i++) {
			basura[i]->Actualiza();
			if (basura[i]->isActivo()) {
				bool choque = basura[i]->Colision(NuevaPos);

				if (choque) {
					VidaActual--;
					if (VidaActual <= 0) {
						Perdiste = true;
						Parado = true;
					}
				}

				//Dibujamos obstáculos
				switch (basura[i]->getTipo())
				{
				case 1:
					basura[i]->Draw(camara->vista, camara->proyeccion, camara->posCam,
						basura[i]->posx, basura[i]->posz, terreno->Superficie(basura[i]->posx, basura[i]->posz), 1, 4, uv1, uv2, uv3, uv4, 0);
					break;
				case 2:
					basura[i]->Draw(camara->vista, camara->proyeccion, camara->posCam,
						basura[i]->posx, basura[i]->posz, terreno->Superficie(basura[i]->posx, basura[i]->posz), 2, 6, uv1, uv2, uv3, uv4, 0);
					break;
				case 3:
					basura[i]->Draw(camara->vista, camara->proyeccion, camara->posCam,
						basura[i]->posx, basura[i]->posz, terreno->Superficie(basura[i]->posx, basura[i]->posz), 3, 7, uv1, uv2, uv3, uv4, 0);
					break;
				}
			}
		}

		//TurnOffAlphaBlending();
		//model->Draw(camara->vista, camara->proyeccion, terreno->Superficie(0, 0) + 10, camara->posCam, 10.0f, 0, 'A', 1, false, tipoCam, CarroAngulo);
		//MIS MODELOS
	
		carroAmarillo->Draw(camara->vista, camara->proyeccion, terreno->Superficie(-8, 150) - 3, camara->posCam, 10.0f, 0, 'A', 5, false, tipoCam, CarroAngulo);
		/*
		* para probar punto de colisión visual
		Barrera->setPosX(NuevaPos[0]);
		Barrera->setPosZ(NuevaPos[1]);
		*/
		Gradas->Draw(camara->vista, camara->proyeccion, terreno->Superficie(-10, 5) - 8.5f, camara->posCam, 10.0f, 0, 'A', 20, false, tipoCam, CarroAngulo);
		Muro->Draw(camara->vista, camara->proyeccion, terreno->Superficie(-15, 4) + 10, camara->posCam, 10.0f, 0, 'A', 7.6f, false, tipoCam, CarroAngulo);
		Semaforo_Grande->Draw(camara->vista, camara->proyeccion, terreno->Superficie(127, 40), camara->posCam, 10.0f, 0, 'A', 3.6f, false, tipoCam, CarroAngulo);
		Semaforo_En_L->Draw(camara->vista, camara->proyeccion, terreno->Superficie(73.5f, 196), camara->posCam, 10.0f, 0, 'A', 4, false, tipoCam, CarroAngulo);
		Semaforo_En_T->Draw(camara->vista, camara->proyeccion, terreno->Superficie(40, 50), camara->posCam, 10.0f, 0, 'A', 4, false, tipoCam, CarroAngulo);
		Cono->Draw(camara->vista, camara->proyeccion, terreno->Superficie(-87.5f, -170), camara->posCam, 10.0f, 0, 'A', 7, false, tipoCam, CarroAngulo);
		Barrera->Draw(camara->vista, camara->proyeccion, terreno->Superficie(-8, 150) - 3, camara->posCam, 10.0f, 0, 'A', 5, false, tipoCam, CarroAngulo);
		Reja->Draw(camara->vista, camara->proyeccion, terreno->Superficie(72, -40), camara->posCam, 10.0f, 0, 'A', 5, false, tipoCam, CarroAngulo);
		SeñalNaranja->Draw(camara->vista, camara->proyeccion, terreno->Superficie(-73, 191), camara->posCam, 10.0f, 0, 'A', 4.5, false, tipoCam, CarroAngulo);
		Vivienda1->Draw(camara->vista, camara->proyeccion, terreno->Superficie(-8, -130), camara->posCam, 10.0f, 0, 'A', 4.5, false, tipoCam, CarroAngulo);
		Vivienda2->Draw(camara->vista, camara->proyeccion, terreno->Superficie(-400, -100), camara->posCam, 10.0f, 0, 'A', 4.5, false, tipoCam, CarroAngulo);

		TurnOnAlphaBlending();

		agua->Draw(camara->vista, camara->proyeccion, terreno->Superficie(0, 0) + 1, camara->posCam, 10.0f, 0, 'A', 30, false, tipoCam, CarroAngulo);

		TurnOffAlphaBlending();
		

		//Movimiento carro
		carro1->setPosX(camara->hdveo.x);
		carro1->setPosZ(camara->hdveo.z);
		carro1->Draw(camara->vista, camara->proyeccion, terreno->Superficie(carro1->getPosX(), carro1->getPosZ()), camara->posCam, 10.0f, CamRot, 'Y', 3, true, tipoCam, CarroAngulo);

		if (Parado) {
			if (!Perdiste) {
				Victoria->DrawText(-.9, .2, "Los policias lograron sacar a todas las personas", 0.015);
				Victoria->DrawText(-.3, 0, "¡Has ganado!", 0.015);
			}
			else {
				Derrota->DrawText(-.7, .2, "La basura ha descompuesto tu auto", 0.015);
				Derrota->DrawText(-.3, 0, "Has perdido...", 0.015);
			}
		}
		else {
			stringstream ss;
			float t = 120 - TiempoJuego->getTiempo();
			if (t < 0) {
				t = 0;
			}
			ss << t;
			TiempoRestante->DrawText(-.95, .95, "Tiempo restante: " + ss.str() + "s", 0.015);
		}

		if (VidaActual > 0) {
			Vida[VidaActual - 1]->Draw(-.95, .5);
		}

		swapChain->Present( 1, 0 );
	}

	bool isPointInsideSphere(float* point, float* sphere) {
		bool collition = false;

		float distance = sqrt((point[0] - sphere[0]) * (point[0] - sphere[0]) +
			(point[1] - sphere[1]) * (point[1] - sphere[1]));

		if (distance < sphere[2])
			collition = true;
		return collition;
	}

	//Activa el alpha blend para dibujar con transparencias
	void TurnOnAlphaBlending()
	{
		float blendFactor[4];
		blendFactor[0] = 0.0f;
		blendFactor[1] = 0.0f;
		blendFactor[2] = 0.0f;
		blendFactor[3] = 0.0f;
		HRESULT result;

		D3D11_BLEND_DESC descABSD;
		ZeroMemory(&descABSD, sizeof(D3D11_BLEND_DESC));
		descABSD.RenderTarget[0].BlendEnable = TRUE;
		descABSD.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		descABSD.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		descABSD.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		descABSD.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		descABSD.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		descABSD.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		descABSD.RenderTarget[0].RenderTargetWriteMask = 0x0f;

		result = d3dDevice->CreateBlendState(&descABSD, &alphaBlendState);
		if(FAILED(result))
		{
			MessageBox(0, L"Error", L"Error al crear el blend state", MB_OK);
			return;
		}

		d3dContext->OMSetBlendState(alphaBlendState, blendFactor, 0xffffffff);
	}

	//Regresa al blend normal(solido)
	void TurnOffAlphaBlending()
	{
		D3D11_BLEND_DESC descCBSD;
		ZeroMemory(&descCBSD, sizeof(D3D11_BLEND_DESC));
		descCBSD.RenderTarget[0].BlendEnable = FALSE;
		descCBSD.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		descCBSD.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		descCBSD.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		descCBSD.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		descCBSD.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		descCBSD.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		descCBSD.RenderTarget[0].RenderTargetWriteMask = 0x0f;
		HRESULT result;

		result = d3dDevice->CreateBlendState(&descCBSD, &commonBlendState);
		if(FAILED(result))
		{
			MessageBox(0, L"Error", L"Error al crear el blend state", MB_OK);
			return;
		}

		d3dContext->OMSetBlendState(commonBlendState, NULL, 0xffffffff);
	}

	void TurnOnDepth()
	{
		D3D11_DEPTH_STENCIL_DESC descDSD;
		ZeroMemory(&descDSD, sizeof(descDSD));
		descDSD.DepthEnable = true;
		descDSD.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		descDSD.DepthFunc = D3D11_COMPARISON_LESS;
		descDSD.StencilEnable=true;
		descDSD.StencilReadMask = 0xFF;
		descDSD.StencilWriteMask = 0xFF;
		descDSD.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDSD.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		descDSD.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDSD.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		descDSD.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDSD.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		descDSD.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDSD.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		d3dDevice->CreateDepthStencilState(&descDSD, &depthStencilState);
		
		d3dContext->OMSetDepthStencilState(depthStencilState, 1);
	}

	void TurnOffDepth()
	{
		D3D11_DEPTH_STENCIL_DESC descDDSD;
		ZeroMemory(&descDDSD, sizeof(descDDSD));
		descDDSD.DepthEnable = false;
		descDDSD.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		descDDSD.DepthFunc = D3D11_COMPARISON_LESS;
		descDDSD.StencilEnable=true;
		descDDSD.StencilReadMask = 0xFF;
		descDDSD.StencilWriteMask = 0xFF;
		descDDSD.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDDSD.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		descDDSD.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDDSD.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		descDDSD.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDDSD.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		descDDSD.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDDSD.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		d3dDevice->CreateDepthStencilState(&descDDSD, &depthStencilDisabledState);
		d3dContext->OMSetDepthStencilState(depthStencilDisabledState, 1);
	}

	void billCargaFuego()
	{
		uv1[0].u = .125;
		uv2[0].u = .125;
		uv3[0].u = 0;
		uv4[0].u = 0;

		uv1[0].v = .25;
		uv2[0].v = 0;
		uv3[0].v = 0;
		uv4[0].v = .25;


		for (int j = 0; j < 8; j++) {
			uv1[j].u = uv1[0].u + (j * .125);
			uv2[j].u = uv2[0].u + (j * .125);
			uv3[j].u = uv3[0].u + (j * .125);
			uv4[j].u = uv4[0].u + (j * .125);

			uv1[j].v = .25;
			uv2[j].v = 0;
			uv3[j].v = 0;
			uv4[j].v = .25;
		}
		for (int j = 0; j < 8; j++) {
			uv1[j + 8].u = uv1[0].u + (j * .125);
			uv2[j + 8].u = uv2[0].u + (j * .125);
			uv3[j + 8].u = uv3[0].u + (j * .125);
			uv4[j + 8].u = uv4[0].u + (j * .125);

			uv1[j + 8].v = .5;
			uv2[j + 8].v = .25;
			uv3[j + 8].v = .25;
			uv4[j + 8].v = .5;
		}

		for (int j = 0; j < 8; j++) {
			uv1[j + 16].u = uv1[0].u + (j * .125);
			uv2[j + 16].u = uv2[0].u + (j * .125);
			uv3[j + 16].u = uv3[0].u + (j * .125);
			uv4[j + 16].u = uv4[0].u + (j * .125);

			uv1[j + 16].v = .75;
			uv2[j + 16].v = .5;
			uv3[j + 16].v = .5;
			uv4[j + 16].v = .75;
		}

		for (int j = 0; j < 8; j++) {
			uv1[j + 24].u = uv1[0].u + (j * .125);
			uv2[j + 24].u = uv2[0].u + (j * .125);
			uv3[j + 24].u = uv3[0].u + (j * .125);
			uv4[j + 24].u = uv4[0].u + (j * .125);

			uv1[j + 24].v = 1;
			uv2[j + 24].v = .75;
			uv3[j + 24].v = .75;
			uv4[j + 24].v = 1;
		}
	}

};
#endif