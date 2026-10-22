```yaml
# example configuration:

light:
  - platform: nezumikun_light
    name: Nezumikun light
    output: led_output

output:
  - platform: neopixelbus
    pin: D1
    type: GRB
    variant: WS2812
    num_leds: 60
    id: led_output
```