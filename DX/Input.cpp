#include "GameProgExercise01.h"
#include "DX\DeviceResources.h"
#include "DX\Input.h"

namespace DX
{

Input::Input():
	m_leftRight( 0.0f),
	m_upDown( 0.0f ),
	m_wasdleftRight(0.0f),
	m_wasdupDown(0.0f)
{
}

Input::~Input()
{
}

void Input::Initialise()
{
}

void Input::Update()
{
}

void Input::Shutdown()
{
}

BOOL Input::HandleSystemMessage( const UINT message, const WPARAM wParam )
{
	BOOL unhandled = FALSE;

	// TODO: Improve this input detection
	switch( message )
	{
		case WM_KEYDOWN:
			switch( wParam )
			{
				case 0x57:	//ASCII code for W key
					m_wasdupDown = 1.0f;
					break;					
				case 0x53://ASCII code for S key
					m_wasdupDown = -1.0f;
					break;
				case 0x41://ASCII code for A key
					m_wasdleftRight = -1.0f;
					break;
				case 0x44://ASCII code for D key
					m_wasdleftRight = 1.0f;
					break;
				case VK_LEFT:
					m_leftRight = -1.0f;
					break;
				case VK_RIGHT:
					m_leftRight = 1.0f;
					break;
				case VK_UP:
					m_upDown = 1.0f;
					break;
				case VK_DOWN:
					m_upDown = -1.0f;
					break;
				default:
					break;
			}
			break;

		case WM_KEYUP:
			switch( wParam )
			{
			case 0x57:	//ASCII code for W key
			case 0x53://ASCII code for S key
				m_wasdupDown = 0.0f;
				break;
			case 0x41://ASCII code for A key
			case 0x44://ASCII code for D key
				m_wasdleftRight = 0.0f;
				break;
				case VK_LEFT:
				case VK_RIGHT:
					m_leftRight = 0.0f;
					break;
				case VK_UP:
				case VK_DOWN:
					m_upDown = 0.0f;
					break;
				default:
					break;
			}
			break;

		default:
			unhandled = TRUE;
			break;
	}

	return unhandled;
}

}