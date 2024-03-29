#include <cassert>

#include "BatchRenderer.hpp"
#include "Renderer.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

namespace Hexeng::Renderer
{
	BatchingShader::BatchingShader(size_t size)
	{
		if (size > BatchInstance::get_max_quads())
			throw std::runtime_error("The hardware this software is running on does not support than many quads in a BatchInstance.");

		std::string vs = batching_vs;
		vs.erase(std::find(vs.begin(), vs.end(), '[') + 1, std::find(vs.begin(), vs.end(), ']'));
		vs.insert(vs.find('[') + 1, std::to_string(size * 12));

		std::string fs = batching_fs;
		fs.erase(std::find(fs.begin(), fs.end(), '[') + 1, std::find(fs.begin(), fs.end(), ']'));
		fs.insert(fs.find('[') + 1, std::to_string(size * 12));

		Shader::operator=(Shader{ {{ShaderType::VERTEX_SHADER, vs}, {ShaderType::FRAGMENT_SHADER, fs}} });

		add_necessary_uniforms();
	}

	BatchingShader::BatchingShader(BatchingShader&& other) noexcept : Shader(std::move(other)) {}
	BatchingShader& BatchingShader::operator=(BatchingShader&& other) noexcept { Shader::operator=(std::move(other)); return *this; }

	VertexLayout BatchQuad::s_vertex_layout;

	ToBeInit BatchQuad::s_init_vl{ std::function<void(void)>{[]() {
		s_vertex_layout = VertexLayout({ { 2, GL_FLOAT }, { 2, GL_FLOAT }, { 1, GL_FLOAT } });
	}} };

	TextureAtlas::TextureAtlas(const std::string& filepath, const Vec2<int>& cell_size, const TexSettList& settings)
		: Texture(filepath, settings),
		m_cell_size(cell_size)
	{
		HXG_ASSERT((m_size.x % cell_size.x == 0) && (m_size.y % cell_size.y == 0),
			HXG_LOG_ERROR("Invalid TextureAtlas format"););
	}

	TextureAtlas::TextureAtlas(TextureAtlas&& other) noexcept
		: Texture(std::move(other)),
		m_cell_size(other.m_cell_size) {}

	TextureAtlas& TextureAtlas::operator=(TextureAtlas&& other) noexcept
	{
		Texture::operator=(std::move(other));
		m_cell_size = other.m_cell_size;
		return *this;
	}

	const Vec2<int>& TextureAtlas::get_cell_size() const
	{
		return m_cell_size;
	}

	BatchInstance::BatchInstance(TextureAtlas* tex_atlas, Shader* shader)
		: texture_atlas(tex_atlas)
	{
		m_shader = shader;

		uniforms.push_back({ &u_transform, &m_transform });
		uniforms.push_back({ &u_rotation_angle, &rotation });
		uniforms.push_back({ &u_scale, &scale });
		uniforms.push_back({ &u_color_filter, &color_filter });
		uniforms.push_back({ &u_color, &color });
	}

	BatchInstance::BatchInstance(BatchInstance&& other) noexcept
		: Mesh(std::move(other)),
		m_raw_ib(std::move(other.m_raw_ib)),
		m_raw_vb(std::move(other.m_raw_vb)),
		m_quads(std::move(other.m_quads)),
		texture_atlas(other.texture_atlas),
		m_index_buffer(std::move(other.m_index_buffer)),
		m_uniforms_id(other.m_uniforms_id)
	{
		for (auto quad : m_quads)
			quad->m_batch_instance = this;
	}

	BatchInstance& BatchInstance::operator=(BatchInstance&& other) noexcept
	{
		Mesh::operator=(std::move(other));
		m_raw_ib = std::move(other.m_raw_ib);
		m_raw_vb = std::move(other.m_raw_vb);
		m_quads = std::move(other.m_quads);
		texture_atlas = other.texture_atlas;
		m_index_buffer = std::move(other.m_index_buffer);
		m_uniforms_id = other.m_uniforms_id;

		for (auto quad : m_quads)
			quad->m_batch_instance = this;

		return *this;
	}

