#include "ShaderFactory.hpp"
#include "SingleColorShaderGroup.hpp"

ShaderFactory::ShaderFactory()
{
	//Register("SingleColor", &DeferredColorShaderGroup::Create);
}

ShaderFactory::~ShaderFactory()
{
}

void ShaderFactory::Register(const std::string& name, ShaderGroupFunction function)
{
	m_map[name] = function;
}

ShaderGroupNew* ShaderFactory::Create(ID3D11Device* device, const std::string& name)
{
	FunctionMap::iterator it = m_map.find(name);

	if (it != m_map.end())
	{
		return it->second(device);
	}

	return nullptr;
}
