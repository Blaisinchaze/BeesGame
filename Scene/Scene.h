#pragma once
#include "Containers/List.h"

namespace scene
{

class TestObject;
class Hive;
class Ground;
class Bee;
class Flower;

// Object containing all the entities and related objects in the scene
class Scene sealed
{
public:
	enum ShaderTypes
	{
		Unlit = 0,
		Lit,

		NumShaderTypes
	};

	Scene();
	~Scene();

	void				Initialise();
	void				Shutdown();

	void				Update();
	void				Render();

	void				ActivateShaders( const ShaderTypes shaderType );
	void				SpawnBee(Hive* startingHive, DirectX::XMFLOAT4 colour, DirectX::XMVECTOR position);
	Flower*				ReturnRandomFlower();
private:
	struct ShaderData
	{
		ShaderData(): inputLayout( nullptr ),
			vertexShader( nullptr ),
			pixelShader( nullptr )
		{
		}

		ID3D11InputLayout* inputLayout;
		ID3D11VertexShader* vertexShader;
		ID3D11PixelShader* pixelShader;
	};
	
	ShaderData m_shaderData[ ShaderTypes::NumShaderTypes ];

	containers::List<Hive*>				m_hiveList;
	containers::List<Bee*>				m_beeList;
	containers::List<Flower*>			m_flowerList;	

	typedef containers::List< Bee* >::iterator BeeListItor;	
	typedef containers::List< Hive* >::iterator HiveListItor;
	typedef containers::List< Flower* >::iterator FlowerListItor;

	Ground*								m_ground;
	Flower*								m_flower;

	float								m_flowerBedScale;
	int									m_hiveCount;

};

} // namespace scene