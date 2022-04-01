#include "Application.h"
#include "Math3D.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
        
    // Keyboard messages
    case WM_KEYDOWN:
    {
        unsigned char key = static_cast<unsigned char>(wParam);
        if (KeyboardClass::IsKeyAutoRepeat())
        {
            KeyboardClass::OnKeyPressed(key);
        }
        else
        {
            const bool wasPressed = lParam &(1<<30); // check the 30th bit to get if the key was pressed 
            if (!wasPressed)
                KeyboardClass::OnKeyPressed(key);
        }
        break;
    }
    case WM_KEYUP:
    {
        unsigned char key = static_cast<unsigned char>(wParam);
        KeyboardClass::OnKeyReleased(key);
        break;
    }
    case WM_CHAR: 
    {
        unsigned char ch = static_cast<unsigned char>(wParam);
        if (KeyboardClass::IsCharsAutoRepeat())
        {
            KeyboardClass::OnCharInput(ch);
        }
        else
        {
            const bool wasPressed = lParam &(1<<30); // check the 30th bit to get if the key was pressed 
            if (!wasPressed)
            {
                KeyboardClass::OnCharInput(ch);
            }
        }
        break;
    }
    // Mouse messages
    case WM_MOUSEMOVE:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        MouseClass::OnMouseMove(x, y);
        return 0;
    }
    case WM_LBUTTONDOWN:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        MouseClass::OnLeftClick(x, y);
        return 0;
    }
    case WM_LBUTTONUP:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        MouseClass::OnLeftRelease(x, y);
        return 0;
    }
    case WM_RBUTTONDOWN:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        MouseClass::OnRightClick(x, y);
        return 0;
    }
    case WM_RBUTTONUP:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        MouseClass::OnRightRelease(x, y);
        return 0;
    }
    case WM_MBUTTONDOWN:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        MouseClass::OnScrollClick(x, y);
        return 0;
    }
    case WM_MBUTTONUP:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        MouseClass::OnScrollRelease(x, y);
        return 0;
    }
    case WM_MOUSEWHEEL:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
        {
            MouseClass::OnScrollUp(x, y);
        }
        else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
        {
            MouseClass::OnScrollDown(x, y);
        }
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

Application::Application()
{
    // devices or rendering
    m_hInst = nullptr;
    m_hWnd = nullptr;
    m_driverType = D3D_DRIVER_TYPE_NULL;
    m_featureLevel = D3D_FEATURE_LEVEL_11_0;
    m_pd3dDevice = nullptr;
    m_pImmediateContext = nullptr;
    m_pSwapChain = nullptr;
    m_pRenderTargetView = nullptr;
    m_pVertexLayout = nullptr;
    // constant buffers
    m_pConstantBuffer = nullptr;
    // textures
    m_pTextureNrms = nullptr;
    m_pTextureSunRV = nullptr;
    m_pTextureMoonRV = nullptr;
    // objs
    m_GameTimer = nullptr;
}

Application::~Application()
{
    Cleanup();
}

HRESULT Application::Initialise(HINSTANCE hInstance, int nCmdShow)
{
    if (FAILED(InitWindow(hInstance, nCmdShow)))
    {
        return E_FAIL;
    }

    RECT rc;
    GetClientRect(m_hWnd, &rc);
    m_WindowWidth = rc.right - rc.left;
    m_WindowHeight = rc.bottom - rc.top;

    if (FAILED(InitDevice()))
    {
        Cleanup();

        return E_FAIL;
    }

    // Initialize the world matrix
    XMStoreFloat4x4(&m_world, XMMatrixIdentity());

    InitCameras();

    // Create the sample state
    D3D11_SAMPLER_DESC sampDesc;

    ZeroMemory(&sampDesc, sizeof(sampDesc));

    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

    m_pd3dDevice->CreateSamplerState(&sampDesc, &m_pSamplerLinear);

    // setup the game timer
    m_GameTimer = make_unique<GameTimer>();
    m_GameTimer->Start();

    // start the game in a paused state
    m_GameTimer->Pause();

    //Initialize the logger
    Log::Init();
    CORE_INFO("Initialized loggers!");

    KeyboardClass::Init();

    return S_OK;
}

HRESULT Application::InitShadersAndInputLayout()
{
    HRESULT hr = S_OK;

    // Define the input layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    
    UINT numElements = ARRAYSIZE(layout);

    m_pTemplateShader = new BaseShader(hr, L"DX11-Framework/Shaders/DX11 Framework.fx", m_pd3dDevice, layout, numElements);
    m_pTemplateShader->BindInputLayout(m_pImmediateContext);

    return hr;
}

