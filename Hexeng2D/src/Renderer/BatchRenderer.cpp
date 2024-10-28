#include <cassert>

#include "BatchRenderer.hpp"
#include "IndexBuffer.hpp"
#include "Renderer.hpp"
#include "VertexBuffer.hpp"

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
        add_uniforms({&TextureAtlas::u_bleeding_correction});
	}

	BatchingShader::BatchingShader(BatchingShader&& other) noexcept : Shader(std::move(other)) {}
	BatchingShader& BatchingShader::operator=(BatchingShader&& other) noexcept { Shader::operator=(std::move(other)); return *this; }

	VertexLayout BatchQuad::s_vertex_layout;

	ToBeInit BatchQuad::s_init_vl{ std::function<void(void)>{[]() {
		s_vertex_layout = VertexLayout({ { 2, GL_FLOAT }, { 2, GL_FLOAT }, { 1, GL_FLOAT } });
	}} };

    uint32_t _pixel_value(uint8_t* pb, int i, int j, int width) {
        return 
            pb[4*(j*width+i)]   *256*256*256 +
            pb[4*(j*width+i)+1] *256*256 +
            pb[4*(j*width+i)+2] *256 +
            pb[4*(j*width+i)+3];
    }

    TALayout::TALayout(const std::string& filepath)
    {
        Image image{ filepath };
        HXG_ASSERT(image.BPP == 4, HXG_LOG_ERROR("Layouts are expected to be 4 bytes per pixel"););

        m_is_grid = false;
        m_size = image.size;
        int width = image.size.x;

        for (int i = 0; i < image.size.x; i++) {
            for (int j = 0; j < image.size.y; j++) {
                uint32_t pixel = _pixel_value(image.pixel_buffer, i, j, width);
                auto it = m_map.find(pixel);
                if (it != m_map.end())
                {
                    int offx = 0;
                    while(i+offx < image.size.x && _pixel_value(image.pixel_buffer, i+offx, j, width) == pixel) offx++;
                    int offy = 0;
                    while(j+offy < image.size.y && _pixel_value(image.pixel_buffer, i, j+offy, width) == pixel) offy++;

                    m_map.insert({pixel, fTexCoord{
                            { (float)i/image.size.x,        (float)j/image.size.y       },
                            { (float)(i+offx)/image.size.x, (float)(j+offy)/image.size.y}
                    }});
                }
            }
        }
    }

    TALayout::TALayout(Vec2<int> cell_size, Vec2<int> grid_size)
        : m_size(grid_size*cell_size), m_cell_size(cell_size), m_is_grid(true) {}

    fTexCoord TALayout::get_coords(Color4 id)
    {
        HXG_ASSERT((!m_is_grid), HXG_LOG_ERROR("access with colors only with custom layouts"););
        auto it = m_map.find(id.R*(1<<24) + id.G*(1<<16) + id.B*(1<<8) + id.A);
        HXG_ASSERT((it != m_map.end()), HXG_LOG_ERROR("This color does not belong to the layout"););
        return it->second;
    }

    fTexCoord TALayout::get_coords(Vec2<int> id)
    {
        HXG_ASSERT(m_is_grid, HXG_LOG_ERROR("access with Vec2<int> only with a grid-like layout"););
		return fTexCoord{
            {(float)(id.x * m_cell_size.x) / m_size.x, (float)(id.y * m_cell_size.y) / m_size.y},
            {(float)(id.x + 1) * m_cell_size.x / m_size.x, (float)(id.y + 1) * m_cell_size.y / m_size.y}
        };
    }

    Uniform<float> TextureAtlas::u_bleeding_correction;

    ToBeInit _init_bc([](){
            TextureAtlas::u_bleeding_correction = Uniform<float>({{UniformArgType::NAME, "u_bleeding_correction"}});
    });

	TextureAtlas::TextureAtlas(const std::string& filepath, const TexSettList& settings)
		: Texture(filepath, settings)
	{
		HXG_ASSERT(m_mag_filter == m_min_filter && (m_mag_filter == GL_LINEAR || m_mag_filter == GL_NEAREST),
			HXG_LOG_ERROR("invalid mag and/or min filter"););
        
        if (m_mag_filter == GL_LINEAR) m_bleeding_correction = 0.5;
        else if (m_mag_filter == GL_NEAREST) m_bleeding_correction = 1.0/512;
	}

	TextureAtlas::TextureAtlas(TextureAtlas&& other) noexcept
		: Texture(std::move(other)),
		m_bleeding_correction(other.m_bleeding_correction) {}

	TextureAtlas& TextureAtlas::operator=(TextureAtlas&& other) noexcept
	{
		Texture::operator=(std::move(other));
        m_bleeding_correction = other.m_bleeding_correction;
		return *this;
	}

	BatchInstance::BatchInstance(TALayout layout, Shader* shader)
		: m_layout(layout)
	{
		this->shader = shader;
	}

	BatchInstance::BatchInstance(BatchInstance&& other) noexcept
		: Mesh(std::move(other)),
		m_raw_ib(std::move(other.m_raw_ib)),
		m_raw_vb(std::move(other.m_raw_vb)),
		m_quads(std::move(other.m_quads)),
        m_layout(other.m_layout),
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
        m_layout = other.m_layout;
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

	void BatchInstance::m_add_quad(BatchQuad* quad, fTexCoord tex_coords)
	{
		HXG_ASSERT((m_quads.size() < 250 || shader != &batching_shader_250),
			HXG_LOG_ERROR("By default, 1 BatchInstance is limited to 250 quads. You may want to use BatchingShader(size).");
			return;);

		HXG_ASSERT((m_quads.size() < get_max_quads()),
			HXG_LOG_ERROR("The hardware this software is running on does not support that much quads in a BatchingInstance !");
		return;);

		m_raw_ib.reserve(m_raw_ib.size() + 6);

		uint32_t last_index = m_raw_ib.size()/6 * 4;

		m_raw_ib.emplace_back(last_index + 0);
		m_raw_ib.emplace_back(last_index + 1);
		m_raw_ib.emplace_back(last_index + 2);
		m_raw_ib.emplace_back(last_index + 2);
		m_raw_ib.emplace_back(last_index + 3);
		m_raw_ib.emplace_back(last_index + 0);

		int halfcell_x = (int)((float)(tex_coords.top_right.x-tex_coords.bot_left.x)*m_layout.m_size.x / 2 * quad->scale);
		int halfcell_y = (int)((float)(tex_coords.top_right.y-tex_coords.bot_left.y)*m_layout.m_size.y / 2 * quad->scale);

		quad->scale = 1.0f;

		float vertecies[20]
		{
			-halfcell_x, -halfcell_y, tex_coords.bot_left.x,  tex_coords.bot_left.y, static_cast<float>(m_quads.size()*4),    // bottom-left 
			-halfcell_x, +halfcell_y, tex_coords.bot_left.x,  tex_coords.top_right.y, static_cast<float>(m_quads.size()*4+1), // top-left 
			+halfcell_x, +halfcell_y, tex_coords.top_right.x, tex_coords.top_right.y, static_cast<float>(m_quads.size()*4+2), // top-right 
			+halfcell_x, -halfcell_y, tex_coords.top_right.x, tex_coords.bot_left.y, static_cast<float>(m_quads.size()*4+3)  // bottom-right 
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
		m_index_buffer = { &m_raw_ib[0], GL_UNSIGNED_INT, static_cast<uint32_t>(m_raw_ib.size()) };

		m_uniforms_id = shader->get_uniform("u_quads_uniforms");
        this->Mesh::operator=({&m_raw_vb[0], 80*m_quads.size(), position, BatchQuad::s_vertex_layout, &m_index_buffer, texture, shader});
	}

	void BatchInstance::draw(std::unordered_map<UniformInterface*, std::vector<void*>>& parents_uniforms)
	{
		if (!*enable_ptr)
			return;

		shader->bind();

		std::vector<float*>& angles = *reinterpret_cast<std::vector<float*>*>(&parents_uniforms[&u_rotation_angle]);
		float total_angle_degree = 0.0f;
		for (auto& angle_degree : angles)
			total_angle_degree += *angle_degree;

		double angle = (total_angle_degree + rotation) * (M_PI / 180.0);

        if (texture)
            TextureAtlas::u_bleeding_correction.refresh(shader, &((TextureAtlas*)texture)->m_bleeding_correction);

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

	BatchQuad::BatchQuad(BatchInstance* bi, Vec2<int> tex_coords, Vec2<float> pos, float size_p, float rotation_p)
		: position(pos),
		scale(size_p),
		rotation(rotation_p),
		m_batch_instance(bi)
	{
		bi->m_add_quad(this, bi->m_layout.get_coords(tex_coords));
	}

	BatchQuad::BatchQuad(BatchInstance* bi, Color4 id, Vec2<float> pos, float size_p, float rotation_p)
		: position(pos),
		scale(size_p),
		rotation(rotation_p),
		m_batch_instance(bi)
	{
		bi->m_add_quad(this, bi->m_layout.get_coords(id));
	}

	BatchQuad::BatchQuad(BatchInstance* bi, const std::string& color_id, Vec2<float> pos, float size_p, float rotation_p)
        : BatchQuad(bi, Color4{color_id}, pos, size_p, rotation_p) {}

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
