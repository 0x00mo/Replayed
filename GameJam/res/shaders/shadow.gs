#version 460 core
layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;
  
uniform vec2 lightPos;
uniform vec2 resolution;

void main() {    
    vec2 v0 = (gl_in[0].gl_Position.xy + 1.0) / 2.0 * resolution;
    vec2 v1 = (gl_in[1].gl_Position.xy + 1.0) / 2.0 * resolution;

    vec2 edge = v1 - v0;
    vec2 lightToV0 = v0 - lightPos;

   //if(edge.x * lightToV0.y - edge.y * lightToV0.x > 0) {
        vec2 rayV0 = v0 + normalize(v0 - lightPos) * 2000.0;
        vec2 rayV1 = v1 + normalize(v1 - lightPos) * 2000.0;

        //if (lightToV0.y > 0.0) {
		//	v0.y -= 1.0;
		//	v1.y -= 1.0;
		//}
		//else {
		//	v0.y += 1.0;
		//	v1.y += 1.0;
		//}

        gl_Position = vec4(rayV0 / resolution * 2.0 - 1.0, 0.0, 1.0);
        EmitVertex();
        gl_Position = vec4(v0 / resolution * 2.0 - 1.0, 0.0, 1.0);
        EmitVertex();
        gl_Position = vec4(rayV1 / resolution * 2.0 - 1.0, 0.0, 1.0);
        EmitVertex();
        gl_Position = vec4(v1 / resolution * 2.0 - 1.0, 0.0, 1.0);
        EmitVertex();
        EndPrimitive();
    //}
} 