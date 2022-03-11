#pragma once

#include <directxmath.h>
#include <d3d11_1.h>
#include <string>
#include "structs.h"

class Transform;
class ParticleModel;
class Appearance;

using namespace DirectX;
using namespace std;

class GameObject
{

protected:
	string m_Type;
	Transform* m_Transform;
	Appearance* m_Appearance;
	ParticleModel* m_ParticleModel;
public:
	GameObject() {}
	GameObject(string type, Geometry geometry, Material* material, float mass);
	~GameObject();

	// returns a string of object type
	inline string GetType() const { return m_Type; }

	inline Transform* GetTransform() const { return m_Transform; }

	inline Appearance* GetAppearance() const { return m_Appearance; }

	inline ParticleModel* GetParticleModel() const { return m_ParticleModel; }

	// Update function which sets the world matrix and takes in deltaTime
	void Update(const float dt);

	// Function for drawing the object
	void Draw(ConstantBuffer& buffer, ID3D11Buffer* constBuffer, ID3D11DeviceContext* immediateContext);
};
