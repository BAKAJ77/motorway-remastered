#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <core/asset_system.h>
#include <graphics/vertex_array.h>
#include <glm/glm.hpp>

class Geometry
{
public:
	enum class RenderFunction
	{
		RENDER_ARRAYS,
		RENDER_ELEMENTS
	};

	enum class PrimitiveType : uint32_t
	{
		POINTS = 0x0000,
		LINES = 0x0001,
		LINE_LOOP = 0x0002,
		LINE_STRIP = 0x0003,
		TRIANGLES = 0x0004,
		TRIANGLE_STRIP = 0x0005,
		TRIANGLE_FAN = 0x0006
	};
	
	struct Transform
	{
		glm::vec3 m_position = glm::vec3(0.0f), m_size = glm::vec3(1.0f), m_rotationAxis = glm::vec3(1.0f);
		float m_rotationAngle = 0.0f;
	};

	struct Material
	{
		glm::vec3 m_ambientColor = glm::vec3(1.0f), m_diffuseColor = glm::vec3(1.0f), m_specularColor = glm::vec3(0.5f);
		Texture2DPtr m_diffuseTexture, m_specularTexture;
		float m_opacity = 1.0f, m_shininess = 32.0f;

		// If enabled then the textures in the material are used and the color vectors are 
		// then used to modify the texture's color.
		bool m_enableTextures = false;
	};
protected:
	Transform m_transformData; // Transform data
	Material m_materialData; // Material data

	VertexArray m_vertexArray; // VAO

	// Rendering parameters
	RenderFunction m_renderFunc;
	PrimitiveType m_primitiveType;
	uint32_t m_count;
protected:
	// Used for initializing the geometry's buffer objects and specifying the rendering parameters.
	virtual void InitGeometryData() = 0;
public:
	Geometry();
	Geometry(const Transform& transform, const Material& material);
	
	virtual ~Geometry() = default;

	// Sets the position of the geometry.
	void SetPosition(const glm::vec3& pos);

	// Sets the size of the geometry.
	void SetSize(const glm::vec3& size);

	// Sets the rotation of the geometry.
	void SetRotation(const glm::vec3& axis, float angle);

	// Sets the ambient color of the geometry.
	// Note that if textures for this geometry are enabled, then the color vector is ignored; the diffuse
	// texture will be used in ambient lighting calculations instead.
	void SetAmbient(const glm::vec3& color);

	// Sets the diffuse color of the geometry.
	// Note that if textures for this geometry are enabled, then the color vector will be used as a modifier 
	// on the diffuse texel colors.
	void SetDiffuse(const glm::vec3& color);

	// Sets the diffuse texture of the geometry.
	// Note that you will need to enable textures using SetTexturesUsage() in order for textures to appear on this geometry.
	void SetDiffuse(Texture2DPtr texture);

	// Sets the specular color of the geometry.
	// Note that if textures for this geometry are enabled, then the color vector will be used as a modifier 
	// on the specular texel colors.
	void SetSpecular(const glm::vec3& color);

	// Sets the specular texture of the geometry.
	// Note that you will need to enable textures using SetTexturesUsage() in order for textures to appear on this geometry.
	void SetSpecular(Texture2DPtr texture);

	// Sets the shininess of the geometry's material.
	void SetShininess(float value);

	// Sets the master opacity of the geometry.
	void SetOpacity(float value);

	// Sets whether the material textures should be used on this geometry.
	// If enabled, material textures should appear on the geometry and color vectors will be used as modifiers for 
	// their respective texture. 
	// For example, the diffuse color vector would be used as a modifier on the diffuse texture.
	void SetTexturesUsage(bool enable);

	// Returns the model matrix computed using the transform data.
	glm::mat4 ComputeModelMatrix() const;

	// Returns the geometry's vertex array.
	const VertexArray& GetVertexArray() const;

	// Returns the geometry's transform data.
	const Transform& GetTransformData() const;

	// Returns the geometry's material data.
	const Material& GetMaterialData() const;

	// Returns an enum specifying the function to use to render the geometry.
	RenderFunction GetRenderFunction() const;

	// Returns an enum specifying the primitive type to use when rendering the geometry.
	PrimitiveType GetPrimitiveType() const;

	// Returns the number of vertices/indices in the geometry.
	// For geometry using RenderFunction::RENDER_ARRAYS, the number of vertices should be returned. 
	// For geometry using RenderFunction::RENDER_ELEMENTS, the number of indices should be returned. 
	uint32_t GetCount() const;
};

class Square : public Geometry
{
protected:
	virtual void InitGeometryData() override;
public:
	Square();
	Square(const Transform& transform, const Material& material);

	virtual ~Square() = default;
};

class Triangle : public Geometry
{
protected:
	virtual void InitGeometryData() override;
public:
	Triangle();
	Triangle(const Transform& transform, const Material& material);

	virtual ~Triangle() = default;
};

class Circle : public Geometry
{
protected:
	virtual void InitGeometryData() override;
public:
	Circle();
	Circle(const Transform& transform, const Material& material);

	virtual ~Circle() = default;
};

class Cube : public Geometry
{
protected:
	virtual void InitGeometryData() override;
public:
	Cube();
	Cube(const Transform& transform, const Material& material);

	virtual ~Cube() = default;
};

#endif