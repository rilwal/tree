#type vertex

layout(location = 0) in vec2 vertex_position;

void main() {
	gl_Position = vec4(vertex_position, 0, 1);
}


#type fragment
out vec4 fragColour; 

uniform vec2 camera_pos = vec2(0, 0);

uniform float vp_height = 1000;
uniform float vp_width = 1920;

uniform float angle = 0;

uniform float scale = 1.0;

uniform float tree_height = 600;

uniform vec3 c_sky_top = vec3(0, .9, .85);
uniform vec3 c_sky_bottom = vec3(1, 1, 1);

uniform vec3 c_ground_color = vec3(.137, .083, .012);
uniform vec3 c_base_tree_color = vec3(.6, 0.45, 0.345);
uniform float tree_base_width = 50;

uniform float ground_height = 300;
uniform float gamma = .69;

uniform vec3 c_light_color = vec3(1, 1, 1);

vec3 lerp(vec3 a, vec3 b, float f) {
    return a * f + b * (1 - f);
}

vec2 lerp(vec2 a, vec2 b, float f) {
    return a * f + b * (1 - f);
}

vec3 aces(vec3 color) {
    return (color * (2.51 * color + 0.3)) / (color * (2.43*color+0.59)+ 0.14);
}

vec2 point_line_proj(vec2 origin, vec2 dir, vec2 pt) {
    return (dot((pt - origin), dir) / dot(dir, dir)) * dir + origin;
}

struct Branch {
    vec2 origin;
    vec2 dir;
    float len;
    float width;
};


vec3 render_branch(vec2 pt, Branch b) {
    vec2 projected = point_line_proj(b.origin, b.dir, pt);
    vec2 perpendicular = pt - projected;

    float dst = length(pt - b.origin);

    vec2 a = b.origin;
    vec2 c = projected;
    vec2 B = b.origin + b.dir * b.len;

    float kac = dot(B-a, c-a);
    float kab = dot(B-a, B-a);
    

    if (pt.y >= ground_height && 0 <= kac && kac <= kab) {
        float tree_top_width = b.width * .8;
        float height_on_tree = dst / b.len;

        float tree_width = b.width * (1.0 - height_on_tree) + tree_top_width * height_on_tree;
        
        float dist_to_tree = length(perpendicular);
        
        if (dist_to_tree < tree_width / 2) {
            float dist_percent = dist_to_tree / (tree_width / 2);
            vec3 front_normal = vec3(0, 0, 1);
            vec3 side_normal = normalize(vec3(perpendicular, 0));

            vec3 normal = normalize(lerp(front_normal, side_normal, 1-dist_percent));
            return normal;
        }
    }

    return vec3(0);
}


void main() {
    vec3 light_pos = vec3(vp_width / 2, vp_height, 500);

    vec3 color = vec3(0);


    float x = ((gl_FragCoord.x / vp_width - .5) * vp_width + camera_pos.x) * scale;
    float y = ((gl_FragCoord.y / vp_height) * vp_height + camera_pos.y) * scale;

    if (y < ground_height) {
        color = c_ground_color;
    } else {
	    float sky_blend_ratio = clamp((y - ground_height) / (vp_height - ground_height), 0, 1);
        color = lerp(c_sky_top, c_sky_bottom, sky_blend_ratio);
    }



    vec2 t_dir = vec2(sin(angle), cos(angle));
    vec2 b_dir = lerp(t_dir, (cross(vec3(t_dir, 0), vec3(0, 0, 1)) / 2).xy, .2);
    vec2 b_dir2 = lerp(t_dir, (cross(vec3(0, 0, 1), vec3(t_dir, 0)) / 2).xy, .2);

    Branch trunk = Branch(vec2(0, ground_height), t_dir, tree_height, tree_base_width);
    Branch b1 = Branch(trunk.origin + t_dir * (trunk.len * 0.4), normalize(b_dir), tree_height * .5, tree_base_width * .4);
    Branch b2 = Branch(trunk.origin + t_dir * (trunk.len * 0.5), normalize(b_dir2), tree_height * .4, tree_base_width * .4);

    Branch branches[3] = Branch[3](b2, b1, trunk);

    for (int i = 0; i < 3; i++) {
        vec3 normal = render_branch(vec2(x, y), branches[i]);

        if (length(normal) > .001) {
            vec3 L = normalize(light_pos - vec3(x, y, 0));
            float LdotN = dot(L, normal);

            color = c_base_tree_color * c_light_color * max(0, LdotN)+
                c_base_tree_color * c_light_color * .5;
        }
    }

    fragColour = vec4(color,1);//vec4(pow(aces(color), vec3(1.0/gamma)), 1);
}



 