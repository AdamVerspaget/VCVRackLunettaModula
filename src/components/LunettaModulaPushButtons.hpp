//----------------------------------------------------------------------------
//	Lunetta Modula Plugin for VCV Rack by Count Modula
//  Custom push buttons
//  Copyright (C) 2020  Adam Verspaget 
//----------------------------------------------------------------------------
#include "componentlibrary.hpp"

using namespace rack;

//----------------------------------------------------------------------------
// Lit push buttons
//----------------------------------------------------------------------------

// base for push button light 
template <typename TBase>
struct LunettaModulaPBLight : TBase {
	void drawLight(const widget::Widget::DrawArgs& args) override {
		nvgBeginPath(args.vg);
		
		// set rounded radius at 12% of overal size - closely matches the rounding on grey area of hte underlying button svg
		float r = std::min( this->box.size.x, this->box.size.y) * 0.12;
		nvgRoundedRect(args.vg, 0.0, 0.0, this->box.size.x, this->box.size.y, r);

		// Background
		if (this->bgColor.a > 0.0) {
			nvgFillColor(args.vg, this->bgColor);
			nvgFill(args.vg);
		}

		// Foreground
		if (this->color.a > 0.0) {
			nvgFillColor(args.vg, this->color);
			nvgFill(args.vg);
		}

		// Border
		if (this->borderColor.a > 0.0) {
			nvgStrokeWidth(args.vg, 0.5);
			nvgStrokeColor(args.vg, this->borderColor);
			nvgStroke(args.vg);
		}
	}
};

// Base for lit buttons
struct LunettaModulaLitPB : SvgSwitch {
	ModuleLightWidget* light;
	
	LunettaModulaLitPB() {
		momentary = false;
 		
		// no shadow for buttons
		shadow->opacity = 0.0f;
	}
	
	// override the base randomizer as it sets switches to invalid values.
	void randomize() override {
		SvgSwitch::randomize();
		
		if (paramQuantity->getValue() > 0.5f)
			paramQuantity->setValue(1.0f);
		else
			paramQuantity->setValue(0.0f);
	}	

	void setFirstLightId(int firstLightId) {

		if (paramQuantity)
			light->module = paramQuantity->module;
		
		light->firstLightId = firstLightId;
		
		// set size to 79% of the bezel size - makes the led close enough in size to the grey area in the button svg
		light->box.size = box.size.mult(0.79);
		
		// Move center of light to center of box
		light->box.pos = box.size.div(2).minus(light->box.size.div(2));
		addChild(light);
	}
	
	void onChange(const event::Change& e) override {

		if (!frames.empty() && paramQuantity) {
			int index = (int) std::round(paramQuantity->getValue() - paramQuantity->getMinValue());
			index = math::clamp(index, 0, (int) frames.size() - 1);
			sw->setSvg(frames[index]);

			light->module->lights[light->firstLightId].setBrightness(index > 0 ? 1.0 : 0.0);
			fb->dirty = true;
		}
		
		ParamWidget::onChange(e);
	}
	
	void step() override{

		if (light->module) {
			light->module->lights[light->firstLightId].setBrightness(paramQuantity->getValue() > 0.5 ? 1.0 : 0.0);
		}
		
		SvgSwitch::step();
	}	
};

// Base for lit momentary buttons
struct LunettaModulaLitPBMomentary : LunettaModulaLitPB {
	LunettaModulaLitPBMomentary() {
		momentary = true;
	}
};

// standard lit button
template <typename TLightBase = WhiteLight>
struct LunettaModulaLEDPushButton : LunettaModulaLitPB {

	LunettaModulaLEDPushButton() {
		light = new LEDBezelLight<TLightBase>;

		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Components/PushButton_0.svg")));
	}
};

// standard momentary lit button
template <typename TLightBase = WhiteLight>
struct LunettaModulaLEDPushButtonMomentary : LunettaModulaLitPBMomentary {

	LunettaModulaLEDPushButtonMomentary() {
		light = new LEDBezelLight<TLightBase>;

		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Components/PushButton_0.svg")));
	}
};

// mini lit button
template <typename TLightBase = WhiteLight>
struct LunettaModulaLEDPushButtonMini : LunettaModulaLitPB {

	LunettaModulaLEDPushButtonMini() {
		light = new LEDBezelLight<TLightBase>;

		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Components/PushButtonMini_0.svg")));
	}
};

// mini momentary lit button
template <typename TLightBase = WhiteLight>
struct LunettaModulaLEDPushButtonMiniMomentary : LunettaModulaLitPBMomentary {

	LunettaModulaLEDPushButtonMiniMomentary() {
		light = new LEDBezelLight<TLightBase>;

		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Components/PushButtonMini_0.svg")));
	}
};

