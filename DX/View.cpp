#include "GameProgExercise01.h"
#include "DX\DeviceResources.h"
#include "DX\View.h"

using namespace DirectX;

namespace DX
{

View::View( DeviceResources* deviceResources ) :
	m_deviceResources( deviceResources ),
	m_vpConstantBuffer( nullptr )
{
	// Initialize the world matrix
	m_viewMatrix = XMMatrixIdentity();
	m_projectionMatrix = XMMatrixIdentity();

	m_viewPosition = XMVectorZero();
	m_viewDirection = XMVECTOR{ -1.0f, 1.0f, 1.0f, 0.0f };
	m_viewDirection = XMVector3Normalize( m_viewDirection );

	m_ambient = XMFLOAT4{ 0.2f, 0.2f, 0.4f, 0.0f };
	m_dirLightDir = XMVECTOR{ 0.25f, -0.75f, -0.25f, 0.0f };
	m_dirLightDir = XMVector3Normalize( m_dirLightDir );
	m_dirLightCol = XMFLOAT4{ 0.8f, 0.8f, 0.6f, 0.0f };
	m_lookAtPoint = XMVECTOR{ 0.0f, 0.0f, 0.0f, 0.0f };
	m_cameraOffset = XMVECTOR{ 0.0f, 0.0f, 0.0f, 0.0f };

	m_distanceFromCentre = 10.0f;
	m_degreesAroundCentre = 45.0f;
	m_cameraHorizontalMoveSpeed = 0.5f;
	m_cameraVerticalMoveSpeed = 0.3f;
	m_cameraZoomSpeed = 10.0f;
}

View::~View()
{
}

void View::Initialise()
{
	ASSERT( m_deviceResources != nullptr, "Device resources doesn't exist.\n" );
	ID3D11DeviceContext* const deviceContext = m_deviceResources->GetD3DDeviceContext();

	ID3D11Device* device = nullptr;
	deviceContext->GetDevice( &device );
	ASSERT( device != nullptr, "Unable to get D3D device.\n" );

	CD3D11_BUFFER_DESC bufferDesc( sizeof( VPConstantBuffer ), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE );
	device->CreateBuffer( &bufferDesc, nullptr, &m_vpConstantBuffer );
	ASSERT( m_vpConstantBuffer != nullptr, "Unable to create constant buffer.\n" );

	// Initialize the projection matrix
	auto size = m_deviceResources->GetOutputSize();
	XMMATRIX projection = XMMatrixPerspectiveFovLH( XM_PIDIV4, float( size.right ) / float( size.bottom ), 0.01f, 100.0f );
	m_projectionMatrix = projection;	
}

void View::Refresh()
{
	static const DirectX::XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0 };

	m_viewPosition.m128_f32[ 3 ] = 0.0f;
	m_viewDirection.m128_f32[ 3 ] = 0.0f;
	m_viewMatrix = XMMatrixLookToLH( m_viewPosition, m_viewDirection, up );

	ASSERT( m_deviceResources != nullptr, "Device resources doesn't exist.\n" );
	ID3D11DeviceContext* const deviceContext = m_deviceResources->GetD3DDeviceContext();

	D3D11_MAPPED_SUBRESOURCE mapped;
	HRESULT hr = S_OK;

	// Set the per-frame constants
	VPConstantBuffer vpConstants = {};

	XMMATRIX viewProjection = XMMatrixMultiply( m_viewMatrix, m_projectionMatrix );

	// Shaders compiled with default row-major matrices
	vpConstants.viewProjectionMatrix = XMMatrixTranspose( viewProjection );
	vpConstants.vAmbient = m_ambient;
	XMStoreFloat4( &vpConstants.vDirLight0, m_dirLightDir );
	vpConstants.vDirLight0Col = m_dirLightCol;

	ASSERT( m_vpConstantBuffer != nullptr, "Constant buffer doesn't exist. Has View::Initialise() been called?\n" );

	hr = deviceContext->Map( m_vpConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped );
	ASSERT_HANDLE( hr );
	memcpy( mapped.pData, &vpConstants, sizeof( VPConstantBuffer ) );
	deviceContext->Unmap( m_vpConstantBuffer, 0 );

	deviceContext->VSSetConstantBuffers( 0, 1, &m_vpConstantBuffer );
	deviceContext->PSSetConstantBuffers( 0, 1, &m_vpConstantBuffer );
}

