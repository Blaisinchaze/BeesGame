#include "GameProgExercise01.h"
#include "DX\DeviceResources.h"
#include "Core.h"
#include "Hive.h"
#include "..\EntityHelper.h"
#include <DirectXMath.h>
#include "Utils\Maths.h"

namespace scene
{

	Hive::Hive()
	{
		m_shaderType = Scene::ShaderTypes::Lit;
		m_spawnTimer = 2.0f;// utils::RandRange(20) / 10;
		m_collectedNectar = 0;
		m_beeCount = 20;
		m_colour = DirectX::XMFLOAT4{ utils::RandRange(10) / 10,utils::RandRange(10) / 10,utils::RandRange(10) / 10, 1.0f };
	}

	void Hive::Initialise()
	{
		Entity::Initialise();

		Core* const core = Core::Get();

		const DX::DeviceResources* const deviceResources = core->GetDeviceResources();

		HRESULT hr = 0;

		ID3D11Device* const device = deviceResources->GetD3DDevice();

		UINT numCubeVertices = 0;
		const Entity::VertexLit* cubeVerts = helper::GetCubeVertices(numCubeVertices);

		D3D11_SUBRESOURCE_DATA initialData = {};
		initialData.pSysMem = cubeVerts;

		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.ByteWidth = numCubeVertices * sizeof(VertexLit);
		bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.StructureByteStride = sizeof(VertexLit);

		// Note the vertex buffer is released in the base class
		hr = device->CreateBuffer(&bufferDesc, &initialData,
			&m_vertexBuffer);
		ASSERT_HANDLE(hr);


		
		SetColour(m_colour);
		SetScale(1.0f);
		//m_SpawnTimer = new float (2.0f);// utils::RandRange(20) / 10;
	}

	void Hive::Shutdown()
	{
		Entity::Shutdown();
	}

	void Hive::Update()
	{
		Entity::Update();
		m_spawnTimer -= 1.0f * utils::Timers::GetFrameTime();
		if (m_spawnTimer < 0 && m_beeCount > 0) {
			Core* const core = Core::Get();
			core->GetScene()->SpawnBee(this, m_colour, Entity::m_position);
			m_spawnTimer = 0.3f;
			--m_beeCount;
			
		}
		float hiveScale = (m_collectedNectar / 10000) + 1;
		SetScale(DirectX::XMVECTORF32{ 0.5f * hiveScale, 1.0f * hiveScale, 0.5f * hiveScale, 1.0f });
	}

	void Hive::Render()
	{
		// Ensure the base class is called as this sets up the shaders for drawing
		Entity::Render();

		Core* const core = Core::Get();

		const DX::DeviceResources* const deviceResources = core->GetDeviceResources();
		ID3D11DeviceContext* const context = deviceResources->GetD3DDeviceContext();

		// Draw the triangles
		UINT strides = sizeof(VertexLit);
		UINT offsets = 0;
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &strides, &offsets);
		context->Draw(NumVertices, 0);
	}

	void Hive::AddNectar(float nectarToStore)
	{
		m_collectedNectar += nectarToStore;
		m_beeCount++;
	}


} // namespace scene