void Application::InitCameras()
{
    // Initialize the camera variables
    XMFLOAT3 Position = { 0.0f, 0.0f,-3.0f };

    // setup the camera
    m_StaticCamera = new Camera(Position, m_WindowWidth, m_WindowHeight, 0.1f, 100.0f);
    m_StaticCamera->Update();

    // setup first person camera
    m_FPCamera = new FP_Camera(Position, m_WindowWidth, m_WindowHeight, 0.1f, 100.0f);
    m_FPCamera->Update();

    // setup topdown camera
    XMFLOAT3 pos = {0.0f, 4.0f, 0.0f};
    XMFLOAT3 look = {0.0f, -1.0f, 0.0f};
    XMFLOAT3 right = {1.0f, 0.0f, 0.0f};
    XMFLOAT3 up = {0.0f, 0.0f, 1.0f};

    m_TopDownCamera = new Camera(pos, look, right, up, m_WindowWidth, m_WindowHeight, 0.1f, 100.0f);

    // set the starting camera
    m_MainCamera = (Camera*)m_FPCamera;
    XMStoreFloat3(&m_cb.EyePosW, m_MainCamera->GetPosition());
}

void Application::InitObjects()
{
    // init the physics system
    m_PhysicsSystem = PhysicsSystem::GetInstance();

    // setup mesh data and material
    MeshData meshData = OBJLoader::Load("DX11-Framework/3D_Models/Blender/MoonTest.obj", m_pd3dDevice);

    Geometry geometry;
    geometry.indexCount = meshData.IndexCount;
    geometry.indexBuffer = meshData.IndexBuffer;
    geometry.vertexBuffer = meshData.VertexBuffer;
    geometry.vertexBufferOffset = meshData.VBOffset;
    geometry.vertexBufferStride = meshData.VBStride;

    Material material;
    material.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    material.specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
    material.ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
    material.specularPower = 10.0f;

    m_Sun = make_unique<GameObject>("TheSun", geometry, &material, 1.0f);
    m_Sun->GetTransform()->SetPosition(0.0f, 0.0f, 0.0f);
    m_Sun->GetTransform()->SetRotation(0.0f, 0.0f, 0.0f);
    m_Sun->GetTransform()->SetScale(1.0f, 1.0f, 1.0f);

    m_Mars = make_unique<GameObject>("Mars", geometry, &material, 1.0f);
    m_Mars->GetTransform()->SetPosition(5.0f, 0.0f, 0.0f);
    m_Mars->GetTransform()->SetRotation(0.0f, 0.0f, 0.0f);
    m_Mars->GetTransform()->SetScale(1.0f, 1.0f, 1.0f);

    m_MainPlayerPawn = new PlayerPawn(OBJLoader::Load("DX11-Framework/3D_Models/Blender/MoonTest.obj", m_pd3dDevice),
        XMFLOAT3(0.0f, 1.0f, -2.0f), m_WindowHeight, m_WindowWidth, 0.1f, 100.0f);
}

void Application::InitLights()
{
    // Light direction from surface 
    m_LightDirection = XMFLOAT3(1, 1.0f, -2.0f);
    m_cb.LightVecW = m_LightDirection;

    // Ambient light info
    m_cb.AmbientMtrl = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    m_cb.AmbientLight = XMFLOAT4(0.05f, 0.05f, 0.05f, 1.0f);

    // Diffuse material properties 
    m_DiffuseMaterial = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
    m_cb.DiffuseMtrl = m_DiffuseMaterial;

    // Diffuse light color
    m_DiffuseLight = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    m_cb.DiffuseLight = m_DiffuseLight;

    // Specular light info
    m_cb.SpecularMaterial = { 0.3f, 0.3f, 0.3f, 1.0f };
    m_cb.SpecularLight = { 1.0f, 1.0f, 1.0f, 1.0f };
    m_cb.SpecularPower = 10.0f;

}

HRESULT Application::InitTextures()
{
    HRESULT hr = CreateDDSTextureFromFile(m_pd3dDevice, L"DX11-Framework/Textures/SunTex.dds", nullptr, &m_pTextureSunRV);

    if (FAILED(hr))
        return hr;

    m_Sun->GetAppearance()->SetTextureRV(m_pTextureSunRV);

    hr = CreateDDSTextureFromFile(m_pd3dDevice, L"DX11-Framework/Textures/MoonTex.dds", nullptr, &m_pTextureMoonRV);

    if (FAILED(hr))
        return hr;

    hr = CreateDDSTextureFromFile(m_pd3dDevice, L"DX11-Framework/Textures/EarthTex.dds", nullptr, &m_pTextureEarthRV);

    if (FAILED(hr))
        return hr;

    hr = CreateDDSTextureFromFile(m_pd3dDevice, L"DX11-Framework/Textures/MarsTex.dds", nullptr, &m_pTextureMarsRV);

    if (FAILED(hr))
        return hr;

    m_Mars->GetAppearance()->SetTextureRV(m_pTextureMarsRV);

    return S_OK;
}

