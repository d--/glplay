In C:
`glBindBuffer(GL_ARRAY_BUFFER, posBufObj);` says that from this line forward,
operations that act on the GL_ARRAY_BUFFER will be acting on posBufObj, because
it has been bound to GL_ARRAY_BUFFER.

In shaders:
If you see an identifier in a GLSL shader that starts with “gl_”, then it must
be a built-in identifier.
