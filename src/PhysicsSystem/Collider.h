#pragma  once
#include "GameObject.h"
#include "Maths/MathLib.h"
#include <vector>

namespace ElysiumEngine
{
    class Transform;
    
	enum ColliderType
	{
		E_BOX,
		E_SPHERE,
		E_MESH,
        E_PLANE,
		E_TOTAL
	};


	class ICollider : public IComponent
	{
	public:
		ICollider(std::string name);
		ICollider(std::string name,const ICollider &rhs);
		virtual ~ICollider();
		virtual void initialize();
		virtual void lateInitialize();
		virtual Vec4 SupportMapping(Vec4 direction) = 0;

		Vec4 &GetPosition();

        ColliderType getType(){return m_Type;}
	protected:
		virtual void VInitialize() = 0;
		virtual void VLateInitialize() = 0;

		Transform *m_Trans;

		ColliderType m_Type;
		//Collision data
		bool m_Static;
		friend class PhysicsSystem;
	};

	class SphereCollider : public ICollider
	{
	public:
		SphereCollider() : ICollider("SphereCollider"), m_Radius(0.5f), m_RadiusScaled(1.0f) {m_Type = E_SPHERE;}
		SphereCollider(const SphereCollider &rhs);
		virtual Vec4 SupportMapping(Vec4 direction);

		virtual void serialize(FileIO::IReader &reader);
		virtual void deserialize();
		virtual IComponent *Clone(){return new SphereCollider(*this);}

		float getRadius();
	private:
		virtual void VInitialize(){}
		virtual void VLateInitialize();

		float m_Radius;//Radius of the unscaled sphere
		float m_RadiusScaled;//Scaled radius
		
		friend class PhysicsSystem;
	};

	class OBBCollider : public ICollider
	{
	public:
		OBBCollider() : ICollider("OBBCollider") {m_Type = E_BOX;}
		OBBCollider(OBBCollider *rhs);

		virtual Vec4 SupportMapping(Vec4 direction);

		virtual void serialize(FileIO::IReader &reader);
		virtual void deserialize();
		virtual IComponent *Clone(){return new OBBCollider(*this);}
		
		Vec4 GetDimensions();
        Maths::OBB GetOBB();
	private:
		virtual void VInitialize(){}
		virtual void VLateInitialize();

		Maths::OBB m_Obb;

		friend class PhysicsSystem;
	};

	class MeshCollider : public ICollider
	{
	public:
		MeshCollider() : ICollider("MeshCollider") {m_Type = E_MESH;}
		
		virtual Vec4 SupportMapping(Vec4 direction);

		virtual void serialize(FileIO::IReader &reader);
		virtual void deserialize();
		virtual IComponent *Clone(){return new MeshCollider(*this);}

		//virtual void HandleMessage(Message::SendMesh &messaage);

	private:
		virtual void VLateInitialize();
		virtual void VInitialize();

		std::vector<Vec4> m_Vertices;

	};
    
    class PlaneCollider: public ICollider
    {
    public:
        PlaneCollider() : ICollider("PlaneCollider") { m_Type = E_PLANE;}
        
        Vec4 SupportMapping(Vec4 direction);
        void serialize(FileIO::IReader &reader);
        void deserialize();
        
        const Vec4 &getNormal(){return normal;}
        float getOffset(){return offset;}
    private:
        virtual void VLateInitialize(){};
		virtual void VInitialize(){};
        
        Vec4 normal;
        float offset;
    };
    
    struct ColliderPair
	{
		ColliderPair(ICollider *one, ICollider *two, Vec4 normal, Vec4 point, float penetrationDistance);
		//Our two colliding objects
		ICollider* m_One;
		ICollider *m_Two;
        
		Vec4 m_CollisionNormal;
		Vec4 m_CollisionPoint;
        float penetrationDistance;
	};

}