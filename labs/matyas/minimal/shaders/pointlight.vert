#version 330

uniform mat4 m_pvm;
uniform mat4 m_viewModel;
uniform mat3 m_normal;

uniform vec4 l_pos;

uniform int time;

in vec4 position;
in vec4 normal;    //por causa do gerador de geometria

out Data {
	vec3 normal;
	vec3 eye;
	vec3 lightDir;
} DataOut;

void main ()
{
	vec4 pos = m_viewModel * position;

	DataOut.normal = normalize(m_normal * normal.xyz);
	DataOut.lightDir = vec3(l_pos - pos);
	DataOut.eye = vec3(-pos);

	float sinTime = ((sin(time / 100000.0) + 1) / 2) * 360;

	vec4 tmp_Position = m_pvm * position;

	//vec4 tmp_Position_new = tmp_Position;
	//tmp_Position_new.x = tmp_Position.x * cos(sinTime) - tmp_Position.z * sin(sinTime);
	//tmp_Position_new.y = tmp_Position.y;
	//tmp_Position_new.z = tmp_Position.x * sin(sinTime) + tmp_Position.z * cos(sinTime);

	//tmp_Position.x = tmp_Position.x + sin(tmp_Position.y*10.0 + sinTime*20.0)*0.1;
	//tmp_Position[3] = tmp_Position[3] + (sinTime+1) * 2;

	gl_Position = tmp_Position;
}