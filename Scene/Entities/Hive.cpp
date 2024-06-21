#include "GameProgExercise01.h"
#include "DX\DeviceResources.h"
#include "Core.h"
#include "Hive.h"
#include "..\EntityHelper.h"
#include <DirectXMath.h>
#include "Utils\Maths.h"

using namespace DirectX;
using namespace utils;


namespace scene
{

	Hive::Hive()
	{
		m_shaderType = Scene::ShaderTypes::Lit;
		m_spawnTimer = RandRange( 2 );
		m_collectedNectar = 0;
		m_beeCount = 15;
		m_colour = XMFLOAT4{ RandRange( 1 ), RandRange( 1 ), RandRange( 1 ) , 1.0f };
	}

	void Hive::Initialise()
	{
		Entity::Initialise();

		Core* const core = Core::Get();

		const DX::DeviceResources* const deviceResources = core->GetDeviceResources();

		HRESULT hr = 0;

		ID3D11Device* const device = deviceResources->GetD3DDevice();

		UINT numCubeVertices = 0;
		const Entity::VertexLit* cubeVerts = helper::GetCubeVertices( numCubeVertices );

		D3D11_SUBRESOURCE_DATA initialData = {};
		initialData.pSysMem = cubeVerts;

		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.ByteWidth = numCubeVertices * sizeof( VertexLit );
		bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.StructureByteStride = sizeof( VertexLit );

		// Note the vertex buffer is released in the base class
		hr = device->CreateBuffer( &bufferDesc, &initialData,
			&m_vertexBuffer );
		ASSERT_HANDLE( hr );


		SetColour( m_colour );
		m_spawnTimer = RandRange( 1 ) + 0.1f;
	}

	void Hive::Shutdown()
	{
		Entity::Shutdown();
	}

	void Hive::Update()
	{
		Entity::Update();

		//Lowers m_spawner by delta time to ensure the spawn counter is accurate. 
		m_spawnTimer -= Timers::GetFrameTime();
		if ( m_spawnTimer < 0 && m_beeCount > 0 ) {
			Core* const core = Core::Get();
			core->GetScene()->SpawnBee( this, m_colour, Entity::m_position );
			m_spawnTimer = RandRange( 1 ) + 0.1f;
			--m_beeCount;
			
		}


		float hiveScale = ( m_collectedNectar / 10000 ) + 1;
		SetScale( XMVECTORF32{ 0.5f * hiveScale, 1.0f * hiveScale, 0.5f * hiveScale, 1.0f } );
	}

	void Hive::Render()
	{
		// Ensure the base class is called as this sets up the shaders for drawing
		Entity::Render();

		Core* const core = Core::Get();

		const DX::DeviceResources* const deviceResources = core->GetDeviceResources();
		ID3D11DeviceContext* const context = deviceResources->GetD3DDeviceContext();

		// Draw the triangles
		UINT strides = sizeof( VertexLit );
		UINT offsets = 0;
		context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
		context->IASetVertexBuffers( 0, 1, &m_vertexBuffer, &strides, &offsets );
		context->Draw( NumVertices, 0 );
	}

	void Hive::AddNectar( float nectarToStore )
	{
		m_collectedNectar += nectarToStore;
		//"m_beeCount" is increased here as the bee is marked for cleanup after this function is called.
		m_beeCount++;
	}


} // namespace scene