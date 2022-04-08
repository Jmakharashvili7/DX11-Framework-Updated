#pragma once
#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include <iostream>
#include <string>
#include <memory>
#include <vector>

#include "DDSTextureLoader.h"
#include "resource.h"	
#include "structs.h"
#include "OBJLoader.h"
#include "Camera.h"
#include "BaseShader.h"
#include "KeyboardClass.h"
#include "MouseClass.h"
#include "FPCamera.h"
#include "GameTime.h"
#include "PlayerPawn.h"
#include "Log.h"
#include "GameObject.h"
#include "Transform.h"
#include "Appearance.h"
#include "ParticleModel.h"
#include "PhysicsSystem.h"
#include "ParticleModel.h"

using namespace DirectX;

class Application
{
private:
	HINSTANCE                 m_hInst;
	HWND                      m_hWnd;
	D3D_DRIVER_TYPE           m_driverType;
	D3D_FEATURE_LEVEL         m_featureLevel;
	ID3D11Device			 *m_pd3dDevice;
	ID3D11DeviceContext      *m_pImmediateContext;
	IDXGISwapChain           *m_pSwapChain;
	ID3D11RenderTargetView   *m_pRenderTargetView;
	ID3D11RasterizerState    *m_pWireFrame;
	ID3D11InputLayout        *m_pVertexLayout;
	ID3D11Buffer			 *m_pConstantBuffer;
	ID3D11DepthStencilView   *m_pDepthStencilView;
	ID3D11Texture2D          *m_pDepthStencilBuffer;
	POINT					 *m_MousePos;
	// Keyboard input manager
	// Shaders
	BaseShader               *m_pTemplateShader;

	// Texture Resource View
	ID3D11ShaderResourceView *m_pTextureSunRV,*m_pTextureMoonRV, *m_pTextureEarthRV, *m_pTextureMarsRV, *m_pTextureNrms;
	ID3D11SamplerState       *m_pSamplerLinear = nullptr;

	// Other texture Rvs
	ID3D11ShaderResourceView* _pGroundTextureRV = nullptr;
	ID3D11ShaderResourceView* _pHerculesTextureRV = nullptr;
	ID3D11SamplerState*		  _pSamplerLinear = nullptr;

	// View Matrices
	XMFLOAT4X4                m_world;
	// Lighting variables	  
	XMFLOAT3				  m_LightDirection;
	XMFLOAT4				  m_DiffuseMaterial, m_DiffuseLight;
	// Consant buffer		  
	ConstantBuffer			  m_cb;
	// Cameras
	Camera                    *m_MainCamera, *m_StaticCamera, *m_TopDownCamera;
	FP_Camera                 *m_FPCamera;

	// OBJ Game Objects
	std::vector<GameObject*>  m_GameObjects;
	unique_ptr<GameObject>	  m_Sun, m_Mars, m_Earth, m_Moon;
	PlayerPawn                *m_MainPlayerPawn;

	// variables for game state
	bool					   m_Typing = false;

	// variable for game time
	unique_ptr<GameTimer>	   m_GameTimer;
	PhysicsSystem*             m_PhysicsSystem;
private:
	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
	HRESULT InitDevice();
	void Cleanup();
	void InitLights();
	HRESULT InitTextures();
	HRESULT InitShadersAndInputLayout();
	void InitCameras();
	void InitObjects();

	UINT m_WindowHeight;
	UINT m_WindowWidth;

public:
	Application();
	~Application();

	HRESULT Initialise(HINSTANCE hInstance, int nCmdShow);

	void Update();
	void Draw();
	void HandleInput();
};