// big lit button
template <typename TLightBase = WhiteLight>
struct LunettaModulaLEDPushButtonBig : LunettaModulaLitPB {

	LunettaModulaLEDPushButtonBig() {
		light = new LEDBezelLight<TLightBase>;

		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Components/PushButtonBig_0.svg")));
	}
};

// big momentary lit button
template <typename TLightBase = WhiteLight>
struct LunettaModulaLEDPushButtonBigMomentary : LunettaModulaLitPBMomentary {

	LunettaModulaLEDPushButtonBigMomentary() {
		light = new LEDBezelLight<TLightBase>;

		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Components/PushButtonBig_0.svg")));
	}
};

// mega lit button
template <typename TLightBase = WhiteLight>
struct LunettaModulaLEDPushButtonMega : LunettaModulaLitPB {

	LunettaModulaLEDPushButtonMega() {
		light = new LEDBezelLight<TLightBase>;

		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Components/PushButtonMega_0.svg")));
	}
};

// mega momentary lit button
template <typename TLightBase = WhiteLight>
struct LunettaModulaLEDPushButtonMegaMomentary : LunettaModulaLitPBMomentary {

	LunettaModulaLEDPushButtonMegaMomentary() {
		light = new LEDBezelLight<TLightBase>;

		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Components/PushButtonMega_0.svg")));
	}
};

// Helper specifically for lit buttons
template <class TParamWidget>
TParamWidget* createParamCentered(math::Vec pos, engine::Module* module, int paramId, int lightId) {
	TParamWidget* o = createParam<TParamWidget>(pos, module, paramId);
	o->box.pos = o->box.pos.minus(o->box.size.div(2));
	
	o->setFirstLightId(lightId);
	
	return o;
}


//-------------------------------------------------------------------
// Unlit Push buttons
//-------------------------------------------------------------------

// unlit push button base
struct LunettaModulaPB :  SvgSwitch {
	LunettaModulaPB() {
		// no shadow for switches or buttons
		shadow->opacity = 0.0f;
	}

	// override the base randomizer as it sets switches to invalid values.
	void randomize() override {
		SvgSwitch::randomize();
		
		if (paramQuantity->getValue() > 0.5f)
			paramQuantity->setValue(1.0f);
		else
			paramQuantity->setValue(0.0f);
	}
};

// srtandard push button
struct LunettaModulaUnlitPushButton : LunettaModulaPB {
    LunettaModulaUnlitPushButton() {
        addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Components/PushButton_0.svg")));
        addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Components/PushButton_0.svg")));
    }
};

struct LunettaModulaUnlitPushButtonMomentary : LunettaModulaPB {
    LunettaModulaUnlitPushButtonMomentary() {
        addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Components/PushButton_0.svg")));
        addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Components/PushButton_0.svg")));

		momentary = true;
    }
}; 
 
// small square push button
struct LunettaModulaUnlitPushButtonMini : LunettaModulaPB {
    LunettaModulaUnlitPushButtonMini() {
        addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Components/PushButtonMini_0.svg")));
        addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Components/PushButtonMini_0.svg")));
    }
};

struct LunettaModulaUnlitPushButtonMiniMomentary : LunettaModulaPB {
    LunettaModulaUnlitPushButtonMiniMomentary() {
        addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Components/PushButtonMini_0.svg")));
        addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Components/PushButtonMini_0.svg")));
		
 		momentary = true;
    }
};
 
// big square push button
struct LunettaModulaUnlitPushButtonSwitchBig : LunettaModulaPB {
    LunettaModulaUnlitPushButtonSwitchBig() {
        addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Components/PushButtonBig_0.svg")));
        addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Components/PushButtonBig_0.svg")));
    }
};

struct LunettaModulaUnlitPushButtonBigMomentary : LunettaModulaPB {
    LunettaModulaUnlitPushButtonBigMomentary() {
        addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Components/PushButtonBig_0.svg")));
        addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Components/PushButtonBig_0.svg")));
		
 		momentary = true;
    }
};
 
// really big square push button
struct LunettaModulaUnlitPushButtonMega : LunettaModulaPB {
    LunettaModulaUnlitPushButtonMega() {
        addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Components/PushButtonMega_0.svg")));
        addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Components/PushButtonMega_0.svg")));
    }
};

struct LunettaModulaUnlitPushButtonMegaMomentary : LunettaModulaPB {
    LunettaModulaUnlitPushButtonMegaMomentary() {
        addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Components/PushButtonMega_0.svg")));
        addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Components/PushButtonMega_0.svg")));

 		momentary = true;
   }
};
