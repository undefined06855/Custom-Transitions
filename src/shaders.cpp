#include "shaders.hpp"

const GLchar* g_transitionShaderVertex = R"(
attribute vec4 a_position;
attribute vec2 a_texCoord;
attribute vec4 a_color;

#ifdef GL_ES
varying mediump vec4 v_fragmentColor;
varying mediump vec2 v_texCoord;
#else
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
#endif

void main() {
    gl_Position = CC_MVPMatrix * a_position;
    v_fragmentColor = a_color;
    v_texCoord = a_texCoord;
}
)";

const GLchar* g_transitionShaderFragment = R"(
#ifdef GL_ES
precision lowp float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
uniform sampler2D CC_Texture0;

uniform float u_edgeSize;

// https://gist.github.com/yiwenl/1c2ce935e66b82c7df5f
float grayscale(vec4 color) {
    return dot(color.rgb, vec3(0.299, 0.587, 0.114));
}

float ease(float t) {
    return sin((t * 3.1416) / 2.0);
}

void main() {
    if (u_edgeSize == -1.0) {
        gl_FragColor = vec4(1.0, 0.0, 1.0, 1.0);
        return;
    }

    float gray = grayscale(texture2D(CC_Texture0, v_texCoord));
    float t = clamp((ease(v_fragmentColor.a) + u_edgeSize - gray) / (2.0 * u_edgeSize), 0.0, 1.0); // no idea how this works
    float alpha = mix(0.0, 1.0, t);

    gl_FragColor = vec4(v_fragmentColor.rgb, alpha);
}
)";

bool g_transitionShaderFailedToLoad = false;

$on_game(TexturesLoaded) {
    auto program = new cocos2d::CCGLProgram;
    bool ret = program->initWithVertexShaderByteArray(g_transitionShaderVertex, g_transitionShaderFragment);
    if (!ret) {
        geode::log::warn("uh oh whoopsie poopsies yo shader failed");
        geode::log::warn("{}", program->fragmentShaderLog());
        g_transitionShaderFailedToLoad = true;
        return;
    }

    program->addAttribute(kCCAttributeNamePosition, cocos2d::kCCVertexAttrib_Position);
    program->addAttribute(kCCAttributeNameTexCoord, cocos2d::kCCVertexAttrib_TexCoords);
    program->addAttribute(kCCAttributeNameColor, cocos2d::kCCVertexAttrib_Color);

    program->link();
    program->updateUniforms();

    geode::log::info("are you ready to sog up your transitions");

    cocos2d::CCShaderCache::sharedShaderCache()->addProgram(program, "transition_fade_shader"_spr);
}
