#include "GameProgExercise01.h"
#include "DX\DeviceResources.h"
#include "Core.h"
#include "Ground.h"
#include "..\EntityHelper.h"

namespace scene
{

	Ground::Ground()
	{
		m_shaderType = Scene::ShaderTypes::Lit;
	}

	void Ground::Initialise()
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

		SetScale(1.0f);
	}

	void Ground::Shutdown()
	{
		Entity::Shutdown();
	}

	void Ground::Update()
	{
		Entity::Update();
	}

	void Ground::Render()
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

} // namespace scene