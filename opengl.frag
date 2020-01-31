uniform sampler2DRect ourTexture0;
uniform sampler2DRect ourTexture1;
uniform sampler2DRect ourTexture2;
uniform int size;
void main() {
	vec2 coord = gl_FragCoord.xy;
	gl_FragColor = 0;
	for(int i=0;i<size;++i)
		gl_FragColor+=texture2DRect(ourTexture0, vec2(i,int(coord.y)))*texture2DRect(ourTexture1,vec2(int(coord.x),i));
}