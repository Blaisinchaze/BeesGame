#pragma once

namespace DX
{

class DeviceResources;

class Input
{
public:
	Input();
	~Input();

	void							Initialise();
	void							Update();
	void							Shutdown();

	BOOL							HandleSystemMessage( const UINT message, const WPARAM wParam );

	inline float					GetLeftRight() const
	{
		return m_leftRight;
	}

	inline float					GetUpDown() const
	{
		return m_upDown;
	}

	inline float					GetWASDUpDown() const
	{
		return m_wasdupDown;
	}

	inline float					GetWASDLeftRight() const
	{
		return m_wasdleftRight;
	}

	inline float					GetScrollWheelValue() const
	{
		return m_scroll;
	}

private:
	float							m_leftRight;
	float							m_upDown;	
	float							m_wasdleftRight;
	float							m_wasdupDown;
	int								m_scroll;
	float							m_scrollTimer;
	bool							m_lMouseButtonPressed;

	DirectX::XMFLOAT2				m_startLocation;
	DirectX::XMFLOAT2				m_currentLocation;
};

} // namespace DX