	int BatchInstance::get_max_quads()
	{
		int max_on_vs, max_on_fs;
		glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &max_on_vs);
		glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &max_on_fs);
		
		return std::min(max_on_vs, max_on_fs)/3 - 1;
	}

	void BatchInstance::m_add_quad(BatchQuad* quad, const Vec2<int>& tex_coords_p)
	{
		HXG_ASSERT((m_quads.size() < 250 || m_shader != &batching_shader_250),
			HXG_LOG_ERROR("By default, 1 BatchInstance is limited to 250 quads. You may want to use BatchingShader(size).");
			return;);

		HXG_ASSERT((m_quads.size() < get_max_quads()),
			HXG_LOG_ERROR("The hardware this software is running on does not support that much quads in a BatchingInstance !");
		return;);

		float tex_coords[4]
		{
			static_cast<float>(tex_coords_p.x * texture_atlas->get_cell_size().x) / texture_atlas->get_width(), // xmin
			static_cast<float>((tex_coords_p.x + 1) * texture_atlas->get_cell_size().x) / texture_atlas->get_width(), // xmax
			static_cast<float>(tex_coords_p.y * texture_atlas->get_cell_size().y) / texture_atlas->get_height(), // ymin
			static_cast<float>((tex_coords_p.y + 1) * texture_atlas->get_cell_size().y) / texture_atlas->get_height() // ymax
		};

		m_raw_ib.reserve(m_raw_ib.size() + 6);

		uint32_t last_index = m_raw_ib.size()/6 * 4;

		m_raw_ib.emplace_back(last_index + 0);
		m_raw_ib.emplace_back(last_index + 1);
		m_raw_ib.emplace_back(last_index + 2);
		m_raw_ib.emplace_back(last_index + 2);
		m_raw_ib.emplace_back(last_index + 3);
		m_raw_ib.emplace_back(last_index + 0);

		int halfcell_x = static_cast<int>(static_cast<float>(texture_atlas->get_cell_size().x) / 2 * quad->scale);
		int halfcell_y = static_cast<int>(static_cast<float>(texture_atlas->get_cell_size().y) / 2 * quad->scale);

		quad->scale = 1.0f;

		float vertecies[20]
		{
			toX(-halfcell_x), toY(-halfcell_y), tex_coords[0], tex_coords[2], static_cast<float>(m_quads.size()),
			toX(-halfcell_x), toY(+halfcell_y), tex_coords[0], tex_coords[3], static_cast<float>(m_quads.size()),
			toX(+halfcell_x), toY(+halfcell_y), tex_coords[1], tex_coords[3], static_cast<float>(m_quads.size()),
			toX(+halfcell_x), toY(-halfcell_y), tex_coords[1], tex_coords[2], static_cast<float>(m_quads.size())
		};

		m_raw_vb.insert(m_raw_vb.end(), std::begin(vertecies), std::end(vertecies));

		m_quads.push_back(quad);

		float quad_uniforms[12]
		{
			toX(quad->position.x), toY(quad->position.y),
			quad->scale, quad->rotation,
			quad->color_filter.R, quad->color_filter.G, quad->color_filter.B, quad->color_filter.A,
			quad->color.R, quad->color.G, quad->color.B, quad->color.A
		};

		m_uniforms.insert(m_uniforms.end(), std::begin(quad_uniforms), std::end(quad_uniforms));
	}

	void BatchInstance::construct_batch()
	{
		m_vb = { &m_raw_vb[0], static_cast<uint32_t>(80 * m_quads.size()) };
		m_index_buffer = { &m_raw_ib[0], GL_UNSIGNED_INT, static_cast<uint32_t>(m_raw_ib.size()) };
		m_ib = &m_index_buffer;
		m_texture = texture_atlas;
		m_vao = { m_vb, BatchQuad::s_vertex_layout, m_index_buffer };

		m_uniforms_id = m_shader->get_uniform("u_quads_uniforms");
	}

	void BatchInstance::draw(std::unordered_map<UniformInterface*, std::vector<void*>>& parents_uniforms)
	{
		if (!*enable_ptr)
			return;

		m_shader->bind();

		std::vector<float*>& angles = *reinterpret_cast<std::vector<float*>*>(&parents_uniforms[&u_rotation_angle]);
		float total_angle_degree = 0.0f;
		for (auto& angle_degree : angles)
			total_angle_degree += *angle_degree;

		double angle = (total_angle_degree + rotation) * (M_PI / 180.0);

		for (size_t i = 0; i < m_quads.size(); i++)
		{
			auto& quad = *m_quads[i];
			m_uniforms[i * 12 + 0] = toX(quad.position.x * cos(angle) + quad.position.y * sin(angle));
			m_uniforms[i * 12 + 1] = toY(quad.position.y * cos(angle) - quad.position.x * sin(angle));
			m_uniforms[i * 12 + 2] = quad.scale;
			m_uniforms[i * 12 + 3] = quad.rotation;
			m_uniforms[i * 12 + 4] = quad.color_filter.R;
			m_uniforms[i * 12 + 5] = quad.color_filter.G;
			m_uniforms[i * 12 + 6] = quad.color_filter.B;
			m_uniforms[i * 12 + 7] = quad.color_filter.A;
			m_uniforms[i * 12 + 8] = quad.color.R;
			m_uniforms[i * 12 + 9] = quad.color.G;
			m_uniforms[i * 12 + 10] = quad.color.B;
			m_uniforms[i * 12 + 11] = quad.color.A;
		}

		HXG_GL(glUniform1fv(m_uniforms_id, m_quads.size() * 12, &m_uniforms[0]));

		Mesh::draw(parents_uniforms);
	}

	BatchQuad::BatchQuad(BatchInstance* bi, const Vec2<int>& tex_coords, const Vec2<int>& pos, float size_p, float rotation_p)
		: position(pos),
		scale(size_p),
		rotation(rotation_p),
		m_batch_instance(bi)
	{
		bi->m_add_quad(this, tex_coords);
	}

	BatchQuad::BatchQuad(BatchQuad&& other) noexcept
		: position(other.position),
		scale(other.scale),
		rotation(other.rotation),
		color(other.color),
		m_batch_instance(other.m_batch_instance),
		color_filter(other.color_filter),
		opacity(color_filter.A)
	{
		auto& quads = m_batch_instance->m_quads;
		auto it = std::find(quads.begin(), quads.end(), &other);
		HXG_ASSERT((it != quads.end()),
			HXG_LOG_ERROR("Other have not been initialized");
			return;);
		*it = this;
	}

	BatchQuad& BatchQuad::operator=(BatchQuad&& other) noexcept
	{
		HXG_ASSERT(!m_batch_instance,
			HXG_LOG_ERROR("This has to be initialized by the default constructor");
			return *this;);

		position = other.position;
		scale = other.scale;
		rotation = other.rotation;
		m_batch_instance = other.m_batch_instance;
		color = other.color;
		color_filter = other.color_filter;

		auto& quads = m_batch_instance->m_quads;
		auto it = std::find(quads.begin(), quads.end(), &other);
		HXG_ASSERT((it != quads.end()),
			HXG_LOG_ERROR("Other have not been initialized");
			return *this;);
		*it = this;

		return *this;
	}

}