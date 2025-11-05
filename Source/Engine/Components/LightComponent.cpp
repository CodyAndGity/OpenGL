#include "EnginePCH.h"

namespace neu {
	FACTORY_REGISTER(LightComponent);

	void LightComponent::Update(float dt){

	}

	void LightComponent::SetProgram(Program& program, const std::string& name, const glm::mat4& view){
		glm::vec3 position = glm::vec3(view * glm::vec4(owner->transform.position, 1));
		glm::vec3 direction = glm::normalize(glm::mat3(view)*owner->transform.Forward());
		program.SetUniform(name+".type", (int)lightType);
		program.SetUniform(name+".position", position);
		program.SetUniform(name+".direction", direction);
		program.SetUniform(name+".color", color);
		program.SetUniform(name+".intensity", intensity);
		program.SetUniform(name+".range", range);
		program.SetUniform(name+".innerCutoff", glm::radians(innerCutoff));
		program.SetUniform(name+".outerCutoff", glm::radians(outerCutoff));
	}

	void LightComponent::Read(const serial_data_t& value){
		std::string type;
		SERIAL_READ_NAME(value,"lightType", type);
		if (equalsIgnoreCase(type, "Point_Light")) {
			lightType = LightType::POINT_LIGHT;
		}
		else if (equalsIgnoreCase(type, "Directional_Light")) {
			lightType = LightType::DIRECTIONAL_LIGHT;
		}else if (equalsIgnoreCase(type, "Spot_Light")) {
			lightType = LightType::SPOT_LIGHT;
		}
		SERIAL_READ(value, color);
		SERIAL_READ(value, intensity);
		SERIAL_READ(value, range);
		SERIAL_READ(value, outerCutoff);
	}

	void LightComponent::UpdateGui(){
		const char* types[] = { "Point Light", "Directional Light", "Spot Light" };
		ImGui::Combo("Type", (int*)(&lightType), types, IM_ARRAYSIZE(types));

		ImGui::ColorEdit3("Light Color", glm::value_ptr(color));
		ImGui::DragFloat("Intensity", &(intensity),0.1f,0.0f);
		if (lightType != LightType::DIRECTIONAL_LIGHT) {
			ImGui::DragFloat("Range", &(range), 0.1f, 0.0f);
		}
		if (lightType == LightType::SPOT_LIGHT) {
			ImGui::DragFloat("InnerCutoff", &(innerCutoff), 0.1f, 0.0f, outerCutoff);
			ImGui::DragFloat("OuterCutoff", &(outerCutoff), 0.1f, innerCutoff);
		}
	}
}