HRESULT Application::InitWindow(HINSTANCE hInstance, int nCmdShow)
{
    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_TUTORIAL1);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = L"TutorialWindowClass";
    wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_TUTORIAL1);
    if (!RegisterClassEx(&wcex))
        return E_FAIL;

    // Create window
    m_hInst = hInstance;
    RECT rc = { 0, 0, 1920, 1080 };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    m_hWnd = CreateWindow(L"TutorialWindowClass", L"DX11 Framework", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
        nullptr);
    if (!m_hWnd)
        return E_FAIL;

    ShowWindow(m_hWnd, nCmdShow);
    ShowCursor(FALSE);
    m_MousePos = new POINT();

    return S_OK;
}

HRESULT Application::InitDevice()
{
    HRESULT hr = S_OK;

    UINT createDeviceFlags = 0;

#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };

    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };

    UINT numFeatureLevels = ARRAYSIZE(featureLevels);

    // setup the buffer
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = m_WindowWidth;
    sd.BufferDesc.Height = m_WindowHeight;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = m_hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
    {
        m_driverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDeviceAndSwapChain(nullptr, m_driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
            D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pd3dDevice, &m_featureLevel, &m_pImmediateContext);
        if (SUCCEEDED(hr))
            break;
    }

    if (FAILED(hr))
        return hr;

    // Create a render target view
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

    if (FAILED(hr))
        return hr;

    hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
    pBackBuffer->Release();

    if (FAILED(hr))
        return hr;

    //
    // Setup the depth buffer/stencil 
    //
    D3D11_TEXTURE2D_DESC depthStencilDesc;

    depthStencilDesc.Width = m_WindowWidth;
    depthStencilDesc.Height = m_WindowHeight;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;

    hr = m_pd3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, &m_pDepthStencilBuffer);
    if (FAILED(hr))
        return hr;

    hr = m_pd3dDevice->CreateDepthStencilView(m_pDepthStencilBuffer, nullptr, &m_pDepthStencilView);
    if (FAILED(hr))
        return hr;

    m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

    // Setup the viewport
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)m_WindowWidth;
    vp.Height = (FLOAT)m_WindowHeight;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    m_pImmediateContext->RSSetViewports(1, &vp);

    InitShadersAndInputLayout();

    InitObjects();

    InitLights();

    InitTextures();

    // Set primitive topology
    m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Create the constant buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(ConstantBuffer);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    hr = m_pd3dDevice->CreateBuffer(&bd, nullptr, &m_pConstantBuffer);

    if (FAILED(hr))
        return hr;

    // Setup rasterizer for wire frame
    D3D11_RASTERIZER_DESC wfdesc;
    ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
    wfdesc.FillMode = D3D11_FILL_WIREFRAME;
    wfdesc.CullMode = D3D11_CULL_NONE;
    hr = m_pd3dDevice->CreateRasterizerState(&wfdesc, &m_pWireFrame);

    return S_OK;
}

void Application::Cleanup()
{
    if (m_pImmediateContext) m_pImmediateContext->ClearState();

    if (m_pConstantBuffer) m_pConstantBuffer->Release();

    // Clean up the planets
    //if (m_Mars) delete m_Mars;
    //if (m_Earth) delete m_Earth;
    //if (m_MoonEarth) delete m_MoonEarth;
    //if (m_MoonMars) delete m_MoonMars;
    //if (m_Pyramid) delete m_Pyramid;

    if (m_pRenderTargetView) m_pRenderTargetView->Release();
    if (m_pSwapChain) m_pSwapChain->Release();
    if (m_pImmediateContext) m_pImmediateContext->Release();
    if (m_pd3dDevice) m_pd3dDevice->Release();
    if (m_pWireFrame) m_pWireFrame->Release();
    if (m_pDepthStencilView) m_pDepthStencilView->Release();
    if (m_pDepthStencilBuffer) m_pDepthStencilBuffer->Release();
    //if (m_GameTimer) delete m_GameTimer;
    //
    //// Cleanup the cameras
    //if (m_MainCamera) delete m_MainCamera;
    //if (m_StaticCamera) delete m_StaticCamera;
    //if (m_FPCamera) delete m_FPCamera;
}