void View::Shutdown()
{
	if( m_vpConstantBuffer != nullptr )
		m_vpConstantBuffer->Release();
}

//Void that rotates the camera to face the look at point. This should be called after all other transformations on the camera.
void View::LookAtFocusPoint()
{
	//Calculate the angle to rotate the camera to face the look at position 
	XMVECTOR directionVector = m_lookAtPoint - m_viewPosition;
	SetViewDirection(XMVector3Normalize(directionVector));
}

//Void that calculates the offset for a camera when rotating around the look at point.
//"directionOfHorizontalMovement" is movement around the yaw-axis
//"directionOfVerticalMovement" is movement around the pitch-axis *STILL UNDER DEVELOPMENT*
void View::RotateCameraAroundPivot( float directionOfHorizontalMovement, float directionOfVerticalMovement )
{	
	//Local declarations used in the function
	const float deltaTime = utils::Timers::GetFrameTime();
	float angleStride = 0;
	const float startingAngleSin = XMScalarSin(m_degreesAroundCentre + angleStride);
	const float startingAngleCos = XMScalarCos(m_degreesAroundCentre + angleStride);

	//Check used to ensure the angleStride is not inf
	if ( directionOfHorizontalMovement != 0 )
	{
		angleStride = ( XM_2PI / -directionOfHorizontalMovement ) * m_cameraHorizontalMoveSpeed * deltaTime;
		
	}

	//Save the camera offset to be used in MoveLookAtPoint
	m_cameraOffset = XMVECTOR{ startingAngleSin * m_distanceFromCentre, m_distanceFromCentre, startingAngleCos * m_distanceFromCentre, 0.0f } ;
	m_degreesAroundCentre = m_degreesAroundCentre + angleStride;
}

//Void that calculates where the look at point should be in the world - 
//"directionOfHorizontalMovement" is movement along the x-axis relative to the camera.
//"directionOfVerticalMovement" is movement along the z-axis relative to the camera.
void View::MoveLookAtPoint( float directionOfHorizontalMovement, float directionOfVerticalMovement )
{
	const float deltaTime = utils::Timers::GetFrameTime();

	XMVECTOR inputDirection = XMVector3Normalize ( XMVECTOR{ directionOfHorizontalMovement, 0.0f, directionOfVerticalMovement, 0.0f } );
	XMVECTOR normalisedCameraFacingDirection = XMVector3Normalize( m_viewDirection );

	// Project the view direction onto the horizontal plane
	XMVECTOR vertical = XMVECTOR{ 0.0f, 1.0f, 0.0f, 1.0f };
	XMVECTOR dotProd = XMVector3Dot( vertical, normalisedCameraFacingDirection );
	XMVECTOR verticalComponent = XMVectorScale(vertical, dotProd.m128_f32[0]);

	// This is the view direction "flattened" (could be zero if looking straight down)
	XMVECTOR horizonalComponent = XMVectorSubtract( normalisedCameraFacingDirection, verticalComponent );
	XMVECTOR viewDirHorizontal = XMVector3NormalizeEst( horizonalComponent );

	// Get the vector at a right angle to this view direction for left-right
	XMVECTOR lateralDirHorizontal = XMVector3Cross( horizonalComponent, vertical ); // May need to change the order of these params as it might be backwards


	// Apply this to our "cross" of basis forward/sideways vectors
	XMVECTOR sidewaysMove = XMVectorScale(lateralDirHorizontal, -directionOfHorizontalMovement * m_cameraVerticalMoveSpeed);
	XMVECTOR forwardMove = XMVectorScale(viewDirHorizontal, directionOfVerticalMovement * m_cameraVerticalMoveSpeed);

	// Put them both together - add this to current camera position
	m_lookAtPoint += XMVectorAdd(sidewaysMove, forwardMove);
	SetViewPosition(m_lookAtPoint + m_cameraOffset);
}

//Void for zooming the camera towards or away from the look at point
//"ZoomValue" is between -1.0f and 1.0f
void View::CameraZoom(float ZoomValue)
{
	if (m_distanceFromCentre - ZoomValue  > 2.0f &&
		m_distanceFromCentre - ZoomValue  < 15.0f)
	{
		m_distanceFromCentre -= m_cameraZoomSpeed * ZoomValue * utils::Timers::GetFrameTime();
	}
}

} // namespace DX
