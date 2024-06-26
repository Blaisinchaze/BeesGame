#include "GameProgExercise01.h"
#include "DX\DeviceResources.h"
#include "Core.h"
#include "Flower.h"
#include "..\EntityHelper.h"
#include "Utils\Maths.h"

using namespace utils;
using namespace DirectX;

namespace scene
{

	Flower::Flower()
	{
		m_shaderType = Scene::ShaderTypes::Lit;
		m_beingCollected = false;
		m_nectarGainRate = 10.0f;
		m_nectarStored = 0.0f;
		m_maxNectarStored = 50.0f;
		m_insideColour = { 0.0f,0.0f ,0.0f ,0.0f };
		m_outsideColour = { 0.0f,0.0f ,0.0f ,0.0f };
	}

	void Flower::Initialise()
	{
		Entity::Initialise();

		Core* const core = Core::Get();

		const DX::DeviceResources* const deviceResources = core->GetDeviceResources();

		HRESULT hr = 0;

		ID3D11Device* const device = deviceResources->GetD3DDevice();

		//Randomise the color of the petals 
		m_insideColour = XMFLOAT4{ RandRange( 1 ), RandRange( 1 ), RandRange( 1 ), 1.0f };
		m_outsideColour = XMFLOAT4{ RandRange( 1 ), RandRange(  1 ), RandRange(  1 ), 1.0f };

		UINT NumPetalVertices = NumPetals * NumVerticesPerPetal;
		VertexLit* const petalVertices = new VertexLit[NumPetalVertices];
		ASSERT( petalVertices != nullptr, "Couldn't create petal vertices.\n" );

		const float angleStride = DirectX::XM_2PI / static_cast<float>( NumPetals );
		const float petalStride = angleStride * 0.85;
		const float gapStride = angleStride - petalStride;

		VertexLit* vertex = petalVertices;
		float startingAngle = 0.0f;
		for ( UINT petalIndex = 0; petalIndex < NumPetals; ++petalIndex )
		{
			const float startingAngleSin = XMScalarSin( ( startingAngle + 
				( gapStride * petalIndex ) ) + ( petalStride * petalIndex ) );
			const float startingAngleCos = XMScalarCos( ( startingAngle + 
				( gapStride * petalIndex ) ) + ( petalStride * petalIndex ) );
			const float startingAnglePlusStrideSin = XMScalarSin( ( startingAngle + 
				( gapStride * petalIndex ) ) + ( petalStride * ( petalIndex + 1 ) ) );
			const float startingAnglePlusStrideCos = XMScalarCos( ( startingAngle + 
				( gapStride * petalIndex ) ) + ( petalStride * ( petalIndex + 1 ) ) );
			// The centre
			vertex->position.x = 0.0f;
			vertex->position.y = 0.0f;
			vertex->position.z = 0.0f;
			vertex->position.w = 1.0f;
			vertex->normal.x = 0.0f;
			vertex->normal.y = 1.0f;
			vertex->normal.z = 0.0f;
			vertex->color = m_insideColour;
			vertex++;
			vertex->position.x = startingAngleSin * (BaseRadius - ((petalIndex % 2) * 0.2f));
			vertex->position.y = 0.2f;
			vertex->position.z = startingAngleCos * (BaseRadius - ((petalIndex % 2) * 0.2f));
			vertex->position.w = 1.0f;
			vertex->normal.x = 0.0f;
			vertex->normal.y = 1.0f;
			vertex->normal.z = 0.0f;
			vertex->color = m_outsideColour;
			vertex++;
			vertex->position.x = startingAnglePlusStrideSin * (BaseRadius - ((petalIndex % 2) * 0.2f));
			vertex->position.y = 0.2f;
			vertex->position.z = startingAnglePlusStrideCos * (BaseRadius - ((petalIndex % 2) * 0.2f));
			vertex->position.w = 1.0f;
			vertex->normal.x = 0.0f;
			vertex->normal.y = 1.0f;
			vertex->normal.z = 0.0f;
			vertex->color = m_outsideColour;
			vertex++;
		}

		D3D11_SUBRESOURCE_DATA initialData = {};
		initialData.pSysMem = petalVertices;

		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.ByteWidth = NumPetalVertices * sizeof( VertexLit );
		bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.StructureByteStride = sizeof( VertexLit );

		// Note the vertex buffer is released in the base class
		hr = device->CreateBuffer( &bufferDesc, &initialData,
			&m_vertexBuffer );
		ASSERT_HANDLE( hr );

		SetScale( 0.0f );


		delete petalVertices;
		
	}

	void Flower::Shutdown()
	{
		Entity::Shutdown();

	}

	void Flower::Update()
	{
		Entity::Update();
		if ( m_maxNectarStored > m_nectarStored ) 
		{
			m_nectarStored += m_nectarGainRate * utils::Timers::GetFrameTime();
		}

		float hiveScale = m_nectarStored / 100;
		SetScale( XMVECTORF32{ hiveScale, hiveScale, hiveScale, 1.0f } );

	}

	void Flower::Render()
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

	void Flower::CollectionChange( bool collectionChange )
	{
		m_beingCollected = collectionChange;
	}

	bool Flower::IsFlowerInUse()
	{
		return m_beingCollected;
	}

	bool Flower::CollectNectar( float rate )
	{
		m_nectarStored -= rate;

		return ( m_nectarStored > 0.0f );
	}

} // namespace scene