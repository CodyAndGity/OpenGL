#pragma once

namespace neu {
	class LightComponent : public Component	{
	public:
		enum class LightType {
			POINT_LIGHT = 0,
			DIRECTIONAL_LIGHT = 1,
			SPOT_LIGHT = 2
		};
	public:
		CLASS_PROTOTYPE(LightComponent)

		void Update(float dt) override;
		void SetProgram(Program& program, const std::string& name, const glm::mat4& view);

		void Read(const serial_data_t& value) override;
		void UpdateGui() override;

	public:
		glm::vec3 color{ 1, 1, 1 };
		float intensity{1};
		float range{ 20 };
		LightType lightType{ LightType::POINT_LIGHT };
		float innerCutoff{ 10.0f };
		float outerCutoff{ 30.0f };
	};
}