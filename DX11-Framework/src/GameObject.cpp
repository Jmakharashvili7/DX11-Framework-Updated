#include "GameObject.h"
#include "Transform.h"
#include "ParticleModel.h"
#include "Appearance.h"

GameObject::GameObject(string type, Geometry geometry, Material* material) :
	m_Type(type),
	m_ParticleModel(new ParticleModel(this)),
	m_Appearance(new Appearance(material, geometry)),
	m_Transform(new Transform(nullptr, Vector3(0,0,0), Vector3(0,0,0), Vector3(1,1,1)))
{
}

GameObject::~GameObject()
{
	if (m_Transform) delete m_Transform;
	if (m_Appearance) delete m_Appearance;
}

void GameObject::Update(const float dt)
{
	m_Transform->Update();
}

void GameObject::Draw(ConstantBuffer& buffer, ID3D11Buffer* constBuffer, ID3D11DeviceContext* immediateContext)
{
	m_Transform->Draw(buffer);
	m_Appearance->Draw(buffer, constBuffer, immediateContext);
}
