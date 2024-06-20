#include "GameProgExercise01.h"
#include "DX\DeviceResources.h"
#include "Core.h"
#include "Bee.h"
#include <Scene/EntityHelper.h>
#include "Utils\Maths.h"
#include <DirectXMath.h>
#include <DirectXMathVector.inl>
using namespace DirectX;



namespace scene
{

	Bee::Bee()
	{
		m_shaderType = Scene::ShaderTypes::Lit;
		m_movementSpeed = utils::RandRange(10.0f)/10.0f + 0.5f;
		m_targetFlower = nullptr;
		m_startingHive = nullptr;
		m_collectionSpeed = 20.0f;
		m_holdingNectar = 0.0f;
	}

	void Bee::Initialise()
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
		m_startingPosition = GetPosition();
		//m_targetPosition = m_position;
	}

	void Bee::Shutdown()
	{
		Entity::Shutdown();
		
	}

	void Bee::Update()
	{
		Entity::Update();
		float collectionThisFrame;
		switch (m_currentState)
		{
		case scene::Bee::SPAWNING:
			m_currentState = SEARCHING;
			break;
		case scene::Bee::SEARCHING:
			if (m_targetFlower == nullptr) 
			{
				Core* const core = Core::Get();
				m_targetFlower = core->GetScene()->ReturnRandomFlower();
			}
			else
			{
				m_currentState = TRAVELLING;
			}
			break;
		case scene::Bee::TRAVELLING:
			
			XMVECTORF32 position = GetPositionAsF32();
			XMVECTORF32 targetPosition = m_targetFlower->GetPositionAsF32();
			if (XMVector3Length(GetPosition() - m_targetFlower->GetPosition()).m128_f32[0] > 0.1f)
			{
				MoveTowards(m_targetFlower->GetPosition());
				
			}
			else
			{
				m_currentState = ARRIVING;
			}
			break;
		case scene::Bee::ARRIVING:
			if (m_targetFlower->IsFlowerInUse()) 
			{
				m_targetFlower = nullptr;
				m_currentState = SEARCHING;
			}
			else
			{
				m_targetFlower->CollectionChange(true);
				m_currentState = COLLECTING;
			}
			break;
		case scene::Bee::COLLECTING:
			collectionThisFrame = (m_collectionSpeed * utils::Timers::GetFrameTime());
			if (m_targetFlower->CollectNectar(collectionThisFrame))
			{
				m_holdingNectar += collectionThisFrame;
			}
			else
			{
				m_targetFlower->CollectionChange(false);
				m_currentState = RETURNING;
			}
			break;
		case scene::Bee::RETURNING:
			position = GetPositionAsF32();
			if (XMVector3Length(GetPosition() - m_startingHive->GetPosition()).m128_f32[0] > 0.1f)
			{
				MoveTowards(m_startingHive->GetPosition());
			}
			else
			{
				m_currentState = DEPOSIT;
			}
			break;
		case scene::Bee::DEPOSIT:
			m_startingHive->AddNectar(m_holdingNectar);

			m_currentState = DESTORY;
			break;
		case scene::Bee::DESTORY:
			break;
		default:
			break;
		}
	}

	void Bee::Render()
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

	void Bee::MoveTowards(DirectX::XMVECTOR targetVector)
	{
		XMVECTOR directionVector =  targetVector - GetPosition();		
		
		XMVECTOR directionFacing = XMVector3Normalize(directionVector);
		SetOrientation(directionFacing);
		const float frameTime = utils::Timers::GetFrameTime();
		const XMVECTOR delta = XMVectorScale(directionFacing, m_movementSpeed * frameTime);
		SetPosition(m_position + delta);
		return;
	}

	void Bee::SetStartingHive(Hive* startingHive) 
	{
		m_startingHive = startingHive;
	}
	
} // namespace scene