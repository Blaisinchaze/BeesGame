#pragma once

namespace DX
{

class DeviceResources;

// Class that encapsulates some viewport and shader constant functionality
// probably best to have separate camera class
class View
{
public:
	struct VPConstantBuffer
	{
		DirectX::XMMATRIX viewProjectionMatrix;
		DirectX::XMFLOAT4 vAmbient;
		DirectX::XMFLOAT4 vDirLight0;
		DirectX::XMFLOAT4 vDirLight0Col;
	};
	static_assert( ( sizeof( VPConstantBuffer ) % 16 ) == 0, "Constant buffer must always be 16-byte aligned" );

	struct WorldConstantBuffer
	{
		DirectX::XMMATRIX worldMatrix;
	};
	static_assert( ( sizeof( WorldConstantBuffer ) % 16 ) == 0, "Constant buffer must always be 16-byte aligned" );

	View( DeviceResources* const deviceResources );
	~View();

	void							Initialise();
	void							Refresh();
	void							Shutdown();

	//Void that rotates the camera to face the look at point. This should be called after all other transformations on the camera.
	void							LookAtFocusPoint();

	//Void that calculates the offset for a camera when rotating around the look at point.
	//"directionOfHorizontalMovement" is movement around the yaw-axis
	//"directionOfVerticalMovement" is movement around the pitch-axis *STILL UNDER DEVELOPMENT*
	void							RotateCameraAroundPivot( float directionOfHorizontalMovement, float directionOfVerticalMovement );
	
	//Void that calculates where the look at point should be in the world - 
	//"directionOfHorizontalMovement" is movement along the x-axis relative to the camera.
	//"directionOfVerticalMovement" is movement along the z-axis relative to the camera.
	void							MoveLookAtPoint( float directionOfHorizontalMovement, float directionOfVerticalMovement );

	//Void for zooming the camera towards or away from the look at point
	//"ZoomValue" is between -1.0f and 1.0f
	void							CameraZoom( float ZoomValue );


	inline void						SetViewPosition( const DirectX::XMVECTOR viewPosition )
	{
		m_viewPosition = viewPosition;
	}

	inline void						SetViewDirection( const DirectX::XMVECTOR viewDirection )
	{
#ifdef _DEBUG
		const DirectX::XMVECTOR vecLength = DirectX::XMVector3LengthEst( viewDirection );
		ASSERT( ( vecLength.m128_f32[ 0 ] < ( 1.0f + Epsilon ) ) && ( vecLength.m128_f32[ 0 ] > ( 1.0f - Epsilon ) ), "View direction vector isn't normalised.\n" );
#endif
		m_viewDirection = viewDirection;
	}

private:
	DeviceResources*				m_deviceResources;
	ID3D11Buffer*					m_vpConstantBuffer;

	DirectX::XMMATRIX				m_viewMatrix;
	DirectX::XMMATRIX				m_projectionMatrix;

	DirectX::XMVECTOR				m_viewPosition;
	DirectX::XMVECTOR				m_viewDirection;

	DirectX::XMFLOAT4				m_ambient;
	DirectX::XMVECTOR				m_dirLightDir;
	DirectX::XMFLOAT4				m_dirLightCol;

	DirectX::XMVECTOR				m_cameraOffset;
	DirectX::XMVECTOR				m_lookAtPoint; 
	float							m_distanceFromCentre;
	float							m_degreesAroundCentre;
	float							m_cameraHorizontalMoveSpeed;
	float							m_cameraVerticalMoveSpeed;
	float							m_cameraZoomSpeed;
};

} // namespace DX