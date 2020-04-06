#include "Common/Log.h"
#include "Space/GameObject.hpp"
#include "Space/Scene.hpp"

namespace Space
{

	Float4x4 PerspectiveCamera::GetViewMatrix() const
	{
		Float4x4 ret;
		Matrix m = MatrixPerspectiveFovLH(m_FieldOfView,m_Aspect, 0.1f, 1000.0f);
		StoreFloat4x4(&ret, m);
		return ret;
	}

	Float4x4 OrthographicCamera::GetViewMatrix() const
	{
		Float4x4 ret;
		Matrix m = MatrixOrthographicLH(m_OrthoWidth, m_OrthoWidth / m_Aspect, 0.1f, 1000.0f);
		StoreFloat4x4(&ret, m);
		return ret;
	}

	void SceneManager::Tick(float elaspTime)
	{
	}

}