#ifndef MODEL_HPP
#define MODEL_HPP
//#include <string>

//class Material;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11Buffer;

//#include <vector>
//class Model_
//{
//	struct float3
//	{
//		float x, y, z;
//	};
//
//public:
//	Model_();
//	~Model_();
//
//	bool LoadFromFile(const char* fileName);
//	bool CreateVertexBuffer(ID3D11Device* device);
//	void Render(ID3D11DeviceContext* deviceContext);
//
//private:
//	std::vector<float3> m_positions;
//	std::vector<float3> m_normals;
//	std::vector<float3> m_colors;
//	std::vector<float3> m_texcoords;
//	std::vector<float3> m_tangents;
//	std::vector<float3> m_binormals;
//};

class Model
{
protected:
	Model();
public:
	virtual ~Model();

	virtual bool LoadFromFile(const char* fileName) = 0;
	virtual bool CreateVertexBuffer(ID3D11Device* device) = 0;
	virtual void Render(ID3D11DeviceContext* deviceContext) = 0;

protected:
	ID3D11Buffer* m_vertexBuffer;

	//Material* m_material;
};

#endif