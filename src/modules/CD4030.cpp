//----------------------------------------------------------------------------
//	Lunetta Modula Plugin for VCV Rack by Count Modula - CD4030
//	Quad 2 Input XOR Gate
//  Copyright (C) 2020  Adam Verspaget
//----------------------------------------------------------------------------
#include "../LunettaModula.hpp"
#include "../inc/Utility.hpp"
#include "../inc/CMOSInput.hpp"

// used by mode management includes
#define MODULE_NAME CD4030

#define NUM_GATES 4

struct CD4030 : Module {
	enum ParamIds {
		NUM_PARAMS
	};
	enum InputIds {
		ENUMS(A_INPUTS, NUM_GATES),
		ENUMS(B_INPUTS, NUM_GATES),
		NUM_INPUTS
	};
	enum OutputIds {
		ENUMS(Q_OUTPUTS, NUM_GATES),
		NUM_OUTPUTS
	};
	enum LightIds {
		ENUMS(Q_LIGHTS, NUM_GATES),
		NUM_LIGHTS
	};
	
	// add the variables we'll use when managing modes
	#include "../modes/modeVariables.hpp"
	
	CMOSInput aInputs[NUM_GATES];
	CMOSInput bInputs[NUM_GATES];
	
	CD4030() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
	}
	
	void onReset() override {
		for (int g = 0; g < NUM_GATES; g++) {
			aInputs[g].reset();
			bInputs[g].reset();
		}
	}
	
	void setIOMode (int mode) {
		
		// set CMOS input properties
		for (int g = 0; g < NUM_GATES; g++) {
			aInputs[g].setMode(mode);
			bInputs[g].setMode(mode);
		}
		
		// set gate voltage
		#include "../modes/setGateVoltage.hpp"
	}

	json_t *dataToJson() override {
		json_t *root = json_object();

		json_object_set_new(root, "moduleVersion", json_integer(1));
		
		// add the I/O mode details
		#include "../modes/dataToJson.hpp"		

		return root;
	}
	
	void dataFromJson(json_t *root) override {

		// grab the I/O mode details
		#include "../modes/dataFromJson.hpp"
	}	

	void process(const ProcessArgs &args) override {
		
		for (int g = 0; g < NUM_GATES; g++) {
			bool a = aInputs[g].process(inputs[A_INPUTS + g].getVoltage());
			bool q = (a == bInputs[g].process(inputs[B_INPUTS + g].getVoltage()));

			outputs[Q_OUTPUTS + g].setVoltage(boolToGateInverted(q));
			lights[Q_LIGHTS + g].setBrightness(boolToLightInverted(q));
		}		
	}
};

struct CD4030Widget : ModuleWidget {
	CD4030Widget(CD4030 *module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/CD4030.svg")));

		// screws
		#include "../components/stdScrews.hpp"	

		int cols[NUM_GATES] = {STD_COL1, STD_COL3, STD_COL1, STD_COL3};
		int rows[NUM_GATES] = {STD_ROW1, STD_ROW1, STD_ROW4, STD_ROW4};
		for (int g = 0; g < NUM_GATES; g++) {
		
			// A/B inputs
			addInput(createInputCentered<LunettaModulaLogicInputJack>(Vec(STD_COLUMN_POSITIONS[cols[g]], STD_ROWS6[rows[g]]), module, CD4030::A_INPUTS + g));
			addInput(createInputCentered<LunettaModulaLogicInputJack>(Vec(STD_COLUMN_POSITIONS[cols[g]], STD_ROWS6[rows[g] + 1]), module, CD4030::B_INPUTS + g));
			
			// Q output
			addOutput(createOutputCentered<LunettaModulaLogicOutputJack>(Vec(STD_COLUMN_POSITIONS[cols[g]], STD_ROWS6[rows[g] + 2]), module, CD4030::Q_OUTPUTS + g));
			
			// lights
			addChild(createLightCentered<SmallLight<RedLight>>(Vec(STD_COLUMN_POSITIONS[cols[g]] + 12, STD_ROWS6[rows[g] + 2] - 19), module, CD4030::Q_LIGHTS + g));
		}
	}

	// include the I/O mode menu item struct we'll need when we add the theme menu items
	#include "../modes/modeMenuItem.hpp"

	void appendContextMenu(Menu *menu) override {
		CD4030 *module = dynamic_cast<CD4030*>(this->module);
		assert(module);

		// blank separator
		menu->addChild(new MenuSeparator());
		
		// add the I/O mode menu items
		#include "../modes/modeMenus.hpp"
	}		
};

Model *modelCD4030 = createModel<CD4030, CD4030Widget>("CD4030");
