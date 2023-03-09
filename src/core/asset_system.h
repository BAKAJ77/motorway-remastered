#ifndef ASSET_SYSTEM_H
#define ASSET_SYSTEM_H

#include <graphics/shader_program.h>
#include <graphics/texture_2d.h>
#include <graphics/vertex_buffer.h>
#include <graphics/index_buffer.h>

#include <unordered_map>
#include <string>
#include <memory>

using ShaderProgramPtr = std::shared_ptr<ShaderProgram>;
using Texture2DPtr = std::shared_ptr<Texture2D>;
using VertexBufferPtr = std::shared_ptr<VertexBuffer>;
using IndexBufferPtr = std::shared_ptr<IndexBuffer>;

class AssetSystem
{
public:
	struct GeometryData
	{
		VertexBufferPtr m_vertexBuffer;
		IndexBufferPtr m_indexBuffer;
	};
private:
	std::unordered_map<std::string, ShaderProgramPtr> m_storedShaders;
	std::unordered_map<std::string, Texture2DPtr> m_storedTextures;
	std::unordered_map<std::string, GeometryData> m_storedGeometryBuffers;

	AssetSystem() = default;
public:
	~AssetSystem() = default;

	// Loads shader from file and keeps a copy of it, which can be accessed using the GetShader() method.
	void LoadShader(std::string_view nameID, std::string_view vshFilePath, std::string_view fshFilePath);

	// Loads image from file and keeps copy of it as a texture, which can be accessed using the GetTexture() method.
	void LoadTexture(std::string_view nameID, std::string_view imageFilePath, bool flipOnLoad, bool srgb);

	// Stores a copy of the geometry buffer objects given.
	// Note that a vertex buffer must be passed, however passing an index buffer is optional.
	void StoreGeometryBuffers(std::string_view nameID, VertexBufferPtr vertexBuffer, IndexBufferPtr indexBuffer = nullptr);

	// Removes the stored shader that is attached to the ID specified.
	void RemoveShader(std::string_view nameID);

	// Removes the stored texture that is attached to the ID specified.
	void RemoveTexture(std::string_view nameID);

	// Removes the stored geometry buffer objects that are attached to the ID specified.
	void RemoveGeometryBuffers(std::string_view nameID);

	// Returns the stored shader that is attached to the ID specified.
	// If no shader is found with the ID specified, then nullptr will be returned.
	ShaderProgramPtr GetShader(std::string_view nameID) const;

	// Returns the stored texture that is attached to the ID specified.
	// If no texture is found with the ID specified, then nullptr will be returned.
	Texture2DPtr GetTexture(std::string_view nameID) const;

	// Returns the geometry buffer objects that are attached to the ID specified.
	// If no geometry buffer objects are found with the ID specified, then nullptr is returned.
	GeometryData* GetGeometryBuffers(std::string_view nameID);

	// Returns shared pointer to newly created vertex buffer.
	static VertexBufferPtr CreateVertexBuffer(const void* data, size_t size, uint32_t usage);

	// Returns shared pointer to newly created index buffer.
	static IndexBufferPtr CreateIndexBuffer(const void* data, size_t size, uint32_t usage);

	// Returns singleton instance of the class.
	static AssetSystem& GetInstance();
};

#endif