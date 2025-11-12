#pragma once
#include "RendererComponent.h"
namespace neu {
	class ModelRenderer :public RendererComponent {
	public:

		CLASS_PROTOTYPE(ModelRenderer);

		void Update(float dt) override;
		void Draw(Renderer& renderer) override;

		void Read(const serial_data_t& value);

		// Inherited via RendererComponent
		void UpdateGui() override;
	public:
		res_t<Model> model;
		res_t<Material> material;
		// Inherited via RendererComponent
		bool enableDepth{ true };
		GLint cullFace{ GL_BACK };

	};
}