#type vertex

layout(location = 0) in vec2 vertex_position;

void main() {
	gl_Position = vec4(vertex_position * .9, 0, 1);
}


#type fragment
out vec4 fragColour; 

uniform float vp_height = 1000;
uniform float vp_width = 1920;

uniform float angle = 0;

uniform float scale = 1.0;

uniform float tree_height = 600;

uniform vec3 sky_top = vec3(0, .9, .85);
uniform vec3 sky_bottom = vec3(1, 1, 1);

uniform vec3 ground_color = vec3(.137, .083, .012);
uniform vec3 base_tree_color = vec3(.6, 0.45, 0.345);
uniform float tree_base_width = 50;

uniform float ground_height = 300;
uniform float gamma = .69;

uniform vec3 light_color = vec3(1, 1, 1);

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


float dist_from_pt_to_line(vec2 l1, vec2 l2, vec2 p) {
    return length(p - point_line_proj(l1, l2, p));
    //return abs((l2.x - l1.x) * (l1.y - p.y) - (l1.x - p.x) * (l2.y - l1.y))
    //    / sqrt(pow(l2.x - l1.x, 2) + pow(l2.y - l1.y, 2));  
}


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
        
        if (dist_to_tree < tree_width) {
            float dist_percent = dist_to_tree / (tree_width);
            vec3 front_normal = vec3(0, 0, 1);
            vec3 side_normal = normalize(vec3(normalize(perpendicular), 0));

            vec3 normal = dist_percent * side_normal +  (1 - dist_percent) * front_normal;
            normal = normalize(normal);

            return normal;
        }
    }

    return vec3(0);
}


void main() {
    vec3 light_pos = vec3(vp_width / 2, vp_height, 30);

    vec3 color = vec3(0);

    vec3 avg_sky_color = (sky_top + sky_bottom) / 2;
    //vec3 light_color = lerp(avg_sky_color, vec3(1), .2);


    float x = (gl_FragCoord.x / vp_width - .5) * vp_width;
    float y = (gl_FragCoord.y / vp_height) * vp_height;

    if (y < ground_height) {
        color = ground_color;
    } else {
	    float sky_blend_ratio = (y - ground_height) / (vp_height - ground_height);
        color = lerp(sky_top, sky_bottom, sky_blend_ratio);
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

        if (length(normal) > .5) {
            vec3 L = normalize(light_pos - vec3(x, y, 0));
            float LdotN = dot(L, normal);
            float d_squared = dot(L, L);

            color = normal;
            //color = base_tree_color * light_color * max(0, LdotN);
        }
    }

    fragColour = vec4(pow(aces(color), vec3(1.0/gamma)), 1);
}



 