void Application::Update()
{
    m_GameTimer->Tick();
    float dt = m_GameTimer->GetDeltaTime();

    HandleInput();
    GetCursorPos(m_MousePos);


    // Reset the mouse if it goes out of bounds
    // 
    if (!m_GameTimer->GetPauseState())
    {
        if (m_MousePos->x < 50 || m_MousePos->x > m_WindowWidth)
        {
            MouseClass::ResetMousePos(m_WindowWidth/2, m_WindowHeight/2);
        }
        
        if (m_MousePos->y < 50 || m_MousePos->y > m_WindowHeight)
        {
            MouseClass::ResetMousePos(m_WindowWidth/2, m_WindowHeight/2);
        }
    }

    // Update the camera
    m_MainCamera->Update();

    XMStoreFloat3(&m_cb.EyePosW, m_MainCamera->GetPosition());

    float t = m_GameTimer->GetGameTime() / 2.0f;

    // Moon for Earth
    //m_MainPlayerPawn->Update();
    m_Sun->Update(dt);
    m_Mars->Update(dt);
}

void Application::Draw()
{
    //
    // Clear the back buffer
    //
    float ClearColor[4] = { 0.0f, 0.0f, 0.3f, 1.0f }; // red,green,blue,alpha   
    m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor);

    //
    // Clear the depth/stencil view
    //
    m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    //
    // Setup the transformation matrices
    //
    XMMATRIX world = XMLoadFloat4x4(&m_world);
    XMMATRIX view = m_MainCamera->GetViewMatrix();
    XMMATRIX projection = m_MainCamera->GetProjMatrix();

    //
    //   Update variables
    //
    m_cb.mWorld = XMMatrixTranspose(world);
    m_cb.mView = XMMatrixTranspose(view);
    m_cb.mProjection = XMMatrixTranspose(projection);
    m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &m_cb, 0, 0);

    //
    // Setup constant buffer and shaders
    //
    m_pImmediateContext->VSSetShader(m_pTemplateShader->GetVertexShader(), nullptr, 0);
    m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
    m_pImmediateContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
    m_pImmediateContext->PSSetShader(m_pTemplateShader->GetPixelShader(), nullptr, 0);
    m_pImmediateContext->PSSetSamplers(0, 1, &m_pSamplerLinear);
    m_pImmediateContext->PSSetShaderResources(0, 1, &m_pTextureSunRV);
    m_pImmediateContext->CSSetShaderResources(1, 1, &m_pTextureNrms);

    //
    // Render Object
    //
    m_Sun->Draw(m_cb, m_pConstantBuffer, m_pImmediateContext);
    m_Mars->Draw(m_cb, m_pConstantBuffer, m_pImmediateContext);

    //
    // Present our back buffer to our front buffer
    //
    m_pSwapChain->Present(0, 0);
}

void Application::HandleInput()
{
    float dt = m_GameTimer->GetDeltaTime();


    if (!m_GameTimer->GetPauseState())
    {
        m_MainCamera->HandleInput(dt);

        // check the key queue for any key inputs (key inputs meant to be repeated should be kept outside this function)
        if (!KeyboardClass::KeyBufferIsEmpty() && !m_Typing)
        {
            unsigned int key = KeyboardClass::ReadKey().GetKeyCode();
            
            // Handle input for all the game objects
            m_Sun->HandleInput(dt, key);

            // Main logic for responding to key input
            switch (key)
            {
                case VK_F1:
                    m_pImmediateContext->RSSetState(m_pWireFrame);
                    break;
                case VK_F2:
                    m_pImmediateContext->RSSetState(nullptr);
                    break;
                case '1':
                    m_MainCamera = (Camera*)m_FPCamera;
                    break;
                case '2':
                    m_MainCamera = m_StaticCamera;
                    break;
                case '3':
                    m_MainCamera = m_TopDownCamera;
                    break;
                case '4':
                    CORE_INFO(m_PhysicsSystem->CollisionCheck(m_Mars.get(), m_Sun.get()));
                    break;
                case VK_ESCAPE:
                    if (KeyboardClass::IsKeyPressed(VK_ESCAPE))
                    {
                        m_GameTimer->Pause();
                        MouseClass::ResetMousePos(m_WindowWidth/2, m_WindowHeight/2);
                    }
                    break;
                default:
                    break;
            }
        }
        if (!MouseClass::IsEventBufferEmpty())
        {
            MouseEvent e = MouseClass::ReadEvent();

            if (e.GetType() == MouseEvent::EventType::MOVE)
            {
                m_MainCamera->RotateP(MouseClass::GetDY());
                m_MainCamera->RotateY(MouseClass::GetDX());
                m_MainCamera->UpdateViewMatrix();
            }
        }
    }
    else 
    {
        if (!KeyboardClass::KeyBufferIsEmpty())
        {
             int key = KeyboardClass::ReadKey().GetKeyCode();
                
             if (key == VK_ESCAPE)
             {
                 if (KeyboardClass::IsKeyPressed(key))
                 {
                     m_GameTimer->Start();
                     MouseClass::ResetMousePos(m_WindowWidth/2, m_WindowHeight/2);
                 }
             }
        }
    }
}