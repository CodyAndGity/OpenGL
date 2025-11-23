#include "PostProcessComponent.h"
#include "Engine.h"
namespace neu {
	FACTORY_REGISTER(PostProcessComponent)

	void PostProcessComponent::Update(float dt)	{
	}

	void PostProcessComponent::Apply(Program& program) {
		program.SetUniform("u_parameters", (uint32_t)parameters);
		program.SetUniform("u_color_tint", colorTint);
		program.SetUniform("u_time", GetEngine().GetTime().GetTime());
		program.SetUniform("u_blend",blend);
		
		program.SetUniform("u_kernel",kernel);
		
	}

	void PostProcessComponent::Read(const serial_data_t& value) {
		Object::Read(value);
	}

	void PostProcessComponent::UpdateGui() {
		uint32_t iparameters = (uint32_t)parameters;

		bool parameter = iparameters & (uint32_t)Parameters::GrayScale;
		if (ImGui::Checkbox("Gray Scale", &parameter)) {
			if (parameter) iparameters |= (uint32_t)Parameters::GrayScale;
			else iparameters &= ~(uint32_t)Parameters::GrayScale;
		}

		parameter = iparameters & (uint32_t)Parameters::ColorTint;
		if (ImGui::Checkbox("Color Tint", &parameter)) {
			if (parameter) iparameters |= (uint32_t)Parameters::ColorTint;
			else iparameters &= ~(uint32_t)Parameters::ColorTint;
		}

		parameter = iparameters & (uint32_t)Parameters::ScanLine;
		if (ImGui::Checkbox("Scan Line", &parameter)) {
			if (parameter) iparameters |= (uint32_t)Parameters::ScanLine;
			else iparameters &= ~(uint32_t)Parameters::ScanLine;
		}

		parameter = iparameters & (uint32_t)Parameters::Grain;
		if (ImGui::Checkbox("Grain", &parameter)) {
			if (parameter) iparameters |= (uint32_t)Parameters::Grain;
			else iparameters &= ~(uint32_t)Parameters::Grain;
		}

		parameter = iparameters & (uint32_t)Parameters::Invert;
		if (ImGui::Checkbox("Invert", &parameter)) {
			if (parameter) iparameters |= (uint32_t)Parameters::Invert;
			else iparameters &= ~(uint32_t)Parameters::Invert;
		}

		parameter = iparameters & (uint32_t)Parameters::EdgeDetection;
		if (ImGui::Checkbox("Edge Detection", &parameter)) {
			if (parameter) iparameters |= (uint32_t)Parameters::EdgeDetection;
			else iparameters &= ~(uint32_t)Parameters::EdgeDetection;
		}

		

		parameters = (Parameters)iparameters;

		ImGui::ColorEdit3("Color Tint", glm::value_ptr(colorTint));
		ImGui::SliderFloat("Blend",&blend,0,1);
		float* smallKernel1[3] = {&kernel[0][0],&kernel[0][1],&kernel[0][2] };
		ImGui::DragFloat3("kernel 1-3", *(smallKernel1));
		float* smallKernel2[3] = { &kernel[1][0],&kernel[1][1],&kernel[1][2] };
		ImGui::DragFloat3("kernel 4-6", *(smallKernel2));
		float* smallKernel3[3] = { &kernel[2][0],&kernel[2][1],&kernel[0][2] };
		ImGui::DragFloat3("kernel 7-9", *(smallKernel3));
	}
}
