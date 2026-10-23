import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light
from esphome.components.light import AddressableLightState
from esphome.const import CONF_ID, CONF_LIGHT_ID

AUTO_LOAD = ['light']
CONF_WIDTH = 'width'
CONF_HEIGHT = 'height'

nezumikun_light_2d_ns = cg.esphome_ns.namespace("nezumikun_light_2d")
NezumikunLight2DComponent = nezumikun_light_2d_ns.class_("NezumikunLight2DComponent", cg.Component)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(NezumikunLight2DComponent),
        cv.Required(CONF_LIGHT_ID): cv.use_id(AddressableLightState),
        cv.Required(CONF_WIDTH): cv.int_range(1, 255),
        cv.Required(CONF_HEIGHT): cv.int_range(1, 255),
    }
).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    light_state = await cg.get_variable(config[CONF_LIGHT_ID])
    cg.add(var.set_strip(light_state))
    cg.add(var.set_canvas_width(config[CONF_WIDTH]))
    cg.add(var.set_canvas_height(config[CONF_HEIGHT]))
    
