uniform sampler2D texture;

uniform float width;
uniform float height;
uniform float zoom;
uniform float radius;
uniform float mx;
uniform float my;
uniform float px;
uniform float py;

void main() {
    vec2 screen = vec2(width, height);
    float yx_ratio = height / width;

    vec2 screen_uv = (gl_FragCoord.xy + vec2(px, py))/ screen;
    vec2 mouse_uv = vec2(mx + px, my + py) / screen;

    mouse_uv.y = 1. - mouse_uv.y;

    const float cell_size = 1.;
    const float border_thickness = .05;

    float vertical = step(cell_size, mod(screen_uv.x * width * zoom - border_thickness * .5, cell_size + border_thickness));
    float horizontal = step(cell_size, mod(screen_uv.y * height * zoom - border_thickness * .5, cell_size + border_thickness));
 
    float dist = length((screen_uv - mouse_uv) * vec2(1., yx_ratio));
    float mask = 1. - dist / radius;

    gl_FragColor = vec4(vec3(normalize(vertical + horizontal)) * mask, 1.);
}