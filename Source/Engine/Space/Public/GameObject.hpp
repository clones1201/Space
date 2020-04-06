#ifndef __SPACE_GAMEOBJECT_HPP__
#define __SPACE_GAMEOBJECT_HPP__

#include "Space/Prerequisites.hpp"
#include "Space/Mesh.hpp"
#include "Space/Material.hpp"
#include "Space/Renderer.hpp"

namespace Space
{
	class GComponent : public PropertyManager
	{
	public:


		virtual void Invoke() = 0;
	protected:
	
	private:

	};

	typedef std::shared_ptr<GComponent> GComponentPtr;
	
	class MeshRendererComponent : public GComponent
	{
	public:
		MeshRendererComponent();

		DEF_BOOL_PROPERTY(CastShadow, TEXT("General"), false);

		DEF_OBJECT_PROPERTY(Mesh, Space::Mesh, TEXT("General"), nullptr);
		DEF_OBJECT_PROPERTY(Material, Space::Material, TEXT("General"), nullptr);

		virtual void Invoke();
	protected:

	private:

		MeshMaterialRendererPtr m_Renderer = nullptr;
	};

	typedef std::shared_ptr<MeshRendererComponent> MeshRendererComponentPtr;

	class GameObject;

	class GameObject : PropertyManager
	{
	public:
		typedef std::list<GComponentPtr> GComponentList;
		typedef std::list<GComponentPtr>::iterator GComponentListIterator;
		typedef std::list<GComponentPtr>::const_iterator GComponentListConstIterator;

		void AttachComponent(GComponentPtr component);
		void DetachComponent(GComponentPtr component);
		
		inline GComponentListIterator ComponentBegin()
		{
			return m_Components.begin();
		}

		inline GComponentListIterator ComponentEnd()
		{
			return m_Components.end();
		}

		inline GComponentListConstIterator ComponentCBegin() const
		{
			return m_Components.cbegin();
		}

		inline GComponentListConstIterator ComponentCEnd() const
		{
			return m_Components.cend();
		}

		typedef std::list<std::shared_ptr<GameObject>> GChildren;
		typedef std::list<std::shared_ptr<GameObject>>::iterator GChildrenIterator;
		typedef std::list<std::shared_ptr<GameObject>>::const_iterator GChildrenConstIterator;

		inline GChildrenIterator ChildrenBegin() 
		{
			return m_Children.begin();
		}

		inline GChildrenIterator ChildrenEnd()
		{
			return m_Children.end();
		}

		inline GChildrenConstIterator ChildrenCBegin() const
		{
			return m_Children.cbegin();
		}

		inline GChildrenConstIterator ChildrenCEnd() const
		{
			return m_Children.cend();
		}

		GameObject* AddChildren();
		void RemoveChildren(GameObject* pChild);
		
		// Name£¬ Categories£¬ Value
		DEF_FLOAT3_PROPERTY(Translate, TEXT("General"), Float3(0.0f, 0.0f, 0.0f));
		DEF_FLOAT3_PROPERTY(Rotation, TEXT("General"), Float3(0.0f, 0.0f, 0.0f));
		DEF_FLOAT3_PROPERTY(Scale, TEXT("General"), Float3(1.0f, 1.0f, 1.0f));

	protected:		
		GChildren m_Children;
		GComponentList m_Components;
		
	private:

	};

	typedef std::shared_ptr<GameObject> GameObjectPtr;

	class Camera : public GameObject
	{
	public:
		virtual Float4x4 GetViewMatrix() const = 0;

		inline bool IsConstrainAspect() const
		{
			return m_ConstrainAspect;
		}
		inline void SetConstrainAspect(bool value)
		{
			m_ConstrainAspect = value;
		}
		inline float GetAspectHbyW() const
		{
			return m_Aspect;
		}
		inline void SetAspectHByW(float value)
		{
			m_Aspect = value;
		}
	protected:
		float m_Aspect = 1.7777f;

	private:
		bool m_ConstrainAspect = true;
	};

	class PerspectiveCamera : public Camera
	{
	public:
		virtual Float4x4 GetViewMatrix() const;

		inline void SetFieldOfView(float fov)
		{
			m_FieldOfView = fov;
		}
		inline float GetFieldOfView() const
		{
			return m_FieldOfView;
		}

	protected:
	private:
		float m_FieldOfView = 90.0_deg;
	};

	class OrthographicCamera : public Camera
	{
	public:
		virtual Float4x4 GetViewMatrix() const;

		inline float GetOrthoWidth() const
		{
			return m_OrthoWidth;
		}
		inline void SetOrthoWidth(float ow)
		{
			m_OrthoWidth = ow;
		}

	protected:
	private:
		float m_OrthoWidth = 512;
	};
	
}

#endif