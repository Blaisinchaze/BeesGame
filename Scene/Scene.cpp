#include "GameProgExercise01.h"
#include "DX\DeviceResources.h"
#include "Core.h"
#include "Utils\File.h"
#include "Scene\Scene.h"
#include "Scene\Entities\Hive.h"
#include "Scene\Entities\Ground.h"
#include "Scene\Entities\Bee.h"
#include "Scene\Entities\Flower.h"
#include "Scene\Entities\TestObject.h"
#include "DX\Input.h"
#include "DX\View.h"
#include "Utils\Maths.h"

using namespace DirectX;

namespace scene
{

Scene::Scene()
{
	m_ground = new Ground();
	m_flower = new Flower();
	m_flowerBedScale = 8;
	m_hiveCount = 10;
}

Scene::~Scene()
{
	delete m_ground;
	delete m_flower;
}

void Scene::Initialise()
{
#pragma region DXSceneInitialisation



	Core* const core = Core::Get();

	const DX::DeviceResources* const deviceResources = core->GetDeviceResources();

	HRESULT hr = 0;
	DWORD size = 0;

	ID3D11Device* const device = deviceResources->GetD3DDevice();

	void* vertShaderData = nullptr;

	HANDLE vsHandle = INVALID_HANDLE_VALUE;
	HANDLE psHandle = INVALID_HANDLE_VALUE;

	// ** UNLIT SHADERS **

	// Load and create the vertex shader for the unlit type of vertices
	vsHandle = utils::file::GetFileData( "VertexShader.cso", &vertShaderData, &size );

	hr = device->CreateVertexShader( vertShaderData, size,
		nullptr, &m_shaderData[ ShaderTypes::Unlit ].vertexShader );
	ASSERT_HANDLE( hr );

	// Create input layout.
	static const D3D11_INPUT_ELEMENT_DESC s_inputElementDescUnlit[ 2 ] =
	{
		{ "POSITION",   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA,  0 },
		{ "COLOR",      0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA , 0 },
	};

	hr = device->CreateInputLayout( s_inputElementDescUnlit, _countof( s_inputElementDescUnlit ),
		vertShaderData, size,
		&m_shaderData[ ShaderTypes::Unlit ].inputLayout );
	ASSERT_HANDLE( hr );

	utils::file::CloseFile( vsHandle );

	void* pixelShaderData = nullptr;

	// Handle loading and creating the pixel shader for the unlit type of vertices
	psHandle = utils::file::GetFileData( "PixelShader.cso", &pixelShaderData, &size );

	hr = device->CreatePixelShader( pixelShaderData, size,
		nullptr, &m_shaderData[ ShaderTypes::Unlit ].pixelShader );
	ASSERT_HANDLE( hr );

	utils::file::CloseFile( psHandle );



	// ** LIT SHADERS **

	// Load and create the vertex shader for the unlit type of vertices
	vsHandle = utils::file::GetFileData( "VertexShaderLit.cso", &vertShaderData, &size );

	hr = device->CreateVertexShader( vertShaderData, size,
		nullptr, &m_shaderData[ ShaderTypes::Lit ].vertexShader );
	ASSERT_HANDLE( hr );

	// Create input layout.
	static const D3D11_INPUT_ELEMENT_DESC s_inputElementDescLit[ 3 ] =
	{
		{ "POSITION",   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA,  0 },
		{ "NORMAL",     0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 16,  D3D11_INPUT_PER_VERTEX_DATA,  0 },
		{ "COLOR",      0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA , 0 }
	};

	hr = device->CreateInputLayout( s_inputElementDescLit, _countof( s_inputElementDescLit ),
		vertShaderData, size,
		&m_shaderData[ ShaderTypes::Lit ].inputLayout );
	ASSERT_HANDLE( hr );

	utils::file::CloseFile( vsHandle );

	pixelShaderData = nullptr;

	// Handle loading and creating the pixel shader for the unlit type of vertices
	psHandle = utils::file::GetFileData( "PixelShaderLit.cso", &pixelShaderData, &size );

	hr = device->CreatePixelShader( pixelShaderData, size,
		nullptr, &m_shaderData[ ShaderTypes::Lit ].pixelShader );
	ASSERT_HANDLE( hr );

	utils::file::CloseFile( psHandle );


#pragma endregion
	// TODO: Replace this with some proper camera code
	DX::View* const view = core->GetView();
	view->SetViewPosition( XMVECTOR{ -5.0f, 10.0f, 5.0f, 0.0f } );
	XMVECTOR viewDirection = XMVector3Normalize( XMVECTOR{ 5.0f, -10.0f, -5.0f, 0.0f } );
	view->SetViewDirection( viewDirection );

	XMVECTOR position;
	XMFLOAT4 colour;

	for (int i = -2; i < m_hiveCount - 2; i++)
	{
		if(i == 0){continue;}

		Hive* tempHive = new Hive();
		tempHive->Initialise();

		const float angleStride = DirectX::XM_2PI / static_cast<float>(m_hiveCount);
		const float startingAngleSin = DirectX::XMScalarSin((angleStride * i));
		const float startingAngleCos = DirectX::XMScalarCos((angleStride * i));


		position = XMVectorSet(startingAngleSin * (m_flowerBedScale / 1.5f), 
			(1.0f), startingAngleCos * (m_flowerBedScale / 1.5f), (0.0f));
		tempHive->SetPosition(position);

		//Calculate the angle to rotate the camera to face 
		XMVECTOR directionVector = XMVECTOR{ 0.0f,1.0f,0.0f,0.0f } - position;
		tempHive->SetOrientation(XMVector3Normalize(directionVector));

		m_hiveList.push_back(tempHive);
	}

	for (int x = 0; x < m_flowerBedScale; x++)
	{
		for (int y = 0; y < m_flowerBedScale; y++)
		{
			Flower* tempFlower = new Flower();
			tempFlower->Initialise();
			float offsetX = static_cast<float>(x);
			float offsetY = static_cast<float>(y);
			position = XMVectorSet((offsetX - m_flowerBedScale / 2) + utils::RandRange(1) / 2, utils::RandRange(1) + 0.2f, (offsetY - m_flowerBedScale / 2) + utils::RandRange(1) / 2, 0.0f);
			tempFlower->SetPosition(position);
			tempFlower->SetOrientation(XMVector3Normalize(XMVECTOR{ utils::RandRange(1),0.0f,utils::RandRange(1),utils::RandRange(1) }));
			m_flowerList.push_back(tempFlower);
		}
	}


	m_ground->Initialise();

	position = XMVectorSet(0.1f, 0.0f, 0.0f, 1.0f);
	m_ground->SetPosition(position);
	m_ground->SetScale(XMVECTORF32{ static_cast<float>(m_flowerBedScale) + 1.0f,
		0.1f,  static_cast<float>( m_flowerBedScale) + 1.0f, 1.0f });
	colour = { 0.6f, 0.361f, 0.267f, 1.0f };
	m_ground->SetColour(colour);

}

void Scene::Shutdown()
{
	HiveListItor hiveItor = m_hiveList.begin();
	while (hiveItor != m_hiveList.end())
	{
		Hive* hive = *hiveItor;
		hive->Shutdown();
		delete hive;
		++hiveItor;
	}

	BeeListItor beeItor = m_beeList.begin();
	while (beeItor != m_beeList.end())
	{
		Bee* bee = *beeItor;
		bee->Shutdown();
		delete bee;
		++beeItor;
	}
	FlowerListItor flowerItor = m_flowerList.begin();
	while (flowerItor != m_flowerList.end())
	{
		Flower* flower = *flowerItor;
		flower->Shutdown();
		delete flower;
		++flowerItor;
	}

	m_ground->Shutdown();

	for( UINT shaderTypeIndex = 0; shaderTypeIndex < ShaderTypes::NumShaderTypes; ++shaderTypeIndex )
	{
		if( m_shaderData[ shaderTypeIndex ].inputLayout != nullptr )
		{
			m_shaderData[ shaderTypeIndex ].inputLayout->Release();
		}
		if( m_shaderData[ shaderTypeIndex ].vertexShader != nullptr )
		{
			m_shaderData[ shaderTypeIndex ].vertexShader->Release();
		}
		if( m_shaderData[ shaderTypeIndex ].pixelShader != nullptr )
		{
			m_shaderData[ shaderTypeIndex ].pixelShader->Release();
		}
	}
}

void Scene::Update()
{

	HiveListItor hiveItor = m_hiveList.begin();
	while (hiveItor != m_hiveList.end())
	{
		Hive* hive = *hiveItor;
		hive->Update();

		++hiveItor;
	}

	BeeListItor beeItor = m_beeList.begin();
	while (beeItor != m_beeList.end())
	{
		Bee* bee = *beeItor;
		bee->Update();

		++beeItor;
	}
	FlowerListItor flowerItor = m_flowerList.begin();
	while (flowerItor != m_flowerList.end())
	{
		Flower* flower = *flowerItor;
		flower->Update();

		++flowerItor;
	}

	m_ground->Update();


	Core* const core = Core::Get();
	DX::Input* input = core->GetInput();
	const float leftRight = input->GetLeftRight();
	const float upDown = input->GetUpDown();

	DX::View* const view = core->GetView();
	view->RotateCamera(leftRight, upDown);

	const float wasdLeftRight = input->GetWASDLeftRight();
	const float wasdUpDown = input->GetWASDUpDown();
	view->MoveLookAtPoint(wasdLeftRight, wasdUpDown);

}

void Scene::Render()
{
	//m_testObject1->Render();
	//m_testObject2->Render();


	HiveListItor hiveItor = m_hiveList.begin();
	while (hiveItor != m_hiveList.end())
	{
		Hive* hive = *hiveItor;
		hive->Render();

		++hiveItor;
	}

	BeeListItor beeItor = m_beeList.begin();
	while (beeItor != m_beeList.end())
	{
		Bee* bee = *beeItor;
		bee->Render();

		++beeItor;
	}

	FlowerListItor flowerItor = m_flowerList.begin();
	while (flowerItor != m_flowerList.end())
	{
		Flower* flower = *flowerItor;
		flower->Render();

		++flowerItor;
	}

	m_ground->Render();
}

void Scene::ActivateShaders( const ShaderTypes shaderType )
{
	ASSERT( shaderType < ShaderTypes::NumShaderTypes, "Shader type ID is out of range.\n" );
	Core* const core = Core::Get();

	const DX::DeviceResources* const deviceResources = core->GetDeviceResources();

	ID3D11DeviceContext* const deviceContext = deviceResources->GetD3DDeviceContext();

	// Set input assembler state.
	deviceContext->IASetInputLayout( m_shaderData[ shaderType ].inputLayout );

	// Set shaders.
	deviceContext->VSSetShader( m_shaderData[ shaderType ].vertexShader, nullptr, 0 );
	deviceContext->GSSetShader( nullptr, nullptr, 0 );
	deviceContext->PSSetShader( m_shaderData[ shaderType ].pixelShader, nullptr, 0 );
}

void Scene::SpawnBee(Hive* startingHive, DirectX::XMFLOAT4 colour, DirectX::XMVECTOR position)
{
	Bee* tempBee = new Bee();
	tempBee->Initialise();
	tempBee->SetPosition(position);
	tempBee->SetScale(XMVECTORF32{ 0.08f, 0.05f, 0.1f, 1.0f });
	tempBee->SetColour(colour);
	tempBee->SetStartingHive(startingHive);
	m_beeList.push_back(tempBee);
}

Flower* Scene::ReturnRandomFlower() 
{
	int randomFlower = static_cast<int>( utils::RandRange(m_flowerBedScale * m_flowerBedScale));
	FlowerListItor flowerItor = m_flowerList.begin();
	for (int i = 0; i < randomFlower; i++)
	{
		++flowerItor;
	}

	Flower* flower = *flowerItor;

	return flower;
}



} // namespace scene